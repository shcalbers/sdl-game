#include "DungeonGame/Entity/Player.hpp"

namespace DungeonGame
{

PlayerController::PlayerController(Player & player) : EntityController(player) {}

void PlayerController::UpdateEntity(void)
{
	SDL_Event event;
	SDL_Keycode keyPressed;

	while(SDL_PollEvent(&event)){
		if(event.type == SDL_KEYUP){
			keyPressed = event.key.keysym.sym;

			switch(keyPressed){
			case SDLK_UP:		case SDLK_w:
			if(ownedEntity.GetDirection() != Entity::Direction::UP) break;
			ownedEntity.SetState(Entity::State::IDLE);
			break;

			case SDLK_RIGHT: 	case SDLK_d:
			if(ownedEntity.GetDirection() != Entity::Direction::RIGHT) break;
			ownedEntity.SetState(Entity::State::IDLE);
			break;

			case SDLK_DOWN: 	case SDLK_s:
			if(ownedEntity.GetDirection() != Entity::Direction::DOWN) break;
			ownedEntity.SetState(Entity::State::IDLE);
			break;

			case SDLK_LEFT: 	case SDLK_a:
			if(ownedEntity.GetDirection() != Entity::Direction::LEFT) break;
			ownedEntity.SetState(Entity::State::IDLE);

			break;
			}
		} else if(event.type == SDL_KEYDOWN){
			keyPressed = event.key.keysym.sym;

			ownedEntity.SetState(Entity::State::IDLE);

			switch(keyPressed){
			case SDLK_UP:		case SDLK_w:
			ownedEntity.SetState(Entity::State::CLIMBING);
			ownedEntity.SetDirection(Entity::Direction::UP);

			break;
			case SDLK_RIGHT: 	case SDLK_d:
			ownedEntity.SetState(Entity::State::WALKING);
			ownedEntity.SetDirection(Entity::Direction::RIGHT);

			break;
			case SDLK_DOWN: 	case SDLK_s:
			ownedEntity.SetState(Entity::State::CLIMBING);
			ownedEntity.SetDirection(Entity::Direction::DOWN);

			break;
			case SDLK_LEFT: 	case SDLK_a:
			ownedEntity.SetState(Entity::State::WALKING);
			ownedEntity.SetDirection(Entity::Direction::LEFT);

			break;
			}
		}
	}

	return;
}

PlayerController::~PlayerController() {}



Player::Player(GameProfile & profile, Dungeon & dungeon, const Point2D & position)//, const GameProfile & profile)
	: spriteTextures( GetRenderer(), "./assets/Textures/Entities/Player.png" ),
	  sprites( &spriteTextures ),
	  Entity(sprites, dungeon, position,
		  move(unique_ptr<PlayerController>{ new PlayerController{ *this } }),
 	  	  { 0, 0, 1, 2 },
		  profile.maxHealth, profile.maxMana) //profile.maxHealth, profile.maxMana)
{
	InitializeAnimations(sprites);
}


Player::~Player() {}



}
