#include "src/include/SDL.h"

#undef main

#include "Utilities.h"
#include "fluid.cpp"
#include "particles.cpp"
#include "UI.cpp"
using namespace PARTICLES;
using namespace UI;


int slice = 0;

SDL_Window* CreateCenteredWindow(uint32_t width, uint32_t height, std::string title)
{
    // Get current deviced's Display Mode to calculate window position
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
    for (auto it = sprites.begin(); it != sprites.end(); ++it){
        SDL_FreeSurface(sprites[it->first]);
    }
}

// Initialize SDL Components 
int32_t Startup(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture)
{
    SDL_Init(SDL_INIT_VIDEO);

    addSprite("Water drop-mini", "Sprites/WaterDrop-mini.bmp", 16);
    assignSpritePositions("Water drop-mini", g_kSelectSlices, 1, g_kSelectRadius);

    addSprite("Sand-mini", "Sprites/Sand-mini.bmp", 16);
    assignSpritePositions("Sand-mini", g_kSelectSlices, 0, g_kSelectRadius);

    addSprite("Stone-mini", "Sprites/Stone-mini.bmp", 16);
    assignSpritePositions("Stone-mini", g_kSelectSlices, 2, g_kSelectRadius);
    
    addSprite("Log-mini", "Sprites/Log-mini.bmp", 16);
    assignSpritePositions("Log-mini", g_kSelectSlices, 3, g_kSelectRadius);

    
    addSprite("Fire-mini", "Sprites/Fire-mini.bmp", 16);
    assignSpritePositions("Fire-mini", g_kSelectSlices, 4, g_kSelectRadius);
    
    addSprite("Plant-mini", "Sprites/Plant-mini.bmp", 16);
    assignSpritePositions("Plant-mini", g_kSelectSlices, 5, g_kSelectRadius);

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
int32_t Render(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTexture, uint32_t * pixels)
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

void SetUI(bool shiftDown, int UIPoints, int selectPointX, int selectPointY, uint32_t * renderPixels, int * slice, int mouseX, int mouseY){
    int pixelR;
    int pixelG;
    int pixelB;
    int pixelAlpha;
    int layerR;
    int layerG;
    int layerB;
    int layerAlpha;
    if (shiftDown){                        
        for (uint32_t i = 0; i < UIPoints; ++i){
            //std::cout<< mouseY + pointsInCircle[i].second <<std::endl;
            if (selectPointX + pointsInCircle[i].first >= 0 && selectPointX + pointsInCircle[i].first < g_kRenderWidth && selectPointY + pointsInCircle[i].second >= 0 && selectPointY + pointsInCircle[i].second < g_kRenderHeight){
                pixelR = ( *(RGB *)&renderPixels[(selectPointX + pointsInCircle[i].first) + (selectPointY + pointsInCircle[i].second) * g_kRenderWidth]).r * 0.5;
                pixelG = ( *(RGB *)&renderPixels[(selectPointX + pointsInCircle[i].first) + (selectPointY + pointsInCircle[i].second) * g_kRenderWidth]).g * 0.5;
                pixelB = ( *(RGB *)&renderPixels[(selectPointX + pointsInCircle[i].first) + (selectPointY + pointsInCircle[i].second) * g_kRenderWidth]).b * 0.5;
                pixelAlpha = ( *(RGB *)&renderPixels[(selectPointX + pointsInCircle[i].first) + (selectPointY + pointsInCircle[i].second) * g_kRenderWidth]).alpha * 0.5;
                layerR = ( *(RGB *)&colors[0]).r * (1 - 0.5);
                layerG = ( *(RGB *)&colors[0]).g * (1 - 0.5);
                layerB = ( *(RGB *)&colors[0]).b * (1 - 0.5);
                layerAlpha = ( *(RGB *)&colors[0]).alpha * (1 - 0.5);
                renderPixels[(selectPointX + pointsInCircle[i].first) + (selectPointY + pointsInCircle[i].second) * g_kRenderWidth] = (ARGB(pixelR + layerR, pixelG + layerG, pixelB + layerB, pixelAlpha + layerAlpha));
            }
        }
        for (uint32_t i = 0; i < g_kSelectPixelsPerSlice * g_kSelectSlices; ++i){
            if (selectPointX + pointsOnCircle[i].first >= 0 && selectPointX + pointsOnCircle[i].first < g_kRenderWidth  && selectPointY + pointsOnCircle[i].second >= 0 && selectPointY + pointsOnCircle[i].second < g_kRenderHeight) renderPixels[(selectPointX + pointsOnCircle[i].first) + (selectPointY + pointsOnCircle[i].second) * g_kRenderWidth] = colors[11];
        }
        *slice = getSlice(std::pair<int,int>(mouseX - selectPointX, mouseY - selectPointY), g_kSelectSlices, g_kSelectRadius);
        for (uint32_t i = 0; i < pointsOnSlice.size(); i++){
            if (selectPointX + pointsOnSlice[i].first >= 0 && selectPointX + pointsOnSlice[i].first < g_kRenderWidth && selectPointY + pointsOnSlice[i].second >= 0 && selectPointY + pointsOnSlice[i].second < g_kRenderWidth){
                renderPixels[(selectPointX + pointsOnSlice[i].first + (selectPointY + pointsOnSlice[i].second) * g_kRenderWidth)] = colors[11];
            }
        }
        for (int x = 0; x < 16; ++x){
            for (int y = 0; y < 16; ++y){
                if (spritePixels["Water drop-mini"][std::pair<int,int>(x,y)] != 0 && x + selectPointX + spritePositions["Water drop-mini"].first >= 0 && x + selectPointX + spritePositions["Water drop-mini"].first < g_kRenderWidth && y + selectPointY + spritePositions["Water drop-mini"].second >= 0 && y + selectPointY + spritePositions["Water drop-mini"].second < g_kRenderHeight) renderPixels[((x + selectPointX + spritePositions["Water drop-mini"].first) + (y + selectPointY + spritePositions["Water drop-mini"].second) * g_kRenderWidth)] = spritePixels["Water drop-mini"][std::pair<int,int>(x,y)];
                if (spritePixels["Sand-mini"][std::pair<int,int>(x,y)] != 0 && x + selectPointX + spritePositions["Sand-mini"].first >= 0 && x + selectPointX + spritePositions["Sand-mini"].first < g_kRenderWidth && y + selectPointY + spritePositions["Sand-mini"].second >= 0 && y + selectPointY + spritePositions["Sand-mini"].second < g_kRenderHeight) renderPixels[((x + selectPointX + spritePositions["Sand-mini"].first) + (y + selectPointY + spritePositions["Sand-mini"].second) * g_kRenderWidth)] = spritePixels["Sand-mini"][std::pair<int,int>(x,y)];
                if (spritePixels["Stone-mini"][std::pair<int,int>(x,y)] != 0 && x + selectPointX + spritePositions["Stone-mini"].first >= 0 && x + selectPointX + spritePositions["Stone-mini"].first < g_kRenderWidth && y + selectPointY + spritePositions["Stone-mini"].second >= 0 && y + selectPointY + spritePositions["Stone-mini"].second < g_kRenderHeight) renderPixels[((x + selectPointX + spritePositions["Stone-mini"].first) + (y + selectPointY + spritePositions["Stone-mini"].second) * g_kRenderWidth)] = spritePixels["Stone-mini"][std::pair<int,int>(x,y)];
                if (spritePixels["Log-mini"][std::pair<int,int>(x,y)] != 0 && x + selectPointX + spritePositions["Log-mini"].first >= 0 && x + selectPointX + spritePositions["Log-mini"].first < g_kRenderWidth && y + selectPointY + spritePositions["Log-mini"].second >= 0 && y + selectPointY + spritePositions["Log-mini"].second < g_kRenderHeight) renderPixels[((x + selectPointX + spritePositions["Log-mini"].first) + (y + selectPointY + spritePositions["Log-mini"].second) * g_kRenderWidth)] = spritePixels["Log-mini"][std::pair<int,int>(x,y)];
                if (spritePixels["Fire-mini"][std::pair<int,int>(x,y)] != 0 && x + selectPointX + spritePositions["Fire-mini"].first >= 0 && x + selectPointX + spritePositions["Fire-mini"].first < g_kRenderWidth && y + selectPointY + spritePositions["Fire-mini"].second >= 0 && y + selectPointY + spritePositions["Fire-mini"].second < g_kRenderHeight) renderPixels[((x + selectPointX + spritePositions["Fire-mini"].first) + (y + selectPointY + spritePositions["Fire-mini"].second) * g_kRenderWidth)] = spritePixels["Fire-mini"][std::pair<int,int>(x,y)];
                if (spritePixels["Plant-mini"][std::pair<int,int>(x,y)] != 0 && x + selectPointX + spritePositions["Plant-mini"].first >= 0 && x + selectPointX + spritePositions["Plant-mini"].first < g_kRenderWidth && y + selectPointY + spritePositions["Plant-mini"].second >= 0 && y + selectPointY + spritePositions["Plant-mini"].second < g_kRenderHeight) renderPixels[((x + selectPointX + spritePositions["Plant-mini"].first) + (y + selectPointY + spritePositions["Plant-mini"].second) * g_kRenderWidth)] = spritePixels["Plant-mini"][std::pair<int,int>(x,y)];
                
            }
        }
    }

}

Uint32 initial_ticks;
uint32_t * pixels;
uint32_t * renderPixels;

uint32_t pixelR;
uint32_t pixelG;
uint32_t pixelB;
uint32_t pixelAlpha;
uint32_t layerR;
uint32_t layerG;
uint32_t layerB;
uint32_t layerAlpha;

PARTICLETYPES * particles;


bool leftMouseDown = false;
bool rightMouseDown = false;
bool shiftDown = false;
bool updated = false;
bool mouseDown = false;
int selectPointX = 0;
int selectPointY = 0;
int mouseX = 0;
int mouseY = 0;
int prevMouseX = 0;
int prevMouseY = 0;
double magnitude;
CaveGenerator caveGenerator(g_kRenderWidth, g_kRenderHeight, 48, WOOD);
CaveGenerator tempCave(g_kRenderWidth, g_kRenderHeight, 48, WALL);
PARTICLETYPES currentCreate = SAND;

double * Vx;
double * Vy;
double * Vx0;
double * Vy0;
double * density;
double * s;
double visc = 0.0000001;
double diff = 0.000001;
double dt = 0.2;


bool diffusionMap = false;
int main()
{
    Vx = (double *)malloc(sizeof(double) * g_kRenderWidth * g_kRenderHeight);
    Vy = (double *)malloc(sizeof(double) * g_kRenderWidth * g_kRenderHeight);
    Vx0 = (double *)malloc(sizeof(double) * g_kRenderWidth * g_kRenderHeight);
    Vy0 = (double *)malloc(sizeof(double) * g_kRenderWidth * g_kRenderHeight);
    density = (double *)malloc(sizeof(double) * g_kRenderWidth * g_kRenderHeight);
    s = (double *)malloc(sizeof(double) * g_kRenderWidth * g_kRenderHeight);
    
    pixels = (uint32_t *)malloc(sizeof(uint32_t) * g_kRenderWidth * g_kRenderHeight);
    renderPixels = (uint32_t *)malloc(sizeof(uint32_t) * g_kRenderWidth * g_kRenderHeight);
    particles = (PARTICLETYPES *)malloc(sizeof(PARTICLETYPES) * g_kRenderWidth * g_kRenderHeight);
    //initUI(g_kSelectSlices);
    getPointsInsideCircle(g_kSelectRadius);
    getPointsOnCircle(g_kSelectRadius, g_kSelectPixelsPerSlice * g_kSelectSlices);
    //getPointsOnCircleSlice(g_kSelectRadius * 2, 2 * M_PI * g_kSelectRadius, 0);
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
    caveGenerator.init();
    caveGenerator.step(50);
    particles = caveGenerator.getMap();
    for (int i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
        pixels[i] = allProperties[particles[i]].pixelColors[FastRand()%3];
        //pixels[i] = colors[1];
    }
    SDL_GL_SetSwapInterval(1);
    initial_ticks = SDL_GetTicks();
    while (running)
    {
        if (!firstFrame)
        {
            
            if (!diffusionMap){
                if (!updated){
                    
                    updated = true;
                    for (int x = g_kRenderWidth - 1; x >= 0; --x){
                        for (int y = g_kRenderHeight - 1; y >= 0; --y){
                            particleUpdate(particles, g_kRenderWidth, g_kRenderHeight, pixels, colors,x , y);
                        }
                    }
                }
                if (SDL_GetTicks() - initial_ticks > g_kMillisecondsPerFrame){
                
                    //particleUpdate(particles, g_kRenderWidth, g_kRenderHeight, pixels, colors);
                    for (uint32_t i = 0; i < g_kRenderWidth * g_kRenderHeight; ++i)
                        renderPixels[i] = pixels[i];
                    std::thread UIThread(SetUI, shiftDown, UIPoints, selectPointX, selectPointY, renderPixels, &slice, mouseX, mouseY);
                    SDL_Event event;
                    while (SDL_PollEvent(&event))
                    {
                        
                            if (event.type == SDL_MOUSEMOTION){
                                mouseX = floor(event.button.x / (g_kWindowWidth / g_kRenderWidth));
                                mouseY = floor(event.button.y / (g_kWindowHeight / g_kRenderHeight));
                                
                            }
                            if (event.type == SDL_QUIT){
                                running = false;
                            }
                            if (event.type == SDL_KEYDOWN){
                                switch(event.key.keysym.sym){
                                    default:
                                        break;
                                    case SDLK_ESCAPE:
                                        running = false;
                                        break;
                                    
                                    case SDLK_SPACE:
                                        tempCave.init();
                                        tempCave.step(50);
                                        particles = tempCave.getMap();
                                        for (int i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
                                            pixels[i] = allProperties[particles[i]].pixelColors[FastRand()%3];
                                            //pixels[i] = colors[1];
                                        } 
                                        break;
                                    case SDLK_LSHIFT:
                                        shiftDown = true;
                                        
                                        break;
                                }
                            }
                            if (event.type == SDL_KEYUP){
                                switch(event.key.keysym.sym){
                                    default:
                                        break;
                                    case SDLK_LSHIFT:
                                        shiftDown = false;
                                        break;
                                }
                            }
                            if (event.type == SDL_MOUSEBUTTONDOWN){
                                switch(event.button.button){
                                    default:
                                        break;
                                    case SDL_BUTTON_LEFT:
                                        leftMouseDown = true;
                                        if (shiftDown){
                                            std::cout<<slice<<std::endl;
                                            switch (slice){
                                                default:
                                                    break;
                                                case 0:
                                                    currentCreate = SAND;
                                                    break;
                                                case 1:
                                                    currentCreate = WATER;
                                                    break;
                                                case 2:
                                                    currentCreate = WALL;
                                                    break;
                                                case 3:
                                                    currentCreate = WOOD;
                                                    break;
                                                case 4:
                                                    currentCreate = FIRE;
                                                    break;
                                                case 5:
                                                    currentCreate = PLANT;
                                                    break;
                                            }
                                        }
                                        break;
                                    case SDL_BUTTON_RIGHT:
                                        rightMouseDown = true;
                                        break;
                                }
                            }
                                
                            if (event.type == SDL_MOUSEBUTTONUP){
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

                    if (leftMouseDown && shiftDown == false){
                        addParticle(particles, mouseX, mouseY, g_kRenderWidth, pixels, colors, currentCreate);
                    }
                    if (rightMouseDown && totalFramesRendered % 2 == 0 && shiftDown == false){
                        removeParticle(particles, mouseX, mouseY, g_kRenderWidth, pixels, colors);
                    }
                    if (shiftDown == false){
                        selectPointX = mouseX;
                        selectPointY = mouseY;
                    }
                    
                    UIThread.join();
                    e(Render(pWindow, pRenderer, pTexture, renderPixels), "Render failed\n");
                    
                    updated = false;
                    initial_ticks = SDL_GetTicks();
                    int64_t currentTick = SDL_GetPerformanceCounter();
                    totalTicks += currentTick - lastTick;
                    lastTick = currentTick;
                    ++totalFramesRendered;
                }
            
                
            }
            // else{
            //     if (!updated){
            //         int64_t currentTick = SDL_GetPerformanceCounter();
            //         totalTicks += currentTick - lastTick;
            //         lastTick = currentTick;
            //         ++totalFramesRendered;
            //         int cx = (int)(0.5 * g_kRenderWidth);
            //         int cy = (int)(0.5 * g_kRenderHeight);
            //         double scalar = FastRand()%100/100+0.5;
            //         for (int i = -1; i <= 1; ++i){
            //             for (int j = -1; j <= 1; ++j){
            //                 FLUID::addDensity(cx + i, cy + i, g_kRenderWidth, FastRand()%400 + 100, density);
            //             }
            //         }
            //         FLUID::addVelocity(cx, cy, g_kRenderWidth, 500 *(mouseX - cx), 500 * (mouseY - cy), Vx, Vy);
                            
            //         FLUID::diffuse(1, Vx0, Vx, visc, dt, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::diffuse(2, Vy0, Vy, visc, dt, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::project(Vx0, Vy0, Vx, Vy, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::advect(1, Vx, Vx0, Vx0, Vy0, dt, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::advect(2, Vy, Vy0, Vx0, Vy0, dt, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::project(Vx, Vy, Vx0, Vy0, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::diffuse(0, s, density, diff, dt, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::advect(0, density, s, Vx, Vy, dt, g_kRenderWidth, g_kRenderHeight);
            //         FLUID::fadeDensity(density);
            //         FLUID::fadeDensity(s);
            //         for (int i = 0;i < g_kRenderWidth * g_kRenderHeight; ++i){
            //             pixelR = std::min(255.0, s[i]);
            //             pixelG = std::min(255.0, s[i]);
            //             pixelB = std::min(255.0, s[i]);
                        
            //             // pixelR = std::max(0.0,std::min(255.0, Vx[i] * 10));
            //             // pixelG = std::max(0.0,std::min(255.0, Vx[i] * 10));
            //             // pixelB = std::max(0.0,std::min(255.0, Vx[i] * 10));
                        

            //             pixelAlpha = ( *(RGB *)&colors[1]).alpha;
            //             renderPixels[i] = ARGB(pixelR, pixelG, pixelB, pixelAlpha);
                        
            //         }
                    
            //         updated = true;
            //     }
            //     if (SDL_GetTicks() - initial_ticks > g_kMillisecondsPerFrame){
            //         updated = false;
            //         initial_ticks = SDL_GetTicks();
            //     }
            //     std::thread renderThread(e, Render(pWindow, pRenderer, pTexture, renderPixels), "Render failed\n");
                
            //     SDL_Event event;
            //     while (SDL_PollEvent(&event)){
            //         if (event.type == SDL_QUIT){
            //             running = false;
            //         }
            //         if (event.type == SDL_KEYDOWN ){
            //             switch (event.key.keysym.sym){
            //                 default:
            //                     break;
            //                 case SDLK_ESCAPE:
            //                     running = false;
            //                     break;
            //             }
            //         }
            //         if (event.type == SDL_MOUSEBUTTONDOWN){
            //             mouseDown = true;
            //         }
            //         if (event.type == SDL_MOUSEBUTTONUP){
            //             mouseDown = false;
            //         }
            //         if (event.type == SDL_MOUSEMOTION){
            //             mouseX = floor(event.button.x / (g_kWindowWidth / g_kRenderWidth));
            //             mouseY = floor(event.button.y / (g_kWindowHeight / g_kRenderHeight));
                                
            //         }
            //     }
            //     renderThread.join();

            // }
                
            
        }
        else
        {
            if (diffusionMap){
                for (uint32_t i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
                    Vx[i] = 0;
                    Vy[i] = 0;
                    Vx0[i] = 0;
                    Vy0[i] = 0;
                    density[i] = 0;
                    s[i] = 0;
                }
            }
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




