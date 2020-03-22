#define ENTIRE_TEXTURE	NULL
#define ENTIRE_RENDERER	NULL

#include "DungeonGame/Client/Menus/MainMenu.hpp"
#include "Debug.hpp"

namespace DungeonGame
{

struct MainMenu::Data
{
	enum Textures
	{
		BUTTON_NORMAL = 0,
		BUTTON_SELECTED = 1
	};

	Rectangle menuViewport;

	Renderer & renderer;

	Texture background;
	Rectangle backgroundViewport;

	ButtonMap<1, 3> buttons;
	Texture buttonTextures[2];

	Audio music;
};

void ButtonInteraction(Menu & menu)
{
	MainMenu & mainMenu = dynamic_cast<MainMenu &>( menu );

	exit(0);
	return;
}

MainMenu::MainMenu(Renderer & renderer)
	:	_data
		(
		unique_ptr<Data>{
			new Data{
			{ 0, 0, 256, 144 },
			renderer,
			Texture{"./assets/mainmenu.png"},
			{ 0, 0, 256, 144 },
			ButtonMap<1, 3>{},
			{ Texture{"./assets/ButtonUntitled.png"},
			  Texture{"./assets/ButtonUntitledSelected.png"} },
			Audio{"./assets/The-Castle-Beyond-the-Forest_Looping.wav", -1}
		 	}
		}
		),
		Menu( { 0, 0, 256, 144} )
{
	_data->buttons[0][0] = MenuButton{
				{96, 56, 64, 16},
 				_data->buttonTextures[Data::BUTTON_NORMAL],
				_data->buttonTextures[Data::BUTTON_SELECTED],
				*this,
				&ButtonInteraction	};

	_data->buttons[0][1] = MenuButton{
				{96, 76, 64, 16},
				_data->buttonTextures[Data::BUTTON_NORMAL],
				_data->buttonTextures[Data::BUTTON_SELECTED],
				*this,
				&ButtonInteraction	};

	_data->buttons[0][2] = MenuButton{
				{96, 96, 64, 16},
				_data->buttonTextures[Data::BUTTON_NORMAL],
				_data->buttonTextures[Data::BUTTON_SELECTED],
				*this,
				&ButtonInteraction	};

	_data->buttons[0][0].SetSelected(true);
}

Menu::MENU_RETURN_TYPE MainMenu::Open(void)
{
	_data->music.Play();

	while(1){
		HandleEvents();
		Draw();
	}

	_data->music.Halt();

	return 0;
}

MainMenu::~MainMenu() {}

void MainMenu::HandleEvents(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_KEYDOWN){
			const SDL_Keycode & keyPressed = event.key.keysym.sym;

			switch(keyPressed){
				case SDLK_w:
					_data->buttons.PositionUp();
					break;
				case SDLK_s:
					_data->buttons.PositionDown();
					break;
				case SDLK_RETURN:
					(_data->buttons.GetSelectedButton()).Interact();
					break;
			}
		}
	}

	return;
}

void MainMenu::Draw(void)
{
	_data->renderer.Clear();

	_data->background.Draw(ENTIRE_TEXTURE, ENTIRE_RENDERER);

	for(ButtonMapRow<1> & row : _data->buttons){
		for(MenuButton & button : row){
			button.Draw();
		}
	}

	_data->renderer.Present();

	return;
}

}
