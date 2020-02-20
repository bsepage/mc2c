#pragma once
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "system.h"

#define	MMAP_PROT	PROT_READ | PROT_WRITE
#define	MMAP_FLAGS	MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_LOCKED

/* ************************************************** */

class map {
private:
	void* 				sp;
	const void* 			bp;
	const void*			ep;
	const size_t 			len;
	std::array<bool, 5> 		ref;
	const uint64_t 			va2pa(void) const;
	const std::array<bool, 5> 	pa2da(const uint64_t pa) const;
	bool 				search(void);
public:
	map(size_t _len);
	~map(void);
	void* 				va(void) const;
	const std::array<bool, 5> 	da(void) const;
	void 				print(std::array<bool, 5> _da) const;
	void* 				find_da(const std::array<bool, 5> da);
};

/* ************************************************** */

inline map::map(size_t _len)
	: bp (NULL), ep (NULL), len (_len)
{
	sp = mmap(0, len, MMAP_PROT, MMAP_FLAGS, -1, 0);
	if (sp==(void*)-1) { LOGERROR("%s", "mmap\n"); }
	if (mlock(sp, len)) { LOGERROR("%s", "mlock\n"); }
	bp = sp;
	ep = reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(sp)+len);
	ref.fill(0);
}

/* ************************************************** */

inline map::~map(void) {
	if (munmap((void*) bp, len)) { LOGERROR("%s", "munmap\n"); }
	sp = NULL;
	bp = NULL;
	ep = NULL;
}

