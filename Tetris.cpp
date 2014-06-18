#include "Tetris.h"
#include <iostream>
#include "hrtime.h"

using namespace std;

int Tetris::getRandTetrimino(void)
{
	return (m_rd() % TETRIMINOS_MAX) + 1;
}

Tetris::Tetris(int width, int height, int mode)
{
	init(width, height, mode);
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

Tetris::~Tetris(void)
{

}