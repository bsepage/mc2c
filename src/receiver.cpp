#include <cassert>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

#include "map.h"
#include "system.h"

extern size_t msr_len;
extern size_t num_pages_recv;

using namespace std;

/* ************************************************** */

long int timerset(int off)
{
	timeval t0;
	if (gettimeofday(&t0, NULL)) { LOGERROR("%s", "gettimeofday\n"); }
	return (t0.tv_sec - (t0.tv_sec)%100 + (long int) off);
}

/* ************************************************** */

uint64_t access(void *addr)
{
	uint32_t ts0h (0), ts0l (0), ts1h (0), ts1l (0);
	__asm volatile("clflush 0(%0)\n" : : "c"(addr) : "rax");
	__asm volatile("cpuid\n" : : : "rax","rbx","rcx","rdx");
	__asm volatile("rdtsc\n\t			\
			mov %%edx, %0\n\t	\
			mov %%eax, %1\n\t" : "=r"(ts0h),"=r"(ts0l) : : "rax","rdx");
	__asm volatile("movq (%0), %%rax\n" : : "c"(addr) : "%rax");
	__asm volatile("rdtscp\n\t			\
			mov %%edx, %0\n\t	\
			mov %%eax, %1\n\t" : "=r"(ts1h),"=r"(ts1l) : : "rax","rdx");
	__asm volatile("cpuid\n" : : : "rax","rbx","rcx","rdx");
	return (((uint64_t) ts1h << 32) | ts1l) - (((uint64_t) ts0h << 32) | ts0l);
}

/* ************************************************** */

void record(const vector<uint16_t> vec, const char *path)
{
    	remove(path);
    	size_t pos (0);
	for (auto it=vec.begin(); it!=vec.end(); it++) {
        char s_buf[256];
        for (size_t j=0; j<sizeof s_buf; j++) { s_buf[j] = 0; }
	        snprintf(s_buf, sizeof s_buf, "%lu\t%u\n", pos, *it);
		FILE *f;
		f = fopen(path, "a");
	        if (f==NULL) { LOGERROR("%s", "fopen"); }
	        size_t k = 0;
	        for (k=0, fputc(s_buf[k], f); s_buf[k]!='\n'; ++k, fputc(s_buf[k], f)) { ; }
	        fclose(f);
        	pos++;
    	}
}

/* ************************************************** */

int main(int argc, char *argv[])
{
	cout << endl;
	assert( argc==2 );
	map m(getpagesize()*num_pages_recv);
	LOGINFO("mapped %lu memory pages\n", num_pages_recv);

	void *va = m.find_da(array<bool, 5> {{0, 0, 0, 0, 0}});
	if (!va) { LOGERROR("%s", "failed to find virtual pointer\n"); }
	LOGINFO("va = %p\n", va);

	vector<uint16_t> msr;
	msr.resize(msr_len);

	long int t1 = timerset(atoi(argv[1]));
	LOGINFO("%s", "counting down...\n");
	timeval t0;
	while (1) {
		if (gettimeofday(&t0, NULL)) { LOGERROR("%s", "gettimeofday"); }
		if (!(t1-(t0.tv_sec))) { break; }
	}
	/** NO CODE HERE **/
	for (auto &n : msr) { n = access(va); }
	LOGINFO("%s", "attack has finished\n");

	record(msr, "sim/measures.dat");
	LOGINFO("%s", "measurements recorded\n");

    return 0;
}

