#define _POSIX_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "utils.h"

int main(int argc, char* argv[]){
	int res,ch;
	int mutex_id;

	if (argc!=2) fail("wrong number of args");
	mutex_id= atoi(argv[1]);

   	res=cs_lock(mutex_id);
	printf("ch: res:%d\n", res);
	if (res<0) printf("ch: errno:%d\n", errno);
	pause();
	exit(0);
}
