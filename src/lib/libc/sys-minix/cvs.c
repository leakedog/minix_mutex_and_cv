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
#include <sys/vm.h>

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>


int cs_lock(int mutex_id) {
    int r;
    do {
        message m;
        m.m1_i1 = mutex_id;
        endpoint_t cvs_endpoint;
        minix_rs_lookup("cvs", &cvs_endpoint);
        r = _syscall(cvs_endpoint, 0, &m);
    } while ((r == -1) && (errno == EINTR));  
    return r;
}

int cs_unlock(int mutex_id) {
    message m;
    m.m1_i1 = mutex_id;
    endpoint_t cvs_endpoint;
    minix_rs_lookup("cvs", &cvs_endpoint);
    return _syscall(cvs_endpoint, 1, &m);
}

int cs_wait(int cond_var_id, int mutex_id) {
    message m;
    m.m1_i1 = cond_var_id;
    m.m1_i2 = mutex_id;
    endpoint_t cvs_endpoint;
    minix_rs_lookup("cvs", &cvs_endpoint); 
    int r = _syscall(cvs_endpoint, 2, &m);
    if (r == 0 || (r == -1 && errno == EINTR)) {
        cs_lock(mutex_id); 
        return 0;
    } else {
        return r;
    }
}

int cs_broadcast(int cond_var_id) {
    message m;
    m.m1_i1 = cond_var_id;
    endpoint_t cvs_endpoint;
    minix_rs_lookup("cvs", &cvs_endpoint);
    return _syscall(cvs_endpoint, 3, &m);
}