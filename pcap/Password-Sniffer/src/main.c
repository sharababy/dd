#include "pcap_boilerplate.h"
#include "pcap_handler.h"
#include <pcap.h>


int main(int argc, char *argv[])
{
	// Determind the device to sniff on
	char *device = choose_sniff_device();
	if(device == NULL) return 2;

	// Create and activate packet capture handle on device
	pcap_t *handle = create_activate_handle(device);
	if(handle == NULL) return 2;

	int datalink_header = pcap_datalink(handle);
	
	// IEEE 802.3 Ethernet header
	if(datalink_header == DLT_EN10MB)
	{
		// Filter out "TCP packets" to ports "80, 8080 or 1000" with POST Message
		char filter_exp[] = "(tcp dst port 80 or tcp dst port 8080 or tcp dst port 1000) "
			"and (tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354)";
		if(compile_set_filter(handle, filter_exp) == -1) return 2;

		pcap_loop(handle, -1, ether_frame_received, NULL);
	}

	// Radiotap link-layer info + 802.11 WLAN header
	else if(datalink_header == DLT_IEEE802_11_RADIO)
	{

	}

	else
	{
		fprintf(stderr, "ERROR: Device '%s' is not supported as it doesn't provide supported link-layer header.\n", device);
		return 2;
	}

	// Close the packet capture session
	pcap_close(handle);

	return 0;
}