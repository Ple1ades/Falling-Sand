all:
	g++ -Isrc/Include -Lsrc/lib/x64 -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++