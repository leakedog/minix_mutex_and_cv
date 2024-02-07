#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

int main(int argc, char* argv[]){

	int res;

	if (fork()==0){
 	        res=cs_lock(1);
		exit(0);
	}
	sleep(1);
 	res=cs_lock(1);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
	return 0;
}
