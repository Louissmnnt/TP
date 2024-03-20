#include <time.h>
#include <stdio.h>

void printEachSec(){
	if (clock()% == 0){
		printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}
}

int main(){
int k;

for (k = 0; k < 100000000000; k++) {
	printf("%ld\n", clock()%CLOCKS_PER_SEC);
	printEachSec();
}

return 0;
}
