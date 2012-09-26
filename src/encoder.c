/**
 * encoder.h
 * 
 * Created by Kainet
 *
 * Function to encode data in speex format
 */

#define SPEEX_FRAME_LEN 110

#include <kaitalk/encoder.h>

/**
 * Initialize speex encoder
 */
struct encoder_speex* kaitalk_encoder_speex_init() {
	int qualty, vbr;
	struct encoder_speex *speex = NULL;

	void *spex_state;
	SpeexBits spex_bits;

	// speex_bits_init() does not initialize all of the |bits| struct.
	memset(&spex_bits, 0, sizeof(spex_bits));

	// Initialization of the structure that holds the bits
	speex_bits_init(&spex_bits);

	//Create a new encoder state in widewband mode
	spex_state = speex_encoder_init(&speex_wb_mode);

	//Set the quality to 8 (15 kbps)
	qualty = 8;
	speex_encoder_ctl(spex_state, SPEEX_SET_QUALITY, &qualty);

	vbr = 1;
	speex_encoder_ctl(spex_state, SPEEX_SET_VBR, &vbr);

	speex = malloc(sizeof(struct encoder_speex));

	speex->spex_state = spex_state;
	speex->spex_bits = spex_bits;

	return speex;
}

/**
 * Encode input buffer
 */
int kaitalk_encoder_speex_encode(struct encoder_speex *speex, short** data_buffer, char** enc_buffer) {
	int nbBytes;

	*enc_buffer = malloc(SPEEX_FRAME_LEN * sizeof(char) + 1);

	/*Flush all the bits in the struct so we can encode a new frame*/
	speex_bits_reset(&(speex->spex_bits));

	/*Encode the frame*/
	speex_encode_int(speex->spex_state, *data_buffer, &(speex->spex_bits));

	/*Copy the bits to an array of char that can be written*/
	nbBytes = speex_bits_write(&(speex->spex_bits), *enc_buffer, SPEEX_FRAME_LEN);

	return nbBytes;
}



