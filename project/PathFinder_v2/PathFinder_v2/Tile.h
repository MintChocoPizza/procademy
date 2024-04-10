
#ifndef __TILE_H__
#define __TILE_H__

const int g_GRID_WIDTH = 100;
const int g_GRID_HEIGHT = 50;

class Tile
{
public:
	Tile();
	~Tile();

private:
	struct GRID
	{
		int state;
	};

	GRID st_GRID[g_GRID_HEIGHT][g_GRID_WIDTH];
	int m_GRID_SIZE;

	int m_startY;
	int m_startX;

	
};


#endif // !__TILE_H__
