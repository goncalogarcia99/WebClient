#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "http.h"

#define URL_HOST_SIZE 256
#define URL_PATH_SIZE 512

int get_url_host(const char *url, char *host, size_t host_size);
int get_url_path(const char *url, char *path, size_t path_size);

int main(int argc, char **argv) {
	if (2 != argc) {
		fprintf(stderr, "\nUsage: %s URL\n", argv[0]);
		return -1;
	}

	char url_host[URL_HOST_SIZE], url_path[URL_PATH_SIZE];
	if (get_url_host(argv[1], url_host, URL_HOST_SIZE) || get_url_path(argv[1], url_path, URL_PATH_SIZE)) {
		fprintf(stderr, "\nInvalid URL.\n");
		return -1;
	}

	/*
	The following structure defines an address:
	struct addrinfo {
		int ai_flags;
		int ai_family;
		int ai_socktype;
		int ai_protocol;
		socklen_t ai_addrlen;
		struct sockaddr * ai_addr;
		char *ai_canonname;
		struct addrinfo *ai_next;
	};
	*/
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints); // Reset hints.
	hints.ai_family = AF_UNSPEC; // AF_UNSPEC: internet addresses are used (data will be shared with a computer, or more, on the internet), IPv4 and IPv6.
	hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM: data is read in a continuous stream. SOCK_DGRAM: data is read in chunks.
	hints.ai_protocol = 0; // No protocol specified, the most appropriate one is chosen - TCP for stream sockets and UDP for datagram sockets.
	struct addrinfo *addr_info;
	if (getaddrinfo(url_host, HTTP_PORT_NUMBER, &hints, &addr_info)) { // int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res).
		fprintf(stderr, "\nError: couldn't get host's info (getaddrinfo()).\n");
		return -1;
	}

	struct addrinfo *i;
	int socket_file_descriptor;
	char host_name[URL_HOST_SIZE];
	// The following loop is responsible for trying each address (found by getaddrinfo()) in addr_info, until a connection is successfully established.
	for (i = addr_info; NULL != i; i = i->ai_next) {
		socket_file_descriptor = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
		if (-1 == socket_file_descriptor) // Couldn't create a socket.
			continue;
		// getnameinfo() is responsible for determining the host's name, which is required for the HTTP request message. This is necessary when the URL
		// contains the host's address, not its name.
		if (getnameinfo(i->ai_addr, i->ai_addrlen, host_name, sizeof host_name, NULL, 0, 0)) {
			fprintf(stderr, "\nError: couldn't get host's info (getnameinfo()).\n");
			close(socket_file_descriptor);
			freeaddrinfo(addr_info);
			return -1;
		}
		if (!connect(socket_file_descriptor, i->ai_addr, i->ai_addrlen)) // Connection successful.
			break;
		close(socket_file_descriptor);
	}
	freeaddrinfo(addr_info);
	if (NULL == i) { // No address succeeded.
		fprintf(stderr, "\nError: couldn't connect.\n");
		return -1;
	}

	Http_request_message http_request_message = {
		.method = "GET",
		.path = url_path,
		.version = HTTP_VERSION,
		.host = host_name}; // Not the host's address.
	char response_message[HTTP_RESPONSE_MESSAGE_SIZE];
	if (http_request_and_response(socket_file_descriptor, &http_request_message, response_message, HTTP_RESPONSE_MESSAGE_SIZE)) {
		fprintf(stderr, "\nError: couldn't write or read.\n");
		close(socket_file_descriptor);
		return -1;
	}

	int status_code = http_get_status_code(response_message);
	if (HTTP_MOVED_PERMANENTLY_STATUS_CODE == status_code || HTTP_FOUND_STATUS_CODE == status_code) { // Redirect.
		char new_url[URL_HOST_SIZE + URL_PATH_SIZE]; // Includes the beginning and terminating slashes.
		if (http_get_new_url(response_message, new_url, URL_HOST_SIZE + URL_PATH_SIZE)) {
			fprintf(stderr, "\nError: couldn't get new location (probably HTTPS, not HTTP).\n");
			close(socket_file_descriptor);
			return -1;
		}
		char new_path[URL_PATH_SIZE];
		if (get_url_path(new_url, new_path, URL_PATH_SIZE)) {
			fprintf(stderr, "\nError: couldn't get new path.\n");
			close(socket_file_descriptor);
			return -1;
		}
		http_request_message.path = new_path;
		if (http_request_and_response(socket_file_descriptor, &http_request_message, response_message, HTTP_RESPONSE_MESSAGE_SIZE)) {
			fprintf(stderr, "\nError: couldn't write or read.\n");
			close(socket_file_descriptor);
			return -1;
		}
		fprintf(stderr, "%s\n\n", "***** REDIRECTING *****");
		fprintf(stderr, "\n%s\n", response_message);
	}

	close(socket_file_descriptor);

	return 0;
}

int get_url_host(const char *url, char *host, size_t host_size) {
	char *aux = strchr(url, '/'); // aux points to the slash after the host, in the URL.
	if (NULL == aux) // The slash wasn't found.
		return -1;
	size_t host_length = aux - url;
	if (host_length >= host_size)
			return -1;
	memcpy(host, url, host_length);
	host[host_length + 1] = '\0';
	return 0;
}

int get_url_path(const char *url, char *path, size_t path_size) {
	char *aux = strchr(url, '/'); // aux points to the slash after the host, in the URL.
	if (NULL == aux) // The slash wasn't found.
		return -1;
	size_t path_length = strlen(aux);
	if (path_length >= path_size)
		return -1;
	memcpy(path, aux, path_length);
	path[path_length + 1] = '\0';
	return 0;
}
