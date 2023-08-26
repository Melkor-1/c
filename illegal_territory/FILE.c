#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
	FILE *fp;
	fp = fopen("foo.txt", "r");

    /* Platform dependent internal structure of a FILE *. */
	printf("_flags:\t\t%d\n", fp->_flags);
	printf("_IO_read_ptr:\t\t%p\n", fp->_IO_read_ptr);
	printf("_IO_read_end:\t\t%p\n", fp->_IO_read_ptr);
	printf("_IO_read_base:\t\t%p\n", fp->_IO_read_base);
	printf("_IO_write_base:\t\t%p\n", fp->_IO_write_base);
	printf("_IO_write_ptr:\t\t%p\n", fp->_IO_write_ptr);
	printf("_IO_write_end:\t\t%p\n", fp->_IO_write_end);
	printf("_IO_buf_base:\t\t%p\n", fp->_IO_buf_base);
	printf("_IO_buf_end:\t\t%p\n", fp->_IO_buf_end);
	printf("_IO_save_base:\t\t%p\n", fp->_IO_save_base);
	printf("_IO_backup_base:\t%p\n", fp->_IO_backup_base);
	printf("_IO_save_end:\t\t%p\n", fp->_IO_save_end);
	printf("_IO_marker *_markers:\t%p\n", fp->_markers);
	printf("_IO_FILE *_chain:\t%p\n", fp->_chain);
	printf("_fileno:\t\t%d\n", fp->_fileno);

	return EXIT_SUCCESS;
}

