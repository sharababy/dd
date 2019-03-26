#ifndef PCAP_HANDLER_H
#define PCAP_HANDLER_H

#include <stdlib.h>
#include <pcap.h>

// Pcap handler call back function for frames with DLT_EN10MB link-layer headers
void ether_frame_received(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

#endif