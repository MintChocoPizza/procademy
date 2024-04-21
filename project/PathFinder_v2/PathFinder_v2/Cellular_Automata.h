#ifndef __CELLULAR_AUTOMATA_H__
#define __CELLULAR_AUTOMATA_H__

#include "Tile.h"

class Cellular_Automata
{
public:
	Cellular_Automata();
	~Cellular_Automata();

	void Update(void);

	void GenerateMap();
	void  MapRandomFill();
	void SmoothMap();

	int** map;
	int smoothNum;
	int seed;

	const int WALL = 1;
	const int ROAD = 0;
};


#endif // !__CELLULAR_AUTOMATA_H__

