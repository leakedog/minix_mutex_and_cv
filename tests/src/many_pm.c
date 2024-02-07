#include "utils.h"

#include <fcntl.h>
#include <sys/stat.h>

#define HALF_MAX_PROCS 16
#define MAX_TURNS 32

int mutex_id,turns;

void mwrite(char n, int fd){
	char mc[2];
	
	mc[0]='a'+n;
	mc[1]='\n';

	if (cs_lock(mutex_id+fd)) fail("lock failed");
	write(fd,&mc,1);
	sleep(1);
	write(fd,&mc,2);
	if (cs_unlock(mutex_id+fd)) fail("unlock failed");
}

void child(char n, int fd0,int fds){
	int i,j;

	for (i=0; i< turns; i++){
		for (j=0; j<fds;j++){
			mwrite(n, fd0+(j*(n+1))%fds );
		}
	}
	exit(0);
}

int main(int argc, char* argv[]){
	int i, res,ch, hprocs,procs,fd0;
	char arg1[2];

	mutex_id= getpid()<<5;
	if (argc!=4){
		fail("wrong number of arguments");
	}

	hprocs=atoi(argv[1]);
	if (hprocs<0 || hprocs >HALF_MAX_PROCS){
		fail("wrong number of processes");
	}

	turns=atoi(argv[2]);
	if (turns<0 || turns >MAX_TURNS){
		fail("wrong number of turns");
	}

	fd0=0;
	for (i=0; i<hprocs+1; i++){
		int next_fd;
		char buf[100];

		sprintf(buf,"%s.%d.out",argv[3],i);

		next_fd= open(buf,O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
		if (next_fd <0) fail("open failed");

		if (fd0==0) fd0=next_fd;
		else if (next_fd != fd0+i) fail("not consecutive fds");
	}

	for (i=0; i<hprocs; i++){
		ch= fork();
		if (ch<0) fail("fork failed");
		if (ch==0){
			child(i,fd0,hprocs+1);
		}
		ch= fork();
		if (ch<0) fail("fork failed");
		if (ch==0){
			child(i,fd0,hprocs+1);
		}
	}

	procs= 2* hprocs;
	while (procs>0){
		if (wait(NULL)>0) procs--;
	}

	return 0;
}
