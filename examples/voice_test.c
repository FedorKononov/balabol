/**
 * voice_test.c
 * libkaitalk
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <kaitalk/voice.h>
#include <kaitalk/gsocket.h>

int main(int argc, char **argv) {
	PaStream *stream;
	char *buffer;
	int buffer_len;

	int sockfd;
	char* response;
	char* tpl = "POST /speech-api/v1/recognize?xjerr=1&pfilter=1&client=chromium&lang=ru-RU HTTP/1.1\r\nHost: www.google.com\r\nUser-Agent: fedor\r\nConnection: keep-alive\r\nContent-Type: audio/x-speex-with-header-byte; rate=16000\r\nContent-Length: %d\r\n\r\n";

	if ((sockfd = kaitalk_socket_connect()) < 1) {
		printf("can't open socket connection\n");
	}

	stream = kaitalk_voice_create_stream();

	while (1) {
		buffer = NULL;
		buffer_len = 0;

		if ((buffer_len = kaitalk_voice_listen_stream(&stream, &buffer)) > 0) {
			printf("%d\n", buffer_len);

			char* query = (char *)malloc(strlen(tpl));
			sprintf(query, tpl, buffer_len);

			if (kaitalk_socket_send(sockfd, &query, strlen(query)) <= 0) {
				printf("kaitalk_socket_send - error\n");
			}

			if (kaitalk_socket_send(sockfd, &buffer, buffer_len) <= 0) {
				printf("kaitalk_socket_send - error\n");
			}

			response = (char *) malloc(2000 * sizeof(char));

			if (kaitalk_socket_read(sockfd, &response) <= 0) {
				printf("kaitalk_socket_send - error\n");
			}

			printf("%s\n", response);

			free(query);

			if (strstr(response, "огонь")) {
				break;
			}
		}
	}

	kaitalk_voice_close_stream(&stream);

	return 1;
}