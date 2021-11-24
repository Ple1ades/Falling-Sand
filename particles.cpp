#include "particles.h"
using namespace PARTICLES;




std::map<PARTICLETYPES, PARTICLES::PARTICLEPROPERTIES> allProperties = 
{
    {
        PARTICLES::NOTHING , { 
        true, 
        {
            colors[1],
            colors[1],
            colors[1]
        },
        false,
        PARTICLES::NOTHING
        }
    },
    {
        PARTICLES::SAND, {
        false, 
        {
            colors[2],
            colors[3],
            colors[4]
        },
        false,
        PARTICLES::SAND
        }
    },
    {
        PARTICLES::WATER, {
        true, {
            colors[5],
            colors[6],
            colors[7]
        },
        false,
        PARTICLES::WATER
        }
    },
    {
        PARTICLES::WALL, {
            false,
            {
                colors[8],
                colors[9],
                colors[10]
            },
        
            false,
            PARTICLES::WALL
        }
    
    },
    {
        PARTICLES::WOOD, {
            false,
            {
                colors[12],
                colors[13],
                colors[14]
            },
        true,
        PARTICLES::FIRE
        }
    },
    {
        PARTICLES::FIRE, {
            false,
            {
                colors[15],
                colors[16],
                colors[17]
            },
            false,
            PARTICLES::FIRE
        }
    },
    {
        PARTICLES::PLANT, {
            false,{
                colors[18],
                colors[19],
                colors[20]
            },
            true,
            PARTICLES::FIRE
        }
    },
    {
        PARTICLES::FLOWER, {
            false, {
                colors[21],
                colors[22],
                colors[23]
            },
            true,
            PARTICLES::FIRE
        }
    }
};


