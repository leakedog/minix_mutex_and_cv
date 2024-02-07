#include "utils.h"

#include <fcntl.h>
#include <sys/stat.h>

#define MAX_CVS 32
#define MAX_TURNS 32


int main(int argc, char* argv[]){
	int i, res,ch,procs,cv_id,mutex_id;
	int cvs[MAX_CVS],mutexes[MAX_CVS];
	char arg1[2];

	srand(getpid());
	if (argc!=2){
		fail("wrong number of arguments");
	}

	procs=atoi(argv[1]);
	if (procs<0 || procs >MAX_CVS){
		fail("wrong number of processes");
	}


	for (i=0; i<procs; i++){
		mutex_id=rand();
		cv_id=rand();
		cvs[i]=cv_id;
		mutexes[i]=mutex_id;

		cs_lock(mutex_id);
		ch= fork();
		if (ch<0) fail("fork failed");
		if (ch==0){
			cs_lock(mutex_id);
			cs_broadcast(cv_id);
			cs_wait(cv_id,mutex_id);
			printf("c:%d\n",i); fflush(stdout);
			cs_broadcast(cv_id);
			cs_unlock(mutex_id);
			exit(0);
		}
		/* parent */
		cs_wait(cv_id,mutex_id);
	}
	
	for (i=procs-1; i>=0; i--){
		cs_broadcast(cvs[i]);
		cs_wait(cvs[i],mutexes[i]);
		cs_unlock(mutexes[i]);
	}

	while (procs>0){
		if (wait(NULL)>0) procs--;
	}

	return 0;
}
