
#include <map>
#include <Windows.h>

#include "resource.h"
#include "framework.h"
#include "RegisterClass.h"
#include "Initstance.h"

#include "Tile.h"

#include "WndProc.h"
#include "PathFinder_v2.h"

#include "JumpPointSearch.h"

using namespace std;

CJumpPointSearch::CJumpPointSearch()
{
}

CJumpPointSearch::~CJumpPointSearch()
{
}

bool CJumpPointSearch::find(int sY, int sX, int eY, int eX, HWND hWnd)
{
	int direction;

	double	G;
	int		H;
	double	F;

	int		curY;
	int		curX;
	int		nY;
	int		nX;

	st_JPS_NODE		st_CurNode;
	st_JPS_NODE		st_NextNode;

	multimap<double, st_JPS_NODE>::iterator		OpenIter;
	map<pair<int, int>, st_JPS_NODE>::iterator	CloseIter;

	_OpenList.clear();
	_CloseList.clear();

	
	// 초기 노드를 세팅해준다. 
	H = abs(sY - eY) + abs(sX - eX);
	st_CurNode = { -1, -1, sY, sX, 0, H, (double)H * 5, true };

	_OpenList.insert(
		pair<double, st_JPS_NODE>(
			(double)H, st_CurNode
		)
	);
	_CloseList.insert(
		make_pair(
			make_pair(sY, sX), st_CurNode
		)
	);



	while (_OpenList.empty() != true)
	{
		OpenIter = _OpenList.begin();
		st_CurNode = OpenIter->second;

		CloseIter = _CloseList.find(make_pair(st_CurNode._Y, st_CurNode._X));
		(CloseIter->second).isOpenList = false;
		_OpenList.erase(OpenIter);

		// 기저사례: 목적지에 도착했다면 종료한다. 
		if (st_CurNode._Y == eY && st_CurNode._X == eX)
		{
			return true;
		}

		// 부모로 부터 8방향을 선택하여 탐색
		// 선택된 방향에서 노드를 만들거나(코너를 찾았다), 길이 막히면 할일이 끝난다. 
		for (direction = 0; direction < 8; ++direction)
		{
			Jump(st_CurNode._Y, st_CurNode._X, static_cast<Direction>(direction));
		}
	}
	return false;
}

void CJumpPointSearch::Jump(int sY, int sX, Direction direction)
{
	int nY;
	int nX;

	nY = sY;
	nX = sX;

	while (1)
	{
		switch (direction)
		{
		case UU:
		{
			nY += _dy[direction];
			nX += _dx[direction];

			//----------------------------------------------
			// 기저 사례
			//----------------------------------------------
			if (nY < 0 || nX < 0 || g_GRID_HEIGHT <= nY || g_GRID_WIDTH <= nX)
			{
				// 맵 크기를 벗어 난 경우 
				return;
			}
			if (g_CTile.st_GRID[nY][nX] == 1)
			{
				// 벽인 경우
				return;
			}

			//----------------------------------------------
			// UU 에서 코너인 경우
			// 1. 위가	 막히고, 대각선 위가 뚤린경우
			// 2. 아래가 막히고, 대각선 아래가 뚤린경우
			//----------------------------------------------
			if (!check(nY - 1, nX) && check(nY - 1, nX + 1) ||
				!check(nY + 1, nX) && check(nY + 1, nX + 1))
			{
				// 코너이다. 
				
			}

			
		}
		break;
		case DD:
			break;
		case LL:
			break;
		case RR:
			break;
		case LU:
			break;
		case RU:
			break;
		case LD:
			break;
		case RD:
			break;
		default:
			break;
		}
	}
}



bool CJumpPointSearch::check(int y, int x)
{
	if (g_CTile.st_GRID[y][x] == 1) return false;
	else return true;
}

