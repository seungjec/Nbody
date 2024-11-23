#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    SDL_Log("==== SDL_START ====");

    const int window_w = 800;
    const int window_h = 600;
    const int FPS = 500;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("SDL_Init : %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("N body", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, 0);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow : %s", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        SDL_Log("SDL_Renderer : %s", SDL_GetError());
        return -1;
    }

    SDL_PixelFormatEnum format = SDL_PIXELFORMAT_RGBA32;
    SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING;
    SDL_Texture* textureBackground = SDL_CreateTexture(renderer, format, access, window_w, window_h);

    size_t buffer_size = sizeof(Uint8) * window_w * window_h * 4;
    Uint8* buffer = NULL;

    SDL_Event event;
    Uint64 frameStart, frameTime, frameDelay, frameCount;
    frameDelay = 1000 / FPS;
    frameCount = 0;

    SDL_Point mouse_position{ -1, -1 };
    bool mouse_clicked = false;
    bool key_space_down = false;

    bool isRunning = true;
    while (isRunning)
    {
        frameStart = SDL_GetTicks64();

        // Get Event
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;

        // Keyboard event //
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            case SDLK_SPACE:
                key_space_down = true;
                SDL_Log("Space key down\n");
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                key_space_down = false;
                SDL_Log("Space key up\n");
                break;
            }
            break;

        // Mouse event //
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                mouse_clicked = true;
                SDL_Log("Mouse Left button Down\n");
                SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                mouse_clicked = false;
                SDL_Log("Mouse Left button Up\n");
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            if (mouse_clicked)
                SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
            break;
        }

        // Update

        if (1)//(mouse_clicked)
        {
            int pitch = window_w * sizeof(Uint8) * 4;
            SDL_LockTexture(textureBackground, NULL, (void**)& buffer, &pitch);
            if (mouse_position.x >= 0 && mouse_position.x < window_w &&
                mouse_position.y >= 0 && mouse_position.y < window_h)
            {
                buffer[mouse_position.x * 4 + 0 + window_w * 4 * mouse_position.y] = 255;
                buffer[mouse_position.x * 4 + 1 + window_w * 4 * mouse_position.y] = 255;
                buffer[mouse_position.x * 4 + 2 + window_w * 4 * mouse_position.y] = 255;
            }
            SDL_UnlockTexture(textureBackground);
        }

        if (key_space_down)
        {
            int pitch = window_w * sizeof(Uint8) * 4;
            if (SDL_LockTexture(textureBackground, NULL, (void**)&buffer, &pitch) != 0)
            {
                SDL_Log("SDL_LockTexture : %s", SDL_GetError());
                return -1;
            }
            memset(buffer, 0, buffer_size);
            SDL_UnlockTexture(textureBackground);
        }

        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureBackground, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Control frame rate
        frameTime = SDL_GetTicks64() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(Uint32(frameDelay - frameTime));
        ++frameCount;
    }

    // Finalized
    if (textureBackground != NULL)
        SDL_DestroyTexture(textureBackground);

    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (window != NULL)
        SDL_DestroyWindow(window);

    SDL_Log("==== SDL_END ====");
    return 0;
}