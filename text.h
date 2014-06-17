#pragma once

#include <SDL.h>
#include <string>

int init_ttf(void);

SDL_Texture* RenderText(std::string message, std::string fontFile, SDL_Color color, int fontSize);

void ConstrainedPrint(int x, int y, SDL_Texture *tex, SDL_Renderer *rend, int maxWidth);

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend, SDL_Rect *clip = NULL);

/* end of file */