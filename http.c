#include "http.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int http_send_request_message(int socket_file_descriptor, Http_request_message *http_request_message) {
	char message[HTTP_REQUEST_MESSAGE_SIZE];
	snprintf(message, sizeof message, "%s %s HTTP/%s\r\nHost: %s\r\n\r\n",
				http_request_message->method,
				http_request_message->path,
				http_request_message->version,
				http_request_message->host);
	size_t message_length = strlen(message);
	if (message_length != write(socket_file_descriptor, message, message_length))
		return -1;
	return 0;
}

int http_receive_response_message(int socket_file_descriptor, char *response_message, size_t response_message_size) {
	size_t bytes_read = read(socket_file_descriptor, response_message, response_message_size - 1);
	if (-1 == bytes_read)
		return -1;
	response_message[bytes_read] = '\0';
	return 0;
}

int http_get_status_code(const char *http_response_message) {
	char status_code[4]; // Three digits and a null character.
	memcpy(status_code, &http_response_message[9], 3); // The three digit status code starts at the nineth position in http_response_message.
	status_code[4] = '\0';
	return atoi(status_code);
}

int http_get_new_url(const char *http_response_message, char *new_url, size_t new_url_size) {
	// new_location points to the beginning of the target string inside http_response_message.
	char *new_location = strstr(http_response_message, HTTP_NEW_LOCATION_TARGET);
	if (NULL == new_location) // The target string wasn't found.
		return -1;
	new_location += strlen(HTTP_NEW_LOCATION_TARGET); // new_location is positioned immediately after the target string inside http_response_message.
	char *aux = strchr(new_location, '\n'); // aux points to the new line character after the URL.
	if (NULL == aux) // The new line character wasn't found.
		return -1;
	size_t new_url_length = aux - new_location - 1 /* '\n' */;
	if (new_url_length >= new_url_size)
		return -1;
	memcpy(new_url, new_location, new_url_length);
	new_url[new_url_length + 1] = '\0';
	return 0;
}
