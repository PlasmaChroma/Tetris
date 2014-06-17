#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "global.h"

class icon
{
public:
	icon(int cx, int cy, std::string file, void(*funcPtr)(void));
	~icon();
	void drawSelf();
	void checkMouse(int cx, int cy);

private:
	int x;
	int y;
	int width;
	int height;
	SDL_Texture* tex;
	void(*callback)(void);
	bool mouseOver;
};

icon::icon(int cx, int cy, std::string file, void(*funcPtr)(void)) :
x(cx), y(cy), callback(funcPtr), mouseOver(false)
{
	tex = nullptr;
	tex = IMG_LoadTexture(renderer, file.c_str());
	if (tex == nullptr)
		throw std::runtime_error("Failed to load image: " + file + IMG_GetError());

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);
}

icon::~icon()
{
	SDL_DestroyTexture(tex);
}

void icon::checkMouse(int cx, int cy)
{
	mouseOver = false;

	if ((cx >= x) && (cx <= (x + width))) {
		if ((cy >= y) && (cy <= (y + height))) {
			mouseOver = true;
		}
	}
}

void icon::drawSelf(void)
{
	/* put this where we want */
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

	/* grab entire source from origin */
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = dst.w;
	src.h = dst.h;

	/* highlight if mouse is over icon */
	SDL_Rect brightBorder = { dst.x - 1, dst.y - 1, dst.w + 2, dst.h + 2 };
	if (mouseOver) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
		SDL_RenderFillRect(renderer, &brightBorder);
		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0);
		SDL_RenderFillRect(renderer, &dst);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}

	SDL_RenderCopy(renderer, tex, &src, &dst);
}