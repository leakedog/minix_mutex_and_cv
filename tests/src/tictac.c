#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "utils.h"

#define MUTEX 1
#define CV 1

#define MAX_DELAY 30

int main(int argc, char* argv[]){

	int res,turns,i;

	char * text;

	if (argc!=2){
		fail("one argument needed");
	}

	turns=atoi(argv[1]);
	if (turns<0 || turns > MAX_DELAY){
		fail("wrong number of turns");
	}
	

       report("p lock:",cs_lock(MUTEX));

	if (fork()){
		/* parent */
		for (i=0;i< turns; i++){
			cs_wait(CV,MUTEX);
			write(1,"TI",2);
			sleep(1);
			write(1,"C\n",2);
			cs_broadcast(CV);
		}
		cs_unlock(MUTEX);
		wait(NULL);
		exit(0);
	} else {
		cs_lock(MUTEX);
		cs_broadcast(CV);
		for (i=0;i< turns; i++){
			cs_wait(CV,MUTEX);
			write(1,"TA",2);
			sleep(1);
			write(1,"C\n",2);
			cs_broadcast(CV);
		}
		cs_unlock(MUTEX);
		exit(0);
	}
}
