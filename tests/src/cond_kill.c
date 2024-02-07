#define _POSIX_SOURCE

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#include "utils.h"

int main(int argc, char* argv[]){

	int res,ch1,ch2;

	setsid();

	ch1=fork();
	if (ch1==0){
 	        res=cs_lock(1);
		cs_wait(1,1);
		exit(0);
	}
	sleep(1);
	ch2=fork();
	if (ch2==0){
 	        res=cs_lock(1);
		cs_wait(1,1);
		write(1,"OK\n",3);
		sleep(1);
		kill(0,SIGTERM);
		exit(0);
	}
	sleep(1);
	kill(ch1,SIGTERM);
	cs_broadcast(1);
	sleep(3);
	printf("FAIL\n");
	return 0;
}
