#include "main.h"

message m;
struct _queue_element processes[NR_PROCS];
struct _mutex mutexes[MUTEX_NR];
struct _mutex condvars[NR_PROCS];

int get_id_array(struct _mutex* arr, int len, int id) {
    int j = -1;
    for (int i = 0; i < len; i++) {
        if (arr[i].id == id) {
            j = i;
            break;
        }
        if (j == -1 && arr[i].owner_endpoint == -1 && arr[i].n_waiting == 0) {
            j = i;
        }
    }
    if (j == -1) {
        panic("Count of mutexes is larger than expected");
    }
    arr[j].id = id;
    return j;
}


int get_array_mutex_id(int mutex_id) {
    return get_id_array(mutexes, MUTEX_NR, mutex_id);
}

int get_array_condvar_id(int cond_var_id) {
    return get_id_array(condvars, NR_PROCS, cond_var_id);
}

void delete_element_from_queue(struct _mutex* queue, struct _queue_element* elem) {
    if (elem) {
        if (queue->n_waiting == 0) {
            panic("Queue is empty, but I am trying to delete");
        }
        queue->n_waiting--;
        if (queue->n_waiting == 0) {
            queue->first = -1;
        } else {
            endpoint_t next = elem->next;
            if (next == -1) panic("wrong next");
            endpoint_t source_of_next = _ENDPOINT_P(next);
            endpoint_t prev = elem->prev;
            if (prev == -1) panic("wrong prev");
            endpoint_t source_of_prev = _ENDPOINT_P(prev);
            processes[source_of_prev].next = next;
            processes[source_of_next].prev = prev;
            if (queue->first == elem->end_point) {
                queue->first = next;
            }
        }
        elem->prev = elem->next = -1;
    }
}

void add_element_to_queue(struct _mutex* queue, struct _queue_element* elem) {
    elem->prev = elem->next = elem->end_point;
    if (queue->n_waiting == 0) {
        queue->first = elem->end_point;
    } else {
        endpoint_t first = queue->first;
        endpoint_t source_of_first = _ENDPOINT_P(first);
        endpoint_t last = processes[source_of_first].prev;
        endpoint_t source_of_last = _ENDPOINT_P(last);

        processes[source_of_last].next = elem->end_point;
        processes[source_of_first].prev = elem->end_point;
        elem->prev = last;
        elem->next = first;
    }
    queue->n_waiting++;
}

char owns_mutex_array_id(endpoint_t endpoint, int mutex_array_id) {
    return (mutexes[mutex_array_id].owner_endpoint == endpoint);
}

void print_queue(struct _mutex* queue) {
    endpoint_t cur = queue->first;
    if (cur != -1) {
        endpoint_t start = cur;
        printf("queue: ");
        do {
            endpoint_t id = _ENDPOINT_P(cur);
            printf("%d %d %d\n", _ENDPOINT_P(processes[id].prev), id, _ENDPOINT_P(processes[id].next));
            cur = processes[_ENDPOINT_P(cur)].next;
        } while (start != cur);
        printf("\n");
    }
}

// guaranted this mutex is owned by this process
void give_mutex_array_id(int mutex_array_id) {
    if (mutexes[mutex_array_id].n_waiting > 0) {
        endpoint_t new_owner_endpoint = mutexes[mutex_array_id].first;
        endpoint_t source_of_new = _ENDPOINT_P(new_owner_endpoint);
        delete_element_from_queue(&mutexes[mutex_array_id], &processes[source_of_new]);
        mutexes[mutex_array_id].owner_endpoint = new_owner_endpoint;
        processes[source_of_new].mutex_id = -1;
        processes[source_of_new].current_type = NOT_WAITING;
        send_result(OK, new_owner_endpoint);
    } else {
        mutexes[mutex_array_id].owner_endpoint = -1;
    }
}

static struct {
    int type;
    void (*func)();
} cvs_calls[] = {
        {0, do_cvs_lock},
        {1, do_cvs_unlock},
        {2, do_cvs_wait},
        {3, do_cvs_broadcast},
        {4, signal_interruption_handler},
        {5, signal_kill_handler},
};


int main(int argc, char *argv[]){
    env_setargs(argc, argv);
    sef_local_startup();

    // init mutexes, condvars and processes
    for(int i = 0; i < MUTEX_NR; i++) {
        mutexes[i].first = mutexes[i].owner_endpoint = -1;
        mutexes[i].n_waiting = 0;  
        mutexes[i].id = -1;      
    }

    for(int i = 0; i < NR_PROCS; i++) {
        condvars[i].first = condvars[i].owner_endpoint = -1;
        condvars[i].n_waiting = 0;  
        condvars[i].id = -1;      
    }

    for(int i = 0; i < NR_PROCS; i++){
        processes[i].next = processes[i].prev = processes[i].mutex_id = processes[i].cond_var_id = -1;
        processes[i].current_type = NOT_WAITING;
    }

    while (TRUE) {
        int r;
        if ((r = sef_receive(ANY, &m)) != OK)
            printf("sef_receive failed %d.\n", r);
        if(m.m_type >= 0 && m.m_type < SIZE(cvs_calls)){
            cvs_calls[m.m_type].func();
        }
    }
    return 0;
}

// ipc sef
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info))
{
    return (OK);
}

static void sef_cb_signal_handler(int signo)
{
    if (signo != SIGTERM) return;
}

static void sef_local_startup()
{
    sef_setcb_init_fresh(sef_cb_init_fresh);
    sef_setcb_init_restart(sef_cb_init_fresh);

    sef_setcb_signal_handler(sef_cb_signal_handler);

    sef_startup();
}
// ipc sef


// send result to process
void send_result(int result, endpoint_t source){
    m.m_type = result;
    sendnb(source, &m);
}


