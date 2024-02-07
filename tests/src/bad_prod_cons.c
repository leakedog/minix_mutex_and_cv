#define _POSIX_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <cv.h>

#include "utils.h"

#define BSIZE 100

#define MAX_PRODS 16
#define MAX_CONS 16
#define MAX_TURNS 10000

int mutex;
int buf_not_empty, buf_not_full;
int pp[2];
char buf[BSIZE+2];
char * count;
char *end_flag;

void producer(int i, int turns);
void consumer(int i);

void msleep(){
	usleep(500000);

}

void read_buf(void){
	if (read(pp[0],buf,BSIZE+2)!=BSIZE+2) fail("read failed");
}

void write_buf(void){
	if (write(pp[1],buf,BSIZE+2)!=BSIZE+2) fail("write failed");
}

void print_buf(){
	int i;
	for (i=0;i<BSIZE; i++){
		printf("%d:", buf[i]);
	}
	printf("\ncount: %d ef: %d\n", *count, *end_flag);
}

int main(int argc, char* argv[]){

	int res,turns,i;
	int prods, cons;

	char * text;

	count=&buf[BSIZE];
	end_flag=&buf[BSIZE+1];

	if (argc!=4){
		fail("syntax: prod_cons prod_nr cons_nr turns");
	}
	prods=atoi(argv[1]);
	if (prods<1 || prods > MAX_PRODS){
		fail("wrong number of producers");
	}
	
	cons=atoi(argv[2]);
	if (cons<0 || cons > MAX_CONS){
		fail("wrong number of consumers");
	}

	turns=atoi(argv[3]);
	if (turns<0 || turns > MAX_TURNS){
		fail("wrong number of turns");
	}
	
	mutex= getpid();
	buf_not_empty= (mutex<<2) +1;
	buf_not_empty= (mutex<<2) +2;

	if (pipe(pp)<0) fail("pipe creation failed");

	write_buf();

	for (i=0; i<prods; i++){
		int ch;
		ch=fork();
		if (ch<0) {
		 	fprintf(stderr,"fork failed");	
			kill(0,SIGTERM);
		}
		if (ch==0){
			producer(i,turns);
			exit(2);
		}
	}
	
	for (i=0; i<cons; i++){
		int ch;
		ch=fork();
		if (ch<0) {
		 	fprintf(stderr,"fork failed");	
			kill(0,SIGTERM);
		}
		if (ch==0){
			consumer(i);
			exit(2);
		}
	}

	while (prods>0){
		if (wait(NULL)>0) prods--;
	}
	cs_lock(mutex);
	read_buf();
	*end_flag=1;
	write_buf();
	cs_broadcast(buf_not_empty);
	cs_unlock(mutex);
	while (wait(NULL)>0);
	return 0;
}


void producer(int n, int turns){
	int t,j,i;
	char d;
	srand(n);

	for (t=0; t<turns; t++){
		d=0;
		msleep();
		while (d==0) {
			d = (char) (rand()%256);
		}
		cs_lock(mutex);
		read_buf();
		while (*count==BSIZE){
			write_buf();
			cs_unlock(mutex);
			cs_lock(mutex);
			read_buf();
		}
		for (i=0;i < BSIZE; i++){
			if (buf[i]==0){
				buf[i]=d;
				(*count)++;
				break;
			}
		}
/*		printf("p:count %d\n", *count);
		print_buf();
*/		
		if (i==BSIZE) fail("p:invalid buffer counter");
		if (*count==1) cs_broadcast(buf_not_empty);
		write_buf();
		cs_unlock(mutex);
	}
	exit(0);
}

void consumer(int n){
	int t,j,i;
	char d;

	srand(n+100);
	for (;;){
		msleep();
		cs_lock(mutex);
		read_buf();
		while (*count==0){
			write_buf();
			if (*end_flag) {
				cs_unlock(mutex);
				exit(0);
			}
			cs_unlock(mutex);
			cs_lock(mutex);
			read_buf();
		}
		for (i=0;i < BSIZE; i++){
			if (buf[i]!=0){
				d=buf[i];
				buf[i]=0;
				(*count)--;
				break;
			}
		}
/*		printf("c:count %d\n", *count);
		print_buf();
*/		
		if (i==BSIZE) fail("c:invalid buffer counter");
		printf("%d\n",d);
		fflush(stdout);
		if (*count==BSIZE-1) cs_broadcast(buf_not_full);
		write_buf();
		cs_unlock(mutex);
	}
}
