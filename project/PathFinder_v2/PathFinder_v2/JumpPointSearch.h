

#ifndef __JUMPPOINTSEARCH_H__
#define __JUMPPOINTSEARCH_H__

enum Direction
{
	UU = 0,
	DD,
	LL,
	RR,

	LU,
	RU,
	LD,
	RD
};

struct st_JPS_NODE
{
	int _pY;
	int _pX;

	int _Y;
	int _X;

	// 가중치 
	double	_G;		// 출발점으로 부터의 이동 거리, 유클리드(대각선)
	int		_H;		// 목적지와의 거리, 맨헤튼(가로 + 세로)
	double	_F;		// G + H

	bool isOpenList;
};

class CJumpPointSearch
{
public:
	CJumpPointSearch();
	~CJumpPointSearch();


	//---------------------------------------------------------------
	// 길찾기를 시작한다. 
	//---------------------------------------------------------------
	bool find(int sY, int sX, int eY, int eX, HWND hWnd);

	//---------------------------------------------------------------
	//	주어진 방향에 대하여 탐색하고, 노드를 저장한다. 
	//---------------------------------------------------------------
	void Jump(int sY, int sX, Direction direction);

	//---------------------------------------------------------------
	// 코너를 검사한다.
	//---------------------------------------------------------------
	bool check(int y, int x);


	void SearchUU(int y, int x);

public:
	// Key: F	... Value: Node
	std::multimap<double, st_JPS_NODE>			_OpenList;

	// Key: <y,x> ... Value: Node
	std::map<std::pair<int, int>, st_JPS_NODE>	_CloseList;


	// 상하좌우 좌상, 우상, 좌하, 우하
	int _dy[8] = { -1, 1, 0 ,0, -1, -1, 1, 1 };
	int _dx[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

};
#endif // !__JUMPPOINTSEARCH_H__
