/**
 * voice.h
 * 
 * Created by Kainet
 *
 * Working with voice using portaudio lib
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <portaudio.h>

/**
 * Voice buffer
 */
struct voice_buffer {
	short *data;
	struct voice_buffer *next;
};

/**
 * Creating portaudio stream to read input data
 */
struct PaStream* kaitalk_voice_create_stream();

/**
 * Closing portaudio stream
 */
int kaitalk_voice_close_stream(PaStream **stream);

/**
 * Listen for stream and return recorded voice command
 */
int kaitalk_voice_listen_stream(PaStream **stream,  char **ret_buffer);
