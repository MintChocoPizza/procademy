#ifndef __CSCREENBUFFER_H__
#define __CSCREENBUFFER_H__



class CScreenBuffer
{
private:
    char ScreenBuffer[24][81];
    static CScreenBuffer _CScreenbuffer;

private:~CScreenBuffer();
private:CScreenBuffer();

public: 
    static CScreenBuffer *GetInstance(void);

public:
    void Flip(void);
    void Clear(void);
    void Draw(int iY, int iX, char chSprite);
};

#endif
