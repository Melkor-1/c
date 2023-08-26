#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>

#include <fcntl.h>
#include <unistd.h>
#include <aio.h>

#define BUF_SIZE 4096

/* Synopsis: Allocates aiocb struct on the heap and reads bytes size data from fp.
*  Returns:  aiocb struct on success, 0 otherwise. Frees the struct on failure. 
*  Remarks:  The caller is responsible for freeing the allocated memory on success.
*/
static struct aiocb *async_read(FILE *fp, char *buf, size_t bytes) 
{
	struct aiocb *const aiocbp = malloc(sizeof(struct aiocb));
	if (!aiocbp) {
		errno = ENOMEM;
  		perror("malloc");
		return 0;
	}
	memset(aiocbp, 0x00, sizeof(*aiocbp));

	int fd = fileno(fp);
	if (fd == -1) {
		free(aiocbp);
		perror("fileno");
		return 0;
	}

	aiocbp->aio_buf = buf;
	aiocbp->aio_fildes = fd;
	aiocbp->aio_nbytes = bytes;
	aiocbp->aio_offset = 0;

	if (aio_read(aiocbp) == -1) {
		free(aiocbp);
		perror("aio_read");
		return 0;
	}
	return aiocbp;
}
	
int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}
	FILE *const fp = fopen(argv[1], "r");
	if (!fp) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	char buf[BUF_SIZE] = {0};
	struct aiocb *const aiocbp = async_read(fp, buf, BUF_SIZE);
	if (!aiocbp) {
	 	if (fclose(fp) == EOF) {
			perror("fclose");
			return EXIT_FAILURE;
		}
		return EXIT_FAILURE;
	}
		
	uint64_t counter = 0;
	while (aio_error(aiocbp) == EINPROGRESS) {
		counter++;
	}
	printf("While we were reading, counter counted up %lu times\n", counter);

	int ret = aio_return(aiocbp);
	if (ret == -1) {
		perror("aio_read");
		return EXIT_FAILURE;
	}

	if (fclose(fp) == EOF) {
		perror("fclose");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

	
