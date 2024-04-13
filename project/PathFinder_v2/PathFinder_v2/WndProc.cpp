
#include <windowsx.h>

#include "resource.h"
#include "framework.h"
#include "PathFinder_v2.h"
#include "RegisterClass.h"
#include "Initstance.h"


#include "WndProc.h"
#include "A_STAR.h"


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
const int GRID_WIDTH = 100;
const int GRID_HEIGHT = 50;

int g_xPos;
int g_yPos;

int g_Tile[50][100];
bool g_bErase = false;
bool g_bDrag = false;
bool findPath = false;  // 길찾기 성공 실패

HPEN g_hGridPen;
HPEN g_hPathPen;

HBRUSH g_hTileBrush;
HBRUSH g_hTileStartBrush;
HBRUSH g_hTileEndBrush;
HBRUSH g_hTileOpenListBrush;
HBRUSH g_hTileCloseListBrush;

HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;

HDC     g_hMemDC;

RECT    g_MemDCRect;

A_STAR g_A_STAR;
st_Position st_Start;
st_Position st_End;
st_Position st_RenderStart;


void RenderGrid(HDC hdc);
void RenderObstacle(HDC hdc);
void RendefUI(HDC hdc);
void RenderPath(HDC hdc);
void RenderList(HDC hdc);
void RenderWeight(HDC hdc);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {

    case WM_PAINT:
    {
        // 메모리 DC를 클리어 하고
        PatBlt(g_hMemDC, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, WHITENESS);

        // RenderObstacle, RenderGrid를 메모리 DC에 출력
        RenderObstacle(g_hMemDC);
        RenderGrid(g_hMemDC);


        RendefUI(g_hMemDC);

        if (findPath == true)
        {
            RenderList(g_hMemDC);
            RenderPath(g_hMemDC);
        }

        // 메모리 DC에 랜더링이 끝나면, 메모리 DC -> 윈도우 DC로의 출력
        hdc = BeginPaint(hWnd, &ps);
        BitBlt(hdc, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);


        // 더블버퍼링 없애기
        //InvalidateRect(hWnd, NULL, true);
        //hdc = BeginPaint(hWnd, &ps);
        //RenderObstacle(hdc);
        //RenderGrid(hdc);
        //EndPaint(hWnd, &ps);

    }
    break;

    case WM_LBUTTONDBLCLK:
    {
        int xPos = GET_X_LPARAM(lParam) - st_RenderStart._x;
        int yPos = GET_Y_LPARAM(lParam) - st_RenderStart._y;
        int iTileX = xPos / GRID_SIZE;
        int iTileY = yPos / GRID_SIZE;


        // 그려진적 없다면 새로 그린다.
        if (st_Start._bErase == false)
        {
            g_Tile[iTileY][iTileX] = 2;

            st_Start._y = iTileY;
            st_Start._x = iTileX;
            st_Start._bErase = true;
        }
        else
        {
            // if(st_Start._bErase == true)
            // 이미 그려져 있을 때

            // 같은 위치라면, 그림을 지운다.
            if (st_Start._y == iTileY && st_Start._x == iTileX)
            {
                st_Start._bErase = false;
                g_Tile[iTileY][iTileX] = 0;
            }
            else
            {
                // 다른위치인 경우
                g_Tile[st_Start._y][st_Start._x] = 0;
                g_Tile[iTileY][iTileX] = 2;
                st_Start._y = iTileY;
                st_Start._x = iTileX;
            }
        }

        // 화면을 다시 그린다.
        InvalidateRect(hWnd, NULL, false);
    }
    break;

    case WM_RBUTTONDBLCLK:
    {
        int xPos = GET_X_LPARAM(lParam) - st_RenderStart._x;
        int yPos = GET_Y_LPARAM(lParam) - st_RenderStart._y;
        int iTileX = xPos / GRID_SIZE;
        int iTileY = yPos / GRID_SIZE;


        // 그려진적 없다면 새로 그린다.
        if (st_End._bErase == false)
        {
            g_Tile[iTileY][iTileX] = 3;

            st_End._y = iTileY;
            st_End._x = iTileX;
            st_End._bErase = true;
        }
        else
        {
            // if(st_Start._bErase == true)
            // 이미 그려져 있을 때

            // 같은 위치라면, 그림을 지운다.
            if (st_End._y == iTileY && st_End._x == iTileX)
            {
                st_End._bErase = false;
                g_Tile[iTileY][iTileX] = 0;
            }
            else
            {
                // 다른위치인 경우
                g_Tile[st_End._y][st_End._x] = 0;
                g_Tile[iTileY][iTileX] = 3;
                st_End._y = iTileY;
                st_End._x = iTileX;
            }
        }

        // 화면을 다시 그린다.
        InvalidateRect(hWnd, NULL, false);
    }
    break;

    case WM_LBUTTONDOWN:
        g_bDrag = true;
        {
            int xPos = GET_X_LPARAM(lParam) - st_RenderStart._x;
            int yPos = GET_Y_LPARAM(lParam) - st_RenderStart._y;
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
        int xPos = GET_X_LPARAM(lParam) - st_RenderStart._x;
        int yPos = GET_Y_LPARAM(lParam) - st_RenderStart._y;

        if (g_bDrag)
        {
            int iTileX = xPos / GRID_SIZE;
            int iTileY = yPos / GRID_SIZE;

            g_Tile[iTileY][iTileX] = !g_bErase;
        }
        InvalidateRect(hWnd, NULL, false);
    }
    break;

    case WM_MOUSEWHEEL:
    {
        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        
        // 마우스 커서의 현재 화면 좌표를 가져온다. 
        ScreenToClient(hWnd, &pt);

        // 마우스 포인터가 그리드를 넘어가면 동작 안함
        if (pt.x >= (st_RenderStart._x + GRID_WIDTH * GRID_SIZE) || pt.y >= (st_RenderStart._y + GRID_HEIGHT * GRID_SIZE))
        {
            break;
        }


        //int xStartPos = st_RenderStart._x;
        //int yStartPos = st_RenderStart._y;

        //int slopeY = pt.y - yStartPos;
        //int slopeX = pt.x = xStartPos;


        // 현재 사각형의 가로, 세로 길이
        int curWidth = GRID_WIDTH * GRID_SIZE;
        int curHeight = GRID_HEIGHT * GRID_SIZE;

        // 마우스 좌표와 그리드 시작 좌표로 그린 삼각형의 밑변의 길이
        int xCurBottom = abs(st_RenderStart._x - pt.x);


        // 그리드 사이즈 변경 후 사각형의 가로, 세로 길이
        int ChangeWidth;
        int ChargeHeight;

        // 그리드 사이즈 변경 비율, 그리는 좌표는 정수이기 때문에 나머지는 과감히 자른다. 
        double Percentage_Changed;

        // 변경된 밑변의 길이
        double xChangedBottom;

        // 변경된 사각형의 시작 좌표
        int xChangedStartPos;
        int yChangedStartPos;

        // 현재 윈도우 스크린 사이즈
        int ScreenSizeX = g_MemDCRect.right;
        int ScreenSizeY = g_MemDCRect.bottom;

        // 그리드 사이즈 변경
        if (zDelta > 0)
        {
            GRID_SIZE += 5;
            ChangeWidth = GRID_WIDTH * GRID_SIZE;
            ChargeHeight = GRID_HEIGHT * GRID_SIZE;

            // 얼마나 커졌는지 확인한다. 
            Percentage_Changed = (double)ChangeWidth / (double)curWidth;

            // 변경된 비율만큼 늘어난 밑변의 길이를 구한다. 
            xChangedBottom = (double)xCurBottom * Percentage_Changed;

            // 현재 마우스의 x좌표를 기준으로 변경된 밑변을 빼서 사각형의 시작 x 좌표를 구한다.
            xChangedStartPos = pt.x - xChangedBottom;

            // 직선의 방정식으로 y좌표도 구한다.
            yChangedStartPos = (pt.y - st_RenderStart._y) / (pt.x - st_RenderStart._x) * (xChangedStartPos - st_RenderStart._x) + st_RenderStart._y;
        }
        else
        {
            GRID_SIZE -= 5;
            GRID_SIZE = max(GRID_SIZE, 6);


            ChangeWidth = GRID_WIDTH * GRID_SIZE;
            ChargeHeight = GRID_HEIGHT * GRID_SIZE;

            // 얼마나 작아졌는지 확인한다. 
            Percentage_Changed = (double)curWidth / (double)ChangeWidth;

            // 변경된 비율만큼 줄어든 밑변의 길이를 구한다. 
            xChangedBottom = (double)xCurBottom / Percentage_Changed;

            // 현재 마우스의 x좌표를 기준으로 변경된 밑변을 빼서 사각형의 시작 x 좌표를 구한다.
            xChangedStartPos = pt.x - xChangedBottom;

            // 직선의 방정식으로 y좌표도 구한다.
            yChangedStartPos = (int)((double)(pt.y - st_RenderStart._y) / (double)(pt.x - st_RenderStart._x) * (xChangedStartPos - st_RenderStart._x) + st_RenderStart._y);
            
        }


  

        st_RenderStart._y = min(yChangedStartPos, 0);
        st_RenderStart._x = min(xChangedStartPos, 0);

        if (ChangeWidth <= ScreenSizeY || ChargeHeight <= ScreenSizeX)
        {
            st_RenderStart._y = 0;
            st_RenderStart._x = 0;
        }
        
        InvalidateRect(hWnd, NULL, false);
    }
    break;

    case WM_KEYDOWN:
    {
        if (wParam == VK_SPACE)
        {
            findPath = false;
            findPath = g_A_STAR.find(st_Start._y, st_Start._x, st_End._y, st_End._x, hWnd);
        }
        else if (wParam == 'W')
        {
            // 현재 윈도우 스크린 세로 사이즈
            int ScreenSizeY = g_MemDCRect.bottom;
            // 현재 사각형의 세로 길이
            int curHeight = GRID_HEIGHT * GRID_SIZE;

            if (curHeight < ScreenSizeY)
                break;

            st_RenderStart._y += GRID_SIZE;
            if (st_RenderStart._y > 0)
                st_RenderStart._y = 0;

          }
        else if (wParam == 'S')
        {
            // 현재 윈도우 스크린 세로 사이즈
            int ScreenSizeY = g_MemDCRect.bottom;
            // 현재 사각형의 세로 길이
            int curHeight = GRID_HEIGHT * GRID_SIZE;

            if (curHeight < ScreenSizeY)
                break;
            
            if (st_RenderStart._y + curHeight + GRID_SIZE < ScreenSizeY)
                break;

            st_RenderStart._y -= GRID_SIZE;
        }
        else if (wParam == 'A')
        {
            // 현재 윈도우 스크린 가로 사이즈
            int ScreenSizeX = g_MemDCRect.right;
            // 현재 사각형의 가로 길이
            int curWidth = GRID_WIDTH * GRID_SIZE;

            if (curWidth < ScreenSizeX)
                break;

            st_RenderStart._x += GRID_SIZE;
            if (st_RenderStart._x > 0)
                st_RenderStart._x = 0;
        }
        else if (wParam == 'D')
        {
            // 현재 윈도우 스크린 가로 사이즈
            int ScreenSizeX = g_MemDCRect.right;
            // 현재 사각형의 가로 길이
            int curWidth = GRID_WIDTH * GRID_SIZE;

            if (curWidth < ScreenSizeX)
                break;

            if (st_RenderStart._x + curWidth + GRID_SIZE < ScreenSizeX)
                break;

            st_RenderStart._x -= GRID_SIZE;
        }
        InvalidateRect(hWnd, NULL, false);
    }
        break;
    case WM_CREATE:
    {
        g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        g_hPathPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

        g_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));
        g_hTileStartBrush = CreateSolidBrush(RGB(0, 255, 0));
        g_hTileEndBrush = CreateSolidBrush(RGB(255, 0, 0));
        g_hTileOpenListBrush = CreateSolidBrush(RGB(0,0,255));
        g_hTileCloseListBrush = CreateSolidBrush(RGB(255,255,0));

        st_RenderStart = { 0, 0 ,0};

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
    int iX = st_RenderStart._x;
    int iY = st_RenderStart._y;
    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

    for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
    {
        MoveToEx(hdc, iX, st_RenderStart._y, NULL);
        LineTo(hdc, iX, st_RenderStart._y + GRID_HEIGHT * GRID_SIZE);
        iX += GRID_SIZE;
    }

    for (int iCntH = 0; iCntH <= GRID_HEIGHT; ++iCntH)
    {
        MoveToEx(hdc, st_RenderStart._x, iY, NULL);
        LineTo(hdc, st_RenderStart._x + GRID_WIDTH * GRID_SIZE, iY);
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
            if (g_Tile[iCntH][iCntW] == 1)
            {
                // 필기구를 선택하고
                hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileBrush);
                SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐

                // x좌표와 y좌표를 계산한다.
                iX = iCntW * GRID_SIZE + st_RenderStart._x;
                iY = iCntH * GRID_SIZE + st_RenderStart._y;

                // 테두리 크기가 있으므로 +2 한다.
                Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
                SelectObject(hdc, hOldBrush);
            }
            else if (g_Tile[iCntH][iCntW] == 2)
            {
                hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileStartBrush);
                SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
                iX = iCntW * GRID_SIZE + st_RenderStart._x;
                iY = iCntH * GRID_SIZE + st_RenderStart._y;
                // 테두리 크기가 있으므로 +2 한다.
                Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
                SelectObject(hdc, hOldBrush);
            }
            else if (g_Tile[iCntH][iCntW] == 3)
            {
                hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileEndBrush);
                SelectObject(hdc, GetStockObject(NULL_PEN));	// 뭔가 테두리가 사라짐
                iX = iCntW * GRID_SIZE + st_RenderStart._x;
                iY = iCntH * GRID_SIZE + st_RenderStart._y;
                // 테두리 크기가 있으므로 +2 한다.
                Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
                SelectObject(hdc, hOldBrush);
            }
		}
	}
    SelectObject(hdc, hOldBrush);
}

