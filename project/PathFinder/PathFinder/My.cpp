
#include <map>
#include <stdlib.h>

#include <windowsx.h>
#include "framework.h"

#include "My.h"

using namespace std;

st_Position st_Start;
st_Position st_End;

HBRUSH	g_hTileBrush;
HBRUSH	g_hTileStartBrush;
HBRUSH	g_hTileEndBrush;



HPEN	g_hGridPen;
int		g_Tile[GRID_HEIGHT][GRID_WIDTH];				// 0 장애물 없음, 1 장애물 있음, 2 시작 좌표, 3, 끝 좌표
int		GRID_SIZE = 16;
int		g_StartX = 0;
int		g_StartY = 0;
int		g_dy[] = { 1, -1, 0, 0, -1, -1, 1, 1 };			// 상하좌우 좌상, 우상, 좌하, 우하
int		g_dx[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

bool	g_bErase = false;
bool	g_bDrag = false;

HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;
HDC		g_hMemDC;
RECT	g_MemDCRect;





void A_START(void)
{
	// Key: _F, Value: st_Data
	// map는 red-black Tree 고로, 넣는 순간 정렬된다. 
	multimap<int, st_Data>						mOpenList;


//	multimap<int, st_Data>						mCloseList;
	// mCloseList 에서 모든 중복체크를 실시한다.
	map<pair<int, int >, st_CloseNode>			mCloseList;

	multimap<int, st_Data>::iterator			OpenIter;
	map<pair<int, st_Data>, st_CloseNode>		CloseIter;

	st_Data										st_CurData;
	int											iCnt;
	int											nY;
	int											nX;

	// 시작 위치를 OpenList에 담는다. 
	int iStartPoint_F = abs(st_Start._y - st_End._y) + abs(st_End._x - st_End._y);
	mOpenList.insert(pair <int, st_Data>(iStartPoint_F, st_Data{ -1, -1, 0, iStartPoint_F, iStartPoint_F, st_Start._y, st_Start._x }));

	while (1)
	{
		// F값이 가장 작은 노드를 꺼낸다. 
		OpenIter = mOpenList.begin();
		st_CurData = st_Data(OpenIter->second);
		
		//--------------------------------------------------------------
		// 여기서 목적지 완료 검사를 할 지 아래서 할 지 고민 중.......
		//--------------------------------------------------------------
		//
		//

		// 8방향으로의 갈 수 있는 노드를 탐색한다. 
		for (iCnt = 0; iCnt < 8; ++iCnt)
		{
			nY = st_CurData._Y + g_dy[iCnt];
			nX = st_CurData._X + g_dx[iCnt];

			//--------------------------------------------------------------
			// 목적지에 도착하면 성공이다. 
			//--------------------------------------------------------------
			if (nY == st_End._y && nX == st_End._x)
			{
				// 나중에 채우기
			}


			//---------------------------------------------------------------------------------------------------------------
			// OpenList와 CloseList를 둘러보고 중복되어 있는지를 검사한다. 
			// 
			// 
			//---------------------------------------------------------------------------------------------------------------
			if (mCloseList.find(make_pair(nY, nX)) != mCloseList.end())
			{
				
			}
			

		}
	}
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
	HBRUSH hOldBrush;
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

			// 색깔이 정해진다면.
			if (g_Tile[iCntH][iCntW] == 1)
			{
				// 필기구를 선택하고
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				
				// x좌표와 y좌표를 계산한다.
				iX = (iCntW - g_StartX) * GRID_SIZE;
				iY = (iCntH - g_StartY) * GRID_SIZE;

				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
				SelectObject(hdc, hOldBrush);
			}
			else if (g_Tile[iCntH][iCntW] == 2)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileStartBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;
				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
				SelectObject(hdc, hOldBrush);
			}
			else if (g_Tile[iCntH][iCntW] == 3)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileEndBrush);
				SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;
				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
				SelectObject(hdc, hOldBrush);
			}
			// 어짜피 루프 도는 김에 Start 좌표와 End 좌표의 색깔도 칠한다.


		}
	}

}
