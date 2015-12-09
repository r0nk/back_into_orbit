#include <SDL.h>
#include <err.h>
#include <math.h>

#include "audio.h"

int freq=143707;

static unsigned char audio_chunk[1000];
static long audio_len;
static char * audio_pos;

int audio_initalized;

void fill_audio(void *udata, Uint8 *stream, int len)
{
	if (audio_len<1)
		return;

	/* Mix as much data as possible */
	len = ( len > audio_len ? audio_len : len );
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

void play_audio(){
	int i;
	for(i=0;i<1000;i++){
		audio_chunk[i]=(unsigned char)(sin(i*(freq/10000.0))*50)+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = strlen((char * )audio_chunk);

	/* Let the callback function play the audio chunk */
	SDL_PauseAudio(0);
}

void init_audio()
{
	int error;
	SDL_AudioSpec wanted;
	
	wanted.freq = 22050;
	wanted.format = AUDIO_S8;
	wanted.channels = 1;
	wanted.samples = 1024;
	wanted.callback = fill_audio;
	wanted.userdata = NULL;

	error = SDL_OpenAudio(&wanted,NULL);
	if(error<0)
		err(-53,"Couldn't initalize audio: %s",SDL_GetError());
	audio_initalized=1;
}
