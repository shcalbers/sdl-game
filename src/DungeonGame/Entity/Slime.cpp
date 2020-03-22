#include "DungeonGame/Entity/Slime.hpp"

namespace DungeonGame
{

AIController::AIController(Entity & entity) : EntityController(entity) {}

void AIController::UpdateEntity(void)
{
	if(shouldChangeDirection(randomNumberGenerator)){
		if(randomBool(randomNumberGenerator)){
			ownedEntity.SetState(Entity::State::WALKING);
			ownedEntity.SetDirection(Entity::Direction::RIGHT);
		} else if(randomBool(randomNumberGenerator)){
			ownedEntity.SetState(Entity::State::WALKING);
			ownedEntity.SetDirection(Entity::Direction::LEFT);
		} else {
			ownedEntity.SetState(Entity::State::IDLE);
		}
	}

	return;
}

Slime::Slime(Dungeon & dungeon, const Point2D & position)
	: spriteTextures( GetRenderer(), "./assets/Textures/Entities/Slime.png" ),
	  sprites( &spriteTextures ),
	  Entity(sprites, dungeon, position,
	  	 move(unique_ptr<AIController>{ new AIController{ *this } }),
	 	 { 0, 0, 1, 1 },
	 	 16, 16)
{
	InitializeAnimations(sprites);
}

Slime::~Slime() {}

}
