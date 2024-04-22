#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

int main(){
	
	struct timespec startt, endt;
	long long tmpsexec;
	int *delay = (int *)100;
	int i = 0; 
	clock_gettime(CLOCK_MONOTONIC, &startt);
	
	while (i != 1001) {
		wait(delay);
		i++;
	}
	
	clock_gettime(CLOCK_MONOTONIC, &endt);
	
	tmpsexec = (endt.tv_sec	- startt.tv_sec)*1000LL;
	tmpsexec += endt.tv_sec - startt.tv_sec;
	
	printf("Le temps d'execution est de %lld ms", tmpsexec);
	
	return 0;
}
