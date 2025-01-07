#include "Utilities.h"
#include "particles.cpp"

class CaveGenerator{
public:
    PARTICLETYPES * map;
    int fillPercent;
    PARTICLETYPES wallType;
    CaveGenerator(int _width, int _height, int _fillPercent, PARTICLETYPES _wallType){
        width = _width;
        height = _height;
        fillPercent = _fillPercent;
        wallType = _wallType;
    }
    void init(){
        map = (PARTICLETYPES *)malloc(width * height * sizeof(PARTICLETYPES));
        for (int i = 0; i < width * height; ++i){
            if (FastRand() % 100 <= fillPercent){
                map[i] = wallType;
            }
            else{
                map[i] = NOTHING;
            }
        }
    }
    PARTICLETYPES * getMap(){
        return map;
    }
        void step(int steps){
        for (int i = 0; i < steps; i++){
            int sum;
            PARTICLETYPES * newMap = (PARTICLETYPES *)malloc(width * height * sizeof(PARTICLETYPES));
            
            for (int x = 0; x < width ; ++x){
                for (int y = 0; y < height; ++y){
                    if (x <= 1) map[x + y * width] = wallType;
                    if (y <= 1) map[x + y * width] = wallType;
                    if (x >= width - 2) map[x + y * width] = wallType;
                    if (y >= height - 2) map[x + y * width] = wallType;
                    sum = getNeighbors(x , y);
                    if (map[x + y * width] == wallType && sum < 4){
                        newMap[x + y * width] = NOTHING;
                    }
                    else if (map[x + y * width] == NOTHING && sum >= 5){
                        newMap[x + y * width] = wallType;
                    }
                    else{
                        newMap[x + y * width] = map[x + y * width];
                    }
                    if (x <= 1) map[x + y * width] = wallType;
                    if (y <= 1) map[x + y * width] = wallType;
                    if (x >= width - 2) map[x + y * width] = wallType;
                    if (y >= height - 2) map[x + y * width] = wallType;
                    //map[x + y * width] = newMap[x + y * width];
                }
            }
            for (int i2 = 0; i2< width * height; i2 ++){
                map[i2] = newMap[i2];
            }
            delete [] newMap;
        }
        
    }

    void stepJagged(int steps){
        for (int i = 0; i < steps; i++){
            int sum;
            PARTICLETYPES * newMap = (PARTICLETYPES *)malloc(width * height * sizeof(PARTICLETYPES));
            
            for (int x = 0; x < width ; ++x){
                for (int y = 0; y < height; ++y){
                    if (x <= 1) map[x + y * width] = wallType;
                    if (y <= 1) map[x + y * width] = wallType;
                    if (x >= width - 2) map[x + y * width] = wallType;
                    if (y >= height - 2) map[x + y * width] = wallType;
                    sum = getNeighbors(x , y);
                    if (map[x + y * width] == wallType && sum <= 4){
                        newMap[x + y * width] = NOTHING;
                    }
                    else if (map[x + y * width] == NOTHING && sum > 4){
                        newMap[x + y * width] = wallType;
                    }
                    else{
                        newMap[x + y * width] = map[x + y * width];
                    }
                    if (x <= 1) map[x + y * width] = wallType;
                    if (y <= 1) map[x + y * width] = wallType;
                    if (x >= width - 2) map[x + y * width] = wallType;
                    if (y >= height - 2) map[x + y * width] = wallType;
                    //map[x + y * width] = newMap[x + y * width];
                }
            }
            for (int i2 = 0; i2< width * height; i2 ++){
                map[i2] = newMap[i2];
            }
            delete [] newMap;
        }
        
    }
    std::vector<std::pair<int,int>> getRegionTiles(int startX, int startY){
        std::vector<std::pair<int,int>> tiles;
        std::map<int,int> mapFlags;

        std::vector<std::pair<int,int>> queue;
        queue.push_back(std::pair<int,int>(startX, startY));
        mapFlags[startX + startY * width] = 1;
        while (queue.size() > 0){
            std::pair<int,int> tile = queue.back();
            tiles.push_back(tile);
            queue.pop_back();
            for (int x = tile.first - 1; x <= tile.first + 1; ++x){
                for (int y = tile.second - 1; y <= tile.second + 1; ++y){
                    if (x >= 0 && x < width && y >= 0 && y < height && (y == tile.second || x == tile.first)){
                        if (mapFlags[x + y * width] == 0 && map[x + y * width] == wallType){
                            mapFlags[x + y * width] = 1;
                            queue.push_back(std::pair<int,int>(x, y));
                        }
                    }
                }
            }
        }
        return tiles;
    }
    std::vector<std::vector<std::pair<int,int>>> getRegions(){
        std::vector<std::vector<std::pair<int,int>>> regions;
        std::map<int,int> mapFlags;
        for (int x = 0; x < width; ++x){
            for (int y = 0; y < height; ++y){
                if (mapFlags[x + y * width] == 0 && map[x + y * width] == wallType){
                    std::vector<std::pair<int,int>> newRegion = getRegionTiles(x, y);
                    regions.push_back(newRegion);

                    for (std::pair<int,int> tile : newRegion){
                        mapFlags[tile.first, tile.second] = 1;
                    }
                }
            } 
        }
        return regions;
    }
    //WARNING: VERY SLOW
    void processMap(){
        //Need to increase speed
        std::vector<std::vector<std::pair<int,int>>> wallRegions = getRegions();
        int wallThresholdSize = 50;
        for (std::vector<std::pair<int,int>> wallRegion : wallRegions){
            if (wallRegion.size() < wallThresholdSize){
                for (std::pair<int,int> tile : wallRegion){
                    map[tile.first + tile.second * width] = NOTHING;
                }
            }
        }
    }
    void deleteMap(){
        delete [] map;
    }
private:
    int width;
    int height;

    int getNeighbors(int x, int y) {
        int neighbors = 0;
        if((x - 1 >= 0 && y - 1 >= 0 && map[x - 1 + (y - 1) * width] == wallType) || (x - 1 < 0 && y - 1 < 0)) {
            neighbors += 1;
        }
        if((y - 1 >= 0 && map[x + (y - 1) * width] == wallType) || (y - 1 < 0)) {
            neighbors += 1;
        }
        if((x + 1 < width && y - 1 >= 0 && map[x + 1 + (y - 1 ) * width] == wallType) || (x + 1 >= width && y - 1 < 0)) {
            neighbors += 1;
        }
        if((x - 1 >= 0 && map[x - 1 + y* width] == wallType) || (x - 1 < 0)) {
            neighbors += 1;
        }
        if((x + 1 < width && map[x + 1 + y * width] == wallType) || (x + 1 >= width)) {
            neighbors += 1;
        }
        if((x - 1 >= 0 && y + 1 < height && map[x - 1 + ( y + 1 ) * width] == wallType) || (x - 1 < 0 && y + 1 >= height)) {
            neighbors += 1;
        }
        if((y + 1 < height && map[x + (y + 1) * width] == wallType) || (y + 1 >= height)) {
            neighbors += 1;
        }
        if((x + 1 < width && y + 1 < height && map[x + 1 + (y + 1) * width] == wallType) || (x + 1 >= width && y + 1 >= height)) {
            neighbors += 1;
        }
        return neighbors;
    }
};