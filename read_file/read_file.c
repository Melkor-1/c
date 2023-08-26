#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>

/* fgets_file & mmap_file both work perfectly fine as of now. But they have design flaws 
 * (return values), so fix them. And fix the memory leaks. 
 */
struct editor_row {
	char *line;
	size_t sz;
};

struct file_buf {
	struct editor_row *erow;
	uintmax_t alloc_sz;
	uintmax_t nrows; 
} f_buf = { 0 };

static ssize_t fgets_file (FILE *stream);
static ssize_t mmap_file (FILE *stream);
static ssize_t getline_file (FILE *stream);

int main (int argc, char *argv[]) 
{
	(void) argc;

	assert (argv[1]);

	FILE *const stream = fopen (argv[1], "r");
	if (!stream) {
		return EXIT_FAILURE;
	}

	assert (!fgets_file (stream));
	//assert (mmap_file (stream));
	//assert (!getline_file (stream));

	if (f_buf.erow) {
		for (size_t i = 0; i < f_buf.nrows; i++) {
			if (f_buf.erow[i].line) {
				puts (f_buf.erow[i].line);
				free (f_buf.erow[i].line);
			}
		}
		free (f_buf.erow);
	}
	fclose (stream);
	return EXIT_SUCCESS;
}

static void *safe_trim (void *p, size_t n) 
{
	void *const cp = realloc (p, n);
	return cp ? cp : p;
}

/* Reads a line of arbitrary length until exhaustion. Returns -1 on fgets() failure, 
 * 0 on malloc() failure, and 1 otherwise.. The caller is responsible for freeing the
 * memory pointed to by BUF.
 */
static ssize_t fget_line (FILE *restrict stream, char **restrict buf, size_t *restrict sz) 
{
	static size_t bufsiz = 0;
	*sz = 0;
	
	if (!*buf) {
		bufsiz += BUFSIZ;
		*buf = malloc (bufsiz);
		if (!*buf) {
			return 0;
		}
	}

	for (;;) {
		if (bufsiz <= *sz) {
			
			/* This is an arbitrary limit.
			 * Does anyone know how to do this without a limit?
	 		 */
			bufsiz += BUFSIZ;
			void *const tmp = realloc (*buf, bufsiz);
			if (!tmp) {
				free (*buf);
				return 0;
			}
			*buf = tmp;
		}

		if (!fgets (*buf + *sz, (int) bufsiz, stream)) {
			free (*buf);
			return -1;	
		}
			
		*sz += strlen (*buf + *sz);

		/* What about to Microsoft's \r\n? */
		if ((*buf)[*sz - 1] != '\n') {
			if (feof (stream)) {
				/* A missing '\n' should be ignored on EOF. */
				return 1;
			} 
			/* A whole line was not read, continue reading. */
			continue;
		}
		break;
	}
	return 1;
}

static ssize_t fgets_file (FILE *stream) 
{
	clearerr (stream);

	char *buf = 0;
	size_t sz = 0;
	ssize_t ret_val = 0;
	bool eof = false;

	for (;;) {
		if (f_buf.alloc_sz <= f_buf.nrows) {
			f_buf.alloc_sz += BUFSIZ;		
			void *const tmp = realloc (0, f_buf.alloc_sz * sizeof (struct editor_row)); 
			
			if (!tmp) {
				goto fatal;
			}
			f_buf.erow = tmp;
		}
		
		ret_val = fget_line (stream, &buf, &sz);
		if (!ret_val) {
			perror ("malloc()");
			goto fatal;
		} else if (ret_val == -1) {
			if (ferror (stream)) {
				goto fatal;
			}
			/* We must have reached end-of-file. */
			break;
		}
		 
		/* Strip newline. strcspn() might be neat, but is slow.
		 */
		if (sz > 0 && buf[sz - 1] == '\n') {
			buf[--sz] = '\0';
		}

		f_buf.erow[f_buf.nrows].line = malloc (sz + 1);

		if (!f_buf.erow[f_buf.nrows].line) {
			goto fatal;
		}
		memcpy (f_buf.erow[f_buf.nrows].line, buf, sz + 1);
		f_buf.erow[f_buf.nrows].sz = sz;
		f_buf.nrows++;

		if (eof) {
			break;
		}
	}
	
	/* Trim to maximum used. Can we update F_BUF.ALLOC_SZ afterwards? 
	 * Does it matter?
	 */
	f_buf.erow = safe_trim (f_buf.erow, f_buf.nrows * sizeof (*(f_buf.erow)));
	free (buf);
	return 0;
  fatal:
  	free (buf);
  	return -1;
}

