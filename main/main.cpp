#include "functions.h"
#include <string>

int main(int argc, char* argv[])
{
    SDL_Log("==== SDL_START ====");
    SDL_Params params;

    try
    {
        Initialize(&params);

        while (params.isRunning)
        {
            params.frameStart = SDL_GetTicks64();

            // Get Event
            EventProcess(&params);

            // Update
            UpdateProcess(&params);

            // Render
            RenderProcess(&params);

            // Control frame rate
            ControlFrameRateProcess(&params);
        }

        // Finalized
        Finalize(params.window, params.renderer, params.textureBackground);

    }
    catch (const std::string& err)
    {
        printf("%s\n", err.c_str());
    }

    SDL_Log("==== SDL_END ====");
    return 0;
}

