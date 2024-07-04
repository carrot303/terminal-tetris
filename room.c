#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#include "room.h"

#define CLIENT_COUNT 2
#define IPV4_LENGTH 15


extern int socket_fd;
extern int client_fd;

void create_room(int port) {
	int opt = 1;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[-] ERROR: failed to create socket");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("[-] ERROR: failed in setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0 ){
		perror("[-] ERROR: failed to bind");
		exit(EXIT_FAILURE);
	}
	if (listen(socket_fd, CLIENT_COUNT) < 0) {
		perror("[-] ERROR: failed to listen");
		exit(EXIT_FAILURE);
	}
	printf("[+] Established at 0.0.0.0:%d\n", port);
	printf("[+] Wait for other player...\n");
	if ((client_fd = accept(socket_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("[-] ERROR: failed to accept connection");
		exit(EXIT_FAILURE);
	}
	printf("[+] player joined\n");
}

void join_room(char* room_ip, int port) {
	struct sockaddr_in serv_addr;
	char resolved_domain[IPV4_LENGTH];

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[-] ERROR: failed to create socket");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (resolve_domain_name(room_ip, resolved_domain) == -1) {
		printf("[-] ERROR: could not resolve domain '%s'\n", room_ip);
		exit(-1);
	}

	if (inet_pton(AF_INET, resolved_domain, &serv_addr.sin_addr) < 0) {
		perror("[-] ERROR: invalid ip given");
		exit(EXIT_FAILURE);
	}

	if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("[-] ERROR: failed to join room");
		exit(EXIT_FAILURE);
	}
}

int resolve_domain_name(char* domain_name, char addr[]) {
	struct hostent *host_entry;
	struct in_addr **addr_list;

	if ((host_entry = gethostbyname(domain_name)) == NULL)
		return -1;

	addr_list = (struct in_addr **)host_entry->h_addr_list;
	if (addr_list[0] != NULL) {
		strcpy(addr, inet_ntoa(*addr_list[0]));
		return 0;
	}
	return -1;
}