void PARTICLES::addParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors, PARTICLETYPES particleType){
    switch(particleTypes[x + y * _width]){
        case NOTHING:
            particleTypes[x + y * _width] = particleType;
            pixels[x + y * _width] = allProperties[particleType].pixelColors[FastRand()%3];
            break;
                
    }
}
void PARTICLES::removeParticle(PARTICLETYPES * particleTypes, int x, int y, int _width, uint32_t * pixels, uint32_t * colors){
    pixels[x + y * _width] = colors[1];
    particleTypes[x + y * _width] = NOTHING;
}
std::pair<int,int> PARTICLES::getChunk(int chunkNum, int chunkWidth, int _width, int _height){
    return std::pair<int,int>(chunkNum%((int)_width/chunkWidth),std::floor(chunkNum/((int)_width/chunkWidth)));
}
void PARTICLES::updateChunk(int chunkNum, int chunkWidth, int _width, int _height, uint32_t * pixels, uint32_t *renderPixels){
    for (int x = getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth + chunkWidth; x > getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth; --x){
        for (int y = getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth + chunkWidth; y >= getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth; --y){
            if (y < _height && y > 0){
                renderPixels[x + y * _width]= (pixels[x + y * _width]);
            }
        }
    }
}
void PARTICLES::getChunkColors(uint32_t * pixels, int _width, int _height, int chunkNum, bool visible, int chunkWidth, uint32_t * renderPixels){
    
    if (!visible){
        
        for (int x = getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth + chunkWidth; x > getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth; --x){
            for (int y = getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth + chunkWidth; y >= getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth; --y){
                renderPixels[x + y * _width] = (pixels[x + y * _width])/2;

            }
        }
    }
    else{
        
        for (int x = getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth + chunkWidth; x > getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth; --x){
            for (int y = getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth + chunkWidth; y >= getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth; --y){
                if (y < _height && y > 0){
                    renderPixels[x + y * _width]= (pixels[x + y * _width]);
                }
            }
        }
    }
}
bool PARTICLES::chunkUpdate(PARTICLETYPES * particleTypes, int _width, int _height, uint32_t * pixels, uint32_t * colors, int chunkNum, int chunkWidth){
    
    int left, right, below, above, aboveLeft, aboveRight, belowRight, belowLeft;
    int touching;
    std::vector<int> touchingPoints;
    std::vector<int> touchingPointsSecondary;
    bool changed = false; 
    for (int x = getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth + chunkWidth; x > getChunk(chunkNum, chunkWidth, _width, _height).first * chunkWidth; --x){

        for (int y = getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth + chunkWidth; y >= getChunk(chunkNum, chunkWidth, _width, _height).second * chunkWidth; --y){
            if (y < _height && y > 0){
                switch ((PARTICLETYPES)particleTypes[x + y * _width]){
                    default:
                        break;
                    case NOTHING:
                        break;
                    case WALL:
                        break;
                    case WOOD:
                        break;
                    case FLOWER:
                        break;
                    case PLANT:
                        touchingPoints.clear();
                        touchingPointsSecondary.clear();
                        if (FastRand()%10 == 1){
                            
                            left = (x - 1) + (y) * _width;
                            right = (x + 1) + (y) * _width;
                            aboveLeft = (x - 1) + (y - 1) * _width;
                            aboveRight = (x + 1) + (y - 1) * _width;
                            above = x + (y - 1 ) * _width;
                            below = x + (y + 1 ) * _width;
                            belowRight = (x + 1) + (y + 1) * _width;
                            belowLeft = (x - 1) + (y + 1) * _width;
                            changed = 
                            particleTypes[left] == NOTHING||
                            particleTypes[right] == NOTHING||
                            particleTypes[aboveLeft] == NOTHING||
                            particleTypes[aboveRight] == NOTHING||
                            particleTypes[above] == NOTHING||
                            particleTypes[below] == NOTHING||
                            particleTypes[belowRight] == NOTHING||
                            particleTypes[belowLeft] == NOTHING||
                            particleTypes[left] == WATER||
                            particleTypes[right] == WATER||
                            particleTypes[aboveLeft] == WATER||
                            particleTypes[aboveRight] == WATER||
                            particleTypes[above] == WATER||
                            particleTypes[below] == WATER||
                            particleTypes[belowRight] == WATER||
                            particleTypes[belowLeft] == WATER;
                            if (particleTypes[left] == NOTHING) touchingPoints.push_back(left);
                            if (particleTypes[right] == NOTHING) touchingPoints.push_back(right);
                            if (particleTypes[aboveLeft] == NOTHING) touchingPoints.push_back(aboveLeft);
                            if (particleTypes[aboveRight] == NOTHING) touchingPoints.push_back(aboveRight);
                            if (particleTypes[above] == NOTHING) touchingPoints.push_back(above);
                            if (particleTypes[below] == NOTHING) touchingPoints.push_back(below);
                            if (particleTypes[belowLeft] == NOTHING) touchingPoints.push_back(belowLeft);
                            if (particleTypes[belowRight] == NOTHING) touchingPoints.push_back(belowRight);
                            if (particleTypes[left] == WATER) touchingPointsSecondary.push_back(left);
                            if (particleTypes[right] == WATER) touchingPointsSecondary.push_back(right);
                            if (particleTypes[aboveLeft] == WATER) touchingPointsSecondary.push_back(aboveLeft);
                            if (particleTypes[aboveRight] == WATER) touchingPointsSecondary.push_back(aboveRight);
                            if (particleTypes[above] == WATER) touchingPointsSecondary.push_back(above);
                            if (particleTypes[below] == WATER) touchingPointsSecondary.push_back(below);
                            if (particleTypes[belowLeft] == WATER) touchingPointsSecondary.push_back(belowLeft);
                            if (particleTypes[belowRight] == WATER) touchingPointsSecondary.push_back(belowRight);
                            if (touchingPoints.size() >= 5){
                                for (int point: touchingPoints){
                                    particleTypes[point] = NOTHING;
                                    pixels[point] = allProperties[NOTHING].pixelColors[0];
                                }
                                int randomPoint = touchingPoints[FastRand()%touchingPoints.size()];
                                pixels[randomPoint] = allProperties[PLANT].pixelColors[FastRand()%3];
                                particleTypes[randomPoint] = PLANT;
                            }
                            else if (touchingPointsSecondary.size() >= 3){
                                for (int point: touchingPointsSecondary){
                                    particleTypes[point] = NOTHING;
                                    pixels[point] = allProperties[NOTHING].pixelColors[0];
                                }
                                int randomPoint = touchingPointsSecondary[FastRand()%touchingPointsSecondary.size()];

                                if (FastRand()%50 == 1 && touchingPointsSecondary.size() >= 6){
                                    pixels[randomPoint] = allProperties[FLOWER].pixelColors[FastRand()%3];
                                    particleTypes[randomPoint] = FLOWER;
                                }
                                else{
                                    pixels[randomPoint] = allProperties[PLANT].pixelColors[FastRand()%3];
                                    particleTypes[randomPoint] = PLANT;
                                }
                            }
                            else if (touchingPoints.size() < 2 && touchingPointsSecondary.size() < 2){
                                changed = false;
                            }
                            
                            
                        }
                        break;
                    case FIRE:
                        touching = 0;
                        left = (x - 1) + (y) * _width;
                        right = (x + 1) + (y) * _width;
                        aboveLeft = (x - 1) + (y - 1) * _width;
                        aboveRight = (x + 1) + (y - 1) * _width;
                        above = x + (y - 1 ) * _width;
                        below = x + (y + 1 ) * _width;
                        belowRight = (x + 1) + (y + 1) * _width;
                        belowLeft = (x - 1) + (y + 1) * _width;
                        changed = 
                            allProperties[particleTypes[left]].burnable||
                            allProperties[particleTypes[right]].burnable||
                            allProperties[particleTypes[aboveLeft]].burnable||
                            allProperties[particleTypes[aboveRight]].burnable||
                            allProperties[particleTypes[above]].burnable||
                            allProperties[particleTypes[below]].burnable||
                            allProperties[particleTypes[belowRight]].burnable||
                            allProperties[particleTypes[belowLeft]].burnable||
                            particleTypes[left] == NOTHING||
                            particleTypes[right] == NOTHING||
                            particleTypes[aboveLeft] == NOTHING||
                            particleTypes[aboveRight] == NOTHING||
                            particleTypes[above] == NOTHING||
                            particleTypes[below] == NOTHING||
                            particleTypes[belowRight] == NOTHING||
                            particleTypes[belowLeft] == NOTHING||
                            particleTypes[left] == FIRE||
                            particleTypes[right] == FIRE||
                            particleTypes[aboveLeft] == FIRE||
                            particleTypes[aboveRight] == FIRE||
                            particleTypes[above] == FIRE||
                            particleTypes[below] == FIRE||
                            particleTypes[belowRight] == FIRE||
                            particleTypes[belowLeft] == FIRE;
                        if (FastRand()%2 == 1){
                            
                            if (y == 0){
                                particleTypes[x + y * _width] = NOTHING;
                                pixels[x + y * _width] = allProperties[NOTHING].pixelColors[0];
                            }
                            else if (y > 0){
                                if (allProperties[particleTypes[left]].burnable){
                                    if (FastRand()%300 == 1){
                                        particleTypes[left] = FIRE; 
                                        pixels[left] = allProperties[FIRE].pixelColors[FastRand()%3];   
                                    }
                                    touching += 1;
                                } 
                                if (allProperties[particleTypes[right]].burnable){
                                    if (FastRand()%300 == 1){
                                            
                                        particleTypes[right] = FIRE;
                                        
                                        pixels[right] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }
                                    touching += 1;
                                } 
                                if (allProperties[particleTypes[aboveLeft]].burnable){
                                    if (FastRand() %300 == 1){
                                        particleTypes[aboveLeft] = FIRE;
                                        pixels[aboveLeft] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }
                                    touching += 1;
                                } 
                                if (allProperties[particleTypes[aboveRight]].burnable){
                                    if (FastRand() % 300 == 1){
                                        particleTypes[aboveRight] = FIRE;
                                    
                                        pixels[aboveRight] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }

                                    touching += 1;
                                }
                                if (allProperties[particleTypes[above]].burnable){
                                    if (FastRand() % 300 == 1){
                                            
                                        particleTypes[above] = FIRE;
                                        
                                        pixels[above] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }
                                    touching += 1;
                                }
                                if (allProperties[particleTypes[below]].burnable){
                                    if (FastRand() % 300 == 1){
                                            
                                        particleTypes[below] = FIRE;
                                        
                                        pixels[below] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }
                                    touching += 1;
                                }
                                if (allProperties[particleTypes[belowLeft]].burnable){
                                    if (FastRand() % 300 == 1){
                                            
                                        particleTypes[belowLeft] = FIRE;
                                        
                                        pixels[belowLeft] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }
                                    touching += 1;
                                }
                                if (allProperties[particleTypes[belowRight]].burnable){
                                    if (FastRand() % 300 == 1){
                                            
                                        particleTypes[belowRight] = FIRE;
                                        
                                        pixels[belowRight] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    }
                                    touching += 1;
                                }
                                
                                
                                
                                else {
                                    switch (FastRand()%4){
                                        default:
                                            // if (particleTypes[above] == NOTHING && FastRand()%40 == 1){
                                            //     particleTypes[above] = FIRE;
                                            //     pixels[above] = allProperties[FIRE].pixelColors[FastRand()%3];
                                            // }
                                            break;
                                        case 0:
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
                                        case 1:
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
                                        case 2:
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
                                            if (FastRand()%5 == 0 && touching == 0){
                                                pixels[x + y * _width] = allProperties[NOTHING].pixelColors[0];
                                                particleTypes[x + y * _width] = NOTHING;   
                                            }
                                            break;
                                    }
                                }
                                if (touching != 0 && particleTypes[x + y * _width] != FIRE){
                                    particleTypes[x + y * _width] = FIRE;
                                    pixels[x + y * _width] = allProperties[FIRE].pixelColors[FastRand()%3];
                                    changed = true;
                                }
                                if (particleTypes[left] == WATER|| particleTypes[right] == WATER || particleTypes[above] == WATER){
                                    pixels[x + y * _width] = allProperties[NOTHING].pixelColors[0];
                                    particleTypes[x + y * _width] = NOTHING;
                                }
                            }
                        }
                        
                        break;
                    case WATER:
                        below = x + (y + 1) * _width;
                        left = (x - 1) + (y) * _width;
                        right = (x + 1) + (y) * _width;
                        belowRight = (x + 1) + (y + 1) * _width;
                        belowLeft = (x - 1) + (y + 1) * _width;
                        changed = (allProperties[particleTypes[below]].shiftable  && particleTypes[below] != WATER) || (allProperties[particleTypes[belowLeft]].shiftable  && particleTypes[belowLeft] != WATER) || (allProperties[particleTypes[belowRight]].shiftable  && particleTypes[belowRight] != WATER) || (allProperties[particleTypes[right]].shiftable) || (allProperties[particleTypes[left]].shiftable);
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
                                else if (allProperties[particleTypes[belowRight]].shiftable){
                                    std::swap(particleTypes[x + y * _width],particleTypes[belowRight]);
                                    std::swap(pixels[x + y * _width], pixels[belowRight]);
                                }
                                
                            }
                            else if (x == _width - 1){
                                if (allProperties[particleTypes[left]].shiftable){
                                    std::swap(particleTypes[x + y * _width],particleTypes[left]);
                                    std::swap(pixels[x + y * _width], pixels[left]);
                                    
                                }
                                else if (allProperties[particleTypes[belowLeft]].shiftable){
                                    std::swap(particleTypes[x + y * _width],particleTypes[belowLeft]);
                                    std::swap(pixels[x + y * _width], pixels[belowLeft]);
                                }
                                
                            }
                            else{
                                if ((int)FastRand()%2 == 0){
                                    if (allProperties[particleTypes[left]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[left]);
                                        std::swap(pixels[x + y * _width], pixels[left]);
                                    
                                    }
                                    else if (allProperties[particleTypes[belowLeft]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[belowLeft]);
                                        std::swap(pixels[x + y * _width], pixels[belowLeft]);
                                    }
                                    else if (allProperties[particleTypes[right]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[right]);
                                        std::swap(pixels[x + y * _width], pixels[right]);
                                    }
                                    else if (allProperties[particleTypes[belowRight]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[belowRight]);
                                        std::swap(pixels[x + y * _width], pixels[belowRight]);
                                    }
                                }
                                else{
                                    if (allProperties[particleTypes[right]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[right]);
                                        std::swap(pixels[x + y * _width], pixels[right]);
                                        
                                    }
                                    else if (allProperties[particleTypes[belowRight]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[belowRight]);
                                        std::swap(pixels[x + y * _width], pixels[belowRight]);
                                    }
                                    else if (allProperties[particleTypes[left]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[left]);
                                        std::swap(pixels[x + y * _width], pixels[left]);
                                    }
                                    else if (allProperties[particleTypes[belowLeft]].shiftable){
                                        std::swap(particleTypes[x + y * _width],particleTypes[belowLeft]);
                                        std::swap(pixels[x + y * _width], pixels[belowLeft]);
                                    }
                                }
                            }
                        }
                        break;
                    case SAND:
                        
                        left = (x - 1) + (y + 1) * _width;
                        below = x + (y + 1) * _width;
                        right = (x + 1) + (y + 1) * _width;
                        bool update = (FastRand()%2 == 1);
                        if (y < _height - 1){
                            //particleTypes[below] = SAND;
                            if (allProperties[particleTypes[below]].shiftable){
                                if (update){
                                    particleTypes[x + y * _width] = particleTypes[below];
                                    particleTypes[below] = SAND;
                                    std::swap(pixels[x + y * _width], pixels[below]);
                                }
                                
                                changed = true;
                            }
                            else if (x == 0){
                                if (allProperties[particleTypes[right]].shiftable){
                                    if (update){
                                        particleTypes[x + y * _width]= particleTypes[right];
                                        particleTypes[right] = SAND;
                                        std::swap(pixels[x + y * _width], pixels[right]);
                                    }
                                    
                                    changed = true;
                                }
                            }
                            else if (x == _width - 1){
                                if (allProperties[particleTypes[left]].shiftable){
                                    if(update){
                                        particleTypes[x + y * _width]= particleTypes[left];
                                        particleTypes[left] = SAND;
                                        
                                        std::swap(pixels[x + y * _width], pixels[left]);
                                    }
                                    changed = true;
                                }
                            }
                            else{
                                if ((int)FastRand()%2 == 0){
                                    if (allProperties[particleTypes[left]].shiftable){
                                        if(update){
                                            particleTypes[x + y * _width]= particleTypes[left];
                                            particleTypes[left] = SAND;
                                            
                                            std::swap(pixels[x + y * _width], pixels[left]);
                                            
                                        }
                                        changed = true;
                                        
                                    }
                                    else if (allProperties[particleTypes[right]].shiftable){
                                        if (update){
                                            particleTypes[x + y * _width]= particleTypes[right];
                                            particleTypes[right] = SAND;
                                            
                                            std::swap(pixels[x + y * _width], pixels[right]);
                                        
                                        }
                                        changed = true;
                                    }
                                }
                                else{
                                    if (allProperties[particleTypes[right]].shiftable){
                                        if (update){
                                            particleTypes[x + y * _width]= particleTypes[right];
                                            particleTypes[right] = SAND;
                                            
                                            std::swap(pixels[x + y * _width], pixels[right]);
                                            
                                        }
                                        changed = true;
                                    }
                                    else if (allProperties[particleTypes[left]].shiftable){
                                        if (update){
                                            particleTypes[x + y * _width]= particleTypes[left];
                                            particleTypes[left] = SAND;
                                            
                                            std::swap(pixels[x + y * _width], pixels[left]);
                                            
                                        }
                                        changed = true;
                                    }
                                }
                            }
                        }
                        break;
                }
                    
            }
            
        }
    }
    return changed;
}

