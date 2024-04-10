
#include <windowsx.h>

#include "resource.h"
#include "framework.h"
#include "PathFinder_v2.h"
#include "RegisterClass.h"
#include "Initstance.h"

#include "WndProc.h"

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

int GRID_SIZE = 16;
int GRID_WIDTH = 100;
int GRID_HEIGHT = 50;

bool g_Tile[50][100];
bool g_bErase = false;
bool g_bDrag = false;

HPEN g_hGridPen;

HBRUSH g_hTileBrush;

HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;

HDC     g_hMemDC;

RECT    g_MemDCRect;


void RenderGrid(HDC hdc);
void RenderObstacle(HDC hdc);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        // 메모리 DC를 클리어 하고
        PatBlt(g_hMemDC, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, WHITENESS);

        // RenderObstacle, RenderGrid를 메모리 DC에 출력
        RenderObstacle(g_hMemDC);
        RenderGrid(g_hMemDC);

        // 메모리 DC에 랜더링이 끝나면, 메모리 DC -> 윈도우 DC로의 출력
        hdc = BeginPaint(hWnd, &ps);
        BitBlt(hdc, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
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
        break;

    case WM_LBUTTONUP:
        g_bDrag = false;
        break;

    case WM_MOUSEMOVE:
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
        }
        InvalidateRect(hWnd, NULL, false);
    }
    
    case WM_CREATE:
    {
        g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        g_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));

        // 윈도우 생성시 현 윈도우 크기와 동일한 메모리 DC 생성
        HDC hdc = GetDC(hWnd);
        GetClientRect(hWnd, &g_MemDCRect);
        g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
        g_hMemDC = CreateCompatibleDC(hdc);
        ReleaseDC(hWnd, hdc);
        g_hMemDCBitmap_old = (HBITMAP)SelectObject(g_hMemDC, g_hMemDCBitmap);
    }
    break;

    case WM_SIZE:
    {
        SelectObject(g_hMemDC, g_hMemDCBitmap_old);
        DeleteObject(g_hMemDC);
        DeleteObject(g_hMemDCBitmap);

        HDC hdc = GetDC(hWnd);

        GetClientRect(hWnd, &g_MemDCRect);
        g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
        g_hMemDC = CreateCompatibleDC(hdc);
        ReleaseDC(hWnd, hdc);

        g_hMemDCBitmap_old = (HBITMAP)SelectObject(g_hMemDC, g_hMemDCBitmap);
    }
    break;

    case WM_DESTROY:
        SelectObject(g_hMemDC, g_hMemDCBitmap_old);
        DeleteObject(g_hTileBrush);
        DeleteObject(g_hGridPen);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void RenderGrid(HDC hdc)
{
    int iX = 0;
    int iY = 0;
    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

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
	HPEN hOldPen;



    hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐

	for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
	{
		for (int iCntH = 0; iCntH < GRID_HEIGHT; ++iCntH)
		{
			// 색깔이 정해진다면.
			if (g_Tile[iCntH][iCntW] == true)
			{
				// x좌표와 y좌표를 계산한다.
				iX = iCntW * GRID_SIZE;
				iY = iCntH * GRID_SIZE;

				// 테두리 크기가 있으므로 +2 한다.
				Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);

			}
			// 어짜피 루프 도는 김에 Start 좌표와 End 좌표의 색깔도 칠한다.
		}
	}
    SelectObject(hdc, hOldBrush);
}