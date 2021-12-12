#include "SDLTiming.h"

#include <SDL2/SDL.h>

float SDLTiming::GetTime()
{
	return (float)SDL_GetTicks() / 1000.0f;
}

void SDLTiming::Sleep(float seconds)
{
	SDL_Delay((unsigned int)(seconds * 1000.0f));
}
