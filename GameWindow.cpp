#include "GameWindow.h"
#include <iostream>
#include <string>
#include "windowEvents.h"
#include "text.h"
#include "Tetris.h"

using namespace std;

GameWindow::GameWindow(int w, int h, int x, int y)
: m_width(w), m_height(h), m_x(x), m_y(y),
  m_window(nullptr), m_renderer(nullptr),
  mouseX(0), mouseY(0)
{
	m_window = SDL_CreateWindow("Tetris", m_x, m_y, m_width, m_height, SDL_WINDOW_SHOWN);
	if (m_window == nullptr){
		std::cout << SDL_GetError() << std::endl;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr){
		std::cout << SDL_GetError() << std::endl;
	}
}

GameWindow::~GameWindow(void)
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

int GameWindow::eventLoop(void)
{
	Tetris game(10, 22, 1);
	// game code eventually goes here
	while (1) {
		SDL_GetMouseState(&mouseX, &mouseY);

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
		SDL_RenderClear(m_renderer);

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

		ConstrainedPrint(10, m_height - 20, image, m_renderer, 400);

		// flip the backbuffer
		SDL_RenderPresent(m_renderer);

		SDL_DestroyTexture(image);

		//SDL_DestroyTexture(plus);

	}

	return 0;
}