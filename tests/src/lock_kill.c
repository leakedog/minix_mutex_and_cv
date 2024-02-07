#define _POSIX_SOURCE

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#include "utils.h"

int main(int argc, char* argv[]){

	int res,ch;

	ch=fork();
	if (ch==0){
 	        res=cs_lock(1);
		pause();
		exit(0);
	}
	sleep(1);
	kill(ch, SIGTERM);
 	res=cs_lock(1);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
	return 0;
}
