#define _POSIX_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "utils.h"

void handler(int sn){
}

void register_handler(int sn){
	struct sigaction sa;

	sa.sa_handler=handler;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	sigaction(sn, &sa, NULL);
}

int main(int argc, char* argv[]){
	int res,ch;
	int mutex_id;

	if (argc!=2) fail("wrong number of args");
	mutex_id= atoi(argv[1]);

	register_handler(SIGUSR1);

        res=cs_lock(mutex_id);
	printf("res:%d\n", res);
	if (res<0) printf("errno:%d\n", errno);

	ch=fork();
	if (ch==0){
        	res=cs_lock(mutex_id);
		printf("res:%d\n", res);
		if (res<0) printf("errno:%d\n", errno);
		exit(0);

	} else {
		sleep(1);
		kill(0, SIGUSR1);
		wait(NULL);
		res=cs_unlock(mutex_id);
		printf("res:%d\n", res);
		if (res<0) printf("errno:%d\n", errno);
		exit(0);
	}
}
