#ifndef MENU_HEADER
#define MENU_HEADER

#include "SDL_Wrapper/SDL_Wrapper.hpp"
#include "SDL_Game/Client/Drawable.hpp"
//#include "SDL_Game/Events.hpp"
#include <memory>
using namespace std;

//enum class MenuEventType;
//struct MenuInputEvent;
class MenuButton;

template<int width, int height>
class ButtonMap;

class Menu;

class MenuButton : public Drawable
{

public:

	MenuButton();
        MenuButton(	const Rectangle && bounds,
			Texture & texture,
			Texture & texture_selected,
			Menu & menu,
			void (* const interactFunc)(Menu &)	);

	MenuButton & operator=(const MenuButton & button);

	/*MenuButton(	const Rectangle & bounds,
			Texture && texture,
			Menu * menu,
			void (* const interactFunc)(Menu *)
			);
	*/

	const Rectangle & GetBounds(void) const;

	void SetSelected(const bool & isSelected);

	void Draw(void);

        void Interact(void);

        ~MenuButton();

private:

	struct MenuButtonData;
	unique_ptr<MenuButtonData> _data;

};

template<int width>
using ButtonMapRow = array<MenuButton, width>;

template<int width, int height>
class ButtonMap : public array< array<MenuButton, width>, height >
{

public:

	MenuButton & GetSelectedButton(void);

	void SetPosition(const int & x, const int & y);

	void PositionLeft(void);
	void PositionRight(void);
	void PositionDown(void);
	void PositionUp(void);

private:

	static constexpr int X_BOUND_LOW = 0;
	static constexpr int X_BOUND_HIGH = width-1;

	static constexpr int Y_BOUND_LOW = 0;
	static constexpr int Y_BOUND_HIGH = height-1;

	struct Position { int x, y; };
	Position selectedButton{0, 0};

};

#include "SDL_Game/Client/Menu.tpp"



class Menu
{

public:

	using MENU_RETURN_TYPE = int;

	Menu(const Rectangle & bounds)
		:	bounds(bounds)
	{};

	virtual MENU_RETURN_TYPE Open(void) = 0;

	~Menu() {};

private:

	Rectangle bounds;

};

#endif
