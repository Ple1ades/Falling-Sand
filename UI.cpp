#include "Utilities.h"
#include "./src/include/SDL.h"

namespace UI{
    std::map<const char *, SDL_Surface *> sprites;
    std::map<const char *, std::map<std::pair<int,int>,uint32_t>> spritePixels;
    std::map<const char *, std::pair<int,int>> spritePositions;
    std::vector<std::pair<int,int>> pointsOnCircle;
    std::vector<std::pair<int,int>> pointsInCircle;
    std::vector<std::pair<int,int>> pointsOnSlice;
    int UIPoints;
    
    void getPointsOnCircle(int r, int totalPoints){
        double theta = ((M_PI * 2)/totalPoints);
        for (int i = 0; i < totalPoints; i ++){
            double angle = (theta * i);

            pointsOnCircle.push_back(std::pair<int,int>(std::round(r * std::cos(angle)),std::round(r*std::sin(angle))));
            pointsInCircle.push_back(std::pair<int,int>(std::round(r * std::cos(angle))-1,std::round(r*std::sin(angle))-1));
        }
    }
    
    Uint32 getPixel(SDL_Surface *surface, int x, int y, int width)
    {
        return ((Uint32 *)surface->pixels)[(width * y) + x];
    }
    void addSprite(const char * spriteName, const char * file, int width){
        sprites[spriteName] =  SDL_LoadBMP(file);
        for (int x = 0; x < 25; ++x){
            for (int y = 0; y < 25; ++y){
                spritePixels[spriteName][std::pair<int,int>(x,y)] = getPixel(sprites[spriteName], x, y, width);
            }
        }
    }
    void getPointsInsideCircle(int r){
        int points = 0;
        int h = r - 1;
        for (int y = -r + 1; y < r; ++y){
            pointsInCircle.push_back(std::pair<int,int>(0,y));
            ++points;
        }
        for (int dx = 1; dx < r; ++dx){
            while (dx * dx + h*h > r * r && h > 0){
                h--;
            }
            for (int y = -h; y <= h; y++){
                pointsInCircle.push_back(std::pair<int,int>(dx,y));
                pointsInCircle.push_back(std::pair<int,int>(-dx,y));
                ++points;
                ++points;
            }
        }
        
        UIPoints = points;
    }
    std::vector<std::pair<int,int>> getPoints(std::pair<int,int> point1, std::pair<int,int> point2, int r){
        std::vector<std::pair<int,int>> points;
        int ydiff = point1.second - point2.second;
        int xdiff = point1.first - point2.first;
        double slope = (double)(ydiff) / (xdiff);
        double x, y;

        for (double i = 0; i < r; i++){
            y = (slope == 0) ? 0 : -ydiff * i/r;
            x = (slope == 0) ? -xdiff * (i / r) : y / slope;
            points.push_back(std::pair<int,int>((int)round(x) + point1.first, (int)round(y) + point1.second));
        }
        points.push_back(point2);
        return points;
    }
    void assignSpritePositions(const char * spriteName, int totalPoints, int slice, int r){
        spritePositions[spriteName] = std::pair<int,int>(std::pair<int,int>(round(cos((slice + 0.5) * (360/totalPoints) * M_PI / 180) * -r / 1.6) - 6, round(sin((slice + 0.5) * 360/totalPoints * M_PI / 180) * -r / 1.6) - 6));
    }
    int getSlice(std::pair<int,int> point, int totalPoints, int r){
        double theta = (360/totalPoints);
        double angle = (atan2(point.second, point.first) * 180 / M_PI) + 180;
        std::pair<int,int> edgePoint;
        edgePoint = point;
        pointsOnSlice.clear();
        std::vector<std::pair<int,int>> points;
        for (int i = 0; i < totalPoints; i++){
            
            if (theta * i< angle && angle <= theta * (i + 1)){
                
                points = getPoints(std::pair<int,int> (0,0), std::pair<int,int>(cos( ((i) * theta) * M_PI / 180) * -r,sin(((i) * theta) * M_PI / 180) * -r), r);
                pointsOnSlice.insert(pointsOnSlice.end(), points.begin(), points.end());
                
                points = getPoints(std::pair<int,int> (0,0), std::pair<int,int>(cos( ((i + 1 ) * theta) * M_PI / 180) * -r,sin(((i + 1) * theta) * M_PI / 180) * -r), r);
                pointsOnSlice.insert(pointsOnSlice.end(), points.begin(), points.end());
                
                return i;
            }
        }
        return 0;
    }
};