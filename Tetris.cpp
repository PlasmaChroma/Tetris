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
	m_blueBlk = LoadImage("assets/DTblue.png");

}

Tetris::~Tetris(void)
{
	SDL_DestroyTexture(m_bgtex);
}

void Tetris::init(int width, int height, int mode)
{
	m_FieldWidth = width;
	m_FieldHeight = height;
	m_mode = mode;

	m_field.clear();
	m_blockRects.clear();

	/* create field of desired size */
	m_field.resize(m_FieldWidth * m_FieldHeight, 0);
	m_blockRects.resize(m_FieldWidth * m_FieldHeight, SDL_Rect{ 0, 0, 0, 0 });

	cout << "Twister seed: " << hrtime::getTimer("now", timer_micro) << endl;
	cout << "Elapsed: " << hrtime::elapsed() << endl;
	hrtime::setTimer("appStartup");

	m_rd.seed((unsigned long)hrtime::getTimer("now", timer_micro));

	m_lines = 0;
	m_points = 0;
	m_level = 1;
	m_pieceStats.fill(0);

	populateBlockRects();
}

void Tetris::draw(void)
{
	drawBackground();
	drawField();
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

void Tetris::drawField(void)
{
	SDL_Rect src;

	/* grab entire source from origin */
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(m_blueBlk, NULL, NULL, &src.w, &src.h);

	for (unsigned int i = 0; i < m_blockRects.size(); ++i)
	{
		SDL_RenderCopy(renderer, m_blueBlk, &src, &m_blockRects[i]);
	}

	SDL_Rect tmp = { 1, 1, 50, 50 };
	SDL_RenderCopy(renderer, m_blueBlk, &src, &tmp);
}

SDL_Rect Tetris::calculateField(void)
{
	/* use captured measurements */
	double x_percent_start = 726.0 / 1920.0;
	double x_percent_end = 1327.0 / 1920.0;
	double y_percent_start = 191.0 / 1017.0;
	double y_percent_end = 880.0 / 1017.0;

	printf("xs: %g xe: %g ys: %g ye: %g \n", 
		x_percent_start, x_percent_end, y_percent_start, y_percent_end);

	printf("Window %d,%d\n", g_windowWidth, g_windowHeight);

	SDL_Rect ret;
	ret.x = (int)(x_percent_start * (double)g_windowWidth);
	ret.w = (int)((x_percent_end * (double)g_windowWidth) - ret.x);
	ret.y = (int)(y_percent_start * (double)g_windowHeight);
	ret.h = (int)((y_percent_end * (double)g_windowHeight) - ret.y);

	return ret;
}

void Tetris::populateBlockRects(void)
{
	SDL_Rect field = calculateField();
	printf("Field dims: %d,%d,%d,%d\n", field.x, field.y, field.w, field.h);

	float blockWidth = (float)field.w / TETRIS_STD_WIDTH;
	float blockHeight = (float)field.h / TETRIS_STD_HEIGHT;

	float bX = field.x;
	float bY = field.y + field.h - blockHeight;
	int inRowCount = 0;

	for (unsigned int i = 0; i < m_blockRects.size(); ++i) {
		m_blockRects[i] = SDL_Rect{ bX, bY, blockWidth, blockHeight };

		if (i < 11)
		printf("%d set to %d,%d,%d,%d\n", i, bX, bY, blockWidth, blockHeight);

		++inRowCount;

		if (inRowCount == TETRIS_STD_WIDTH) {
			inRowCount = 0;
			bX = field.x; /* reset X */
			bY = bY - blockHeight; /* raise up Y */
		} else {
			/* increment X cord */
			bX += blockWidth;
		}
		
	}
}