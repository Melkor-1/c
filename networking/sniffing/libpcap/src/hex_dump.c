/* 	Dumps raw memory in hex byte and printable split format
*/

#include <stdio.h>
#include <stdlib.h>

void dump(const unsigned char *buffer, const size_t len) 
{
	unsigned char byte;

	for (size_t i = 0, j; i < len; i++) {
		byte = buffer[i];
		printf("%02x ", buffer[i]);	/* Displays in hex */
		if ((i % 16 == 15) || (i == len - 1)) {
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
