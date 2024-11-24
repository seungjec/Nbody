#include "functions.h"
#include <string>
#include <format>

void Initialize(SDL_Params* params)
{
    // Set parameters
    params->window_w = 800;
    params->window_h = 600;

    int Box_w = 30;
    int Box_h = 15;
    params->rectRedBox = { 0, 0, Box_w, Box_h };
    params->rectGreenBox = { Box_w, 0, Box_w, Box_h };
    params->rectBlueBox = { Box_w * 2, 0, Box_w, Box_h };
    params->rectColorBox = { 0, Box_h, Box_w * 3, Box_h };

    params->buffer_size = sizeof(Uint8) * params->window_w * params->window_h * 4;

    params->FPS = 500;
    params->frameDelay = 1000 / params->FPS;
    params->frameCount = 0;

    params->format = SDL_PIXELFORMAT_RGBA32;
    params->access = SDL_TEXTUREACCESS_STREAMING;

    params->mouse_position = { -1, -1 };
    params->mouse_clicked = false;
    params->key_space_down = false;

    params->color = { 255, 255, 255, 255 };
    params->color_mode = { 0, 0, 0, 0 };

    params->isRunning = true;

    // SDL Initialize
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        GetError(-1, __FUNCTION__, __LINE__);

    params->window = SDL_CreateWindow("N body", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, params->window_w, params->window_h, 0);
    if (params->window == NULL)
        GetError(-1, __FUNCTION__, __LINE__);

    params->renderer = SDL_CreateRenderer(params->window, -1, 0);
    if (params->renderer == NULL)
        GetError(-1, __FUNCTION__, __LINE__);

    params->textureBackground = SDL_CreateTexture(params->renderer, params->format, params->access, params->window_w, params->window_h);
    if (params->textureBackground == NULL)
        GetError(-1, __FUNCTION__, __LINE__);
}

void Finalize(SDL_Params* params)
{
    if (params->textureBackground == NULL)
        GetError(-1, __FUNCTION__, __LINE__);
    SDL_DestroyTexture(params->textureBackground);

    if (params->renderer == NULL)
        GetError(-1, __FUNCTION__, __LINE__);
    SDL_DestroyRenderer(params->renderer);

    if (params->window == NULL)
        GetError(-1, __FUNCTION__, __LINE__);
    SDL_DestroyWindow(params->window);
}

