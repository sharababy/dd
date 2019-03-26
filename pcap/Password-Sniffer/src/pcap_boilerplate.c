#include "pcap_boilerplate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcap.h>

// Determine the deivce to sniff on
char *choose_sniff_device()
{
	int i;

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *interfaces, *temp;

	if(pcap_findalldevs(&interfaces, errbuf) == -1)
	{
		fprintf(stderr, "ERROR: pcap failed to find all devices. %s.\n", errbuf);
		return NULL;
	}

	int device_index, choosen_device_index;

	for(temp=interfaces, device_index=0; temp; temp=temp->next)
		printf("%d. %s\n", device_index++, temp->name);

	printf("Select device to sniff on [0-%d]: ", device_index-1);
	
	scanf("%d", &choosen_device_index);
	while(! ((choosen_device_index >= 0) && (choosen_device_index < device_index)) )
	{
		printf("Invalid selection: %d. Select device to sniff on [0-%d]: ", choosen_device_index, device_index-1);
		scanf("%d", &choosen_device_index);
	}

	for(temp=interfaces, i=0; i<choosen_device_index; ++i, temp=temp->next);

	char *device = (char *)malloc(sizeof(char)*(strlen(temp->name)+1));
	strcpy(device, temp->name);

	pcap_freealldevs(interfaces);

	return device;
}

// create and activate packet capture handle handle
pcap_t *create_activate_handle(char *device)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Opening the device for sniffing */
	pcap_t *handle = pcap_create(device, errbuf);
	if(handle == NULL)
	{
		fprintf(stderr, "ERROR: Failed to open device '%s': %s.\n", device, errbuf);
		return NULL;
	}

	pcap_set_snaplen(handle, BUFSIZ);
	pcap_set_promisc(handle, 1);
	pcap_set_timeout(handle, 1000);

	int can_set_rfmon = pcap_can_set_rfmon(handle);
	char set_rfmon_option;

	if(can_set_rfmon == 1)
	{
		printf("Device '%s' supports monitor mode. Enable monitor mode [y/n]: ", device);
		// fflush(stdout);
		getchar();
		set_rfmon_option = getchar();

		if(set_rfmon_option == 'y')
		{
			pcap_set_rfmon(handle, 1);
			fprintf(stderr, "INFO: Monitor mode was set on device '%s'.\n", device);
		}	
	}

	else if(can_set_rfmon == 0)
		fprintf(stderr, "INFO: Monitor mode not supported by device '%s'.\n", device);

	else
	{
		fprintf(stderr, "ERROR: Failed to set monitor mode on '%s': %s\n", device, pcap_geterr(handle));
		return NULL;
	}

	int handle_active_status = pcap_activate(handle);

	if(handle_active_status > 0)
		fprintf(stderr, "WARNING: Activated capture handle on device '%s': %s.\n", device, pcap_geterr(handle));

	else if(handle_active_status < 0)
	{
		fprintf(stderr, "ERROR: Failed to activate capture handle on device '%s': %s.\n", device, pcap_geterr(handle));
		return NULL;
	}

	return handle;
}

int compile_set_filter(pcap_t *handle, char *filter_exp)
{
	// Compiling and setting the filter on handle
	struct bpf_program fp;

	if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
	{
		fprintf(stderr, "ERROR: Failed to parse filter '%s': %s\n", filter_exp, pcap_geterr(handle));
		return -1;
	}

	if(pcap_setfilter(handle, &fp) == -1)
	{
		fprintf(stderr, "ERROR: Failed to install filter '%s': %s\n", filter_exp, pcap_geterr(handle));
		return -1;
	}

	return 0;
}