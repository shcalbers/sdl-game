#include "DungeonGame/Dungeon/Dungeon.hpp"
//#include <cstdlib>
//#include <ctime>

namespace DungeonGame
{

Block::Block(const BlockType & type)
	: _type(type),
	  _solid(type >= SOLID_BLOCKS ? true : false),
	  _textureIndex(TextureIndex{	(type)%(TYPE_COUNT),
	   				(type)/(TYPE_COUNT)	})
{
	DEBUG_LOG("Creating a Block....");
}

const Block::BlockType & Block::GetType(void) const
{
	return _type;
}

bool Block::IsSolid(void) const
{
	return _solid;
}

void Block::Draw(const Rectangle * dstrect)
{
	//DEBUG_LOG("Drawing Block at (" << dstrect->x << "," << dstrect->y << ")....");
	blocks[_textureIndex.y][_textureIndex.x].Draw(GetRenderer(), dstrect);
}

Block::~Block() {}



Dungeon::Room::Room()
{
	DEBUG_LOG("Creating a Room (empty)....");
}

Dungeon::Room::Room( 	Dungeon & dungeon, const Rectangle & bounds,
			bool isTopOpen, bool isRightOpen, bool isBottomOpen, bool isLeftOpen)
	: _open( { isTopOpen, isRightOpen, isBottomOpen, isLeftOpen } )
{
	DEBUG_LOG("Creating a Room....");

	for(int y = 0; y < VER_BLOCKS; y++){
		for(int x = 0; x < HOR_BLOCKS; x++){
			_blocks[y][x] = &dungeon.blocks[y + bounds.y][x + bounds.x];
		}
	}

	InitializeRoom();

	DEBUG_LOG("Room created.");
}

bool 	Dungeon::Room::IsOpenAtTop(void) 	const { return _open[Side::TOP]; }
bool 	Dungeon::Room::IsOpenAtRight(void) 	const { return _open[Side::RIGHT]; }
bool 	Dungeon::Room::IsOpenAtBottom(void) 	const { return _open[Side::BOTTOM]; }
bool 	Dungeon::Room::IsOpenAtLeft(void) 	const { return _open[Side::LEFT]; }

Dungeon::Room::~Room() {}

void Dungeon::Room::InitializeRoom(void)
{
	InitializeTop();
	InitializeRight();
	InitializeBottom();
	InitializeLeft();
	InitializeCenter();

	return;
}

void Dungeon::Room::InitializeTop(void)
{
	if(_open[Side::TOP]){
		*_blocks[0][1] = Block{ Block::WALL };
		*_blocks[0][2] = Block{ Block::WALL };
		*_blocks[0][3] = Block{ Block::LADDER };
		*_blocks[0][4] = Block{ Block::WALL };
		*_blocks[0][5] = Block{ Block::WALL };
	} else {

		for(int x = 1; x < 6; x++){
			*_blocks[0][x] = Block{ Block::WALL };
		}
	}
}

void Dungeon::Room::InitializeRight(void)
{
	if(_open[Side::RIGHT]){
		*_blocks[0][6] = Block{ Block::WALL };
		*_blocks[1][6] = Block{ Block::WALL };
		*_blocks[2][6] = Block{ Block::WALL };
		*_blocks[6][6] = Block{ Block::FLOOR_NORMAL };
	} else {

		for(int y = 0; y < VER_BLOCKS; y++){
			*_blocks[y][6] = Block{ Block::WALL };
		}
	}
}

void Dungeon::Room::InitializeBottom(void)
{
	if(_open[Side::BOTTOM]){
		*_blocks[6][1] = _open[Side::LEFT] ? Block{ Block::FLOOR_NORMAL } : Block{ Block::FLOOR_LEFT_END };
		*_blocks[6][2] = Block{ Block::FLOOR_RIGHT_END };
		*_blocks[6][3] = Block{ Block::LADDER };
		*_blocks[6][4] = Block{ Block::FLOOR_LEFT_END };
		*_blocks[6][5] = _open[Side::RIGHT] ? Block{ Block::FLOOR_NORMAL } : Block{ Block::FLOOR_RIGHT_END };
	} else {

		*_blocks[6][1] = _open[Side::LEFT] ? Block{ Block::FLOOR_NORMAL } : Block{ Block::FLOOR_LEFT_END };

		for(int x = 2; x < 5; x++){
			*_blocks[6][x] = Block{ Block::FLOOR_NORMAL };
		}

		*_blocks[6][5] = _open[Side::RIGHT] ? Block{ Block::FLOOR_NORMAL } : Block{ Block::FLOOR_RIGHT_END };

	}
}

void Dungeon::Room::InitializeLeft(void)
{
	if(_open[Side::LEFT]){
		*_blocks[0][0] = Block{ Block::WALL };
		*_blocks[1][0] = Block{ Block::WALL };
		*_blocks[2][0] = Block{ Block::WALL };
		*_blocks[6][0] = Block{ Block::FLOOR_NORMAL };
	} else {

		for(int y = 0; y < VER_BLOCKS; y++){
			*_blocks[y][0] = Block{ Block::WALL };
		}
	}
}

void Dungeon::Room::InitializeCenter(void)
{
	if(_open[Side::TOP]){

		for(int y = 1; y < 6; y++){
			*_blocks[y][3] = Block{ Block::LADDER };
		}

	} else if (_open[Side::BOTTOM]){
		*_blocks[5][3] = Block{ Block::LADDER };
	} else {
		//place random block in center.
	}

	return;
}



Dungeon::Dungeon()//GameProfile & profile)
	: dungeonTextureCache( GetRenderer(), HOR_BLOCKS * Block::WIDTH, VER_BLOCKS * Block::HEIGHT )
//	: player(profile, *this, { 2.0, 4.0 })
{
	InitializeDungeon();
	InitializeDrawCache();
}

Dungeon::EXIT_CODE Dungeon::Start(GameProfile & profile)
{
	player = new Player{ profile, *this, { 2.0, 4.0 } };

	Audio audio{ "./assets/8-bit-Monsters.wav", -1 };
	audio.Play();

	EXIT_CODE exit_code = NORMAL;

	chrono::time_point<chrono::system_clock> startTick;
	chrono::time_point<chrono::system_clock> nextTick;
	chrono::time_point<chrono::system_clock> endTick;

	while(1){

		startTick = chrono::system_clock::now();
		nextTick = startTick + 16667us;

		GetRenderer().Clear();

		player->Update();

		for(int i = 0; i < 10; i++)
			slimes[i].Update();

		Draw(256, 144);

		GetRenderer().Present();

		while((endTick = chrono::system_clock::now()) < nextTick) SDL_PumpEvents();
		DEBUG_LOG(1000000 / chrono::duration_cast<chrono::microseconds>(endTick - startTick).count() );
	}

	audio.Halt();

	return exit_code;
}

const Block & Dungeon::GetBlockAt(const int & x, const int & y) const
{
	return blocks[y][x];
}

const float & Dungeon::GetGravity(void) const
{
	return gravity;
}

void Dungeon::Draw(const int & width, const int & height)
{
	DEBUG_LOG("Drawing the Dungeon....");

	constexpr int TOTAL_WIDTH = (HOR_BLOCKS * Block::WIDTH);
	constexpr int TOTAL_HEIGHT = (VER_BLOCKS * Block::HEIGHT);

	static auto X_OFFSET = (width - (player->GetSize().w * Block::WIDTH)) / 2;
	static auto Y_OFFSET = (height - (player->GetSize().h * Block::HEIGHT)) / 2;

	Rectangle viewport{ 0, 0, width, height };
	Rectangle dstrect{ 0, 0, width, height };

	viewport.x = (player->GetPosition().x * Block::WIDTH) - X_OFFSET;
	viewport.y = (player->GetPosition().y * Block::HEIGHT) - Y_OFFSET;

	if(viewport.x >= TOTAL_WIDTH || viewport.y >= TOTAL_HEIGHT) return;

	if(viewport.x < 0){
		viewport.x = 0;
	} else if((viewport.x + width) >= TOTAL_WIDTH){
		viewport.x = TOTAL_WIDTH - width;
	}

	if(viewport.y < 0){
		viewport.y = 0;
	} else if((viewport.y + height) >= TOTAL_HEIGHT){
		viewport.y = TOTAL_HEIGHT - height;
	}

	GetRenderer().DrawTexture(dungeonTextureCache, &viewport, &dstrect);
	player->Draw(&viewport);

	for(int i = 0; i < 10; i++)
		slimes[i].Draw(&viewport);

	DEBUG_LOG("Dungeon drawn.");

	return;
}

Dungeon::~Dungeon() {}

void Dungeon::InitializeDrawCache(void)
{
	GetRenderer().SetRenderTarget(&dungeonTextureCache);

	DEBUG_LOG("Initializing Draw Cache....");

	Rectangle dstrect{ 0, 0, Block::WIDTH, Block::HEIGHT };

	for(int y = 0; y < VER_BLOCKS; y++){
		for(int x = 0; x < HOR_BLOCKS; x++){
			blocks[y][x].Draw(&dstrect);

			dstrect.x += Block::WIDTH;
		}

		dstrect.x = 0;
		dstrect.y += Block::HEIGHT;
	}

	DEBUG_LOG("Draw Cache Initialized.");

	GetRenderer().SetRenderTarget(NULL);

	return;
}

void Dungeon::UpdateBlockInCache(const int & x, const int & y)
{
	GetRenderer().SetRenderTarget(&dungeonTextureCache);

	Rectangle dstrect{ x, y, Block::WIDTH, Block::HEIGHT };
	blocks[y][x].Draw(&dstrect);

	GetRenderer().SetRenderTarget(NULL);

	return;
}

void Dungeon::InitializeDungeon(void)
{
	//#define RANDOM_BOOL rand() % 2

	DEBUG_LOG("Initializing the Dungeon....");

	for(int y = 0; y < VER_ROOMS; y++){
		for(int x = 0; x < HOR_ROOMS; x++){
			bool topOpen = (y == 0) ? false : rooms[y-1][x].IsOpenAtBottom();
			bool rightOpen = (x == HOR_ROOMS-1) ? false : randomBool(randomNumberGenerator);//RANDOM_BOOL;
			bool bottomOpen = (y == VER_ROOMS-1) ? false : randomBool(randomNumberGenerator);//RANDOM_BOOL;
			bool leftOpen = (x == 0) ? false : rooms[y][x-1].IsOpenAtRight();

			rooms[y][x] = Room{ *this,
				 	    { x * Room::HOR_BLOCKS, y * Room::VER_BLOCKS,
					      Room::HOR_BLOCKS, Room::VER_BLOCKS },
				            topOpen, rightOpen, bottomOpen, leftOpen };
		}
	}

	DEBUG_LOG("Dungeon initialized.");

	//#undef RANDOM_BOOL

	return;
}

}
