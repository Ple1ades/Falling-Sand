#include "./src/include/SDL.h"

#undef main

#include "Utilities.h"
#include "particles.cpp"
#include "UI.cpp"
using namespace PARTICLES;
using namespace UI;

constexpr static const int32_t g_kWindowWidth             = 1920;
constexpr static const int32_t g_kWindowHeight            = 1080;
constexpr static const int32_t g_kRenderWidth             = g_kWindowWidth / 3;
constexpr static const int32_t g_kRenderHeight            = g_kWindowHeight / 3;
constexpr static const int32_t g_kRenderDeviceFlags       = -1;
constexpr static const int32_t g_kErrorOccurred           = -1;

constexpr static const int g_kSelectRadius                = 20;
constexpr static const int g_kSelectPixelsPerSlice        = 10;
constexpr static const int g_kSelectSlices                = 4;

constexpr static const char* g_kWindowTitle =             "PixelPusher";


SDL_Window* CreateCenteredWindow(uint32_t width, uint32_t height, std::string title)
{
    // Get current device's Display Mode to calculate window position
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    // Calculate where the upper-left corner of a centered window will be
    const int32_t x = DM.w / 2 - width / 2;
    const int32_t y = DM.h / 2 - height / 2;

    // Create the SDL window
    SDL_Window* pWindow = SDL_CreateWindow(g_kWindowTitle, x, y, width, height,
        SDL_WINDOW_ALLOW_HIGHDPI);

    if (e(!pWindow, "Failed to create Window\n"));

    return pWindow;
}

// Create SDL renderer and configure whether or not to use Hardware Acceleration
SDL_Renderer* CreateRenderer(SDL_Window* pWindow, bool hardwareAccelerated)
{
    if (hardwareAccelerated)
        return SDL_CreateRenderer(pWindow, g_kRenderDeviceFlags, SDL_RENDERER_ACCELERATED);
    else
        return SDL_CreateRenderer(pWindow, g_kRenderDeviceFlags, SDL_RENDERER_SOFTWARE);
}

// Create an SDL Texture to use as a "back buffer"
SDL_Texture* CreateBackBufferTexture(SDL_Renderer* pRenderer)
{
    SDL_Texture* pTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, g_kRenderWidth, g_kRenderHeight);

    if (e(!pTexture, "Failed to create Back Buffer Texture\n"));

    return pTexture;
}

// Free resources 
void Shutdown(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture)
{
    // Free the Back Buffer
    if (ppTexture)
    {
        SDL_DestroyTexture(*ppTexture);
        *ppTexture = nullptr;
    }

    // Free the SDL renderer
    if (ppRenderer)
    {
        SDL_DestroyRenderer(*ppRenderer);
        *ppRenderer = nullptr;
    }

    // Free the SDL window
    if (ppWindow)
    {
        SDL_DestroyWindow(*ppWindow);
        *ppWindow = nullptr;
    }
}

// Initialize SDL Components 
int32_t Startup(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture)
{
    SDL_Init(SDL_INIT_VIDEO);

    if (e(!ppWindow, "Potiner to Window* was null\n")) return -1;

    *ppWindow = CreateCenteredWindow(g_kWindowWidth, g_kWindowHeight, g_kWindowTitle);

    if (e(!*ppWindow, "No Window. Aborting..."))
    {
        Shutdown(ppWindow, ppRenderer, ppTexture);

        return -1;
    }

    if (e(!ppRenderer, "Pointer to Renderer* was null\n")) return -1;

    *ppRenderer = CreateRenderer(*ppWindow, true);

    if (e(!ppRenderer, "No Renderer. Aborting..."))
    {
        Shutdown(ppWindow, ppRenderer, ppTexture);

        return -1;
    }

    if (e(!ppTexture, "Pointer to Texture* was null\n")) return -1;

    *ppTexture = CreateBackBufferTexture(*ppRenderer);

    if (e(!*ppTexture, "No back buffer Texture. Aborting..."))
    {
        Shutdown(ppWindow, ppRenderer, ppTexture);

        return -1;
    }

    return 0;
}




