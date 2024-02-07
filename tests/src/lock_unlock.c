#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "utils.h"

int main(int argc, char* argv[]){

	int res;

    res=cs_lock(1);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);

 	res= cs_unlock(2);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
	
 	res= cs_unlock(1);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
	
 	res= cs_unlock(1);
	printf("res:%d\n", res);
	if (res)
		printf("errno:%d \n", errno);
	
	return 0;
}
