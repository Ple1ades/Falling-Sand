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
            x = (slope == 0) ? xdiff * (9 / r) : y / slope;
            points.push_back(std::pair<int,int>((int)round(x) + point1.first, (int)round(y) + point1.second));
        }
        points.push_back(point2);
        return points;
    }
    void getSlice(std::pair<int,int> point, int totalPoints, int r){
        double theta = (360/totalPoints);
        double angle = (atan2(point.second, point.first) * 180 / M_PI);
        std::pair<int,int> edgePoint;
        edgePoint = point;
        pointsOnSlice.clear();
        std::vector<std::pair<int,int>> points = getPoints(std::pair<int,int>(0,0),point,floor(sqrt(point.first * point.first + point.second * point.second)) + 1);
        pointsOnSlice.insert(pointsOnSlice.end(), points.begin(), points.end());

        for (int i = 0; i < totalPoints; i++){
            if (theta * i< angle && angle <= theta * i + 1){
                
            }
            points = getPoints(std::pair<int,int> (0,0), std::pair<int,int>((int)cos(theta * i * M_PI / 180) * r,(int)cos(theta * (i + 1) * M_PI / 180) * r), r);
            pointsOnSlice.insert(pointsOnSlice.end(), points.begin(), points.end());
            
        }
    }
};