void EventProcess(SDL_Params* params)
{
    SDL_PollEvent(&params->event);
    switch (params->event.type)
    {
    case SDL_QUIT:
        params->isRunning = false;
        break;

    // Keyboard event //
    case SDL_KEYDOWN:
        switch (params->event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            params->isRunning = false;
            break;
        case SDLK_SPACE:
            params->key_space_down = true;
            break;
        case SDLK_1:
            params->color_mode.r = 1;
            break;
        case SDLK_2:
            params->color_mode.g = 1;
            break;
        case SDLK_3:
            params->color_mode.b = 1;
            break;
        }
        break;
    case SDL_KEYUP:
        switch (params->event.key.keysym.sym)
        {
        case SDLK_SPACE:
            params->key_space_down = false;
            break;
        case SDLK_1:
            params->color_mode.r = 0;
            break;
        case SDLK_2:
            params->color_mode.g = 0;
            break;
        case SDLK_3:
            params->color_mode.b = 0;
            break;
        }
        break;

    // Mouse event //
    case SDL_MOUSEBUTTONDOWN:
        switch (params->event.button.button)
        {
        case SDL_BUTTON_LEFT:
            params->mouse_clicked = true;
            SDL_GetMouseState(&params->mouse_position.x, &params->mouse_position.y);
            params->last_position = params->mouse_position;
            break;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        switch (params->event.button.button)
        {
        case SDL_BUTTON_LEFT:
            params->mouse_clicked = false;
            break;
        }
        break;
    case SDL_MOUSEMOTION:
        if (params->mouse_clicked)
        {
            SDL_GetMouseState(&params->mouse_position.x, &params->mouse_position.y);
        }
        break;
    case SDL_MOUSEWHEEL:
        if (params->color_mode.r == 1)
        {
            params->color.r += params->event.wheel.y * 10;
        }
        if (params->color_mode.g == 1)
        {
            params->color.g += params->event.wheel.y * 10;
        }
        if (params->color_mode.b == 1)
        {
            params->color.b += params->event.wheel.y * 10;
        }

        //printf("params->color.r : %d\n", params->color.r);
        break;
    }
}

void UpdateProcess(SDL_Params* params)
{
    if (params->mouse_clicked)
    {
        int pitch = params->window_w * sizeof(Uint8) * 4;
        if (SDL_LockTexture(params->textureBackground, NULL, (void**)&params->buffer, &pitch) != 0)
            GetError(-1, __FUNCTION__, __LINE__);

        DrawPoint(params, &params->color);
        DrawLine(params, &params->color);

        SDL_UnlockTexture(params->textureBackground);
        params->last_position = params->mouse_position;
    }

    if (params->key_space_down)
    {
        int pitch = params->window_w * sizeof(Uint8) * 4;
        if (SDL_LockTexture(params->textureBackground, NULL, (void**)&params->buffer, &pitch) != 0)
            GetError(-1, __FUNCTION__, __LINE__);

        memset(params->buffer, 0, params->buffer_size);
        SDL_UnlockTexture(params->textureBackground);
    }
}

void RenderProcess(SDL_Params* params)
{
    GetError(SDL_RenderClear(params->renderer), __FUNCTION__, __LINE__);
    GetError(SDL_RenderCopy(params->renderer, params->textureBackground, NULL, NULL), __FUNCTION__, __LINE__);

    SDL_SetRenderDrawColor(params->renderer, params->color.r, 0, 0, params->color.a);
    SDL_RenderFillRect(params->renderer, &params->rectRedBox);

    SDL_SetRenderDrawColor(params->renderer, 0, params->color.g, 0, params->color.a);
    SDL_RenderFillRect(params->renderer, &params->rectGreenBox);

    SDL_SetRenderDrawColor(params->renderer, 0, 0, params->color.b, params->color.a);
    SDL_RenderFillRect(params->renderer, &params->rectBlueBox);

    SDL_SetRenderDrawColor(params->renderer, params->color.r, params->color.g, params->color.b, params->color.a);
    SDL_RenderFillRect(params->renderer, &params->rectColorBox);

    SDL_RenderPresent(params->renderer);
}

void ControlFrameRateProcess(SDL_Params* params, bool checkTime)
{
    params->frameTime = SDL_GetTicks64() - params->frameStart;
    if (checkTime)
        getElapsedTime("Before Delay", params->start);

    if (params->frameDelay > params->frameTime)
        SDL_Delay(Uint32(params->frameDelay - params->frameTime));
    ++params->frameCount;

    if (checkTime)
        getElapsedTime("After  Delay", params->start);
}

void FillBuffer(Uint8* buffer, int x, int y, int w, int h, const SDL_Color* color)
{
    if (x >= 0 && x < w &&
        y >= 0 && y < h)
    {
        buffer[4 * x + 0 + 4 * w * y] = color->r;
        buffer[4 * x + 1 + 4 * w * y] = color->g;
        buffer[4 * x + 2 + 4 * w * y] = color->b;
        buffer[4 * x + 3 + 4 * w * y] = color->a;
    }
}

void DrawPoint(SDL_Params* params, const SDL_Color* color)
{
    FillBuffer(params->buffer, params->mouse_position.x, params->mouse_position.y, params->window_w, params->window_h, color);

}

void DrawLine(SDL_Params* params, const SDL_Color* color)
{
    int x1 = params->last_position.x;
    int y1 = params->last_position.y;
    int x2 = params->mouse_position.x;
    int y2 = params->mouse_position.y;
    int w = params->window_w;
    int h = params->window_h;

    float a = (float)params->window_h;
    float b = 0;
    float c = (float)params->window_h;
    float d = 0;

    if (x1 != x2)
    {
        a = (float)(y2 - y1) / (float)(x2 - x1);
        b = (float)y1 - a * x1;

        if (abs(a) <= 1.f)
        {
            int nPixel = abs(x2 - x1);
            int direction = x2 > x1 ? 1 : -1;

            for (int i = 1; i < nPixel; ++i)
            {
                int x = x1 + direction * i;
                int y = (int)round(a * x + b);

                FillBuffer(params->buffer, x, y, params->window_w, params->window_h, color);
            }
        }
    }
    
    if (y1 != y2)
    {
        c = (float)(x2 - x1) / (float)(y2 - y1);
        d = (float)x1 - c * y1;

        if (abs(c) < 1.f)
        {
            int nPixel = abs(y2 - y1);
            int direction = y2 > y1 ? 1 : -1;

            for (int i = 1; i < nPixel; ++i)
            {
                int y = y1 + direction * i;
                int x = (int)round(c * y + d);

                FillBuffer(params->buffer, x, y, params->window_w, params->window_h, color);
            }
        }
    }
}

void GetError(int err, const char* msg, int nline)
{
    if (err != 0)
        throw std::string{ std::format("error : {} [{}] - {}", msg, nline, SDL_GetError()) };
}

void setElapsedTime(std::chrono::system_clock::time_point& start)
{
    start = std::chrono::system_clock::now();
}

float getElapsedTime(const char* msg, std::chrono::system_clock::time_point& start, bool print, bool restart)
{
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::microseconds usec = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float currentTime = usec.count() / 1000.f;

    if (print)
        printf("%s : %.03f msec\n", msg, currentTime);

    if (restart)
        start = std::chrono::system_clock::now();

    return currentTime;
}