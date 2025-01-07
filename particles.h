#pragma once
#include "Utilities.h"
#include <map>
#include <algorithm>

#ifndef colors_H
#define colors_H
uint32_t colors[] = {
    //black
    ARGB((uint32_t)0, (uint32_t)0,(uint32_t)0,0),
    //white
    ARGB((uint32_t)255, (uint32_t)255,(uint32_t)255,0),
    //sand color 1
    ARGB((uint32_t)234, (uint32_t)206,(uint32_t) 106, 0),
    //sand color 2
    ARGB((uint32_t)236, (uint32_t)211, (uint32_t)120,0),
    //sand color 3
    ARGB((uint32_t)238, (uint32_t)216, (uint32_t)136, 0),
    //water color 1
    ARGB((uint32_t)15, (uint32_t)94, (uint32_t)156, 0),
    //water color 2
    ARGB((uint32_t)35, (uint32_t)137, (uint32_t)218, 0),
    //water color 3
    ARGB((uint32_t)28, (uint32_t)163, (uint32_t)236, 0),
    //wall color 1
    ARGB((uint32_t)32, (uint32_t)32, (uint32_t)32, 0),
    //wall color 2
    ARGB((uint32_t)37, (uint32_t)37, (uint32_t)37, 0),
    //wall color 3
    ARGB((uint32_t)80, (uint32_t)80, (uint32_t)80, 0),
    //mouse UI
    ARGB((uint32_t)54, (uint32_t)61, (uint32_t)87, 0),
    //wood color 1
    ARGB((uint32_t)57, (uint32_t)31, (uint32_t)28, 0),
    //wood color 2
    ARGB((uint32_t)163, (uint32_t)95, (uint32_t)44, 0),
    //wood color 3
    ARGB((uint32_t)171, (uint32_t)130, (uint32_t)81, 0),
    //fire color 1
    ARGB((uint32_t)226, (uint32_t)88, (uint32_t)34, 0),
    //fire color 2
    ARGB((uint32_t)255, (uint32_t)123, (uint32_t)0, 0),
    //fire color 3
    ARGB((uint32_t)255, (uint32_t)186, (uint32_t)46, 0),
    //plant color 1
    ARGB((uint32_t)91, (uint32_t)208, (uint32_t)78, 0),
    //plant color 2
    ARGB((uint32_t)113, (uint32_t)184, (uint32_t)86, 0),
    //plant color 3
    ARGB((uint32_t)158, (uint32_t)228, (uint32_t)128, 0),
    //flower color 1
    ARGB((uint32_t)218, (uint32_t)112, (uint32_t)214, 0),
    //flower color 2
    ARGB((uint32_t)194, (uint32_t)30, (uint32_t)86, 0),
    //flower color 3
    ARGB((uint32_t)52, (uint32_t)116,(uint32_t)216, 0)
};
#endif
#ifndef RGB_H
#define RGB_H
struct RGB{
    unsigned int b:8;
    unsigned int g:8;
    unsigned int r:8;
    unsigned int alpha:8;
};
#endif

#ifndef PARTICLES_H
#define PARTICLES_H
namespace PARTICLES{
    enum PARTICLETYPES{
        NOTHING = 0,
        SAND = 1,
        WATER = 2,
        WALL = 3,
        WOOD = 5,
        FIRE = 6,
        PLANT = 7,
        FLOWER = 8,
        ICE = 9
    };
    struct PARTICLEPROPERTIES{
        bool shiftable;
        uint32_t pixelColors[3];
        bool burnable;
        PARTICLETYPES burnCreates;
    };
    //std::map<PARTICLETYPES, PARTICLEPROPERTIES> allProperties;
    void addParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors, PARTICLETYPES particleType);
    void removeParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors);
    std::pair<int,int> getChunk(int chunkNum, int chunkWidth, int _width, int _height);
    void updateChunk(int chunkNum, int chunkWidth, int _width, int _height, uint32_t * pixels, uint32_t *renderPixels);
    void getChunkColors(uint32_t * pixels, int _width, int _height, int chunkNum, bool visible, int chunkWidth, uint32_t * renderPixels);
    bool chunkUpdate(PARTICLETYPES * particleTypes, int _width, int _height, uint32_t * pixels, uint32_t * colors, int chunkNum, int chunkWidth);
    void getSolidParticles(PARTICLETYPES * particleTypes, int _width, int _height, bool * solidPixels);
    std::vector<SDL_Rect> checkPointBorder(int x, int width, int scaleFactor, bool * solidPixels);
}
#endif