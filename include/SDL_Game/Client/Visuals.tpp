#include "SDL_Game/Client/Visuals.hpp"

/*
template<int frames>
struct Animation<frames>::AnimationData
{
	chrono::milliseconds interval;
	chrono::time_point<chrono::system_clock> nextTick;

	int loops, remainingLoops;

	int pos;
};
*/


template<int width, int height>
SpriteSheet<width, height>::SpriteSheet() {}

template<int width, int height>
SpriteSheet<width, height>::SpriteSheet(Texture * texture)
	: sheetTexture(texture)
{
	const int sprite_width = (texture->GetWidth() / width);
	const int sprite_height = (texture->GetHeight() / height);

	Rectangle bounds{
		0, 0, sprite_width, sprite_height
	};

	for(int y = 0; y < height; y++){
		bounds.x = 0;

		for(int x = 0; x < width; x++){
			(*this)[y][x] = Sprite(sheetTexture, bounds);

			bounds.x += sprite_width;
		}

		bounds.y += sprite_height;
	}
}

template<int width, int height>
SpriteSheet<width, height>::~SpriteSheet() {}



template<int frames>
Animation<frames>::Animation() {}

template<int frames>
Animation<frames>::Animation(	const array<Sprite, frames> & allFrames,
	 			const chrono::milliseconds & interval,
				const int & loops	)
	: array<Sprite, frames>(allFrames),
	  _interval(interval),
	  _nextTick((chrono::system_clock::now() + interval)),
	  _loops(loops),
	  _remainingLoops(loops),
	  _pos(0)/*
	  _data( unique_ptr<AnimationData>{
			new AnimationData{
				interval,
				(chrono::system_clock::now() + interval),
				loops,
				loops,
				0 }
			}	)*/
{}

template<int frames>
void Animation<frames>::Reset(void)
{
	_nextTick = chrono::system_clock::now() + _interval;
	_remainingLoops = _loops;

	_pos = 0;

	return;
}

template<int frames>
void Animation<frames>::Draw(Renderer & renderer, const Rectangle * dstrect)
{
	Sprite & frame = (*this)[_pos];
	frame.Draw(renderer, dstrect);

	Update();
}

template<int frames>
bool Animation<frames>::HasEnded(void)
{
	return (_remainingLoops == 0);
}

template<int frames>
void Animation<frames>::Update(void)
{
	if(HasEnded() || (chrono::system_clock::now() < _nextTick)) return;

	_nextTick = chrono::system_clock::now() + _interval;

	if(++(_pos) == frames){
		_pos = 0;

		if(_remainingLoops != INFINITE_LOOPS)
			(_remainingLoops)--;
	}

	return;
}

template<int frames>
Animation<frames>::~Animation() {}
