/**
 * encoder.h
 * 
 * Created by Kainet
 *
 * Function to encode data in speex format
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <speex/speex.h>

/**
 * Speex structure values
 */
struct encoder_speex {
	void *spex_state;
	SpeexBits spex_bits;
};

/**
 * Initialize speex encoder
 */
struct encoder_speex* kaitalk_encoder_speex_init();

/**
 * Encode input buffer
 */
int kaitalk_encoder_speex_encode(struct encoder_speex *speex, short** data_buffer, char** enc_buffer);