static ssize_t mmap_file (FILE *stream)
{
	char *lhs;
	char *rhs;
	ptrdiff_t sz = 0;
	struct stat st = { 0 };
	int fd = fileno (stream);
	bool exit_status = false;
	
	if (fstat (fd, &st) == -1) {
		perror ("fstat()");
		goto fatal;
	}
	
	char *const map = mmap (0, (size_t) st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (!map) {
		perror ("mmap()");
		goto fatal;
	}
		
	for (lhs = map; lhs < &map[st.st_size]; lhs = rhs + 1) {
		rhs = strchr (lhs, '\n');

		if (!rhs) {	
			/* We have reached end-of-file or the file is malformed. */
			break;
		}
		
		sz = rhs - lhs;

		/* Strip traling newline. */
		if (sz > 0) {
			--sz;
		}

		if (f_buf.alloc_sz <= f_buf.nrows) {
			f_buf.alloc_sz += BUFSIZ;
			
			void *const tmp = realloc (f_buf.erow, f_buf.alloc_sz * sizeof (*(f_buf.erow)));
			
			if (!tmp) {
				munmap (map, (size_t) st.st_size);
				goto fatal;
			}
			f_buf.erow = tmp;
		}

		f_buf.erow[f_buf.nrows].line = malloc ((size_t) sz + 1);

		if (!f_buf.erow[f_buf.nrows].line) {
			munmap (map, (size_t) st.st_size);
			goto fatal;
		}
		memcpy (f_buf.erow[f_buf.nrows].line, lhs, (size_t) sz);
		f_buf.erow[f_buf.nrows].line[sz] = '\0';
		f_buf.erow[f_buf.nrows].sz = (size_t) sz;
		f_buf.nrows++;
	}
	/* Ignore errors on read-only file. */
	munmap (map, (size_t) st.st_size);
	close (fd);
	exit_status = true;

	/* Trim to maximum used. */
	f_buf.erow = safe_trim (f_buf.erow, f_buf.nrows * sizeof (*(f_buf.erow)));

  fatal:
  	close (fd);
  	return exit_status;
}

static ssize_t getline_file (FILE *stream)
{
	char *line = 0;
	size_t capacity = 0;
	ssize_t nread = 0;

	for (;;) {
		if (f_buf.alloc_sz <= f_buf.nrows) {
			f_buf.alloc_sz += BUFSIZ;		
			void *const tmp = realloc (0, f_buf.alloc_sz * sizeof (struct editor_row)); 
			
			if (!tmp) {
				goto fatal;
			}
			f_buf.erow = tmp;
		}
		
		nread = getline (&line, &capacity, stream);
		if (nread == -1) {
			if (feof (stream)) {
				/* Input ended due to end-of-file. */
				break;
			}

			if (errno == ENOMEM) {
				perror ("malloc()");
				free (line);
				goto fatal;
			}
		}

		if (line[nread - 1] == '\n') {
			line[--nread] = '\0';
		}
		
		f_buf.erow[f_buf.nrows].line = malloc ((size_t) nread + 1);

		if (!f_buf.erow[f_buf.nrows].line) {
			free (line);
			goto fatal;
		}
		memcpy (f_buf.erow[f_buf.nrows].line, line, (size_t) nread);
		f_buf.erow[f_buf.nrows].line[nread] = '\0';
		f_buf.erow[f_buf.nrows].sz = (size_t) nread;
		f_buf.nrows++;
	}
	
	/* Trim to maximum used, */
	free (line);
	f_buf.erow = safe_trim (f_buf.erow, f_buf.nrows * sizeof (*(f_buf.erow)));
	return 0;

  fatal:
  	return -1;
}
