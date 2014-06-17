#pragma once
#include <SDL.h>

class GameWindow
{
public:
	GameWindow(int w, int h, int x, int y);
	~GameWindow(void);
	SDL_Renderer* getRenderer(void) { return m_renderer; }
	int eventLoop(void);
protected:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	int mouseX, mouseY;
private:
	int m_width, m_height, m_x, m_y;

};