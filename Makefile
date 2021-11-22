all:
	g++ -Isrc/Include -Lsrc/lib/x64 -o build/main main.cpp -lmingw32 -lSDL2main -lSDL2 -static -static-libgcc -static-libstdc++ -std=c++17