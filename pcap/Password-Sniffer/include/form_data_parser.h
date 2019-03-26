#ifndef FORM_DATA_PARSER_H
#define FORM_DATA_PARSER_H

// Node in SLL of url encoded form data
struct field
{
	char *name;
	char *value;

	struct field *next;
};

// Parse the query string as name,value pairs
struct field *parse_query_string(char *query_string);

void extract_uname_passwd(char *http_payload);

#endif