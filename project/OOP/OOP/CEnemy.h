#ifndef __CENEMY_H__
#define __CENEMY_H__

class CEnemy : public CBaseObject
{
private:
	char		_Skin;
	int			_Hp;
	int			_iY;
	int			_iX;

	struct Moving
	{
		int _CoolTime;
		int dY;
		int	dX;
	};

	CList<Moving> MovingList;
	CList<Moving>::iterator iter;

	
public:
	CEnemy(int ObjectType, char cSkin, int iHp, int iY, int iX, char* cMoveFile, char* GunFile);
	virtual ~CEnemy();

	virtual bool	Update(void);
	virtual void	Render(void);

private:
	void GetMoving(char* cMoveFile);
	void Move(void);

};

#endif // !__CENEMY_H__

