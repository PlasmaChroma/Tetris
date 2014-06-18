#include "Tetris.h"
#include <ctime>
#include <iostream>

using namespace std;

int Tetris::getRandTetrimino(void)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "twister: ";
		for (int j = 0; j < 10; j++)
		{
			cout << (m_rd() % 7) + 1 << " ";
		}
		cout << endl;
	}
		

	return 0;
}

Tetris::Tetris(int width, int height, int mode) :
m_width(width), m_height(height), m_mode(mode)
{
	m_field.clear();

	/* create field of desired size */
	m_field.resize(m_width * m_height, 0);
	
	time_t seedTime = time(nullptr);
	printf("seed time: %lu\n", seedTime);
	m_rd.seed(seedTime);

	getRandTetrimino();
}

Tetris::~Tetris(void)
{

}