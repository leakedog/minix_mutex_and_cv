#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

int main(int argc, char* argv[]){

	int res,i;

	i=1;
	if (fork()==0){
		i+=fork();	
		i+=fork();	
		i+=fork();	
		i+=fork();	
 	        res=cs_lock((i%4)+1);
		exit(0);
	}
	sleep(1);
 	res=cs_lock(1);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
 	res=cs_lock(2);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
 	res=cs_lock(3);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
 	res=cs_lock(4);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
	return 0;
}
