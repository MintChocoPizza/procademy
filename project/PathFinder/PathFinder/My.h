#pragma once

#include <map>
using namespace std;

// 확대시 값을 변경하기 위하여
// #define GRID_SIZE			16
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
extern HPEN		g_hParentPointerPen;
extern HBRUSH	g_hTileStartBrush;
extern HBRUSH	g_hTileEndBrush;
extern HBRUSH	g_hTileOpenListBrush;
extern HBRUSH	g_hTileCloseListBrush;

extern int		g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음 
extern int		GRID_SIZE;
extern int		g_iX;
extern int		g_iY;
extern int		g_StartX;
extern int		g_StartY;
extern bool		g_bErase;
extern bool		g_bDrag;

extern HBITMAP	g_hMemDCBitmap;
extern HBITMAP	g_hMemDCBitmap_old;
extern HDC		g_hMemDC;
extern RECT		g_MemDCRect;

extern bool		findPath;

void RenderPath(HDC hdc);
void RenderGrid(HDC hdc);
void RenderObstacle(HDC hdc);

