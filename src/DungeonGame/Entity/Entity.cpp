#include "DungeonGame/Dungeon/Dungeon.hpp"
#include "DungeonGame/Entity/Entity.hpp"

namespace DungeonGame
{

EntityController::EntityController(Entity & entity) : ownedEntity(entity) {}

EntityController::~EntityController() {}



Entity::Entity( SpriteSheet<2, ANIMATIONS> & sprites,
		Dungeon & dungeon, Point2D position,
		unique_ptr<EntityController> && controller,
		Rectangle box,
		Health healthMax, Mana manaMax)

	:	health( Stat{ healthMax, healthMax } ),
	  	mana( Stat{ manaMax, manaMax } ),
		currentState( IDLE ),
	  	currentDirection( RIGHT ),
	  	location( Location{ dungeon, position } ),
		entityController( move(controller) ),
		hitbox( box )
{
	InitializeAnimations(sprites);
}

const 	int 	& Entity::GetMaxHealth(void) 	const	{ return health.max; }
const 	int 	& Entity::GetHealth(void) 	const	{ return health.current; }

const 	int 	& Entity::GetMaxMana(void)	const 	{ return mana.max; }
const 	int	& Entity::GetMana(void)		const	{ return mana.current; }

const Dungeon 	& Entity::GetDungeon(void) 	const	{ return location.dungeon; }
const Point2D 	& Entity::GetPosition(void) 	const	{ return location.position; }

const Rectangle & Entity::GetSize(void)		const	{ return hitbox; }

const Entity::State & Entity::GetState(void) const
{
	return currentState;
}

const Entity::Direction & Entity::GetDirection(void) const
{
	return currentDirection;
}

void Entity::SetHealth(const Health & newHealth)
{
	health.current = (newHealth > health.max) ? health.max : newHealth;

	return;
}

void Entity::SetMana(const Mana & newMana)
{
	mana.current = (newMana > mana.max) ? mana.max : newMana;

	return;
}

void Entity::Damage(const int & damage)
{
	health.current -= damage;

	if(health.current <= 0) currentState = DEAD;

	return;
}

void Entity::Damage(Entity & attacker, const int & damage)
{
	lastAttacker = &attacker;
	Damage(damage);

	return;
}

void Entity::SetState(const State & state)
{
	if(state == CLIMBING){
		const int x = location.position.x + (hitbox.w / 2.0F);

		const int yE = location.position.y + hitbox.h;
		for(int y = location.position.y; y < yE; y++){
			if(location.dungeon.GetBlockAt(x, y).GetType() == Block::LADDER){
				location.position.x = (int) x;
				currentState = state;
				break;
			}
		}
	} else {
		currentState = state;
	}

	return;
}

void Entity::SetDirection(const Direction & direction)
{
	currentDirection = direction;

	return;
}

void Entity::Draw(const Rectangle * viewport)
{
	Rectangle dstrect;
	dstrect.x = (location.position.x * Block::WIDTH) - viewport->x;
	dstrect.y = (location.position.y * Block::HEIGHT) - viewport->y;
	dstrect.w = hitbox.w * Block::WIDTH;
	dstrect.h = hitbox.h * Block::HEIGHT;

	switch(currentState)
	{
		case IDLE:
			if(currentDirection == LEFT){
				animations[1].Draw(GetRenderer(), &dstrect);
			} else {
				animations[3].Draw(GetRenderer(), &dstrect);
			}

			break;

		case WALKING:
			if(currentDirection == LEFT){
				animations[0].Draw(GetRenderer(), &dstrect);
			} else {
				animations[4].Draw(GetRenderer(), &dstrect);
			}

			break;

		case CLIMBING:
			animations[2].Draw(GetRenderer(), &dstrect);

	}

	return;
}

void Entity::Update(void)
{
	entityController->UpdateEntity();
	Move();
}

void Entity::InitializeAnimations(SpriteSheet<2, ANIMATIONS> & sprites)
{
	for(int i = 0; i < ANIMATIONS; i++){
		animations[i] = move(Animation<2>{ sprites[i], 1s, -1 });
	}

	return;
}

Entity::~Entity() {}

void Entity::Move(void)
{
	static constexpr auto MOVEMENT_SPEED = 0.0926F;

	static Point2D previousPosition = location.position;

	switch(currentState)
	{
		case WALKING:
			CorrectCollisions(previousPosition);
			previousPosition = location.position;
			location.position.y += location.dungeon.GetGravity();
			CorrectCollisions(previousPosition);
			previousPosition = location.position;

			switch(currentDirection){
				case LEFT:
				location.position.x -= MOVEMENT_SPEED;
				CorrectCollisions(previousPosition);
				previousPosition = location.position;

				break;

				case RIGHT:
				location.position.x += MOVEMENT_SPEED;
				CorrectCollisions(previousPosition);
				previousPosition = location.position;

				break;
			}

			break;

		case IDLE:
			CorrectCollisions(previousPosition);
			previousPosition = location.position;
			location.position.y += location.dungeon.GetGravity();

			CorrectCollisions(previousPosition);
			previousPosition = location.position;
			break;

		case CLIMBING:

		switch(currentDirection){
			case UP:

			if(location.dungeon.GetBlockAt(location.position.x + (hitbox.w / 2.0F), location.position.y+1.0F).GetType() == Block::LADDER){
				location.position.y -= MOVEMENT_SPEED;
				//CorrectCollisions(previousPosition);
				previousPosition = location.position;
			}
			break;

			case DOWN:

			//if(location.dungeon.GetBlockAt(location.position.x + (hitbox.w / 2.0F), location.position.y + (float)hitbox.h).GetType() == Block::LADDER){
				CorrectCollisions(previousPosition);
				previousPosition = location.position;
				location.position.y += MOVEMENT_SPEED;
				CorrectCollisions(previousPosition);
				previousPosition = location.position;
			//}
			break;
		}
		break;
	}



	return;
}

bool Entity::SectionCollides(Point2D & collision_out, const float & x, const float & y) const
{
	static constexpr auto DIST_TO_CORNER =
		(float)(Block::WIDTH-1) / (float)Block::WIDTH;

	if(location.dungeon.GetBlockAt(x, y).IsSolid()){
		collision_out.x = x; collision_out.y = y;
		return true;
	}

	if(location.dungeon.GetBlockAt(x + DIST_TO_CORNER, y).IsSolid()){
		collision_out.x = x + DIST_TO_CORNER; collision_out.y = y;
		return true;
	}

	if(location.dungeon.GetBlockAt(x, y + DIST_TO_CORNER).IsSolid()){
		collision_out.x = x; collision_out.y = y + DIST_TO_CORNER;
		return true;
	}

	if(location.dungeon.GetBlockAt(x + DIST_TO_CORNER, y + DIST_TO_CORNER).IsSolid()){
		collision_out.x = x + DIST_TO_CORNER; collision_out.y = y + DIST_TO_CORNER;
		return true;
	}

	return false;
}

void Entity::CorrectCollisions(const Point2D & previousPosition)
{
	const auto X_END = location.position.x + hitbox.w;
	const auto Y_END = location.position.y + hitbox.h;

	Point2D collision;

	for(float y = location.position.y; y < Y_END; y++){
		for(float x = location.position.x; x < X_END; x++){
			if(SectionCollides(collision, x, y)){
				CorrectCollision(previousPosition, move(collision));
				return;
			}
		}
	}
}

void Entity::CorrectCollision(const Point2D & previousPosition, Point2D && collision)
{

	if(collision.y < (previousPosition.y + (collision.y - location.position.y))){//movement.imag() < 0){
		location.position.y += ((((int) collision.y) + (16.0F / Block::HEIGHT)) - collision.y);
		return;
	} else if(collision.y > (previousPosition.y + (collision.y - location.position.y))){//movement.imag() > 0){
		location.position.y += ((((int) collision.y) - ( 1.0F / Block::HEIGHT)) - collision.y);
		return;
	}

	if(collision.x < (previousPosition.x + (collision.x - location.position.x))){//movement.real() < 0){
		location.position.x += ((((int) collision.x) + (16.0F / Block::WIDTH)) - collision.x);
		return;
	} else if(collision.x > (previousPosition.x + (collision.x - location.position.x))){//movement.real() > 0){
		location.position.x += ((((int) collision.x) - ( 1.0F / Block::WIDTH)) - collision.x);
		return;
	}

	return;
}

}
