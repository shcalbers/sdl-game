#ifndef SLIME_ENTITY_HEADER
#define SLIME_ENTITY_HEADER

#include "DungeonGame/Entity/Entity.hpp"
#include <chrono>
#include <random>
using namespace std;

namespace DungeonGame
{

class AIController : public EntityController
{

public:

	AIController(Entity & entity);

	void UpdateEntity(void);

private:

	inline static minstd_rand randomNumberGenerator{ chrono::system_clock::now().time_since_epoch().count() };
	inline static bernoulli_distribution shouldChangeDirection{ 0.01 };
	inline static bernoulli_distribution randomBool{ 0.5 };

};

class Slime : public Entity
{

public:

	Slime(Dungeon & dungeon, const Point2D & position);

	~Slime();

private:

	Texture spriteTextures;
	SpriteSheet<2, 5> sprites;

};

}

#endif
