#pragma once

#include <SDL.h>

struct SDL_Params
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* textureBackground = NULL;

    int window_w;
    int window_h;
    int FPS;

    size_t buffer_size;
    Uint8* buffer = NULL;

    SDL_Event event;
    Uint64 frameStart, frameTime, frameDelay, frameCount;

    SDL_PixelFormatEnum format;
    SDL_TextureAccess access;

    SDL_Point mouse_position;
    SDL_Point last_position;
    bool mouse_clicked;
    bool key_space_down;

    bool isRunning;
};

void Initialize(SDL_Params* params);
void Finalize(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Texture*& textureBackground);
void EventProcess(SDL_Params* params);
void UpdateProcess(SDL_Params* params);
void RenderProcess(SDL_Params* params);
void ControlFrameRateProcess(SDL_Params* params);

void FillBuffer(Uint8* buffer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawLine(SDL_Params* params);
void GetError(int err, const char* msg, int nline);