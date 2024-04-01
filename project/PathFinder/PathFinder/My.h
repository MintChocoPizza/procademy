#pragma once


#define GRID_SIZE			16
#define	GRID_WIDTH			100
#define GRID_HEIGHT			50

extern HBRUSH	g_hTileBrush;
extern HPEN	g_hGridPen;
extern bool	g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음 
extern bool	g_bErase;
extern bool	g_bDrag;

extern HBITMAP g_hMemDCBitmap;
extern HBITMAP g_hMemDCBitmap_old;
extern HDC		g_hMemDC;
extern RECT	g_MemDCRect;

void RenderGrid(HDC hdc);
void RenderObstacle(HDC hdc);
void wm_LButtonDown(HDC hdc, PAINTSTRUCT ps, LPARAM lParam);
void wm_LButtonUp(void);
void wm_MouseMove(HWND hWnd, LPARAM lParam);
void wm_Create(void);
void wm_Paint(HWND hWnd, HDC hdc, PAINTSTRUCT ps);
void wm_Destroy(void);
