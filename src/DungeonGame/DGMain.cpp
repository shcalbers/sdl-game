#include "DungeonGame/DGMain.hpp"
#include "DungeonGame/Dungeon/Dungeon.hpp"

namespace DungeonGame
{

namespace
{
	bool isDungeonGameInitialized = false;
	Window * dungeonWindow;
	Renderer * dungeonRenderer;
}

DungeonObject::DungeonObject()
{
	if(!isDungeonGameInitialized){
		DEBUG_LOG("ERROR: Please initialize the Dungeon Game before using using any Dungeon Game object!"
				<< endl
				<< "See: void InitializeDungeonGame(void) { .... } in \"./include/DungeonGame/DGMain.hpp\""
				<< endl
			);
		exit(NOT_INITIALIZED);
	}
}

Window & DungeonObject::GetWindow(void)
{
	return *dungeonWindow;
}

Renderer & DungeonObject::GetRenderer(void)
{
	return *dungeonRenderer;
}

DungeonObject::~DungeonObject() {}



void InitializeDungeonGame(Window & window, Renderer & renderer)
{
	dungeonWindow = &window;
	dungeonRenderer = &renderer;

	Block::Initialize();

	isDungeonGameInitialized = true;

	return;
}

}
