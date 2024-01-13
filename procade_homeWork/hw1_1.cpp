#include <stdio.h>

int main() {
    
    // 1byte 8bit
    // 0000 0000
    unsigned char ch = 54;
    unsigned char mask = 0x80;

    printf("숫자를 입력하세요: ");
    scanf("%d", &ch);
    printf("%d\n", ch);

    printf("1번 방법: ");
    unsigned char copy = ch;
    for(int i=0; i<8; ++i)
    {
        if((copy & mask) == mask)
            printf("1");
        else 
            printf("0");
        copy = copy << 1;
    }

    printf("\n2번 방법: ");
    copy = ch;
    for(int i=7; i>=0; --i)
    {
        if(((copy>>i) & 1) == 1)
        {
            printf("1");
        }
        else 
        {
            printf("0");
        }
    }

    // 안됨
    printf("\n3번 방법: ");
    copy = ch;
    for(int i=0; i<7; ++i)
    {
        if(((copy >> i)&1) == 1)
        {
            printf("1");
        }
        else 
        {
            printf("0");
        }
    }

    return 0;
}
