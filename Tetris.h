#pragma once

#include <vector>
#include <random>

#define TETRIMINOS_MAX 7

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
protected:
	std::mt19937 m_rd; /* random number gen */
private:
	int getRandTetrimino(void);
	int m_width;
	int m_height;
	int m_mode;
	std::vector<int> m_field;
};