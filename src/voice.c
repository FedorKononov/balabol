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
struct PaStream* kaitalk_voice_create_stream() {
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
		if (stream)
			kaitalk_voice_close_stream(&stream);
		else
			Pa_Terminate();
	}

	return stream;
}

/**
 * Listen for stream and return recorded voice command
 */
int kaitalk_voice_listen_stream(PaStream **stream, struct voice_buffer **buffer_head) {
	char *sampleBlock;
	int j, numBytes;
	double average;
	int silence_state = 0;
	char sampleItem;
	struct voice_buffer *buffer = NULL;
	
	numBytes = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE ;
	sampleBlock = (char *) malloc(numBytes);
	
	CLEAR(sampleBlock);

	while (1) {
		Pa_ReadStream(*stream, sampleBlock, FRAMES_PER_BUFFER);

		// calc average
		average = 0.0;
		for (j=0; j<FRAMES_PER_BUFFER*SAMPLE_SIZE; j++)	{
			sampleItem = sampleBlock[j];
			if (sampleItem < 0) sampleItem = -sampleItem; // abs()
			average += sampleItem;
		}
		
		// если поймана речь начинаем писать буфер и активируем счетчик тишины для прекрашения записи если в эфире будет тишина
		if (average > NOISE_THRESHOLD) {
			if (silence_state == 0)	{
				printf("record start\n");
				silence_state = 1;

				// инициализируем голову буфера
				*buffer_head = malloc(sizeof(struct voice_buffer));
				buffer = *buffer_head;
			} else
				silence_state = 2;
			
		}

		if (silence_state > 0) {
			if (silence_state > 1) {
				// инициаллизируем следующий блок буфера
				buffer->next = malloc(sizeof(struct voice_buffer));
				buffer = buffer->next;
			}

			printf("sample average = %lf\n", average);

			// пишем семпл в буфер
			buffer->data = (short *) malloc(numBytes);
			memcpy(buffer->data, sampleBlock, numBytes);
			buffer->next = 0;

			// тишину мы тоже пишим но увеличиваем счетчик
			if (average < NOISE_THRESHOLD)
				silence_state++;
		

			if (silence_state > QUIT_HOLDING_TIME) {
				printf("record stop\n");
				return 123;// buffer len
			}
		}
	}
}


/**
 * Closing portaudio stream
 */
int kaitalk_voice_close_stream(PaStream **stream) {
	Pa_AbortStream(*stream);
	Pa_CloseStream(*stream);
	Pa_Terminate();

	return 0;
}
