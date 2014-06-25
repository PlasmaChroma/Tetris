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
	m_blueBlk = LoadImage("assets/NinBlue.png");

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

	for (unsigned int i = 0; i < m_field.size(); ++i)
	{
		if (m_field[i] > 0) {
			SDL_RenderCopy(renderer, m_blueBlk, &src, &m_blockRects[i]);
		}
	}

	//SDL_Rect tmp = { 1, 1, 50, 50 };
	//SDL_RenderCopy(renderer, m_blueBlk, &src, &tmp);
}

SDL_Rect Tetris::calculateField(void)
{
	// x, y, width, height
	SDL_Rect ret = { 726, 203, 601, 726 }; // 1080p
	//SDL_Rect ret = { 1209, 338, 1001, 1209 }; // 1800
	return ret;
}

void Tetris::populateBlockRects(void)
{
	SDL_Rect field = calculateField();
	printf("Field dims: %d,%d,%d,%d\n", field.x, field.y, field.w, field.h);

	int blockWidth = (float)field.w / TETRIS_STD_WIDTH;
	int blockHeight = (float)field.h / (TETRIS_STD_HEIGHT - 2); // the invisible buffer up top

	int bX = field.x;
	int bY = field.y + field.h - blockHeight;
	int inRowCount = 0;

	for (unsigned int i = 0; i < m_blockRects.size() - 20; ++i) {
		m_blockRects[i] = SDL_Rect{ bX, bY, blockWidth, blockHeight };

		//if (i < 11)
		//printf("%d set to %d,%d,%d,%d\n", i, bX, bY, blockWidth, blockHeight);

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