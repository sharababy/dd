#include "http_parser.h"
#include <stdlib.h>
#include <string.h>

//parses the request to extract METHOD, URL, HTTP Version and Header names,values and body
struct request *parse_request(char *request_buffer)
{
	struct request *client_request = (struct request *)malloc(sizeof(struct request));
	client_request -> header_head = NULL;
	client_request -> body = NULL;

	struct header *new_header;
	struct header *prev_header;

	//Ignore '\r\n' (empty lines) before request line
	while((*request_buffer == '\r') || (*request_buffer == '\n'))
		request_buffer++;

	//Parsing the request METHOD
	client_request -> method = request_buffer;

	while((*request_buffer != ' ') && (*request_buffer != '\t'))
		request_buffer ++;

	while((*request_buffer == ' ') || (*request_buffer == '\t'))
		*(request_buffer ++) = '\0';

	//Parsing the request URI
	client_request -> uri = request_buffer;

	while((*request_buffer != ' ') && (*request_buffer != '\t'))
		request_buffer ++;

	while((*request_buffer == ' ') || (*request_buffer == '\t'))
		*(request_buffer ++) = '\0';

	// Parsing the http version
	request_buffer += 5;
	client_request -> http_version = request_buffer;

	while((*request_buffer != '\r') && (*(request_buffer+1) != '\n'))
		request_buffer ++;

	if(*request_buffer == '\r')
		*(request_buffer ++) = '\0';

	if(*request_buffer == '\n')
		*(request_buffer ++) = '\0';

	//Note RFC : HTTP Header names are case-insensitive
	while((*request_buffer != '\r') || (*(request_buffer+1) != '\n'))
	{
		new_header = (struct header *)malloc(sizeof(struct header));
		new_header -> next = NULL;

		new_header -> header_name = request_buffer;

		while(*request_buffer != ':')
			request_buffer ++;

		while((*request_buffer == ':') || (*request_buffer == ' ') || (*request_buffer == '\t') || (*request_buffer == '\r') || (*request_buffer == '\n'))
			*(request_buffer ++) = '\0';

		new_header -> header_value = request_buffer;

		while((*request_buffer != '\r') || (*(request_buffer+1) != '\n'))
			request_buffer ++;

		*(request_buffer ++) = '\0';
		*(request_buffer ++) = '\0';		

		//if it's the first header
		if(client_request -> header_head == NULL)
		{
			client_request -> header_head = new_header;
			prev_header = new_header;
		}

		else
		{
			prev_header -> next = new_header;
			prev_header = new_header;
		}
	}

	request_buffer += 2;
	client_request -> body = request_buffer;	

	return client_request;
}

//find the header value for a given header name
char *get_header_value(struct request *client_request, const char *header_name)
{
	struct header *cur_header = client_request -> header_head;

	while(cur_header != NULL)
	{
		if(strcasecmp(cur_header -> header_name, header_name) == 0 )
			break;

		cur_header = cur_header->next;
	}

	//if header not found
	if(cur_header == NULL)
		return NULL;

	else
		return cur_header -> header_value;
}