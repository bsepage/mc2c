#pragma once
#include <cerrno>
#include <cstdlib>

extern bool verbose;

#define LOGERROR(f, ...)	\
	do{ printf("[%-5s] ", "ERROR"); printf(f, __VA_ARGS__); exit(EXIT_FAILURE); }while (0)

#define LOGINFO(f, ...)		\
	do{ if (verbose){ printf("[%-5s] ", "INFO"); printf(f, __VA_ARGS__); }}while (0);

