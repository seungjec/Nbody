#include "functions.h"
#include <string>

int main(int argc, char* argv[])
{
    SDL_Log("==== PROGRAM_START ====");
    SDL_Params params;
    bool checkTime = false;

    try
    {
        Initialize(&params);

        while (params.isRunning)
        {
            params.frameStart = SDL_GetTicks64();
            setElapsedTime(params.start);

            // Get Event
            EventProcess(&params);

            // Update
            UpdateProcess(&params);

            // Render
            RenderProcess(&params);

            // Control frame rate
            ControlFrameRateProcess(&params, checkTime);
            
        }

        // Finalized
        Finalize(&params);

    }
    catch (const std::string& err)
    {
        printf("%s\n", err.c_str());
    }

    SDL_Log("==== PROGRAM_END ====");
    return 0;
}

