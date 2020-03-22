#ifndef PLAYER_ENTITY_HEADER
#define PLAYER_ENTITY_HEADER

#include "DungeonGame/Entity/Entity.hpp"

namespace DungeonGame
{

struct GameProfile;
class PlayerController;
class Player;

struct GameProfile
{
	int maxHealth;
	int maxMana;
};



class PlayerController : public EntityController
{

public:

	PlayerController(Player & player);

	void UpdateEntity(void);

	~PlayerController();

};



class Player : public Entity
{

public:

	Player(GameProfile & profile, Dungeon & dungeon, const Point2D & position);//const GameProfile & profile);

	//void OpenInventory();

	~Player();

private:

	Texture spriteTextures;
	SpriteSheet<2, 5> sprites;

};

}

#endif
