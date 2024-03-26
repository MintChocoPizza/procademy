#ifndef __CTITLEOBJECT_H__
#define __CTITLEOBJECT_H__


class CTitleObject : public CBaseObject
{
private:
	unsigned char* _Buff;
	char cGameName[30];
	char cGameVersion[10];


public:
	CTitleObject(const char* ccpFileName, const char* ccpGameInfo, int ObjectType);
	virtual ~CTitleObject();

	virtual bool Update(void);
	virtual void Render(void);
};


#endif // !__CTITLEOBJECT_H__