// Call this within every render loop
// Fills screen with randomly generated colored pixels
int32_t Render(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTexture, uint32_t * pixels, int mouseX, int mouseY, uint32_t colors[])
{
    // The Back Buffer texture may be stored with an extra bit of width (pitch) on the video card in order to properly
    // align it in VRAM should the width not lie on the correct memory boundary (usually four bytes).
    int32_t pitch = 0;

    // This will hold a pointer to the memory position in VRAM where our Back Buffer texture lies
    uint32_t* pPixelBuffer = nullptr;
    SDL_RenderClear(pRenderer);
    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(pTexture, NULL, (void**)&pPixelBuffer, &pitch))
    {
        // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
        // as it will always be a multiple of four
        pitch /= sizeof(uint32_t);
        //memcpy(pPixelBuffer, pixels, sizeof(uint32_t) * g_kRenderHeight * g_kRenderHeight);
        // Fill texture with randomly colored pixels
        for (uint32_t i = 0; i < g_kRenderWidth * g_kRenderHeight; ++i)
            pPixelBuffer[i] = pixels[i];
        // UI
        for (uint32_t i = 0; i < g_kSelectPixelsPerSlice * g_kSelectSlices; ++i){
            if (mouseX + pointsOnCircle[i].first > 0 && mouseX + pointsOnCircle[i].first < g_kRenderWidth - 1 && mouseY + pointsOnCircle[i].second > 0 && mouseY + pointsOnCircle[i].second < g_kRenderHeight - 1) pPixelBuffer[(mouseX + pointsOnCircle[i].first) + (mouseY + pointsOnCircle[i].second) * g_kRenderWidth] = colors[11];
        }
        // Unlock the texture in VRAM to let the GPU know we are done writing to it
        SDL_UnlockTexture(pTexture);

        // Copy our texture in VRAM to the display framebuffer in VRAM
        SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);

        // Copy the VRAM framebuffer to the display
        SDL_RenderPresent(pRenderer);

        return 0;
    }
    else
        return g_kErrorOccurred;
}



