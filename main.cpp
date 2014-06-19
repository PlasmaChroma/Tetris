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
#include "hrtime.h"

using namespace std;

SDL_Renderer* renderer = nullptr;
unique_ptr<GameWindow> g_win;

const int WINDOW_X = 50;
const int WINDOW_Y = 50;
//int WINDOW_HEIGHT = 768;
//int WINDOW_WIDTH = 1024;
int g_windowWidth = 1024;
int g_windowHeight = 768;

int CONSOLE_X = WINDOW_X + g_windowWidth + 50;
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
	hrtime::init();
	hrtime::setTimer(APP_START);

	if (startup() != 0) {
		return 1;
	} else {
		cout << "Startup OK. (" << hrtime::elapsed() << ")" << endl;
	}

	if (g_win->eventLoop() != 0) {
		return 1;
	}

	/* cleanup */
	shutdown();

	return 0;
}

int startup(void)
{
	resizeConsole();

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	g_win = make_unique<GameWindow>(g_windowWidth, g_windowHeight, WINDOW_X, WINDOW_Y);
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