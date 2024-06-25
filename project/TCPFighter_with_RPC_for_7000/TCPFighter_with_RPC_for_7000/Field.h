
//--------------------------------------------------------------------------------------------------------------------
// 필드 클래스를 만든 이유:
// 
// 유저들의 위치를 필드 단위로 설정하는 방법:
//	1. 모든 유저들의 x,y 좌표를 그리드 단위로 나누어 해당하는 그리드를 위치를 구해낸다.  
//		단점: 패킷을 보낼 때 마다, 모든 유저들을 대상으로 x,y 좌표를 기준으로 연산하여 그리드 위치를 구해내야 한다. 
// 
//	2. 생성 이동, 등등 플레이어의 움직임이 있을 때 마다, 해당 그리드에 포함되는 플레이어들을 저장해둔다 .
//		플레이어들의 위치가 필요할 때 마다, 그냥 해당 그리드를 탐색하면 해당 위치에 있는 플레이어를 한번에 알 수 있다. 
//--------------------------------------------------------------------------------------------------------------------

#ifndef __FIELD_H__
#define __FIELD_H__

constexpr int dfRANGE_MOVE_TOP = 0;
constexpr int dfRANGE_MOVE_LEFT = 0;
constexpr int dfRANGE_MOVE_RIGHT = 6400;
constexpr int dfRANGE_MOVE_BOTTOM = 6400;

using namespace std;

class C_Field
{
	enum
	{
		Grid_X_Size = 100,
		Grid_Y_Size = 100
	};

public:
	

public:
	static C_Field* GetInstance(void);

private:
	C_Field();
	~C_Field();

private:
	static C_Field _C_Field;

public:
	// Key: <GridY, GridX>, Value: SessionID
	std::unordered_multimap<std::pair<int, int>, DWORD> _Grid;
};

#endif // !__FIELD_H__
