#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/time.h>

int main(void)
{
	struct timeval t;
	if (gettimeofday(&t, NULL)) { perror("gettimeofday"); }
	printf("%ld\n", t.tv_sec%100);
    	exit(EXIT_SUCCESS);
}
