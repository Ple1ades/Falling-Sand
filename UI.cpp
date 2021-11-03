#include "Utilities.h"


namespace UI{
    std::vector<std::pair<int,int>> pointsOnCircle;
    double theta = ((M_PI * 2)/totalPoints);
    void getPointsOnCircle(int r, int totalPoints){
        
        for (int i = 0; i < totalPoints; i ++){
            double angle = (theta * i);

            pointsOnCircle.push_back(std::pair<int,int>(std::round(r * std::cos(angle)),std::round(r*std::sin(angle))));
        }
    }
    void getSlice(std::pair<int,int> point1, std::pair<int,int> point2, int totalPoints){
        double angle = atan2(point1.second - point2.second, point1.first - point2.first) * 180/M_PI;
        for (int i = 0; i < totalPoints - 1; i++){
            if (i * theta)
        }
    }
};