// lock cvs with some mutex_id
void do_cvs_lock() {
    endpoint_t system_endpoint = m.m_source;
    endpoint_t source_endpoint = _ENDPOINT_P(system_endpoint);
    int mutex_id = m.m1_i1;
    int mutex_array_id = get_array_mutex_id(mutex_id);
    if (mutexes[mutex_array_id].owner_endpoint == -1) {
        mutexes[mutex_array_id].owner_endpoint = system_endpoint;
        send_result(OK, system_endpoint);
    } else {
        struct _queue_element elem;
        elem.cond_var_id = -1;
        elem.mutex_id = mutex_id;
        elem.end_point = system_endpoint;
        elem.current_type = WAIT_MTX;
        elem.prev = -1;
        elem.next = -1;
        processes[source_endpoint] = elem;
        add_element_to_queue(&mutexes[mutex_array_id], &processes[source_endpoint]);
    }
}

// unlock mutex, give ownership to the next process if need
void do_cvs_unlock() {
    endpoint_t system_endpoint = m.m_source;
    endpoint_t source_endpoint = _ENDPOINT_P(system_endpoint);
    int mutex_id = m.m1_i1;
    int mutex_array_id = get_array_mutex_id(mutex_id);
    if (!owns_mutex_array_id(system_endpoint, mutex_array_id)) {
        send_result(EPERM, system_endpoint);
        return;
    } 
    give_mutex_array_id(mutex_array_id);
    send_result(OK, system_endpoint);
}

// wait on conditional variable, check mutex free it. Implementing cs_wait(int cond_var_id, int mutex_id) 
void do_cvs_wait() {
    endpoint_t system_endpoint = m.m_source;
    endpoint_t source_endpoint = _ENDPOINT_P(system_endpoint);
    int condvar_id = m.m1_i1;
    int mutex_id = m.m1_i2;
    int mutex_array_id = get_array_mutex_id(mutex_id);
    if (!owns_mutex_array_id(system_endpoint, mutex_array_id)) {
        send_result(EINVAL, system_endpoint);
        return;
    }
    give_mutex_array_id(mutex_array_id);
    int condvar_array_id = get_array_condvar_id(condvar_id);
    struct _queue_element elem;
    elem.cond_var_id = condvar_id;
    elem.mutex_id = mutex_id;
    elem.end_point = system_endpoint;
    elem.current_type = WAIT_CV;
    elem.prev = -1;
    elem.next = -1;
    processes[source_endpoint] = elem;
    add_element_to_queue(&condvars[condvar_array_id], &processes[source_endpoint]);
}

// notify on conditional variable, clear queue and notify everyone
void do_cvs_broadcast() {
    endpoint_t system_endpoint = m.m_source;
    int condvar_id = m.m1_i1;
    int condvar_array_id = get_array_condvar_id(condvar_id);
    while (condvars[condvar_array_id].n_waiting) {
        endpoint_t cur_endpoint = condvars[condvar_array_id].first;
        if (cur_endpoint == -1) panic("queue is not free, but first is -1");
        endpoint_t source_of_point = _ENDPOINT_P(cur_endpoint);
        delete_element_from_queue(&condvars[condvar_array_id], &processes[source_of_point]);
        processes[source_of_point].cond_var_id = -1;
        processes[source_of_point].current_type = NOT_WAITING;
        send_result(OK, cur_endpoint);
    }
    send_result(OK, system_endpoint);
}

char is_in_mutex_queue() {
    endpoint_t source_endpoint = _ENDPOINT_P(m.m1_i1);
    return  (processes[source_endpoint].current_type == WAIT_MTX);
}

char is_in_condvar_queue() {
    endpoint_t source_endpoint = _ENDPOINT_P(m.m1_i1);
    return  (processes[source_endpoint].current_type == WAIT_CV);
}

void remove_cur_from_condvar_queue() {
    endpoint_t source_endpoint = _ENDPOINT_P(m.m1_i1);
    int condvar_id = processes[source_endpoint].cond_var_id;
    int condvar_array_id = get_array_condvar_id(condvar_id);
    delete_element_from_queue(&condvars[condvar_array_id], &processes[source_endpoint]);
    processes[source_endpoint].current_type = NOT_WAITING;
}

void remove_cur_from_mutex_queue() {
    endpoint_t source_endpoint = _ENDPOINT_P(m.m1_i1);
    int mutex_id = processes[source_endpoint].mutex_id;
    int mutex_array_id = get_array_mutex_id(mutex_id);
    delete_element_from_queue(&mutexes[mutex_array_id], &processes[source_endpoint]);
    processes[source_endpoint].current_type = NOT_WAITING;
}

void signal_interruption_handler() {
    for (int mutex_array_id = 0; mutex_array_id < MUTEX_NR; mutex_array_id++) {
        if (owns_mutex_array_id(m.m1_i1, mutex_array_id)) {
            give_mutex_array_id(mutex_array_id);
        }
    }
    if (is_in_condvar_queue()) {
        remove_cur_from_condvar_queue();
        send_result(EINTR, m.m1_i1);
    } else if (is_in_mutex_queue()) {
        remove_cur_from_mutex_queue();
        send_result(EINTR, m.m1_i1);
    }
}

void signal_kill_handler() {
    for (int mutex_array_id = 0; mutex_array_id < MUTEX_NR; mutex_array_id++) {
        if (owns_mutex_array_id(m.m1_i1, mutex_array_id)) {
            give_mutex_array_id(mutex_array_id);
        }
    }
    if (is_in_condvar_queue()) {
        remove_cur_from_condvar_queue();
    } else if (is_in_mutex_queue()) {
        remove_cur_from_mutex_queue();
    }
} 