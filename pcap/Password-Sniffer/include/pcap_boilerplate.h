#ifndef PCAP_BOILERPLATE_H
#define PCAP_BOILERPLATE_H

#include <pcap.h>

// Determine the deivce to sniff on
char *choose_sniff_device();

// create and activate packet capture handle
pcap_t *create_activate_handle(char *device);

// Compile filter expression and set it on the capture handle
int compile_set_filter(pcap_t *handle, char *filter_exp);

#endif