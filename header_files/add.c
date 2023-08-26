#include <limits.h>
#include <stddef.h>

#include "add.h"

int add(int a, int b) {
	if (b > 0 && a > INT_MAX - b) { /* 'a + x' would overflow */
		return 0;
	} 
	else if (b < 0 && a < INT_MIN - b ) { /* 'a - x' would underflow */
		return 0;
	}

	return a + b;
}

