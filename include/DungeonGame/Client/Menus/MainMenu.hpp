#ifndef MAIN_MENU_HEADER
#define MAIN_MENU_HEADER

#include "SDL_Game/Client/Menu.hpp"
#include "SDL_Wrapper/SDL_Wrapper.hpp"

namespace DungeonGame
{

void ButtonInteraction(Menu & menu);

class MainMenu;

class MainMenu : public Menu
{

	friend void ButtonInteraction(Menu & menu);

public:

	MainMenu(Renderer & renderer);

	MENU_RETURN_TYPE Open(void);

	~MainMenu();

private:

	struct Data;
	unique_ptr<Data> _data;

	operator Data&() { return *_data; }

	void HandleEvents(void);
	void Draw(void);

};

}

#endif
