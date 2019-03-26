// Define _GNU_SOURCE to support non-standard extensions
#define _GNU_SOURCE

/* Decoding URL encoded query strings
Reference: https://www.rosettacode.org/wiki/URL_decoding#C */
#include "form_data_parser.h"
#include "http_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int ishex(int x)
{
	return	(x >= '0' && x <= '9')	||
		(x >= 'a' && x <= 'f')	||
		(x >= 'A' && x <= 'F');
}

char *url_decode(const char *url_encoded_str)
{
	char *url_decoded_str = (char *)malloc(sizeof(char)*(strlen(url_encoded_str)+1));

	char *o;
	const char *end = url_encoded_str + strlen(url_encoded_str);
	int c;
 
	for (o = url_decoded_str; url_encoded_str <= end; o++) {
		c = *url_encoded_str++;
		if (c == '+') c = ' ';
		else if (c == '%' && (!ishex(*url_encoded_str++) ||	!ishex(*url_encoded_str++) || !sscanf(url_encoded_str - 2, "%2x", &c)))
			return NULL;
 
		if (url_decoded_str) *o = c;
	}
 
	return url_decoded_str;
}

// Parse the query string as name,value pairs
struct field *parse_query_string(char *query_string)
{
	char *query_string_cpy = (char *)malloc(sizeof(char)*(strlen(query_string)+1));
	strcpy(query_string_cpy, query_string);

	struct field *head = NULL, *last_field = NULL;
	char *field_name_value, *field_name_val_delim, *field_delimiter;

	do
	{
		struct field *new_field = (struct field *)malloc(sizeof(struct field));
		new_field -> next = NULL;

		if(head == NULL)
			head = new_field;

		else
			last_field -> next = new_field;

		last_field = new_field;

		field_name_value = query_string_cpy;

		field_delimiter = strchr(query_string_cpy, '&');
		if(field_delimiter != NULL)
		{
			*field_delimiter = '\0';
			query_string_cpy = field_delimiter + 1;
		}

		field_name_val_delim = strchr(field_name_value, '=');
		*field_name_val_delim = '\0';

		new_field -> name = url_decode(field_name_value);
		new_field -> value = url_decode(field_name_val_delim + 1);

	} while(field_delimiter != NULL);

	return head;
}

// Extract potential username and password fields from the url encoded http body
#define MAX_UNAME_PASSWD_SUBSTR_LEN 64
char USERNAME_SUBSTR[][MAX_UNAME_PASSWD_SUBSTR_LEN] = {"name", "user", "usr"};
char PASSWORD_SUBSTR[][MAX_UNAME_PASSWD_SUBSTR_LEN] = {"pass", "pwd"};

// Extract potential username and password fields from the url encoded http body
void extract_uname_passwd(char *http_payload)
{
	// Parsing the HTTP requests and filtering packets with url encoded data
	struct request *client_request = parse_request(http_payload);
	char *content_type = get_header_value(client_request, "Content-Type");

	if(strcmp(content_type, "application/x-www-form-urlencoded") == 0)
	{
		struct field *form_data = parse_query_string(client_request -> body);
		struct field *temp_field;

		printf("\n************************************************************\n");

		char *referer, *host;
		if((referer = get_header_value(client_request, "Referer")) != NULL)
			printf("Referer: %s\n", referer);

		if((host =  get_header_value(client_request, "Host")) != NULL)
			printf("Host URL: %s%s\n\n", host, client_request -> uri);

		// printf("Data: %s\n", client_request -> body);
		
		int n_username_substr, n_password_substr;
		n_username_substr = sizeof(USERNAME_SUBSTR)/(sizeof(char)*MAX_UNAME_PASSWD_SUBSTR_LEN);
		n_password_substr = sizeof(PASSWORD_SUBSTR)/(sizeof(char)*MAX_UNAME_PASSWD_SUBSTR_LEN);

		bool uname_fields_exist, passwd_fields_exist;
		uname_fields_exist = passwd_fields_exist = false;

		int i;

		printf("POTENTIAL USERNAME FIELDS\n");
		for(temp_field = form_data; temp_field != NULL; temp_field = temp_field -> next)
		{
			for(i=0; i<n_username_substr; ++i)
				if(strcasestr(temp_field->name, USERNAME_SUBSTR[i]) != NULL)
					break;

			if(i<n_username_substr)
			{
				printf("'%s' : '%s'\n", temp_field->name, temp_field->value);
				uname_fields_exist = true;
			}
		}

		if(uname_fields_exist == false)
			printf("No potential username fields could be found !\n");


		printf("\nPOTENTIAL PASSWORD FIELDS\n");
		for(temp_field = form_data; temp_field != NULL; temp_field = temp_field -> next)
		{
			for(i=0; i<n_password_substr; ++i)
				if(strcasestr(temp_field->name, PASSWORD_SUBSTR[i]) != NULL)
					break;

			if(i<n_password_substr)
			{
				printf("'%s' : '%s'\n", temp_field->name, temp_field->value);
				passwd_fields_exist = true;
			}
		}

		if(passwd_fields_exist == false)
			printf("No potential password fields could be found !\n");


		printf("************************************************************\n");
	}
}