#pragma once

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

struct st_Data
{
	st_Data()
	{}
	st_Data(int PY, int PX, int G, int H, int F, int y, int x) :
		_PY(PY), _PX(PX), _G(G), _H(H), _F(F), _Y(y), _X(x)
	{

	}
	st_Data(const st_Data& obj)
	{
		_PY = obj._PY;
		_PX = obj._PX;
		_G = obj._G;
		_H = obj._H;
		_F = obj._F;
		_Y = obj._Y;
		_X = obj._X;
	}
	// 부모의 좌표
	int _PY;
	int _PX;

	short	_G; // 출발점 부터의 이동 거리, 유클리드(대각선)
	int		_H;	// 목적지와의 거리, 맨헤튼(가로+세로)
	int		_F; // G+H

	int		_Y;
	int		_X;
};


struct st_CloseNode
{
	bool	OpenCheck;
	int		_x;
	int		_y;
	int		_pX;
	int		_pY;
};

extern st_Position st_Start;
extern st_Position st_End;


extern HBRUSH	g_hTileBrush;
extern HPEN		g_hGridPen;
extern HBRUSH	g_hTileStartBrush;
extern HBRUSH	g_hTileEndBrush;
extern int		g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음 
extern int		GRID_SIZE;
extern int		g_StartX;
extern int		g_StartY;
extern bool		g_bErase;
extern bool		g_bDrag;

extern HBITMAP	g_hMemDCBitmap;
extern HBITMAP	g_hMemDCBitmap_old;
extern HDC		g_hMemDC;
extern RECT		g_MemDCRect;

void A_START(void);
void RenderGrid(HDC hdc);
void RenderObstacle(HDC hdc);

