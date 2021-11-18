#include "Utilities.h"


struct FLUIDVALUES{
    double diffusion;
    std::pair<double,double> velocity;
};
namespace FLUID{
    double calculateDiffusion(int x, int y, int width, int height, FLUIDVALUES * fluid, double k){
        // Surrounding fluid[(x + 1) + y * width].diffusion + fluid[(x - 1) + y * width].diffusion + fluid[(x) + (y + 1) * width].diffusion + fluid[(x) + (y - 1) * width].diffusion) / 4
        return (fluid[x + y * width].diffusion + k * ((
            fluid[std::clamp(x + 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].diffusion + 
            fluid[std::clamp(x - 1, 0, width - 1) + std::clamp(y, 0, height - 1) * width].diffusion + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y + 1, 0, height - 1) * width].diffusion + 
            fluid[std::clamp(x, 0, width - 1) + std::clamp(y - 1, 0, height - 1) * width].diffusion) / 4)) / ( 1 + k );
    }
}