all:
	g++ -Isrc/Include -Lsrc/lib -o main main.cpp map.cpp textures.cpp game.cpp abilities.cpp player.cpp render.h -lSDL2 -lSDL2_image -lmingw32