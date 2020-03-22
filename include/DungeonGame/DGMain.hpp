#ifndef DUNGEON_MAIN_HEADER
#define DUNGEON_MAIN_HEADER

#include "Debug.hpp"
#include "SDL_Wrapper/SDL_Wrapper.hpp"

namespace DungeonGame
{

enum DUNGEON_ERROR_CODES
{
	NO_ERROR = 0x0,
	NOT_INITIALIZED = 0x1
};



class DungeonObject
{

public:

	~DungeonObject();

	static Window & GetWindow(void);
	static Renderer & GetRenderer(void);

protected:

	DungeonObject();

};


void InitializeDungeonGame(Window & window, Renderer & renderer);

}

#endif
