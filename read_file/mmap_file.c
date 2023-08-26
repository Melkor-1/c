#include <stdio.h>

#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Synopsis: takes a FILE pointer, and maps it to the memory.
*  Returns: a char * on success, NULL otherwise.
*  Caller must unmap the returned char * with munmap.
*/
static char *read_file(FILE *fp) 
{
	struct stat sb;
	if (fstat(fileno(fp), &sb) == -1) {	
		return 0;
	}
	char *file = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file == MAP_FAILED) {
		return 0;
	}

	return file;
}
		

