/* 	Dumps raw memory in hex byte and printable split format
*/

#include "hex_dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void dump(const unsigned char *buffer, ssize_t len) 
{
	unsigned char byte;
    size_t length = (size_t) len;

	for (size_t i = 0, j; i < length; i++) {
		byte = buffer[i];
		printf("%02x ", buffer[i]);	/* Displays in hex */
		if ((i % 16 == 15) || (i == length - 1)) {
			for ( j = 0; j < 15 - (i % 16); j++) {
				printf("	");
			}
			printf("|  ");
			for (j = (i - (i % 16)) ; j  <= i; j++) {	/* Display printable bytes */
				byte = buffer[j];
				if ((byte > 31) && (byte < 127)) {
					printf("%c", byte);
				} else {
					printf(".");
				}
			}
			printf("\n");	/* End of the dump line (each line is 16 bytes) */
		}	
	} 
}
