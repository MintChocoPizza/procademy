
#include <cmath>

#include <windowsx.h>
#include "framework.h"
#include <map>
#include "My.h"

#include "A_STAR.h"


extern MSG msg;
extern HACCEL hAccelTable;

A_STAR::A_STAR()
{
}

A_STAR::~A_STAR()
{
}

A_STAR::A_STAR(int Width, int Height) : _Width(Width), _Height(Height)
{

}

bool A_STAR::find(int sY, int sX, int eY, int eX, HWND hWnd)
{
	double G;
	int H;
	double F;
	int iCnt;
	int nY;
	int nX;
	st_FindNode st_ParentsNode;
	st_FindNode	st_ChildNode;
	multimap<double, st_FindNode>::iterator		OpenIter;
	map<pair<int, int>, st_FindNode>::iterator	CloseIter;


	_eY = eY;
	_eX = eX;


	// 시작 위치를 OpenList와 CloseList에 담는다. 
	// whild문에서 Node를 뽑고 실제 방문 처리인 Close 처리를 한다.
	H = abs(sY - eY) + abs(sX - eX);
	st_ParentsNode = { -1, -1, sY, sX, 0, H, (double)H, true};
	_OpenList.insert(
		pair<double, st_FindNode>(
			(double)H, st_ParentsNode
		)
	);
	_CloseList.insert(
		make_pair(
			make_pair(sY, sX), st_ParentsNode
		)
	);


	while (1)
	{
		if (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		InvalidateRect(hWnd, NULL, false);

		//----------------------------------------------
		// F값이 가장 작은 노드를 꺼낸다.
		//----------------------------------------------
		OpenIter = _OpenList.begin();
		st_ParentsNode = OpenIter->second;

		// _OpenList에서 부모 노드를 제거하고, CloseList에서 OpenList 값을 false로 바꾼다.
		CloseIter = _CloseList.find(make_pair(st_ParentsNode._Y, st_ParentsNode._X));
		(CloseIter->second).OpenList = false;
		_OpenList.erase(OpenIter);

		// 목적지에 도착하면 루프를 끝낸다. 
		if (st_ParentsNode._Y == eY && st_ParentsNode._X == eX)
		{
			_iter = CloseIter;
			return true;
		}

		// 8 방향으로 갈 수 있는 노드를 탐색한다.
		for (iCnt = 0; iCnt < 8; ++iCnt)
		{
			nY = st_ParentsNode._Y + dy[iCnt];
			nX = st_ParentsNode._X + dx[iCnt];


			//----------------------------------------------
			// 예외 처리
			// 
			//----------------------------------------------
			if (nY < 0 || nX < 0 || _Height <= nY || _Width <= nX)
			{
				// 맵 크기를 벗어 난 경우 
				continue;
			}
			if (g_Tile[nY][nX] == 1)
			{
				// 벽인 경우
				continue;
			}

			//----------------------------------------------
			// G, H, F 값 계산
			//----------------------------------------------
			//G = st_ParentsNode._G +
			//	sqrt(
			//	pow(nY - st_ParentsNode._Y, 2) + pow(nX - st_ParentsNode._X, 2)
			//);
			G = st_ParentsNode._G;
			if (st_ParentsNode._Y == nY || st_ParentsNode._X == nX)
				G += 1;
			else
				G += 1.5;
			H = abs(nY - eY) + abs(nX - eX);
			F = G + H;


			//----------------------------------------------
			// 경로의 중복 검사 
			// 
			//----------------------------------------------
			CloseIter = _CloseList.find(make_pair(nY, nX));
			if (CloseIter != _CloseList.end())
			{
				st_ChildNode = CloseIter->second;
				// 예외처리
				if (st_ChildNode.OpenList == false)
					continue;
				if (st_ChildNode._G < G)
					continue;

				//---------------------------------
				// _CloseList
				// 1. 부모 좌표 변경하기
				// 2. G, F값 변경하기
				//---------------------------------
				(CloseIter->second)._G = G;
				//(CloseIter->second)._F = F;
				(CloseIter->second)._PY = st_ParentsNode._Y;
				(CloseIter->second)._PX = st_ParentsNode._X;




				//---------------------------------
				// _OpenList
				// 1. 노드 찾아서 삭제하기
				// 2. 새로운 정보가 기록된 노드저장
				//---------------------------------
				multimap<double, st_FindNode>::iterator iter;
				for (iter = _OpenList.lower_bound(st_ChildNode._F); iter != _OpenList.upper_bound(st_ChildNode._F); ++iter)
				{
					if ((iter->second)._Y != nY || (iter->second)._X != nX)
						continue;

					_OpenList.erase(iter);
					break;
				}
				(CloseIter->second)._F = F;
				_OpenList.insert(
					pair<double, st_FindNode>(
						F, st_FindNode{ st_ParentsNode._Y, st_ParentsNode._X, nY, nX, G, H, F }
					)
				);

			}
			else
			{
				// 중복이 아닌 경우
				st_ChildNode = { st_ParentsNode._Y, st_ParentsNode._X, nY, nX, G, H, F, true };
				_OpenList.insert(
					pair<double, st_FindNode>(
						(double)H, st_ChildNode
					)
				);
				_CloseList.insert(
					make_pair(
						make_pair(nY, nX), st_ChildNode
					)
				);
			}

		}
	}

	return false;
}

st_FindNode* A_STAR::GetNextPosition(void)
{
	st_FindNode* temp = &(_iter->second);

	if (temp->_PY == -1 && temp->_PX == -1)
	{
		_iter = _CloseList.find(make_pair(_eY, _eX));
		return NULL;
	}

	_iter = _CloseList.find(make_pair(temp->_PY, temp->_PX));

	return temp;
}


void A_STAR::GetMap(int Width, int Height)
{
	_Width = Width;
	_Height = Height;
}

bool A_STAR::GetST(st_FindNode*st, int iY, int iX)
{
	map<pair<int, int>, st_FindNode>::iterator	iter;
	iter = _CloseList.find(make_pair(iY, iX));

	if (iter == _CloseList.end())
		return false;


	*st = iter->second;

	return true;
}


