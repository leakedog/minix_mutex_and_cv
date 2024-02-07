#include "utils.h"

#define MAX_PROCS 32
#define MAX_TURNS 32

int mutex_id,procs,turns;

void child(char n){
	char mc[2];
	int i;

	mc[0]='a'+n;
	mc[1]='\n';

	if (cs_lock(mutex_id)) fail("lock failed");
	write(1,&mc,2);
	if (cs_unlock(mutex_id)) fail("unlock failed");
	exit(0);
}

int main(int argc, char* argv[]){
	int i, res,ch;

	mutex_id= getpid();
	if (argc!=2){
		fail("wrong number of arguments");
	}

	procs=atoi(argv[1]);
	if (procs<0 || procs >MAX_PROCS){
		fail("wrong number of processes");
	}

	if (cs_lock(mutex_id)) fail("lock failed");

	for (i=0; i<procs; i++){
		ch= fork();
		if (ch<0) fail("fork failed");
		if (ch==0){
			child(i);
		}
		sleep(1);
	}

	if (cs_unlock(mutex_id)) fail("unlock failed");

	while (procs>0){
		if (wait(NULL)>0) procs--;
	}

	return 0;
}
