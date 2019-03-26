#include "pcap_handler.h"
#include "protocol_headers.h"
#include "form_data_parser.h"

// Pcap handler call back function for frames with DLT_EN10MB link-layer headers
void ether_frame_received(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	// const struct sniff_ethernet *ethernet; // Ethernet header
	const struct sniff_ip *ip; // IP header
	const struct sniff_tcp *tcp; // TCP header
	char *payload; // Packet payload

	u_int size_ip;
	u_int size_tcp;

	// Typecasting the packet data into headers and payload
	// ethernet = (struct sniff_ethernet *)packet;

	ip = (struct sniff_ip *)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if(size_ip < 20)
	{
		fprintf(stderr, "Invalid IP header length: %u bytes.\n", size_ip);
		return;
	}

	tcp = (struct sniff_tcp *)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if(size_tcp < 20)
	{
		fprintf(stderr, "Invalid TCP header length: %u bytes.\n", size_tcp);
		return;
	}

	payload = (char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

	extract_uname_passwd(payload);
}