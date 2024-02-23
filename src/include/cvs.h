#ifndef SO_MINIX_CVS_TOLSTIK_H
#define SO_MINIX_CVS_TOLSTIK_H

int cs_lock(int mutex_id);

int cs_unlock(int mutex_id);

int cs_wait(int cond_var_id, int mutex_id);

int cs_broadcast(int cond_var_id);

#endif //SO_MINIX_CVS_TOLSTIK_H
