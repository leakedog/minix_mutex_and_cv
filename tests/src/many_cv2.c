#include "utils.h"

#include <fcntl.h>
#include <sys/stat.h>

#define MAX_CVS 32
#define MAX_TURNS 32


int main(int argc, char* argv[]){
	int i, res,ch,procs,cv_id,mutex_id;

	int cv1, cv2;
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

	cv1= rand();
	cv2= rand();
	mutex_id= rand();
	cs_lock(mutex_id);
	for (i=0; i<procs; i++){
		ch= fork();
		if (ch<0) fail("fork failed");
		if (ch==0){
			cs_lock(mutex_id);
			cs_broadcast(cv2);
			cs_wait(cv1,mutex_id);
			printf("c:%d\n",i); fflush(stdout);
			cs_unlock(mutex_id);
			exit(0);
		}
		/* parent */
		cs_wait(cv2,mutex_id);
	}
	
	cs_broadcast(cv1);
	cs_unlock(mutex_id);

	while (procs>0){
		if (wait(NULL)>0) procs--;
	}

	return 0;
}
