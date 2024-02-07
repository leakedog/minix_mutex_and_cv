#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cv.h>

void fail(char * str){
	fprintf(stderr,"FAIL: %s.\n",str);
	exit(2);
}

void report(char * str, int res){
	int err= errno;
	printf("%s %d\n", str, res);
	if (res != 0){
		printf("errno:%d\n",err);
	}
}

