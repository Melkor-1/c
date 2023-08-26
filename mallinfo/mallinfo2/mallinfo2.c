#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#define MAX_ALLOCS (4096 * 100)

static void display_mallinfo2(void) 
{
	struct mallinfo2 mi = mallinfo2();
            
    printf("Total non-mmapped bytes (arena):       %zu\n", mi.arena);
    printf("# of free chunks (ordblks):            %zu\n", mi.ordblks);
    printf("# of free fastbin blocks (smblks):     %zu\n", mi.smblks);
    printf("# of mapped regions (hblks):           %zu\n", mi.hblks);
    printf("Bytes in mapped regions (hblkhd):      %zu\n", mi.hblkhd);
    printf("Max. total allocated space (usmblks):  %zu\n", mi.usmblks);
    printf("Free bytes held in fastbins (fsmblks): %zu\n", mi.fsmblks);
    printf("Total allocated space (uordblks):      %zu\n", mi.uordblks);
    printf("Total free space (fordblks):           %zu\n", mi.fordblks);
    printf("Topmost releasable block (keepcost):   %zu\n", mi.keepcost);
}

int main(void) 
{
	printf("-=-=-=-=-=-=-=-=-BEFORE MALLOC-=-=-=-=-=-=-=-=-=-=\n");
	display_mallinfo2();

	char *const address = malloc(MAX_ALLOCS);
	if (!address) {
		errno = ENOMEM;
		perror("malloc");
		return EXIT_FAILURE;
	}
	memset(address, 0x0F, 1);

	printf("\n-=-=-=-=-=-=-=-=-AFTER MALLOC-=-=-=-=-=-=-=-=-=-\n");
	display_mallinfo2();
	free(address);

	printf("\n-=-=-=-=-=-=-=-=-BEFORE MMAP-=-=-=-=-=-=-=-=-=-\n");
	display_mallinfo2();

	char *const new = mmap(0, 2 * MAX_ALLOCS, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new == MAP_FAILED) {
		perror("mmap");
		return EXIT_FAILURE;
	}
	memset(new, 0x0A, 1);

	printf("\n-=-=-=-=-=-=-=-=-AFTER MMAP-=-=-=-=-=-=-=-=-=-=-\n");
	display_mallinfo2();
	if (munmap(new, MAX_ALLOCS) == -1) {
		perror("munmap");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
	


