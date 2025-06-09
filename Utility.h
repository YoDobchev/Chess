#ifndef UTILITY_H
#define UTILITY_H

#include <cstddef>

class Utility {
  public:
	static size_t strlen(const char* src) {
		size_t len = 0;
		while (*src++)
			len++;
		return len;
	}

	static char* strcpy(char* dest, const char* src) {
		char* original = dest;
		while ((*dest++ = *src++) != '\0') {
		}
		return original;
	}

	static int strcmp(const char* s1, const char* s2) {
		while (*s1 && (*s1 == *s2)) {
			s1++;
			s2++;
		}
		return (unsigned char)*s1 - (unsigned char)*s2;
	}
};

#endif