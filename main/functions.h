#pragma once

#include <SDL.h>
#include <chrono>

struct SDL_Params
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* textureBackground = NULL;

    int window_w;
    int window_h;
    SDL_Rect rectRedBox;
    SDL_Rect rectGreenBox;
    SDL_Rect rectBlueBox;
    SDL_Rect rectColorBox;

    size_t buffer_size;
    Uint8* buffer = NULL;

    SDL_Event event;

    int FPS;
    Uint64 frameStart, frameTime, frameDelay, frameCount;
    std::chrono::system_clock::time_point start;

    SDL_PixelFormatEnum format;
    SDL_TextureAccess access;

    SDL_Point mouse_position;
    SDL_Point last_position;
    bool mouse_clicked;
    bool key_space_down;

    SDL_Color color;
    SDL_Color color_mode;
    bool isRunning;
};

void Initialize(SDL_Params* params);
void Finalize(SDL_Params* params);
void EventProcess(SDL_Params* params);
void UpdateProcess(SDL_Params* params);
void RenderProcess(SDL_Params* params);
void ControlFrameRateProcess(SDL_Params* params, bool checkTime);

void FillBuffer(Uint8* buffer, int x, int y, int w, int h, const SDL_Color* color);
void DrawPoint(SDL_Params* params, const SDL_Color* color);
void DrawLine(SDL_Params* params, const SDL_Color* color);

void GetError(int err, const char* msg, int nline);
void setElapsedTime(std::chrono::system_clock::time_point& start);
float getElapsedTime(const char* msg, std::chrono::system_clock::time_point& start, bool print = false, bool restart = false);

