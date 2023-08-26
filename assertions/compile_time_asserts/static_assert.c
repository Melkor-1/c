#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) 
{
	struct packed {
		char c;
		size_t i;
	};
	static_assert(sizeof(struct packed) == sizeof(size_t) + sizeof(char),
				 "struct packed must not have any padding");
	
	return EXIT_SUCCESS;
}





