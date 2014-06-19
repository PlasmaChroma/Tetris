#include "Tetris.h"
#include <iostream>
#include <SDL_image.h>
#include "hrtime.h"
#include "global.h"

using namespace std;

SDL_Texture* LoadImage(std::string file)
{
	SDL_Texture* tex = nullptr;
	tex = IMG_LoadTexture(renderer, file.c_str());
	if (tex == nullptr)
		throw std::runtime_error("Failed to load image: " + file + IMG_GetError());
	return tex;
}

int Tetris::getRandTetrimino(void)
{
	return (m_rd() % TETRIMINOS_MAX) + 1;
}

Tetris::Tetris(int width, int height, int mode)
{
	init(width, height, mode);

	/* load the background texture */
	m_bgtex = LoadImage("assets/blank_typea_crop.png");

}

Tetris::~Tetris(void)
{
	SDL_DestroyTexture(m_bgtex);
}

void Tetris::init(int width, int height, int mode)
{
	m_width = width;
	m_height = height;
	m_mode = mode;

	m_field.clear();

	/* create field of desired size */
	m_field.resize(m_width * m_height, 0);

	cout << "Twister seed: " << hrtime::getTimer("now", timer_micro) << endl;
	cout << "Elapsed: " << hrtime::elapsed() << endl;
	hrtime::setTimer("appStartup");

	m_rd.seed((unsigned long)hrtime::getTimer("now", timer_micro));

	m_lines = 0;
	m_points = 0;
	m_level = 1;
	m_pieceStats.fill(0);
}

void Tetris::draw(void)
{
	drawBackground();
}

void Tetris::drawBackground(void)
{
	SDL_Rect src, dst;

	/* grab entire source from origin */
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(m_bgtex, NULL, NULL, &src.w, &src.h);

	/* we want to fill the entire window */
	dst.x = 0;
	dst.y = 0;
	dst.w = g_windowWidth;
	dst.h = g_windowHeight;

	SDL_RenderCopy(renderer, m_bgtex, &src, &dst);
}

SDL_Rect Tetris::calculateField(void)
{
	/* use captured measurements */
	double x_percent_start = 724 / 1920;
	double x_percent_end = 1326 / 1920;
	double y_percent_start = 191 / 1017;
	double y_percent_end = 878 / 1017;

	SDL_Rect ret;
	ret.x = (int)(x_percent_start * m_width);
	ret.w = (int)((x_percent_end * m_width) - ret.x);
	ret.y = (int)(y_percent_start * m_height);
	ret.h = (int)((y_percent_end * m_height) - ret.y);

	return ret;
}

void Tetris::populateBlockRects(void)
{
	SDL_Rect field = calculateField();

	int blockWidth = field.w / TETRIS_STD_WIDTH;
	int blockHeight = field.h / TETRIS_STD_HEIGHT;


}