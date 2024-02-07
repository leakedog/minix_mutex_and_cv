#define _POSIX_SOURCE

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#include "utils.h"

int main(int argc, char* argv[]){

	int res,i;

	i=1;
	if (fork()==0){
		if (fork()==0){
			setsid();
			if (fork()!=0){
				sleep(2);
				kill(0,SIGTERM);
			}
			i+=(fork()==0);	
			i+=(fork()==0);	
			i+=(fork()==0);	
 	       		cs_lock((i%4)+1);
 	       		cs_wait((i%4)+1,(i%4)+1);
			exit(0);
		} else {
			i+=(fork()==0);	
			i+=(fork()==0);	
			i+=(fork()==0);	
 	       		cs_lock((i%4)+1);
 	       		cs_wait((i%4)+1,(i%4)+1);
			write(1,"OK\n",3);
			exit(0);
		}
	}
	sleep(4);
	write(1,"1:",2);
	cs_broadcast(1);
	sleep(1);
	write(1,"2:",2);
	cs_broadcast(2);
	sleep(1);
	write(1,"3:",2);
	cs_broadcast(3);
	sleep(1);
	write(1,"4:",2);
	cs_broadcast(4);
	sleep(1);

	while (wait(NULL)>0);
	return 0;
}
