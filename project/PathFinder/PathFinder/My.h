#pragma once


#define GRID_SIZE			16
#define	GRID_WIDTH			100
#define GRID_HEIGHT			50


struct st_Position
{
	bool _bErase;
	int _y;
	int _x;
};

extern st_Position st_Start;
extern st_Position st_End;


extern HBRUSH	g_hTileBrush;
extern HPEN		g_hGridPen;
extern HBRUSH	g_hTileStartBrush;
extern HBRUSH	g_hTileEndBrush;
extern int		g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음 
extern bool		g_bErase;
extern bool		g_bDrag;

extern HBITMAP	g_hMemDCBitmap;
extern HBITMAP	g_hMemDCBitmap_old;
extern HDC		g_hMemDC;
extern RECT		g_MemDCRect;

void RenderGrid(HDC hdc);
void RenderObstacle(HDC hdc);

