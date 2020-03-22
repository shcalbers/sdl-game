COMPILER = g++
STANDARD = -std=c++1z
OPTIONS = `sdl2-config --cflags --libs` -lSDL2_mixer -lSDL2_image

HEADER_FILES 	:= 	$(wildcard ./*.hpp) 	$(wildcard ./include/*.hpp)	$(wildcard ./include/**/*.hpp)	$(wildcard ./include/**/**/*.hpp)	$(wildcard ./include/**/**/**/*.hpp)
SOURCE_FILES 	:=	$(wildcard ./*.cpp)	$(wildcard ./src/**/*.cpp)	$(wildcard ./src/**/**/*.cpp)	$(wildcard ./src/**/**/**/*.cpp)
OBJ = $(HEADER_FILES) $(SOURCE_FILES)

game : $(OBJ)
	$(COMPILER) $(STANDARD) -I ./include/ $(OPTIONS) -o $@ $^

game_debug : $(OBJ)
	$(COMPILER) $(STANDARD) -I ./include/ -I ./src -D DEBUGGING_ON $(OPTIONS) -o $@ $^

g++ -std=c++1z -I "./include/" -D DEBUGGING_ON `sdl2-config --cflags --libs` -lSDL2_mixer -lSDL2_image -o ./linux_game ./main_dungeon.cpp ./src/DungeonGame/DGMain.cpp ./src/DungeonGame/Dungeon/Dungeon.cpp ./src/DungeonGame/Entity/Entity.cpp ./src/DungeonGame/Entity/Player.cpp ./src/DungeonGame/Entity/Slime.cpp ./src/SDL_Game/Client/Visuals.cpp ./src/SDL_Wrapper/SDL_Audio.cpp ./src/SDL_Wrapper/SDL_Video.cpp
