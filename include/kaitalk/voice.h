/**
 * voice.c
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
 * Creating portaudio stream to read input data
 */
struct PaStream* kaitak_voice_create_stream();