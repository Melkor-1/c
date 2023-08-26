#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned char u_char;
typedef unsigned int u_int;
typedef unsigned short u_short;

#include <pcap.h>

#include "hex_dump.h"

int main(void) 
{
	struct pcap_pkthdr header;
	const unsigned char *packet;
	char err_buf[PCAP_ERRBUF_SIZE];
	char *device;
	pcap_t *pcap_handle;

	device = pcap_lookupdev(err_buf);
	if (!device) {
		fprintf(stderr, "pcap_lookdevup: %s\n", err_buf);
		return EXIT_FAILURE;
	}
	printf("Sniffing on device %s...\n", device);

	pcap_handle = pcap_open_live(device, 4096, 1, 0, err_buf);
	if (!pcap_handle) {
		fprintf(stderr, "pcap_open_live: %s\n", err_buf);
		return EXIT_FAILURE;
	}

	while (true) {
		packet = pcap_next(pcap_handle, &header);
		printf("Got a %d byte packet..\n", header.len);
		dump(packet, header.len);
	}

	pcap_close(pcap_handle);
	return EXIT_SUCCESS;
}
