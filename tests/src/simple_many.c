#include "utils.h"

int main(int argc, char* argv[]){

	int res;
	int magicP=(1 << 28)-273;
	int i;
	
	for (i=1; i<= 1024; i++){
		res=cs_lock(i*magicP);
		if (res) fail("lock failed (1)");
		printf("l: %d\n",i);
	}

	for (i=1; i<= 1024; i++){
		res=cs_unlock(i*magicP);
		if (res) fail("unlock failed (1)");
		printf("u: %d\n",i);
	}
	
	for (i=1+1024; i<= 2* 1024; i++){
		res=cs_lock(i*magicP);
		if (res) fail("lock failed (2)");
		printf("l: %d\n",i);
	}

	for (i=2*1024; i>=1+ 1024; i--){
		res=cs_unlock(i*magicP);
		if (res) fail("unlock failed (2)");
		printf("u: %d\n",i);
	}

	for (i=1; i<= 1024; i++){
		res=cs_lock(i*magicP);
		if (res) fail("lock failed (3)");
		printf("l: %d\n",i);
	}

	for (i=1; i<= 1024; i++){
		int m= ((i*173)%1024)+1;
		res=cs_unlock(m*magicP);
		if (res) fail("unlock failed (3)");
		printf("u: %d\n",m);
	}

	return 0;
}
