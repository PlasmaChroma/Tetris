#pragma once

#include <vector>
#include <random>
#include <array>
#include <SDL.h>

#define TETRIMINOS_MAX 7
#define TETRIS_STD_WIDTH 10
#define TETRIS_STD_HEIGHT 22

enum tetriminos {
	tm_block = 1,
	tm_t = 2,
	tm_line = 3,
	tm_l = 4,
	tm_j = 5,
	tm_s = 6,
	tm_z = 7
};

class Tetris
{
public:
	Tetris(int width, int height, int mode);
	~Tetris(void);
	void init(int width, int height, int mode);
	void draw(void);
	void drawBackground(void);
protected:
	std::mt19937 m_rd; /* random number gen */
private:
	int getRandTetrimino(void);
	SDL_Rect calculateField(void);

	int m_width;
	int m_height;
	int m_mode;
	std::vector<int> m_field;

	int m_lines;
	int m_points;
	int m_level;
	std::array<int, TETRIMINOS_MAX> m_pieceStats;

	SDL_Texture* m_bgtex;
};