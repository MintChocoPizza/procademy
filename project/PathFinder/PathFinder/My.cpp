
#include <windowsx.h>
#include "framework.h"

#include "My.h"


HBRUSH	g_hTileBrush;
HPEN	g_hGridPen;
bool	g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음 
bool	g_bErase = false;
bool	g_bDrag = false;

HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;
HDC		g_hMemDC;
RECT	g_MemDCRect;



void RenderGrid(HDC hdc)
{
	int iX = 0;
	int iY = 0;
	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

	// 그리드의 마지막 선을 추가로 그리기 위해 <= 의 반복 조건.
	for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
	{
		MoveToEx(hdc, iX, 0, NULL);
		LineTo(hdc, iX, GRID_HEIGHT * GRID_SIZE);
		iX += GRID_SIZE;
	}

	for (int iCntH = 0; iCntH <= GRID_HEIGHT; ++iCntH)
	{
		MoveToEx(hdc, 0, iY, NULL);
		LineTo(hdc, GRID_WIDTH * GRID_SIZE, iY);
		iY += GRID_SIZE;
	}
	SelectObject(hdc, hOldPen);
}

void RenderObstacle(HDC hdc)
{
	int iX = 0;
	int iY = 0;
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	// 사각형의 테두리를 안보이도록 하기 위해 NULL_PEN을 지정한다.
	// CreatePen 으로 NULL_PEN을 생성해도 되지만, GetStockObject를 사용하여 
	// 이미 시스템에 만들어져 있는 고정 GDI Object를 사용해본다. 
	// GetStockObject는 시스템의 고정적인 범용 GDI Object로서 삭제가 필요 없다.
	// 시스템 전역적인 GDI Object를 얻어서 사용한다는 개념
	for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
	{
		for (int iCntH = 0; iCntH < GRID_HEIGHT; ++iCntH)
		{
			if (g_Tile[iCntH][iCntW])
			{
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;
				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
			}
		}
	}
	SelectObject(hdc, hOldBrush);
}

void wm_LButtonDown(HDC hdc, PAINTSTRUCT ps, LPARAM lParam)
{
	g_bDrag = true;
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int iTileX = xPos / GRID_SIZE;
		int iTileY = yPos / GRID_SIZE;

		// 첫 선택 타일이 장애물이면 지우기 모드 아니면 장애물 넣기 모드
		if (g_Tile[iTileY][iTileX] == 1)
			g_bErase = true;
		else
			g_bErase = false;
	}
}

void wm_LButtonUp(void)
{
	g_bDrag = false;
}

void wm_MouseMove(HWND hWnd, LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	if (g_bDrag)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		int iTileX = xPos / GRID_SIZE;
		int iTileY = yPos / GRID_SIZE;

		g_Tile[iTileY][iTileX] = !g_bErase;
		InvalidateRect(hWnd, NULL, true);
	}
}

void wm_Create(void)
{
	g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	g_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));
}

void wm_Paint(HWND hWnd, HDC hdc, PAINTSTRUCT ps)
{
	hdc = BeginPaint(hWnd, &ps);
	RenderObstacle(hdc);
	RenderGrid(hdc);
	EndPaint(hWnd, &ps);
}

void wm_Destroy(void)
{
	DeleteObject(g_hTileBrush);
	DeleteObject(g_hGridPen);
	PostQuitMessage(0);
}

