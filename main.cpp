#define SDL_MAIN_HANDLED
#include "include/DungeonGame/Client/Menus/MainMenu.hpp"
#include "include/SDL_Wrapper/SDL_Wrapper.hpp"
#include "include/SDL_Game/Events.hpp"
#include "include/SDL_Game/Client/Visuals.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;

enum EventType
{
	ENTITY_MOVE_EVENT 	= 0,
	ENTITY_DAMAGE_EVENT 	= 1,
	ENTITY_DEATH_EVENT 	= 2,
	ENTITY_SPAWN_EVENT 	= 3
};

void listenerFunc(unique_ptr< Event<EventType> > & eventPtr)
{
	const EventType & eventType = eventPtr->GetEventType();

	switch(eventType){
		case ENTITY_MOVE_EVENT:
			cout << "ENTITY_MOVE_EVENT" << endl;
			break;
		case ENTITY_DAMAGE_EVENT:
			cout << "ENTITY_DAMAGE_EVENT" << endl;
			break;
		case ENTITY_DEATH_EVENT:
			cout << "ENTITY_DEATH_EVENT" << endl;
			break;
		case ENTITY_SPAWN_EVENT:
			cout << "ENTITY_SPAWN_EVENT" << endl;
			break;
		default:
			cout << "ERROR: No EventType." << endl;
			break;
	}

	if(eventPtr->IsCancelled()){
		cout << "Event is cancelled." << endl;
	} else {
		cout << "Event is not cancelled" << endl;
	}

	return;
}

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
	//MainMenu menu;

	EventManager<EventType> eventManager;
	eventManager.RegisterListener(
		EventListener<EventType>{	ENTITY_MOVE_EVENT,
				 		EventPriority::LOW_PRIORITY,
						&listenerFunc	}
	);

	eventManager.QueueEvent( unique_ptr< Event<EventType> >{
		 			new Event<EventType>{
						ENTITY_MOVE_EVENT
					}
				 } );

	Window window{ 	"MainMenu",
	 		SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
		 	1920,
			1080,
			SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS	};

	Renderer renderer{ window, -1, SDL_RENDERER_ACCELERATED };
	renderer.SetLogicalResolution(256, 144);
	renderer.Clear();

	//DungeonGame::MainMenu mainMenu;
	//mainMenu.Draw();

	DungeonGame::MainMenu mainMenu{ renderer };

	Texture tiles{ "./assets/mainmenu.png" };
	SpriteSheet<16, 9> spriteSheet{ &tiles };

	spriteSheet[4][4].Draw( { 0,  0, 16, 16} );
	spriteSheet[1][7].Draw( {16, 16, 16, 16} );

	Texture flameTexture{ "./assets/flame.png" };
	SpriteSheet<2, 1> flame{ &flameTexture };
	Animation<2, 1> animation{ flame, 400ms, INFINITE_LOOPS };

	renderer.Present();

	mainMenu.Open();

	/*

	while(!animation.HasEnded()){

		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYDOWN){
				SDL_Keycode keyPressed = event.key.keysym.sym;

				switch(keyPressed){
					case SDLK_w:
						mainMenu.ProcessEvent(
							unique_ptr<MenuInputEvent>{
								new MenuInputEvent{
									shared_ptr<DungeonGame::MainMenu>{ &mainMenu },
							 		MenuInputEvent::MenuAction::KEY_PRESS_UP
								}
							}	);

							break;
					case SDLK_s:
						mainMenu.ProcessEvent(
							unique_ptr<MenuInputEvent>{
								new MenuInputEvent{
									shared_ptr<DungeonGame::MainMenu>{ &mainMenu },
									MenuInputEvent::MenuAction::KEY_PRESS_DOWN
								}
							}
						);

						break;
					case SDLK_RETURN:
						mainMenu.ProcessEvent(
							unique_ptr<MenuInputEvent>{
								new MenuInputEvent{
									shared_ptr<DungeonGame::MainMenu>{ &mainMenu },
									MenuInputEvent::MenuAction::KEY_PRESS_ENTER
								}
							}
						);

						break;
				}
			}
		}

		renderer.Clear();

		mainMenu.Draw();

		animation.Draw( { 0, 0, 16, 16 } );

		renderer.Present();
	}

	*/

	//eventManager.PushEvents(); //is now a protected member function.

	clock_t timeE = clock();

	cout << "Opening the Main Menu took: " << (((double) (timeE - time0)) / CLOCKS_PER_SEC) << " seconds" << endl;
/*

	//Open Window.
	float aspect_ratio = 1920.00F/1080.00F;
	int height = 512;
	int width = ((float) height) * aspect_ratio;

	Window window(	"SDL Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_BORDERLESS	);

*/

/* DISABLE AUDIO

	//Initialize SDL_mixer.
	int mixinit_flag = Mix_Init(MIX_INIT_MP3);
	//if SDL_mixer could not be initialized.
	if((mixinit_flag & MIX_INIT_MP3) != MIX_INIT_MP3){
		//log error and exit program.
		cout << "Main.cpp:\n" << "ERROR: Could not initialize MP3 support: " << Mix_GetError() << endl;
		exit(1);
	}

	int openaudio_returncode = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
	//if Audio could not be opened.
	if(openaudio_returncode == -1){
		//log error and exit program.
		cout << "Main.cpp:\n" << "ERROR: Could not open Audio: " << Mix_GetError() << endl;
		exit(1);
	}

	//Open Audio and play Audio.
	Audio audio{ "/home/arkwandyr/PWS/Game_Project/assets/The-Castle-Beyond-the-Forest_Looping.wav", -1 };
	audio.Play();

	//Display Music Volume.
	cout << "Current Music Volume: " << Mix_VolumeMusic(-1) << endl;

	//if Music is playing.
	if(Mix_PlayingMusic()){
		//log message.
		cout << "Mix_PlayingMusic() returned 1." << endl;
	}

 DISABLE AUDIO */

	//Wait till user enters 'x'.
	char c = '\0';
	while(c != 'x'){
		cin >> c;
	}

/*

	//Destroy Window.
	window.~Window();
*/

/* DISABLE AUDIO

	//Stop Audio.
	audio.Halt();

	//Quit SDL_mixer.
	Mix_CloseAudio();
	while(Mix_Init(0)){
		Mix_Quit();
	}

 DISABLE AUDIO */

	//Quit SDL.
	SDL_Quit();

	//return success.
	return 0;
}
