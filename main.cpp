#include <string>
#include "./src/include/SDL.h"
#include <math.h>
#undef main

#include "Utilities.h"
#include "pixelPhysics.cpp"


constexpr static const int32_t g_kWindowWidth             = 1920;
constexpr static const int32_t g_kWindowHeight            = 1080;
constexpr static const int32_t g_kRenderWidth             = g_kWindowWidth / 2;
constexpr static const int32_t g_kRenderHeight            = g_kWindowHeight / 2;
constexpr static const int32_t g_kRenderDeviceFlags       = -1;
constexpr static const int32_t g_kErrorOccurred           = -1;

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
int32_t Render(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTexture, uint32_t * pixels)
{
    // The Back Buffer texture may be stored with an extra bit of width (pitch) on the video card in order to properly
    // align it in VRAM should the width not lie on the correct memory boundary (usually four bytes).
    int32_t pitch = 0;

    // This will hold a pointer to the memory position in VRAM where our Back Buffer texture lies
    uint32_t* pPixelBuffer = nullptr;

    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(pTexture, NULL, (void**)&pPixelBuffer, &pitch))
    {
        // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
        // as it will always be a multiple of four
        pitch /= sizeof(uint32_t);

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
uint32_t * pixels;

PARTICLETYPES * particles;
bool leftMouseDown = false;
bool rightMouseDown = false;
int mouseX = 0;
int mouseY = 0;
int main()
{

    pixels = (uint32_t *)malloc(sizeof(uint32_t) * g_kRenderWidth * g_kRenderHeight);
    particles = (PARTICLETYPES *)malloc(sizeof(PARTICLETYPES) * g_kRenderWidth * g_kRenderHeight);
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

    for (int i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
        if ((int)FastRand() % 3 == 1){
            particles[i] = SAND;
        }
        else if ((int)FastRand() % 2 == 1){
            particles[i] = WATER;
        }
        else{
            particles[i] = NOTHING;
        }
    }
    for (int i = 0; i < g_kRenderWidth * g_kRenderHeight;i++){
        switch (particles[i]){
            case NOTHING:
                pixels[i] = allProperties[NOTHING].pixelColors[FastRand()%3];
                break;
            case SAND:
                pixels[i] = allProperties[SAND].pixelColors[FastRand()%3];
                break;
            case WATER:
                pixels[i] = allProperties[WATER].pixelColors[FastRand()%3];
                break;
            default:
                pixels[i] = colors[1];
                break;
        }
        //pixels[i] = colors[1];
    }
    SDL_GL_SetSwapInterval(1);
    while (running)
    {
        if (!firstFrame)
        {
            
            particleUpdate(particles, g_kRenderWidth, g_kRenderHeight, pixels, colors);

            if (e(Render(pWindow, pRenderer, pTexture, pixels), "Render failed\n")) break;

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
                            case SDLK_SPACE:
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

            if (leftMouseDown){
                addParticle(particles, mouseX, mouseY, g_kRenderWidth, pixels, colors, SAND);
            }
            if (rightMouseDown){
                removeParticle(particles, mouseX, mouseY, g_kRenderWidth, pixels, colors);
            }

            uint64_t currentTick = SDL_GetPerformanceCounter();
            totalTicks += currentTick - lastTick;
            lastTick = currentTick;
            ++totalFramesRendered;
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