class CaveGenerator{
public:
    bool finished;
    PARTICLETYPES * map;
    CaveGenerator(int _width, int _height, int fillPercent){
        width = _width;
        height = _height;
        map = (PARTICLETYPES *)malloc(width * height * sizeof(PARTICLETYPES));
        for (int i = 0; i < _width * _height; ++i){
            if (FastRand() % 100 <= fillPercent){
                map[i] = WALL;
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
        finished = false;
        for (int i = 0; i < steps; i++){
            int sum;
            PARTICLETYPES * newMap = (PARTICLETYPES *)malloc(width * height * sizeof(PARTICLETYPES));
            
            for (int x = 0; x < width ; ++x){
                for (int y = 0; y < height; ++y){
                    if (x <= 1) map[x + y * width] = WALL;
                    if (y <= 1) map[x + y * width] = WALL;
                    if (x >= width - 2) map[x + y * width] = WALL;
                    if (y >= height - 2) map[x + y * width] = WALL;
                    sum = getNeighbors(x , y);
                    if (map[x + y * width] == WALL && sum < 4){
                        newMap[x + y * width] = NOTHING;
                    }
                    else if (map[x + y * width] == NOTHING && sum >= 5){
                        newMap[x + y * width] = WALL;
                    }
                    else{
                        newMap[x + y * width] = map[x + y * width];
                    }
                    if (x <= 1) map[x + y * width] = WALL;
                    if (y <= 1) map[x + y * width] = WALL;
                    if (x >= width - 2) map[x + y * width] = WALL;
                    if (y >= height - 2) map[x + y * width] = WALL;
                    //map[x + y * width] = newMap[x + y * width];
                }
            }
            for (int i2 = 0; i2< width * height; i2 ++){
                map[i2] = newMap[i2];
            }
            delete [] newMap;
        }
        finished = true;
        
    }



    void deleteMap(){
        delete [] map;
    }
private:
    int width;
    int height;

    int getNeighbors(int x, int y) {
        int neighbors = 0;
        if((x - 1 >= 0 && y - 1 >= 0 && map[x - 1 + (y - 1) * width] == WALL) || (x - 1 < 0 && y - 1 < 0)) {
            neighbors += 1;
        }
        if((y - 1 >= 0 && map[x + (y - 1) * width] == WALL) || (y - 1 < 0)) {
            neighbors += 1;
        }
        if((x + 1 < width && y - 1 >= 0 && map[x + 1 + (y - 1 ) * width] == WALL) || (x + 1 >= width && y - 1 < 0)) {
            neighbors += 1;
        }
        if((x - 1 >= 0 && map[x - 1 + y* width] == WALL) || (x - 1 < 0)) {
            neighbors += 1;
        }
        if((x + 1 < width && map[x + 1 + y * width] == WALL) || (x + 1 >= width)) {
            neighbors += 1;
        }
        if((x - 1 >= 0 && y + 1 < height && map[x - 1 + ( y + 1 ) * width] == WALL) || (x - 1 < 0 && y + 1 >= height)) {
            neighbors += 1;
        }
        if((y + 1 < height && map[x + (y + 1) * width] == WALL) || (y + 1 >= height)) {
            neighbors += 1;
        }
        if((x + 1 < width && y + 1 < height && map[x + 1 + (y + 1) * width] == WALL) || (x + 1 >= width && y + 1 >= height)) {
            neighbors += 1;
        }
        return neighbors;
    }

};

uint32_t * pixels;
PARTICLETYPES * particles;
bool leftMouseDown = false;
bool rightMouseDown = false;
bool ctrlDown = false;
int mouseX = 0;
int mouseY = 0;
CaveGenerator caveGenerator(g_kRenderWidth, g_kRenderHeight, 48);
void caveStep(CaveGenerator * cave, int steps){
    cave->step(steps);
}
int main()
{

    pixels = (uint32_t *)malloc(sizeof(uint32_t) * g_kRenderWidth * g_kRenderHeight);
    particles = (PARTICLETYPES *)malloc(sizeof(PARTICLETYPES) * g_kRenderWidth * g_kRenderHeight);
    getPointsOnCircle(g_kSelectRadius, g_kSelectPixelsPerSlice * g_kSelectSlices);
    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;
    SDL_Texture* pTexture = nullptr;

    if (e(Startup(&pWindow, &pRenderer, &pTexture), "Startup Failed. Aborting...\n"))
    {
        Shutdown(&pWindow, &pRenderer, &pTexture);
        return -1;
    }

    bool running = true;
    bool firstFrame = true;

    uint64_t totalTicks = 0;
    uint64_t totalFramesRendered = 0;
    uint64_t lastTick = 0;
    
    caveGenerator.step(50);
    particles = caveGenerator.getMap();
    for (int i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
        pixels[i] = allProperties[particles[i]].pixelColors[FastRand()%3];
        //pixels[i] = colors[1];
    }
    SDL_GL_SetSwapInterval(1);
    while (running)
    {
        if (!firstFrame)
        {
            
            std::thread particleThread(particleUpdate, particles, g_kRenderWidth, g_kRenderHeight, pixels, colors);
            //particleUpdate(particles, g_kRenderWidth, g_kRenderHeight, pixels, colors);
            std::thread renderThread(e, Render(pWindow, pRenderer, pTexture, pixels, mouseX, mouseY, colors), "Render failed\n");

            SDL_Event event;
            
            // Process all events and return whether or not to quit
            while (SDL_PollEvent(&event))
            {
                mouseX = floor(event.button.x / (g_kWindowWidth / g_kRenderWidth));
                mouseY = floor(event.button.y / (g_kWindowHeight / g_kRenderHeight));
                // Handle relevant SDL events
                switch (event.type)
                {
                    
                    default:
                        break;
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym){
                            default:
                                break;
                            case SDLK_ESCAPE:
                                running = false;
                                break;
                            case SDLK_LCTRL:
                                break;
                            case SDLK_SPACE:
                                CaveGenerator tempCave(g_kRenderWidth, g_kRenderHeight, 48);
                                std::thread caveThread(caveStep, &tempCave, 50);
                                if (tempCave.finished){
                                    caveThread.join();
                                    particles = tempCave.getMap();
                                    for (int i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
                                        pixels[i] = allProperties[particles[i]].pixelColors[FastRand()%3];
                                        //pixels[i] = colors[1];
                                    }
                                }
                                //particleUpdate(particles, g_kRenderWidth, g_kRenderHeight, pixels, colors);
                                break;
                            
                        }
                    case SDL_MOUSEBUTTONDOWN:
                        switch(event.button.button){
                            default:
                                break;
                            case SDL_BUTTON_LEFT:
                                leftMouseDown = true;
                                break;
                            case SDL_BUTTON_RIGHT:
                                rightMouseDown = true;
                                break;
                        }
                        break;
                    case SDL_MOUSEBUTTONUP:
                        switch (event.button.button){
                            default:
                                break;
                            case SDL_BUTTON_LEFT:
                                leftMouseDown = false;
                                break;
                            case SDL_BUTTON_RIGHT:
                                rightMouseDown = false;
                                break;
                        }
                    
                }
            }

            if (leftMouseDown && totalFramesRendered % 2 == 0){
                addParticle(particles, mouseX, mouseY, g_kRenderWidth, pixels, colors, SAND);
            }
            if (rightMouseDown && totalFramesRendered % 2 == 0){
                addParticle(particles, mouseX, mouseY, g_kRenderWidth, pixels, colors, WATER);
            }

            uint64_t currentTick = SDL_GetPerformanceCounter();
            totalTicks += currentTick - lastTick;
            lastTick = currentTick;
            ++totalFramesRendered;
            particleThread.join();
            renderThread.join();
        }
        else
        {

            lastTick = SDL_GetPerformanceCounter();
            firstFrame = false;
        }
    }
    
    // Display render and timing information
    std::cout << "Total Frames:    " << totalFramesRendered << "\n";
    std::cout << "Total Time:      " << static_cast<double>(totalTicks) / SDL_GetPerformanceFrequency() << "s\n";
    std::cout << "Average FPS:     " << static_cast<double>(totalFramesRendered)* SDL_GetPerformanceFrequency() / totalTicks << "\n";
    delete [] pixels;
    delete [] particles;
    Shutdown(&pWindow, &pRenderer, &pTexture);
    
    return 0;
}




