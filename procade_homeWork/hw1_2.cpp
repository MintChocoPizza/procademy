#include <stdio.h>

int main()
{
    // 2byte 16bit
    unsigned short s = 0;
    int position;
    int bit;

    while(1)
    {
        printf("1~16비트 변경하고 싶은 자리를 누르거나 끝내고 싶다면 0을 누르세요: ");
        scanf("%d", &position);

        if(position == 0)
        {
            break;
        }
        if(position > 16)
        {
            break;
        }
        
        printf("1 on, 0 off 를 입력하세요: ");
        scanf("%d", &bit);
        
        printf("현재 바꿀 비트 위치 %d, 비트 %d \n", position, bit);

        if(bit == 1)
        {
            s = s | (1 << (position-1));
        }
        else 
        {
            s = s & ~(1 << (position-1));
        }

        // 출력
        for(int i=15; i>=0; --i)
        {
            if(((s >> i)&1) == 1)
                printf("1");
            else 
                printf("0");
        }
        printf("\n");
    }
    


    return 0;
}
