/**
 * voice.c
 * 
 * Created by Kainet
 *
 * Working with voice using portaudio lib
 */

#define SAMPLE_RATE 16000
#define FRAMES_PER_BUFFER 320
#define NUM_CHANNELS 1
#define NOISE_THRESHOLD 21000
#define QUIT_HOLDING_TIME 40

#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SIZE 2
#define CLEAR(a) memset((a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE)

#include <kaitalk/voice.h>

/**
 * Creating portaudio stream to read input data
 */
struct PaStream* kaitak_voice_create_stream() {
	PaStreamParameters inputParameters;
	PaStream *stream = NULL;
	PaError err;

	if ((err = Pa_Initialize()) != paNoError)
		Pa_Terminate();

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff, /* we won't output out of range samples so don't bother clipping them */
		NULL, /* no callback, use blocking API */
		NULL /* no callback, so no callback userData */
	); 

	if (err != paNoError)
		Pa_Terminate();

	if ((err = Pa_StartStream(stream)) != paNoError) {
		if (stream)	{
			Pa_AbortStream(stream);
			Pa_CloseStream(stream);
		}

		Pa_Terminate();
	}

	return stream;
}