void RendefUI(HDC hdc)
{
    HFONT hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PALETTE | FF_DONTCARE, L"Tahoma");
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    //SetBkMode(hdc, )
    TextOut(hdc, 10, 10, L"안녕하세요", lstrlen(L"안녕하세요"));
}

void RenderPath(HDC hdc)
{
    st_A_START_NODE* temp = g_A_STAR.GetNextPosition();
    int sY;
    int sX;
    int eY;
    int eX;
    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hPathPen);

    while (temp != NULL)
    {
        sY = temp->_Y * GRID_SIZE + GRID_SIZE / 2 + st_RenderStart._y;
        sX = temp->_X * GRID_SIZE + GRID_SIZE / 2 + st_RenderStart._x;
        eY = temp->_pY * GRID_SIZE + GRID_SIZE / 2 + st_RenderStart._y;
        eX = temp->_pX * GRID_SIZE + GRID_SIZE / 2 + st_RenderStart._x;
        MoveToEx(hdc, sX, sY, NULL);
        LineTo(hdc, eX, eY);

        temp = g_A_STAR.GetNextPosition();
    }

    SelectObject(hdc, hOldPen);
}

void RenderList(HDC hdc)
{
    std::map<pair<int, int>, st_A_START_NODE>::iterator iter;
    st_A_START_NODE tempNode;
    HBRUSH hOldBrush;
    HFONT hFont;
    int iX;
    int iY;
    wchar_t Buff[50];

    hFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PALETTE | FF_DONTCARE, L"Tahoma");
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(0, 0, 0));

    for (iter = g_A_STAR.BeginIter(); iter != g_A_STAR.EndIter(); ++iter)
    {
        tempNode = iter->second;

        if (tempNode._X == st_Start._x && tempNode._Y == st_Start._y)
            continue;
        if (tempNode._X == st_End._x && tempNode._Y == st_End._y)
            continue;

        if (tempNode.isOpenList == true)
        {
            hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileOpenListBrush);

            iX = tempNode._X * GRID_SIZE + st_RenderStart._x;
            iY = tempNode._Y * GRID_SIZE + st_RenderStart._y;
            Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
            SelectObject(hdc, hOldBrush);
        }
        else
        {
            hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileCloseListBrush);
            iX = tempNode._X * GRID_SIZE + st_RenderStart._x;
            iY = tempNode._Y * GRID_SIZE + st_RenderStart._y;
            Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
            SelectObject(hdc, hOldBrush);
        }

        if (GRID_SIZE > 46)
        {
            swprintf_s(Buff, L"G: %0.1lf", tempNode._G);
            TextOut(hdc, iX + 1, iY + 1, Buff, lstrlen(Buff));

            swprintf_s(Buff, L"H: %d", tempNode._H);
            TextOut(hdc, iX + 1, iY + 14, Buff, lstrlen(Buff));

            swprintf_s(Buff, L"F: %0.1lf", tempNode._F);
            TextOut(hdc, iX + 1, iY + 27, Buff, lstrlen(Buff));
        }
    }
}

