#include "Utilities.h"
#include <map>
#include <algorithm>


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
    ARGB((uint32_t)255, (uint32_t)186, (uint32_t)46, 0)
};
struct RGB{
    unsigned int b:8;
    unsigned int g:8;
    unsigned int r:8;
    unsigned int alpha:8;
};


namespace PARTICLES{
    enum PARTICLETYPES{
        NOTHING = 0,
        SAND = 1,
        WATER = 2,
        WALL = 3,
        WOOD = 5,
        FIRE = 6
    };

    struct PARTICLEPROPERTIES{
        bool shiftable;
        uint32_t pixelColors[3];
        bool burnable;
        PARTICLETYPES burnCreates;
    };
    std::map<PARTICLETYPES, PARTICLEPROPERTIES> allProperties = 
    {
        {
            NOTHING , { 
            true, 
            {
                colors[1],
                colors[1],
                colors[1]
            },
            false,
            NOTHING
            }
        },
        {
            SAND, {
            false, 
            {
                colors[2],
                colors[3],
                colors[4]
            },
            false,
            SAND
            }
        },
        {
            WATER, {
            true, {
                colors[5],
                colors[6],
                colors[7]
            },
            false,
            WATER
            }
        },
        {
            WALL, {
                false,
                {
                    colors[8],
                    colors[9],
                    colors[10]
                },
            
                false,
                WALL
            }
        
        },
        {
            WOOD, {
                false,
                {
                    colors[12],
                    colors[13],
                    colors[14]
                },
            true,
            FIRE
            }
        },
        {
            FIRE, {
                false,
                {
                    colors[15],
                    colors[16],
                    colors[17]
                },
            false,
            FIRE
            }
        }
    };


