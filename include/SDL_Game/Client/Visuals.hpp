#ifndef VISUALS_HEADER
#define VISUALS_HEADER

#include "SDL_Game/Client/Drawable.hpp"
#include "SDL_Wrapper/SDL_Wrapper.hpp"
#include <array>
#include <chrono>
using namespace std;

class Sprite;

template<int width, int height>
class SpriteSheet;

template<int frames>
class Animation;

constexpr char INFINITE_LOOPS = -1;

class Sprite : public Rectangle
{

public:

	Sprite();
	Sprite(Texture * texture, const Rectangle & bounds);

	void Draw(Renderer & renderer, const Rectangle * dstrect);

	~Sprite();

private:

	Texture * sheetTexture;

};

template<int width, int height>
struct SpriteSheet : public array< array<Sprite, width>, height >
{

public:

	SpriteSheet();
	SpriteSheet(Texture * texture);

	~SpriteSheet();

private:

	Texture * sheetTexture;

};

template<int frames>
class Animation : private array<Sprite, frames>
{

	static constexpr int INFINITE_LOOPS = -1;

public:

	Animation();
	Animation(const array<Sprite, frames> & allFrames, const chrono::milliseconds & interval, const int & loops);

	void Reset(void);
	void Draw(Renderer & renderer, const Rectangle * dstrect);

	bool HasEnded(void);

	~Animation();

private:

	chrono::milliseconds _interval;
	chrono::time_point<chrono::system_clock> _nextTick;

	int _loops, _remainingLoops;

	int _pos;

	void Update(void);

};

#include "SDL_Game/Client/Visuals.tpp"

#endif
