#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <cstdlib>
#include <memory>

#include "text.h"
#include "icon.h"
#include "windowEvents.h"
#include "GameWindow.h"

using namespace std;

SDL_Renderer* renderer = nullptr;
unique_ptr<GameWindow> g_win;

int WINDOW_X = 50;
int WINDOW_Y = 50;
int WINDOW_HEIGHT = 768;
int WINDOW_WIDTH = 1024;

int CONSOLE_X = WINDOW_X + WINDOW_WIDTH + 50;
int CONSOLE_Y = 50;
int CONSOLE_HEIGHT = 480;
int CONSOLE_WIDTH = 780;

size_t ENV_CHAR_MAX = 256;

int mouseX = 0;
int mouseY = 0; 

/* local prototypes */
int eventLoop(void);
int startup(void);
int shutdown(void);
void resizeConsole(void);
SDL_Texture* LoadImage(std::string file);
void ImgDraw(int x, int y, SDL_Texture *tex, SDL_Renderer *rend);


string getFontPath(string fontName)
{
	char* pRoot;
	_dupenv_s(&pRoot, &ENV_CHAR_MAX, "systemroot");
	string pSystemRoot(pRoot);
	free(pRoot);

	pSystemRoot += "\\Fonts\\" + fontName;

	//cout << "System Root at: " << pSystemRoot;

	return pSystemRoot;
}

int main(int argc, char ** argv)
{
	if (startup() != 0) {
		return 1;
	} else {
		cout << "Startup OK." << endl;
	}

	if (g_win->eventLoop() != 0) {
		return 1;
	}

	/* cleanup */
	shutdown();

	return 0;
}

int eventLoop(void)
{
	icon plusIco(5, 5, "assets/edit-add-2.png", nullptr);

	// game code eventually goes here
	while (1) {
		SDL_GetMouseState(&mouseX, &mouseY);
		plusIco.checkMouse(mouseX, mouseY);

		// event handling
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			PrintEvent(&e);
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
		}

		// clear the screen
		SDL_RenderClear(renderer);

		try {
			plusIco.drawSelf();
		}
		catch (const std::runtime_error &e) {
			std::cout << e.what() << std::endl;
			return 4;
		}
		
		/* draw some text on screen */
		SDL_Texture *image = nullptr;
		try {
			SDL_Color color = { 255, 255, 255 };
			//string myFont = getFontPath("comic.ttf");
			string myFont = "assets/PressStart2P.ttf";
			string myText = " X: " + to_string(mouseX) + " Y: " + to_string(mouseY);
			image = RenderText(myText, myFont, color, 12);
		}
		catch (const std::runtime_error &e) {
			std::cout << e.what() << std::endl;
			return 4;
		}

		ConstrainedPrint(10, WINDOW_HEIGHT - 20, image, renderer, 400);

		// flip the backbuffer
		SDL_RenderPresent(renderer);

		SDL_DestroyTexture(image);

		//SDL_DestroyTexture(plus);

	}

	return 0;
}

int startup(void)
{
	resizeConsole();

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	g_win = make_unique<GameWindow>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_X, WINDOW_Y);
	renderer = g_win->getRenderer();

	if (init_ttf() != 0) {
		return 1;
	}

	return 0;
}

int shutdown(void)
{
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void resizeConsole(void)
{
	// Get console window handle
	HWND wh = GetConsoleWindow();

	// Move window to required position
	MoveWindow(wh, CONSOLE_X, CONSOLE_Y, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);
}

SDL_Texture* LoadImage(std::string file)
{
	SDL_Texture* tex = nullptr;
	tex = IMG_LoadTexture(renderer, file.c_str());
	if (tex == nullptr)
		throw std::runtime_error("Failed to load image: " + file + IMG_GetError());
	return tex;
}

void ImgDraw(int x, int y, SDL_Texture *tex, SDL_Renderer *rend)
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

	SDL_RenderCopy(rend, tex, &src, &dst);
}