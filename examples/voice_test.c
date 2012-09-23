/**
 * voice_test.c
 * libkaitalk
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <kaitalk/voice.h>

int main(int argc, char **argv) {
	PaStream *stream;
	struct voice_buffer *buffer_head;

	stream = kaitalk_voice_create_stream();

	while (1) {
		buffer_head = NULL;
		printf("%d\n", kaitalk_voice_listen_stream(&stream, &buffer_head));
		break;
	}

	kaitalk_voice_close_stream(&stream);

	return 1;
}