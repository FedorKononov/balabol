/**
 * voice_test.c
 * libkaitalk
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <kaitalk/kaitalk.h>

// команда
struct command {
	char code[100];
	float confidence;
} command;

int main(int argc, char **argv) {
	PaStream *stream;
	char *buffer;
	int buffer_len;

	int sockfd;
	char *response;
	char *tpl = "POST /speech-api/v1/recognize?xjerr=1&pfilter=1&client=chromium&lang=ru-RU HTTP/1.1\r\nHost: www.google.com\r\nUser-Agent: fedor\r\nConnection: keep-alive\r\nContent-Type: audio/x-speex-with-header-byte; rate=16000\r\nContent-Length: %d\r\n\r\n";
	char *cmd_p_start, *cmd_p_end;

	struct command *cmd = NULL;

	char *sub_str_start = "utterance";
	char *sub_str_end = "confidence";

	if ((sockfd = kaitalk_socket_connect()) < 1) {
		printf("can't open socket connection\n");
	}

	stream = kaitalk_voice_create_stream();

	while (1) {
		buffer = NULL;
		buffer_len = 0;

		if ((buffer_len = kaitalk_voice_listen_stream(&stream, &buffer)) > 0) {
			printf("%d\n", buffer_len);

			char *query = (char *)malloc(strlen(tpl));
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

			// вытаскиваем подстроку с командой

			cmd_p_start = strstr(response, sub_str_start);
			if (cmd_p_start) {

				cmd_p_end = strstr(response, sub_str_end);
				if (cmd_p_end) {
					cmd = malloc(sizeof(command));

					memcpy(cmd->code, cmd_p_start + strlen(sub_str_start) + 3, (cmd_p_end - cmd_p_start - 6 - strlen(sub_str_start)));
					printf("cmd : '%s'  ", cmd->code);

					char tmp_confidence[4];
					memcpy(tmp_confidence, cmd_p_end + strlen(sub_str_end) + 2, 4);
					
					cmd->confidence = atof(tmp_confidence);
					printf("confidence : %f\n", cmd->confidence);

					kaitalk_vassal_run_action(cmd->code);

					free(cmd);
				}
			}
		}
	}

	kaitalk_voice_close_stream(&stream);

	return 1;
}