    void addParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors, PARTICLETYPES particleType){
        switch(particleTypes[x + y * _width]){
            case NOTHING:
                particleTypes[x + y * _width] = particleType;
                pixels[x + y * _width] = allProperties[particleType].pixelColors[FastRand()%3];
                break;
                    
        }
    }
    void removeParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors){
        pixels[x + y * _width] = colors[1];
        particleTypes[x + y * _width] = NOTHING;
    }
    void particleUpdate(PARTICLETYPES * particleTypes, int _width, int _height, uint32_t * pixels, uint32_t * colors){
        
        int left, right, below, above, aboveLeft, aboveRight, belowRight, belowLeft;
        bool touching;
        for (int x = _width - 1; x >= 0; --x){
            for (int y = _height - 1; y >= 0; --y){
                switch ((PARTICLETYPES)particleTypes[x + y * _width]){
                    default:
                        break;
                    case NOTHING:
                        break;
                    case WALL:
                        break;
                    case WOOD:
                        break;
                    case FIRE:
                        touching = false;
                        left = (x - 1) + (y) * _width;
                        right = (x + 1) + (y) * _width;
                        aboveLeft = (x - 1) + (y - 1) * _width;
                        aboveRight = (x + 1) + (y - 1) * _width;
                        above = x + (y - 1 ) * _width;
                        below = x + (y + 1 ) * _width;
                        belowRight = (x + 1) + (y + 1) * _width;
                        belowLeft = (x - 1) + (y + 1) * _width;
                        if (y == 0){
                            particleTypes[x + y * _width] = NOTHING;
                            pixels[x + y * _width] = allProperties[NOTHING].pixelColors[0];
                        }
                        else if (y > 0){
                            if (allProperties[particleTypes[left]].burnable){
                                if (FastRand()%100 == 1){
                                        
                                    particleTypes[left] = allProperties[particleTypes[left]].burnCreates; 
                                    pixels[left] = allProperties[allProperties[particleTypes[left]].burnCreates].pixelColors[FastRand()%3];   
                                }
                                touching = true;
                            } 
                            if (allProperties[particleTypes[right]].burnable){
                                if (FastRand()%100 == 1){
                                        
                                    particleTypes[right] = allProperties[particleTypes[right]].burnCreates;
                                    
                                    pixels[right] = allProperties[allProperties[particleTypes[right]].burnCreates].pixelColors[FastRand()%3];
                                }
                                touching = true;
                            } 
                            if (allProperties[particleTypes[aboveLeft]].burnable){
                                if (FastRand() %100 == 1){
                                    particleTypes[aboveLeft] = allProperties[particleTypes[aboveLeft]].burnCreates;
                                    pixels[aboveLeft] = allProperties[allProperties[particleTypes[aboveLeft]].burnCreates].pixelColors[FastRand()%3];
                                }
                                touching = true;
                            } 
                            if (allProperties[particleTypes[aboveRight]].burnable){
                                if (FastRand() % 100 == 1){
                                    particleTypes[aboveRight] = allProperties[particleTypes[aboveRight]].burnCreates;
                                
                                    pixels[aboveRight] = allProperties[allProperties[particleTypes[aboveRight]].burnCreates].pixelColors[FastRand()%3];
                                }

                                touching = true;
                            }
                            if (allProperties[particleTypes[above]].burnable){
                                if (FastRand() % 100 == 1){
                                        
                                    particleTypes[above] = allProperties[particleTypes[above]].burnCreates;
                                    
                                    pixels[above] = allProperties[allProperties[particleTypes[above]].burnCreates].pixelColors[FastRand()%3];
                                }
                                touching = true;
                            }
                            if (allProperties[particleTypes[below]].burnable){
                                if (FastRand() % 100 == 1){
                                        
                                    particleTypes[below] = allProperties[particleTypes[below]].burnCreates;
                                    
                                    pixels[below] = allProperties[allProperties[particleTypes[below]].burnCreates].pixelColors[FastRand()%3];
                                }
                                touching = true;
                            }
                            if (allProperties[particleTypes[belowLeft]].burnable){
                                if (FastRand() % 100 == 1){
                                        
                                    particleTypes[belowLeft] = allProperties[particleTypes[belowLeft]].burnCreates;
                                    
                                    pixels[belowLeft] = allProperties[allProperties[particleTypes[belowLeft]].burnCreates].pixelColors[FastRand()%3];
                                }
                                touching = true;
                            }
                            if (allProperties[particleTypes[belowRight]].burnable){
                                if (FastRand() % 100 == 1){
                                        
                                    particleTypes[belowRight] = allProperties[particleTypes[belowRight]].burnCreates;
                                    
                                    pixels[belowRight] = allProperties[allProperties[particleTypes[belowRight]].burnCreates].pixelColors[FastRand()%3];
                                }
                                touching = true;
                            }
                            
                            if (particleTypes[left] == WATER|| particleTypes[right] == WATER || particleTypes[above] == WATER){
                                pixels[x + y * _width] = allProperties[NOTHING].pixelColors[0];
                                particleTypes[x + y * _width] = NOTHING;
                            }
                            else {
                                switch (FastRand()%20){
                                    default:
                                        // if (particleTypes[above] == NOTHING && FastRand()%40 == 1){
                                        //     particleTypes[above] = FIRE;
                                        //     pixels[above] = allProperties[FIRE].pixelColors[FastRand()%3];
                                        // }
                                        break;
                                    case 0: case 5: case 10:
                                        if (particleTypes[aboveLeft] == NOTHING || particleTypes[aboveLeft] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[aboveLeft]);
                                            std::swap(pixels[x + y * _width], pixels[aboveLeft]);
                                        }
                                        else if (particleTypes[aboveRight] == NOTHING || particleTypes[aboveRight] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[aboveRight]);
                                            std::swap(pixels[x + y * _width], pixels[aboveRight]);
                                        }
                                        else if (particleTypes[above] == NOTHING || particleTypes[above] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[above]);
                                            std::swap(pixels[x + y * _width], pixels[above]);    
                                        }
                                        break;
                                    case 1: case 6: case 11:
                                        if (particleTypes[aboveRight] == NOTHING || particleTypes[aboveRight] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[aboveRight]);
                                            std::swap(pixels[x + y * _width], pixels[aboveRight]);
                                        }
                                        else if (particleTypes[above] == NOTHING || particleTypes[above] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[above]);
                                            std::swap(pixels[x + y * _width], pixels[above]);
                                        }
                                        else if (particleTypes[aboveLeft] == NOTHING || particleTypes[aboveLeft] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[aboveLeft]);
                                            std::swap(pixels[x + y * _width], pixels[aboveLeft]);    
                                        }
                                        break;
                                    case 2: case 7: case 12:
                                        if (particleTypes[above] == NOTHING || particleTypes[above] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[above]);
                                            std::swap(pixels[x + y * _width], pixels[above]);
                                        }
                                        else if (particleTypes[aboveLeft] == NOTHING || particleTypes[aboveLeft] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[aboveLeft]);
                                            std::swap(pixels[x + y * _width], pixels[aboveLeft]);
                                        }
                                        else if (particleTypes[aboveRight] == NOTHING || particleTypes[aboveRight] == FIRE){
                                            std::swap(particleTypes[x + y * _width],particleTypes[aboveRight]);
                                            std::swap(pixels[x + y * _width], pixels[aboveRight]);    
                                        }
                                        break;
                                    case 3:
                                        if (FastRand()%5 == 0 && !touching){
                                            pixels[x + y * _width] = allProperties[NOTHING].pixelColors[0];
                                            particleTypes[x + y * _width] = NOTHING;   
                                        
                                        }
                                        break;
                                }
                            }
                        }
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
                        break;
                    case SAND:
                        if (FastRand()%2 == 1){
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
                            break;

                        }
                }
            }
            
        }
        //return particleTypes;
    }
};
