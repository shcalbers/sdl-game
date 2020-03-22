#define SDL_MAIN_HANDLED
#include "include/SDL_Wrapper/SDL_Wrapper.hpp"
#include "include/SDL_Game/Events.hpp"
#include "include/SDL_Game/Client/Visuals.hpp"
#include "DungeonGame/DGMain.hpp"
#include "include/DungeonGame/Dungeon/Dungeon.hpp"
#include "include/DungeonGame/Entity/Player.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
using namespace std;

int main(void)
{
	SDL_SetMainReady();

	//Initialize AUDIO and VIDEO subsystems
	int init_returncode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	//if Initialization failed.
	if(init_returncode != 0){
		//log error and exit program.
		cout << "ERROR: Could not initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	int openaudio_returncode = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
	//if Audio could not be opened.
	if(openaudio_returncode == -1){
		//log error and exit program.
		cout << "Main.cpp:\n" << "ERROR: Could not open Audio: " << Mix_GetError() << endl;
		exit(1);
	}

	clock_t time0 = clock();

	Window window{ 	"Dungeon",
	 		SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
		 	1920,
			1080,
			SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS	};

	Renderer renderer{ window, -1, SDL_RENDERER_ACCELERATED };
	renderer.SetLogicalResolution(256, 144);
	renderer.Clear();

	DungeonGame::InitializeDungeonGame(window, renderer);

	DungeonGame::GameProfile profile{ 16, 16 };
	DungeonGame::Dungeon dungeon{};// profile };

	dungeon.Start(profile);

	/*
	Texture playerTexture{ "./assets/Textures/Entities/Player.png" };
	SpriteSheet<2, 5> animations{ &playerTexture };
	Animation<2> animation{ animations[4], 250ms, -1 };

	Rectangle player_dstrect{ 32, 64, 16, 32 };
	*/

	//DungeonGame::Player & player = dungeon.GetPlayerTMP();

	/*
	Rectangle viewport{ (player.GetPosition().x * DungeonGame::Block::WIDTH) - 112,
		 	    (player.GetPosition().y * DungeonGame::Block::HEIGHT) - 40,
			    256, 144 };
	*/

	//Rectangle player_dstrect{ 32, 64, 16, 32 };

	//Rectangle dstrect{ 0, 0, 256, 144 };

	/*
	Audio audio{ "./assets/8-bit-Monsters.wav", -1 };
	audio.Play();

	const Uint8 * keyboardState = SDL_GetKeyboardState(NULL);
	while(1){

		if(keyboardState[SDL_SCANCODE_RETURN]) break;

		renderer.Clear();

		player.SetState(DungeonGame::Entity::State::IDLE);

		if(keyboardState[SDL_SCANCODE_W]){
			player.SetState(DungeonGame::Entity::State::CLIMBING);
			player.SetDirection(DungeonGame::Entity::Direction::UP);
			/*
			(dstrect.y)--;
			(player_dstrect.y)--;
			*//*
		}
		if(keyboardState[SDL_SCANCODE_S]){
			player.SetState(DungeonGame::Entity::State::CLIMBING);
			player.SetDirection(DungeonGame::Entity::Direction::DOWN);
			/*
			(dstrect.y)++;
			(player_dstrect.y)++;
			*//*
		}

		if(keyboardState[SDL_SCANCODE_A]){
			player.SetState(DungeonGame::Entity::State::WALKING);
			player.SetDirection(DungeonGame::Entity::Direction::LEFT);
			/*
			(dstrect.x)--;
			(player_dstrect.x)--;
			*//*
		}
		if(keyboardState[SDL_SCANCODE_D]){
			player.SetState(DungeonGame::Entity::State::WALKING);
			player.SetDirection(DungeonGame::Entity::Direction::RIGHT);
			/*
			(dstrect.x)++;
			(player_dstrect.x)++;
			*//*
		}

		player.Update();

//		viewport.x = (player.GetPosition().x * DungeonGame::Block::WIDTH) - 120;
//		viewport.y = (player.GetPosition().y * DungeonGame::Block::HEIGHT) - 56;

		dungeon.Draw(256, 144);

//		player.Draw(&viewport);

		renderer.Present();

		SDL_PumpEvents();

		//::this_thread::sleep_for(4s);
	}

	audio.Halt();*/

	//dungeon.Draw(&dstrect);

	//renderer.Present();

	clock_t timeE = clock();

	cout << "Opening the Main Menu took: " << (((double) (timeE - time0)) / CLOCKS_PER_SEC) << " seconds" << endl;

	//Quit SDL.
	SDL_Quit();

	//return success.
	return 0;
}
