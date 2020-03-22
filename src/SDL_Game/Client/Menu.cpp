#include "SDL_Game/Client/Menu.hpp"
#include "Debug.hpp"

struct MenuButton::MenuButtonData
{
	Rectangle bounds;
	Texture & texture;
	Texture & texture_selected;
	bool isSelected;

	Menu & menu;
	void (* const interactFunc)(Menu &);
};

/*
struct Menu::MenuData
{
	Rectangle bounds;
};
*/


MenuButton::MenuButton()
	: _data( unique_ptr<MenuButtonData>{ nullptr } )
{}

MenuButton::MenuButton( const Rectangle && bounds, Texture & texture, Texture & texture_selected,
	 Menu & menu, void (* const interactFunc)(Menu &) )
	:	_data( unique_ptr<MenuButtonData>{
			new MenuButtonData{ bounds, texture, texture_selected, false, menu, interactFunc }
				} )
{}

MenuButton & MenuButton::operator=(const MenuButton & button)
{
	_data = unique_ptr<MenuButtonData>{ new MenuButtonData{ *(button._data) } };

	return *this;
}

/*
MenuButton & MenuButton::operator=(const MenuButton & button)
{
	DEBUG_LOG("Copy assignment operator called.");

	_data = unique_ptr<MenuButtonData>{ new MenuButtonData{ button._data->bounds,
								move(button._data->texture),
								move(button._data->texture_selected),
								button._data->isSelected,
								button._data->menu,
								button._data->interactFunc
							 	}
							};

	return *this;
}
*/

const Rectangle & MenuButton::GetBounds(void) const { return _data->bounds; }

void MenuButton::SetSelected(const bool & isSelected)
{
	_data->isSelected = isSelected;

	return;
}

void MenuButton::Draw(void)
{
	Texture & texture = _data->isSelected ? _data->texture_selected : _data->texture;
	texture.Draw(NULL, &(_data->bounds));

	return;
}

void MenuButton::Interact(void){ _data->interactFunc(_data->menu); }

MenuButton::~MenuButton()
{}

/*

Menu::Menu(const Rectangle & bounds)
	:	_data( unique_ptr<MenuData>{	new MenuData{ bounds }	} )
{}

void Menu::ProcessEvent(unique_ptr< MenuEvent > && event)
{
	this->QueueEvent( move(event) );
	this->PushEvents();

	return;
}

const Rectangle & Menu::GetBounds(void) const
{
	return _data->bounds;
}

Menu::~Menu()
{}

*/
