#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

// Node of a SLL of HTTP headers
struct header
{
	char *header_name;
	char *header_value;

	struct header *next;
};

// HTTP Request structure
struct request
{
	char *method;
	char *uri;
	char *http_version;

	//a SLL of header names and values
	struct header *header_head;

	char *body;
};

//parses the request to extract METHOD, URL, HTTP Version and Header names,values and body
struct request *parse_request(char *request_buffer);

//find the header value for a given header name
char *get_header_value(struct request *client_request, const char *header_name);

#endif