#include "SDL_Wrapper/SDL_Audio.hpp"
#include "Debug.hpp"
#include <iostream>
using namespace std;

struct AudioEffect::AudioEffectData
{
	unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> audio_effect;
	int loops;
	int channel;
};

constexpr int NOT_ALLOCATED = -1;
constexpr int GET_CHANNELS = -1;
constexpr int FIRST_FREE_CHANNEL = -1;
constexpr int UNALLOCATE_ALL_CHANNELS = 0;

/*
AudioEffect::AudioEffect()
	: _data(nullptr)	{}
*/

AudioEffect::AudioEffect(const char * file_name, int loops)
{
	//Attempt to open file.
	Mix_Chunk * audio_effect = Mix_LoadWAV(file_name);

	//If file could not be opened.
	if(audio_effect == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not open audio file: " << Mix_GetError());
		exit(1);
	}

	//Initialize AudioEffectData _data.
	_data = unique_ptr<AudioEffectData>{ new AudioEffectData{
											unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>{ move(audio_effect), &Mix_FreeChunk }
										 }
									   };
	_data->loops = move(loops);
	_data->channel = NOT_ALLOCATED;
}

void AudioEffect::Play(void)
{
	if(_data->channel != NOT_ALLOCATED){
		return;
	}

	int allocatedChannels = Mix_AllocateChannels(GET_CHANNELS);
	int playingChannels = Mix_Playing(GET_CHANNELS);

	//If all Channels are playing.
	if(allocatedChannels == playingChannels){
		//Allocate a new Channel.
		Mix_AllocateChannels(allocatedChannels+1);
	}

	//Attempt to play the Audio File.
	int channel = Mix_PlayChannel(FIRST_FREE_CHANNEL, (_data->audio_effect).get(), _data->loops);
	//If Audio File could not be played.
	if(channel == NOT_ALLOCATED){
		DEBUG_LOG("ERROR: Could not play audio file: " << Mix_GetError());
		return;
	}

	_data->channel = channel;
}

void AudioEffect::Pause(void)
{
	//Check if Audio is playing.
	if((_data->channel == NOT_ALLOCATED) || Mix_Paused(_data->channel)){
		return;
	}

	Mix_Pause(_data->channel);

	return;
}

void AudioEffect::Resume(void)
{
	//Check if Audio is paused.
	if((_data->channel == NOT_ALLOCATED) || !Mix_Paused(_data->channel)){
		return;
	}

	Mix_Resume(_data->channel);

	return;
}

void AudioEffect::Halt(void)
{
	//Check if Audio is active.
	if((_data->channel == NOT_ALLOCATED) || !Mix_Playing(_data->channel)){
		return;
	}

	Mix_HaltChannel(_data->channel);

	int playingChannels = Mix_Playing(GET_CHANNELS);
	if(playingChannels == 0){
		Mix_AllocateChannels(UNALLOCATE_ALL_CHANNELS);
	}

	_data->channel = NOT_ALLOCATED;

	return;
}

AudioEffect::~AudioEffect()
{
	//Make sure Channel is halted.
	Halt();
}

struct Audio::AudioData
{
	unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> music;
	int loops;
	bool isActive;
};

/*
Audio::Audio()
	: _audio_data(nullptr)	{}
*/

Audio::Audio(const char * Audio_File_name, int loops)
{
	//if SDL_Mixer is not initialized
	if(SDL_WasInit(SDL_INIT_AUDIO) == 0){
		//log error and exit program.
		DEBUG_LOG("SDL_Mixer is not initialized.");
		exit(1);
	}

	//attempt to load the Music.
	Mix_Music * music = Mix_LoadMUS(Audio_File_name);

	//if Music could not be loaded.
	if(music == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not load music: " << Mix_GetError());
		exit(1);
	}

	//allocate memory for AudioData _audio_data.
	_data = unique_ptr<AudioData>{ new AudioData{
									unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>{ move(music), &Mix_FreeMusic }
								   }
							     };

	_data->loops = move(loops);

	_data->isActive = false;
}

void Audio::Play(void)
{
	//if another Audio is already playing.
	if(Mix_PlayingMusic()){
		//log error and return.
		DEBUG_LOG("ERROR: Another audio is already playing.");
		return;
	}

	//attempt to play the music.
	int playMusicFailed = Mix_PlayMusic((_data->music).get(), _data->loops);

	//if music could not be played.
	if(playMusicFailed){
		//log error and return.
		DEBUG_LOG("ERROR: Could not play audio: " << Mix_GetError());
		return;
	}

	//success, indicate that the Audio is now active.
	_data->isActive = true;

	return;
}

void Audio::Pause(void)
{
	//if this Audio is currently playing.
	if(_data->isActive){
		//pause Audio.
		Mix_PauseMusic();
	}

	return;
}

void Audio::Resume(void)
{
	//if this Audio is currently playing.
	if(_data->isActive){
		//resume Audio.
		Mix_ResumeMusic();
	}

	return;
}

void Audio::Halt(void)
{
	//if this Audio is currently playing.
	if(_data->isActive){
		//halt Audio.
		Mix_HaltMusic();

		//set Audio inactive.
		_data->isActive = false;
	}

	return;
}

Audio::~Audio()
{
	//make sure the audio is no longer playing.
	Halt();
}
