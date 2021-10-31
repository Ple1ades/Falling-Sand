#include "Utilities.h"
#include <map>
#include <algorithm>

enum PARTICLETYPES{
    NOTHING = 0,
    SAND = 1,
    WATER = 2
};

struct PARTICLEPROPERTIES{
    bool shiftable;
    uint32_t pixelColors[3];
};

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
    ARGB((uint32_t)28, (uint32_t)163, (uint32_t)236, 0)
};

std::map<PARTICLETYPES, PARTICLEPROPERTIES> allProperties = 
{
    {NOTHING , { 
        true, 
        {
            colors[1],
            colors[1],
            colors[1]}
        }
    },
    {SAND, {
        false, 
        {
            colors[2],
            colors[3],
            colors[4]}
        }
    },
    {WATER, {
        true, {
            colors[5],
            colors[6],
            colors[7]}
        }
    }
};


void addParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors, PARTICLETYPES particleType){
    switch(particleTypes[x + y * _width]){
        case NOTHING:
            particleTypes[x + y * _width] = particleType;
            switch (particleType){
                case SAND:
                    pixels[x + y * _width] = colors[2+FastRand()%3];
                    break;
            }
            break;
                
    }
}
void removeParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors){
    pixels[x + y * _width] = colors[1];
    particleTypes[x + y * _width] = NOTHING;
}
void particleUpdate(PARTICLETYPES * particleTypes, int _width, int _height, uint32_t * pixels, uint32_t * colors){
    
    int left, right, below;
    
    for (int x = _width - 1; x >= 0; --x){
        for (int y = _height - 1; y >= 0; --y){
            switch ((PARTICLETYPES)particleTypes[x + y * _width]){
                default:
                    break;
                case WATER:
                    below = x + (y + 1) * _width;
                    left = (x - 1) + (y) * _width;
                    right = (x + 1) + (y) * _width;
                    if (y < _height - 1){
                        if (allProperties[particleTypes[below]].shiftable && particleTypes[below] != WATER){
                            std::swap(particleTypes[x + y * _width],particleTypes[below]);
                            std::swap(pixels[x + y * _width], pixels[below]);
                        }
                        else if (x == 0){
                            if (allProperties[particleTypes[right]].shiftable){
                                std::swap(particleTypes[x + y * _width],particleTypes[right]);
                                std::swap(pixels[x + y * _width], pixels[right]);
                            }
                        }
                        else if (x == _width - 1){
                            if (allProperties[particleTypes[left]].shiftable){
                                std::swap(particleTypes[x + y * _width],particleTypes[left]);
                                std::swap(pixels[x + y * _width], pixels[left]);
                                
                            }
                        }
                        else{
                            if ((int)FastRand()%2 == 0){
                                if (allProperties[particleTypes[left]].shiftable){
                                    std::swap(particleTypes[x + y * _width],particleTypes[left]);
                                    std::swap(pixels[x + y * _width], pixels[left]);
                                    
                                }
                                else if (allProperties[particleTypes[right]].shiftable){
                                    std::swap(particleTypes[x + y * _width],particleTypes[right]);
                                    std::swap(pixels[x + y * _width], pixels[right]);
                                }
                            }
                            else{
                                if ((int)FastRand()%2 == 0){
                                    if (allProperties[particleTypes[right]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[right]);
                                        std::swap(pixels[x + y * _width], pixels[right]);
                                        
                                    }
                                    else if (allProperties[particleTypes[left]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[left]);
                                        std::swap(pixels[x + y * _width], pixels[left]);
                                    }
                                }
                            }
                        }
                    }
                    break;
                case SAND:
                    left = (x - 1) + (y + 1) * _width;
                    below = x + (y + 1) * _width;
                    right = (x + 1) + (y + 1) * _width;
                    if (y < _height - 1){
                        //particleTypes[below] = SAND;
                        if (allProperties[particleTypes[below]].shiftable){
                            
                            particleTypes[x + y * _width] = particleTypes[below];
                            particleTypes[below] = SAND;
                            std::swap(pixels[x + y * _width], pixels[below]);
                        }
                        else if (x == 0){
                            if (allProperties[particleTypes[right]].shiftable){
                                particleTypes[x + y * _width]= particleTypes[right];
                                particleTypes[right] = SAND;
                                std::swap(pixels[x + y * _width], pixels[right]);
                            }
                        }
                        else if (x == _width - 1){
                            if (allProperties[particleTypes[left]].shiftable){
                                particleTypes[x + y * _width]= particleTypes[left];
                                particleTypes[left] = SAND;
                                
                                std::swap(pixels[x + y * _width], pixels[left]);
                                
                            }
                        }
                        else{
                            if ((int)FastRand()%2 == 0){
                                if (allProperties[particleTypes[left]].shiftable){
                                    particleTypes[x + y * _width]= particleTypes[left];
                                    particleTypes[left] = SAND;
                                    
                                    std::swap(pixels[x + y * _width], pixels[left]);
                                    
                                }
                                else if (allProperties[particleTypes[right]].shiftable){
                                    particleTypes[x + y * _width]= particleTypes[right];
                                    particleTypes[right] = SAND;
                                    
                                    std::swap(pixels[x + y * _width], pixels[right]);
                                    
                                }
                            }
                            else{
                                if ((int)FastRand()%2 == 0){
                                    if (allProperties[particleTypes[right]].shiftable){
                                        particleTypes[x + y * _width]= particleTypes[right];
                                        particleTypes[right] = SAND;
                                        
                                        std::swap(pixels[x + y * _width], pixels[right]);
                                        
                                    }
                                    else if (allProperties[particleTypes[left]].shiftable){
                                        particleTypes[x + y * _width]= particleTypes[left];
                                        particleTypes[left] = SAND;
                                        
                                        std::swap(pixels[x + y * _width], pixels[left]);
                                        
                                    }
                                }
                            }
                        }
                    }
                    break;
            }
        }
        
    }
    //return particleTypes;
}