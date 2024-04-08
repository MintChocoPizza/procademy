
#include <map>
#include <stdlib.h>
#include <cmath>

#include <windowsx.h>
#include "framework.h"

#include "A_STAR.h"
#include "My.h"

using namespace std;

st_Position st_Start;
st_Position st_End;

HBRUSH	g_hTileBrush;
HBRUSH	g_hTileStartBrush;
HBRUSH	g_hTileEndBrush;
HBRUSH	g_hTileOpenListBrush;
HBRUSH	g_hTileCloseListBrush;
HFONT	g_hTileFont;


HPEN	g_hGridPen;
HPEN	g_hParentPointerPen;
int		g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음, 2 시작 좌표, 3, 끝 좌표
int		GRID_SIZE = 16;
int		g_iX = 0;
int		g_iY = 0;
int		g_StartX = 0;
int		g_StartY = 0;
int		g_dy[] = { -1, 1,  0, 0, -1, -1, 1, 1 };			// 상하좌우 좌상, 우상, 좌하, 우하
int		g_dx[] = {  0, 0, -1, 1, -1, 1, -1, 1 };

bool	g_bErase = false;
bool	g_bDrag = false;

HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;
HDC		g_hMemDC;
RECT	g_MemDCRect;

extern A_STAR cA_Star;

void RenderPath(HDC hdc)
{
	st_FindNode* temp = cA_Star.GetNextPosition();
	int sY;
	int sX;
	int eY; 
	int eX;
	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

	while (temp != NULL)
	{
		sY = temp->_Y * GRID_SIZE + GRID_SIZE / 2;
		sX = temp->_X * GRID_SIZE + GRID_SIZE / 2;
		eY = temp->_PY * GRID_SIZE + GRID_SIZE / 2;
		eX = temp->_PX * GRID_SIZE + GRID_SIZE / 2;
		MoveToEx(hdc, sX, sY, NULL);
		LineTo(hdc, eX, eY);

		temp = cA_Star.GetNextPosition();
	}

	SelectObject(hdc, hOldPen);
}

//-------------------------------------------------------------------------------------
// 초기 계획: 그리드 자체를 옮기려고 했는데 굳이 그리드를 옮겨야함? 
// 
// 실제 필요한건 타일에 칠해진 색깔.
// 
// 
//-------------------------------------------------------------------------------------
void RenderGrid(HDC hdc)
{
	int iX = 0;
	int iY = 0;
	// g_hGridPen으로 펜 오브젝트를 변경한다.
	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

	//------------------------------------------------------------------------
	// 그리드의 마지막 선을 추가로 그리기 위해 <= 의 반복 조건.
	// 
	//------------------------------------------------------------------------
	for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
	{
		// MoveToEx(hdc, iX + g_StartX, 0 + g_StartY, NULL);
		// LineTo(hdc, iX + g_StartX, GRID_HEIGHT * GRID_SIZE + g_StartY);
		MoveToEx(hdc, iX, 0, NULL);
		LineTo(hdc, iX, GRID_HEIGHT * GRID_SIZE);
		iX += GRID_SIZE;
	}

	for (int iCntH = 0; iCntH <= GRID_HEIGHT; ++iCntH)
	{
		//MoveToEx(hdc, 0 + g_StartX, iY + g_StartY, NULL);
		//LineTo(hdc, GRID_WIDTH * GRID_SIZE + g_StartX, iY + g_StartY);
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
	HBRUSH hOldBrush;
	HPEN hOldPen;
	//---------------------------------------------------------------------------------------------------------------
	// 사각형의 테두리를 안보이도록 하기 위해 NULL_PEN을 지정한다.
	// CreatePen 으로 NULL_PEN을 생성해도 되지만, GetStockObject를 사용하여 
	// 이미 시스템에 만들어져 있는 고정 GDI Object를 사용해본다. 
	// GetStockObject는 시스템의 고정적인 범용 GDI Object로서 삭제가 필요 없다.
	// 시스템 전역적인 GDI Object를 얻어서 사용한다는 개념
	// 
	// 0인 경우는 아무것도 안칠함 -> 흰색으로 칠해지는 효과
	// 
	// 
	//---------------------------------------------------------------------------------------------------------------

	// 배열의 모든 위치를 탐색한다.
	for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
	{
		for (int iCntH = 0; iCntH < GRID_HEIGHT; ++iCntH)
		{

			st_FindNode st;
			int sY;
			int sX;
			int eY;
			int eX;
			if (cA_Star.GetST(&st, iCntH, iCntW))
			{
				// 파랑 노랑 색깔 칠하기
				if (st.OpenList == true)
					hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileOpenListBrush);
				else
					hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileCloseListBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				//iX = iCntW * GRID_SIZE + g_StartX;
				//iY = iCntH * GRID_SIZE + g_StartY;
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE ;
				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
				SelectObject(hdc, hOldBrush);


				//// 노드에 대한 글자 출력
				//if (GRID_SIZE > 16)
				//{
				//	HFONT hOldFont = (HFONT)SelectObject(hdc, g_hTileFont);
				//	wchar_t wcBuff[50];

				//	swprintf_s(wcBuff,50, L"X: %d  Y: %d", st._X, st._Y);
				//	TextOut(hdc, (st._X + g_StartX + 1) * GRID_SIZE, (st._Y + g_StartY + 2) * GRID_SIZE, wcBuff, wcslen(wcBuff));

				//	




				//	SelectObject(hdc, hOldFont);
				//}



			}


			// 색깔이 정해진다면.
			if (g_Tile[iCntH][iCntW] == 1)
			{
				// 필기구를 선택하고
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				
				// x좌표와 y좌표를 계산한다.
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;

				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX + g_StartX, iY+g_StartY, iX + GRID_SIZE + 2 +g_StartX, iY + GRID_SIZE + 2 +g_StartY);
				SelectObject(hdc, hOldBrush);
			}
			else if (g_Tile[iCntH][iCntW] == 2)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileStartBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;
				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX + g_StartX, iY + g_StartY, iX + GRID_SIZE + 2 + g_StartX, iY + GRID_SIZE + 2 + g_StartY);
				SelectObject(hdc, hOldBrush);
			}
			else if (g_Tile[iCntH][iCntW] == 3)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileEndBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;
				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX + g_StartX, iY + g_StartY, iX + GRID_SIZE + 2 + g_StartX, iY + GRID_SIZE + 2 + g_StartY);
				SelectObject(hdc, hOldBrush);
			}
			// 어짜피 루프 도는 김에 Start 좌표와 End 좌표의 색깔도 칠한다.
		}
	}

}

