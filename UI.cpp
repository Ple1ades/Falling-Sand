#include "Utilities.h"


namespace UI{
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
    void getPointsInsideCircle(int r){
        int points = 0;
        int h = r;
        for (int y = -r; y <= r; ++y){
            pointsInCircle.push_back(std::pair<int,int>(0,y));
            ++points;
        }
        for (int dx = 1; dx <= r; ++dx){
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

    // std::vector<std::pair<int,int>> getSlice(std::pair<int,int> point1, std::pair<int,int> point2, int totalPoints, int r){
    //     double theta = ((M_PI * 2)/totalPoints);
    //     double angle = atan2(point1.second - point2.second, point1.first - point2.first) * 180/M_PI;
    //     for (int i = 0; i < totalPoints - 1; i++){
    //         if (i * theta > angle && angle >= (i+1) * theta){
    //             for (int i2 = 0; i2 < r; i2++){
    //                 //pointsOnSlice.push_back(std::pair<int,int>())
    //             }
    //         }
    //     }
    // }
};