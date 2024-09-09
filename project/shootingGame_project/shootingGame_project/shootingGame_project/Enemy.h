#ifndef __ENEMY_H__
#define __ENEMY_H__

#define MAX_ENEMY				20
#define MAX_MOVING_PATTERN		10
#define MAX_ENEMY_BULLET		200
#define MAX_SHOT_ONE			5

struct st_ENEMY_BULLET
{
	bool bVisible;			

	char cBulletSkin;		// 총알 모양

	int iDamage;

	float fY;				// 현재 총알의 좌표
	float fX;				//


	int iDireY;				// 총알이 움직일 상대 좌표
	int iDireX;				//

	float fBulletSpeed;		// 총알의 속도

};

struct st_ENEMY
{
	bool bVisible;			// 적의 생존 여부

	int iHp;				// 적의 HP

	char cEnemySkin;		// 적의 모습

	float fY;				// 적의 위치 정보
	float fX;				//



	//------------------------------------------------------------
	// 적의 움직임
	// 
	//------------------------------------------------------------
	int iMovingPatternCnt;						// 움직임의 갯수
												// 최대 10개

	int iNextPattern;							// 다음 행동 가리킴
	float fMovingCoolTimeCounter;						// 움직임에 대한 쿨타임

	int iDireY[MAX_MOVING_PATTERN];				// 움직임 상대 좌표
	int iDireX[MAX_MOVING_PATTERN];				//
	
	float fMovingCoolTime[MAX_MOVING_PATTERN];	// 움직임의 쿨타임
	float fMovingSpeed[MAX_MOVING_PATTERN];		// 움직이는 속도


	//------------------------------------------------------------
	// 총알
	// 
	//------------------------------------------------------------

	char cBulletSkin[MAX_SHOT_ONE];				// 총알 모양
	int iShotOne;								// 한번에 쏠 총알의 수
	int iDamage[MAX_SHOT_ONE];
	int iBulletDireY[MAX_SHOT_ONE];
	int iBulletDireX[MAX_SHOT_ONE];
	float fCoolTimeCounter[MAX_SHOT_ONE];	// 총알의 쿨타임 측정

	float fBulletCoolTime[MAX_SHOT_ONE];	// 총알의 연사 속도

	float fBulletSpeed[MAX_SHOT_ONE];		// 총알의 속도
	
};

//---------------------------------------------------------------------------------
// 적 정보를 초기화 한다.
// 전역 변수인데 왜함?
// 필요 없음
// 
//---------------------------------------------------------------------------------
void enmy_Initial(void);


//---------------------------------------------------------------------------------
// 적 정보를 불러온다.
// 
//---------------------------------------------------------------------------------
bool enmy_Is_Get_Info(char* cEnemyInfo);

//---------------------------------------------------------------------------------
// 적 움직임을 불러온다.
// 
//---------------------------------------------------------------------------------
bool enmy_Is_Get_Moving_Info(char* pMovingFileText, int* iMovingPatternCnt, float* fMovingCoolTime, int* iDireY, int* iDireX, float* fMovingSpeed);


//---------------------------------------------------------------------------------
// 총알 정보를 불러온다.
// 
//---------------------------------------------------------------------------------
bool enmy_Is_Get_Bullet_Info(char* pBulletFileText, int* iShotOne, char* cBulletSkin, int* iDamage, int* iDireY, int* iDireX, float* fBulletCoolTime, float* fBulletSpeed);






//---------------------------------------------------------------------------------
// 적 이동
// 
//---------------------------------------------------------------------------------
void enmy_Move_Enemy(void);

//---------------------------------------------------------------------------------
// 총알 발싸
// 
//---------------------------------------------------------------------------------
void enmy_Shoot_Bullet(void);
//---------------------------------------------------------------------------------
// 총알 이동
// 
//---------------------------------------------------------------------------------
void enmy_Move_Bullet(void);
//---------------------------------------------------------------------------------
// 적 총알과 플레이어와 충돌 확인
// 
//---------------------------------------------------------------------------------
void enmy_Bullet_Hit_Player(void);
//---------------------------------------------------------------------------------
// 적과 충동판정
// 
//---------------------------------------------------------------------------------
bool enmy_Is_Hit(int iY, int iX, int IDamage);
//---------------------------------------------------------------------------------
// 적이 다 죽어서 이겼는지 확인한다.
// 
//---------------------------------------------------------------------------------
bool enmy_Is_Win(void);




//---------------------------------------------------------------------------------
// 스크린 버퍼에 적을 그린다.
// 
//---------------------------------------------------------------------------------
void enmy_Draw_Enemy(void);
//---------------------------------------------------------------------------------
// 스크린 버퍼에 총알을 그린다.
// 
//---------------------------------------------------------------------------------
void enmy_Draw_Enemy_Bullet(void);










//---------------------------------------------------------------------------------
// 쿨타임
// 
//---------------------------------------------------------------------------------
void enmy_CoolTime(void);


//---------------------------------------------------------------------------------
// 움직임에 대한 쿨타임
// 
//---------------------------------------------------------------------------------
void enmy_Moving_CoolTime(void);



//---------------------------------------------------------------------------------
// 총알에 대한 쿨타임
// 
//---------------------------------------------------------------------------------
void enmy_Bullet_CoolTime(void);






#endif // !__ENEMY_H__
