#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include "text.h"
#include "global.h"

int init_ttf(void)
{
	if (TTF_Init() == -1){
		std::cout << TTF_GetError() << std::endl;
		return 2;
	}

	/* success */
	return 0;
}

TTF_Font *font = nullptr;
SDL_Texture* RenderText(std::string message, std::string fontFile, SDL_Color color, int fontSize)
{
	//Open the font
	if (font == nullptr) {
		font = TTF_OpenFont(fontFile.c_str(), fontSize);
	}

	if (font == nullptr)
		throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());

	//Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	//Clean up unneeded stuff
	SDL_FreeSurface(surf);
	//TTF_CloseFont(font);

	return texture;
}

void ConstrainedPrint(int x, int y, SDL_Texture *tex, SDL_Renderer *rend, int maxWidth)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = dst.w;
	src.h = dst.h;

	if (dst.w > maxWidth) {
		dst.w = maxWidth; /* only placing this much */
		src.w = maxWidth; /* only grabbing this much */
	}
	
	SDL_RenderCopy(rend, tex, &src, &dst);
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend, SDL_Rect *clip)
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;

	//Detect if we should use clip width settings or texture width
	if (clip != NULL){
		pos.w = clip->w;
		pos.h = clip->h;
	} else {
		SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	}

	SDL_RenderCopy(rend, tex, clip, &pos);
}