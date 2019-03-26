#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <pcap.h>

#define MAX_FILTER_EXP_LEN 256

void packetRecieved(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main(int argc, char *argv[])
{
	/* Identify the link-layer header type provided by device */
	int datalink_layer = pcap_datalink(handle);
	char filter_exp[MAX_FILTER_EXP_LEN] = "";

	if(datalink_layer == DLT_EN10MB)
		strncpy(filter_exp, "(tcp dst port 80 or tcp dst port 8080 or tcp dst port 1000) and (tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354)", MAX_FILTER_EXP_LEN);

	else if(datalink_layer == DLT_IEEE802_11_RADIO)
	{
		// strncpy(filter_exp, "radio[radio[2:2] + 0x1E + ((radio[radio[2:2]+0x1E: 1] & 0x0f) << 2)]", MAX_FILTER_EXP_LEN);
		strncpy(filter_exp, "", MAX_FILTER_EXP_LEN);
	}

	else
	{
		fprintf(stderr, "ERROR: Device '%s' is not supported as it doesn't provide supported headers.\n", device);
		return 2;
	}

	/* Grabbing packets */
	pcap_loop(handle, -1, packetRecieved, (u_char *)(long)datalink_layer);

	/* Close the session */
	pcap_close(handle);

	return 0;
}

void packetRecieved(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	int datalink_layer = (int)(long)args;

	if(datalink_layer == DLT_IEEE802_11_RADIO)
	{
		const struct ieee80211_radiotap_header *radiotap;
		radiotap = (struct ieee80211_radiotap_header *)packet;

		packet += RADIO_OFF(radiotap);

		const struct ieee80211_hdr *wlan;
		wlan = (struct ieee80211_hdr *)packet;

		packet += SIZE_WLAN;
	}

	else if(datalink_layer == DLT_EN10MB)
	{
		const struct sniff_ethernet *ethernet; // Ethernet header
		ethernet = (struct sniff_ethernet *)packet;

		packet += SIZE_ETHERNET;
	}

	
	const struct sniff_ip *ip; // IP header
	const struct sniff_tcp *tcp; // TCP header
	char *payload; // Packet payload

	u_int size_ip;
	u_int size_tcp;

	// Typecasting the packet data into headers and payload
	
	ip = (struct sniff_ip *)packet;
	size_ip = IP_HL(ip)*4;
	if(size_ip < 20)
	{
		printf("Invalid IP header length: %u bytes.\n", size_ip);
		return;
	}

	tcp = (struct sniff_tcp *)(packet + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if(size_tcp < 20)
	{
		printf("Invalid TCP header length: %u bytes.\n", size_tcp);
		return;
	}

	payload = (u_char *)(packet + size_ip + size_tcp);
	printf("%s\n", payload);

	// extractUnamePasswd(payload);

	return;
}