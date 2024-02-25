#ifndef SO_MINIX_CVS_MAIN_TOLSTIK_H
#define SO_MINIX_CVS_MAIN_TOLSTIK_H

// includes from ipc
#define _POSIX_SOURCE      1	/* tell headers to include POSIX stuff */
#define _MINIX             1	/* tell headers to include MINIX stuff */
#define _SYSTEM            1    /* get OK and negative error codes */

#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/config.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/sysutil.h>
#include <minix/const.h>
#include <minix/type.h>
#include <minix/syslib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <machine/vm.h>
#include <machine/vmparam.h>
#include <sys/vm.h>

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define MUTEX_NR 2 * 1024
#define SIZE(a) (sizeof(a)/sizeof(a[0]))

#define NOT_WAITING 0
#define WAIT_MTX 1
#define WAIT_CV 2

static void sef_local_startup();
static void sef_cb_signal_handler(int signo);
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info));

struct _queue_element {
    endpoint_t end_point;
    endpoint_t prev; // not -1 if in queue
    endpoint_t next; // not -1 if in queue
    int mutex_id;
    int cond_var_id;
    int current_type; // 0 - not in queue, 1 - in mutex queue, 2 - in condvar queue
};

struct _mutex {
    int id;    
    int n_waiting;
    endpoint_t owner_endpoint;
    endpoint_t first;
};

// queue_utils
int get_id_array(struct _mutex* arr, int len, int id);
int get_array_mutex_id(int mutex_id);
int get_array_condvar_id(int cond_var_id);
void delete_element_from_queue(struct _mutex* queue, struct _queue_element* elem);
void add_element_to_queue(struct _mutex* queue, struct _queue_element* elem);
char owns_mutex_array_id(endpoint_t endpoint, int mutex_array_id);
void give_mutex_array_id(int mutex_array_id);
char is_in_mutex_queue();
char is_in_condvar_queue();
void remove_cur_from_condvar_queue();
void remove_cur_from_mutex_queue();
void print_queue(struct _mutex* queue);

// helper functions
void send_result(int result, endpoint_t source);


// handler functions
void do_cvs_lock();
void do_cvs_unlock();
void do_cvs_wait();
void do_cvs_broadcast();
void signal_interruption_handler();
void signal_kill_handler();

#endif //SO_MINIX_CVS_MAIN_TOLSTIK_H
