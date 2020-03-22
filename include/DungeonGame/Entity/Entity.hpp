#ifndef ENTITY_HEADER
#define ENTITY_HEADER

namespace DungeonGame
{
	class Dungeon;
}

//#include "DungeonGame/Dungeon/Dungeon.hpp"
#include "DungeonGame/DGMain.hpp"
#include "SDL_Game/Client/Visuals.hpp"
#include "SDL_Game/Physics/Physics.hpp"
#include "SDL_Wrapper/SDL_Wrapper.hpp"
#include <memory>
using namespace std;

namespace DungeonGame
{

class EntityController;
class Entity;

class EntityController : private DungeonObject
{

public:

	virtual void UpdateEntity(void) = 0;

	~EntityController();

protected:

	EntityController(Entity & entity);

	Entity & ownedEntity;
};



class Entity : protected DungeonObject
{

	struct Stat	{ int max, current; };
	struct Location	{ Dungeon & dungeon; Point2D position; };

	using Health = int;
	using Mana = int;

	using EntityID = size_t;

	static constexpr auto ANIMATIONS = 5;

public:

	enum State
	{
		DEAD,
		IDLE,
		WALKING,
		CLIMBING,
		STATES
	};

	enum Direction
	{
		NONE,
		UP,
		RIGHT,
		DOWN,
		LEFT,
		DIRECTIONS
	};

	const 	int 	& 	GetMaxHealth(void) 	const;
	const 	int 	& 	GetHealth(void) 	const;

	const 	int 	& 	GetMaxMana(void)	const;
	const 	int	&	GetMana(void)		const;

	const Dungeon 	& 	GetDungeon(void) 	const;
	const Point2D 	& 	GetPosition(void) 	const;

	const Rectangle & 	GetSize(void)		const;

	const State 	& 	GetState(void)		const;
	const Direction	& 	GetDirection(void) 	const;

	void SetHealth(const Health & newHealth);
	void SetMana(const Mana & newMana);

	void Damage(const int & damage);
	void Damage(Entity & attacker, const int & damage);

	void SetState(const State & state);
	void SetDirection(const Direction & direction);

	void Draw(const Rectangle * dstrect);

	virtual void Update(void);

	~Entity();

protected:

	Entity( SpriteSheet<2, ANIMATIONS> & sprites,
		Dungeon & dungeon, Point2D position,
		unique_ptr<EntityController> && controller,
		Rectangle box,
		Health healthMax, Mana manaMax);

	void InitializeAnimations(SpriteSheet<2, ANIMATIONS> & sprites);

	void Move(void);

private:

	unique_ptr<EntityController> entityController;

	Stat health, mana;
	Entity * lastAttacker;

	State currentState;
	Direction currentDirection;

	Location 	location;
	Rectangle	hitbox;

	array< Animation<2>, ANIMATIONS> animations;

	inline static EntityID EntityIDCount = 0;
	EntityID ID{ EntityIDCount++ };

	bool SectionCollides(Point2D & collision_out, const float & x, const float & y) const;
	void CorrectCollisions(const Point2D & previousPosition);
	void CorrectCollision(const Point2D & previousPosition, Point2D && collision);

};

}

#endif
