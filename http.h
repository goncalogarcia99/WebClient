#ifndef HTTP_H
#define HTTP_H

#include <stdlib.h> // For size_t.

#define HTTP_PORT_NUMBER "80"
#define HTTP_VERSION "1.1"
#define HTTP_MOVED_PERMANENTLY_STATUS_CODE 301
#define HTTP_FOUND_STATUS_CODE 302
#define HTTP_NEW_LOCATION_TARGET "Location: http://"

#define HTTP_REQUEST_MESSAGE_SIZE 512
#define HTTP_RESPONSE_MESSAGE_SIZE 512000 // 500 MB

typedef struct Http_request_message {
	const char *method;
	const char *path;
	const char *version;
	const char *host;
} Http_request_message;

int http_request_and_response(int socket_file_descriptor, Http_request_message *http_request_message, char *response_message, size_t response_message_size);
int http_get_status_code(const char *http_response_message);
int http_get_new_url(const char *http_response_message, char *new_url, size_t new_url_size);

#endif
