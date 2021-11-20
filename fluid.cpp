#include "Utilities.h"


struct FLUIDVALUES{
    double diffusion;
    std::pair<double,double> velocity;
};
namespace FLUID{
    double calculateDiffusionDensity(int x, int y, int width, int height, FLUIDVALUES * fluid, double k){
        // Surrounding fluid[(x + 1) + y * width].diffusion + fluid[(x - 1) + y * width].diffusion + fluid[(x) + (y + 1) * width].diffusion + fluid[(x) + (y - 1) * width].diffusion) / 4
        return (fluid[x + y * width].diffusion + k * ((
            fluid[std::clamp(x + 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].diffusion + 
            fluid[std::clamp(x - 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].diffusion + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y + 1, 0, height - 1) * width].diffusion + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y - 1, 0, height - 1) * width].diffusion) / 4)) / ( 1 + k );
    }
    std::pair<double,double> calculateDiffusionVelocity(int x, int y, int width, int height, FLUIDVALUES * fluid, double k){
        // Surrounding fluid[(x + 1) + y * width].diffusion + fluid[(x - 1) + y * width].diffusion + fluid[(x) + (y + 1) * width].diffusion + fluid[(x) + (y - 1) * width].diffusion) / 4
        return std::pair<double,double>((fluid[x + y * width].velocity.first + k * ((
            fluid[std::clamp(x + 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].velocity.first + 
            fluid[std::clamp(x - 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].velocity.first + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y + 1, 0, height - 1) * width].velocity.first + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y - 1, 0, height - 1) * width].velocity.first) / 4)) / ( 1 + k ),
            (fluid[x + y * width].velocity.second + k * ((
            fluid[std::clamp(x + 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].velocity.second + 
            fluid[std::clamp(x - 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].velocity.second + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y + 1, 0, height - 1) * width].velocity.second + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y - 1, 0, height - 1) * width].velocity.second) / 4)) / ( 1 + k ));
    }
    FLUIDVALUES * calculateVelocity(int width, int height, FLUIDVALUES * fluid){
        int i, j, i0, j0, i1, j1;
        float x, y, s0, t0, s1, t1, dt0;
        FLUIDVALUES * tempFluid;
        dt0 = 1;
        tempFluid = (FLUIDVALUES *)malloc(sizeof(FLUIDVALUES) * width * height);
        tempFluid = fluid;
        for ( i = 1 ; i < width - 2; ++i){
            for ( j = 1; j < height - 2; ++j ){
                x = i - dt0 * fluid[i + j * width].velocity.first;
                y = j - dt0 * fluid[i + j * width].velocity.second;

                
                if (x < 0.5) x = 0.5;
                if (x > width - 1) x = width - 1 + 0.5;
                if (y < 0.5) y = 0.5;
                if (y > height - 1) y = height - 1 + 0.5;

                i0 = (int)x;
                i1 = i0 + 1;
                j0 = (int)y;
                j1 = j0 + 1;

                s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
                tempFluid[i + j * width].diffusion = s0*(t0*fluid[i0 + j0 * width].diffusion+t1*fluid[i0 +j1 * width].diffusion)+s1*(t0*fluid[i1 + j0 * width].diffusion+t1*fluid[i1 + j1 * width].diffusion); 
            }
        }
        return tempFluid;
    }
}