#ifndef DUNGEON_HEADER
#define DUNGEON_HEADER

#include "Debug.hpp"
#include "DungeonGame/DGMain.hpp"
#include "DungeonGame/Entity/Entity.hpp"
#include "DungeonGame/Entity/Player.hpp"
#include "DungeonGame/Entity/Slime.hpp"
#include "SDL_Wrapper/Drawable.hpp"
#include "SDL_Wrapper/SDL_Wrapper.hpp"
#include "SDL_Game/Client/Visuals.hpp"
#include "SDL_Game/Physics/Physics.hpp"
#include <array>
#include <chrono>
#include <random>
using namespace std;

namespace DungeonGame
{

class Block : private DungeonObject
{

public:

	enum BlockType
	{
		AIR,
		LADDER,
		SOLID_BLOCKS,
		WALL = SOLID_BLOCKS,
		FLOOR_LEFT_END,
		FLOOR_NORMAL,
		FLOOR_RIGHT_END,
		TYPE_COUNT
	};

	static constexpr int WIDTH = 16, HEIGHT = 16;

	Block(const BlockType & type = AIR);

	const BlockType & GetType(void) const;
	bool IsSolid(void) const;

	void Draw(const Rectangle * dstrect);

	~Block();

	static void Initialize()
	{
		static bool initialized = false;
		if(initialized) return;

		sheetTexture = move( Texture{ GetRenderer(), "./assets/Textures/Blocks/Blocks.png" } );
		blocks = move( SpriteSheet<6, 1>{ &sheetTexture } );

		initialized = true;

		return;
	}

private:

	BlockType _type;
	bool _solid;

	struct TextureIndex{
		int x, y;
	} _textureIndex;

	inline static Texture sheetTexture{};//{ "./assets/Textures/Blocks/Blocks.png" };
	inline static SpriteSheet<6, 1> blocks{};//{ &sheetTexture };

};

class Dungeon : private DungeonObject
{

	friend class DungeonGenerator;

	class Room : public Rectangle, private DungeonObject
	{

		friend class Dungeon;

		static constexpr int HOR_BLOCKS = 7;
		static constexpr int VER_BLOCKS = 7;
		using Blocks = array< array<Block *, HOR_BLOCKS>, VER_BLOCKS>;

		enum Side { TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3 };

	public:

		Room();
		Room(	Dungeon & dungeon, const Rectangle & bounds,
			bool isTopOpen, bool isRightOpen, bool isBottomOpen, bool isLeftOpen);

		bool 	IsOpenAtTop(void) 	const;
		bool 	IsOpenAtRight(void) 	const;
		bool 	IsOpenAtBottom(void) 	const;
		bool 	IsOpenAtLeft(void) 	const;

		~Room();

	private:

		Blocks _blocks;
		array<bool, 4> _open;

		void InitializeRoom(void);

		void InitializeTop(void);
		void InitializeRight(void);
		void InitializeBottom(void);
		void InitializeLeft(void);

		void InitializeCenter(void);

	};

	static constexpr int HOR_ROOMS = 7;
	static constexpr int VER_ROOMS = 7;
	static constexpr int HOR_BLOCKS = HOR_ROOMS * Room::HOR_BLOCKS;
	static constexpr int VER_BLOCKS = VER_ROOMS * Room::VER_BLOCKS;

	using Rooms = array< array<Room, HOR_ROOMS>, VER_ROOMS >;
	using Blocks = array< array<Block, HOR_BLOCKS>, VER_BLOCKS >;

public:

	enum EXIT_CODE
	{
		NORMAL = 0,
		REGENERATE_DUNGEON = 1
	};

	Dungeon();//GameProfile & profile);

	EXIT_CODE Start(GameProfile & profile);

	//inline Player & GetPlayerTMP(void) { return player; }

	const Block & GetBlockAt(const int & x, const int & y) const;

	const float & GetGravity(void) const;

	void Draw(const int & width, const int & height);

	~Dungeon();

private:

	inline static minstd_rand randomNumberGenerator{ chrono::system_clock::now().time_since_epoch().count() };
	inline static bernoulli_distribution randomBool{ 0.58 };

	Blocks blocks;
	Rooms rooms;

	Texture dungeonTextureCache;

	const float gravity{ 0.1635F };

	Player * player;
	array<Slime, 10> slimes{ Slime{ *this, { 8.0, 5.0 } }, Slime{ *this, { 8.0, 12.0 } }, Slime{ *this, { 15.0, 19.0 } }, Slime{ *this, { 22.0, 12.0 } }, Slime{ *this, { 29.0, 12.0 } },
				Slime{ *this, { 8.0, 19.0 } }, Slime{ *this, { 15.0, 12.0 } }, Slime{ *this, { 22.0, 26.0 } }, Slime{ *this, { 29.0, 5.0 } }, Slime{ *this, { 22.0, 19.0 } }	};

	void InitializeDrawCache(void);
	void UpdateBlockInCache(const int & x, const int & y);

	void InitializeDungeon(void);

};

}

#endif
