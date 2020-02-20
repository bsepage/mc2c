#include <array>
#include <unistd.h>

#include "map.h"
#include "system.h"

/* ************************************************** */

void* map::va(void) const
{ return sp; }

/* ************************************************** */

const std::array<bool, 5> map::da(void) const
{ return ref; }

/* ************************************************** */

void map::print(std::array<bool, 5> _da) const
{
	for (auto& bit: _da)
		std::cout << bit;
	std::cout << std::endl;
}

/* ************************************************** */

const uint64_t map::va2pa(void) const
{
	uint64_t fno (0), off (0);
	FILE *pagemap = fopen("/proc/self/pagemap", "rb");
	if (pagemap==NULL)
		LOGERROR("%s", "fopen\n");
	off = ((uint64_t) sp / (uint64_t) getpagesize()) * 8;
	if (fseek(pagemap, (long) off, SEEK_SET))
		LOGERROR("%s", "fseek\n");
	fread(&fno, 1, 7, pagemap);
	if (fclose(pagemap))
		LOGERROR("%s", "fclose\n");
	return ((fno & 0x7fffffffffffff) << 12) + ((uint64_t) sp % (uint64_t) getpagesize());
}

/* ************************************************** */

/** Skylake: (b13); (b14 ^ b18); (b15 ^ b19); (b16 ^ b20); (b17 ^ b21); **/
const std::array<bool, 5> map::pa2da(const uint64_t pa) const
{
	return std::array<bool, 5> { ((pa >> 13) & 1), ((pa >> 14) & 1) ^ ((pa >> 18) & 1), ((pa >> 15) & 1) ^ ((pa >> 19) & 1),
		((pa >> 16) & 1) ^ ((pa >> 20) & 1), ((pa >> 17) & 1) ^ ((pa >> 21) & 1) };
}

/* ************************************************** */

bool map::search(void)
{
	if (sp>=ep)
		return false;
	sp = reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(sp)+512);
	return (pa2da(va2pa())==ref) ? true : search();
}

/* ************************************************** */

void* map::find_da(const std::array<bool, 5> da)
{
	sp = const_cast<void *>(bp);
	ref = da;
	return (search()) ? sp : NULL;
}

