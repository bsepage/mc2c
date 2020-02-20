#include <cassert>
#include <ctime>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/time.h>
#include <cmath>
#include <cstdint>

#include "map.h"
#include "system.h"

extern size_t msr_len;
extern size_t num_pages_send;
extern size_t num_bits;

using namespace std;

/* ************************************************** */

long int timerset(int off)
{
	timeval t0;
	if (gettimeofday(&t0, NULL))
		LOGERROR("%s", "gettimeofday\n");
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

int main(int argc, char *argv[])
{
	cout << endl;
	assert( argc==2 );
	map m(getpagesize()*num_pages_send);
	LOGINFO("mapped %lu memory pages\n", num_pages_send);

	void *va1 = m.find_da(array<bool, 5> {{0, 0, 0, 0, 1}});
	if (!va1)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va1= %p\n", va1);

	void *va2 = m.find_da(array<bool, 5> {{0, 0, 0, 1, 0}});
	if (!va2)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va2= %p\n", va2);
	
	void *va3 = m.find_da(array<bool, 5> {{0, 0, 0, 1, 1}});
	if (!va3)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va3= %p\n", va3);
	
	void *va4 = m.find_da(array<bool, 5> {{0, 0, 1, 0, 0}});
	if (!va4)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va4= %p\n", va4);
	
	void *va5 = m.find_da(array<bool, 5> {{0, 0, 1, 0, 1}});
	if (!va5)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va5= %p\n", va5);
	
	void *va6 = m.find_da(array<bool, 5> {{0, 0, 1, 1, 0}});
	if (!va6)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va6= %p\n", va6);
	
	void *va7 = m.find_da(array<bool, 5> {{0, 0, 1, 1, 1}});
	if (!va7)
		LOGERROR("%s", "failed to find virtual pointer\n");
	LOGINFO("va7= %p\n", va7);

	int b (0);
	long int t1 = timerset(atoi(argv[1]));
	LOGINFO("%s", "counting down...\n");
	timeval t0;
	while (1)
	{
		if (gettimeofday(&t0, NULL))
			LOGERROR("%s", "gettimeofday");
		if (!(t1-(t0.tv_sec)))
			break;
	}
	/** NO CODE HERE **/
	for (size_t i (0); i<num_bits; ++i)
	{
		if (b==1)
		{
			for (size_t j (0); j<(msr_len/num_bits)/7; ++j)
			{
				access(va1);
				access(va2);
				access(va3);
				access(va4);
				access(va5);
				access(va6);
				access(va7);
			}
		}
		else
		{
			for (size_t j (0); j<(msr_len/num_bits)/7; ++j)
			{
				access(va1);
				access(va1);
				access(va1);
				access(va1);
				access(va1);
				access(va1);
				access(va1);
			}
		}
		b ^= 1;
	}

	return 0;
}

