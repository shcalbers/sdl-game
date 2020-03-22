#ifndef SDL_AUDIO_HEADER
#define SDL_AUDIO_HEADER

#include "SDL.h"
#include "SDL_mixer.h"
#include <memory>
using namespace std;

class AudioEffect
{

public:

	//AudioEffect();
	AudioEffect(const char * file_name, int loops);

	AudioEffect(const AudioEffect &) = delete;
	AudioEffect & operator=(const AudioEffect &) = delete;

	void Play(void);
	void Pause(void);
	void Resume(void);
	void Halt(void);

	~AudioEffect();

private:

	struct AudioEffectData;
	unique_ptr<AudioEffectData> _data;

};

class Audio
{

public:

	//Audio();
	Audio(const char * Audio_File_name, int loops);

	Audio(const Audio &) = delete;
	Audio & operator=(const Audio &) = delete;

	void Play(void);
	void Pause(void);
	void Resume(void);
	void Halt(void);

	~Audio();

private:

	struct AudioData;
	unique_ptr<AudioData> _data;

};

#endif
