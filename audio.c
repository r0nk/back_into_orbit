#include <SDL.h>
#include <err.h>
#include <math.h>

#include "audio.h"

int freq=143707;

#define AUDIO_BUFF_SIZE 10000

static unsigned char audio_chunk[AUDIO_BUFF_SIZE];
static long audio_len;
static char * audio_pos;

int audio_initalized;
int audio_enabled;

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

void tzztzzz(){
	int i;
	for(i=0;i<1000;i++){
		audio_chunk[i]=(unsigned char)
			(sin(i*(freq/1000.0))*80)+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = 100;

	if(audio_enabled)
	SDL_PauseAudio(0);
}

void pew(){
	int i;
	int r = 1+rand()%4;
	for(i=0;i<AUDIO_BUFF_SIZE;i++){
		audio_chunk[i]=(unsigned char)
			(sin(i*(freq/(1+(r*(i+100.0)))))*5)+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = AUDIO_BUFF_SIZE;

	if(audio_enabled)
	SDL_PauseAudio(0);
}

void teledice_sound(int a, int b)
{
	int i;
	int length = AUDIO_BUFF_SIZE/3;
	for(i=0;i<length;i++){
		audio_chunk[i]=(unsigned char)
			(sin(i*(freq/(a*1000.0)))*50)+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = length;
	if(audio_enabled)
	SDL_PauseAudio(0);
}
void bzewerwwww(int to, int from)
{
	int i;
	int length = AUDIO_BUFF_SIZE/2;
	for(i=0;i<length/2;i++){
		audio_chunk[i]=(unsigned char)
			(sin(i*(freq/(from+1)))*20)+1;
	}
	for(;i<length;i++){
		audio_chunk[i]=(unsigned char)
			(sin(i*(freq/(to+1)))*20)+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = length;

	if(audio_enabled)
	SDL_PauseAudio(0);
}

void deathplosion()
{
	int i;
	for(i=0;i<AUDIO_BUFF_SIZE;i++){
		audio_chunk[i]=(unsigned char)
			((sin(i*(freq/(1922+(rand()%5000))))*20))+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = AUDIO_BUFF_SIZE- 3000;

	if(audio_enabled)
	SDL_PauseAudio(0);
}

/* TODO this POS doesn't even remotely do what I want it to */
void sin_sound(double f)
{
	int i;
	int length = AUDIO_BUFF_SIZE;

	for(i=0;i<length;i++){
		audio_chunk[i]=(unsigned char)
			(sin(i*(freq/(f+1)))*20)+1;
	}
	audio_pos = (char *)audio_chunk;
	audio_len = length;

	if(audio_enabled)
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
	audio_enabled=0;
}
