/**
 * test.c
 * libkaitalk
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <kaitalk/gsocket.h>

int main(int argc, char **argv) {
	int sockfd;
	char *read_buf;

	if ((sockfd = kaitalk_socket_connect()) < 1) {
		printf("can't open socket connection\n");
	}

	char* tmp = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";

	if (kaitalk_socket_send(sockfd, &tmp, strlen(tmp)) <= 0) {
		printf("kaitalk_socket_send - error\n");
	}


	read_buf = (char *) malloc(2000 * sizeof(char));

	if (kaitalk_socket_read(sockfd, &read_buf) <= 0) {
		printf("kaitalk_socket_send - error\n");
	}

	printf("%s\n", read_buf);

	return 1;
}
