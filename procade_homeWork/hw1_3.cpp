#include <stdio.h>

int main()
{
    // 4byte 32bit;
    unsigned int num = 0;
    unsigned int bytePosition;
    unsigned int insertByte;

    while(1)
    {
        printf("1~4의 바이트 위치를 입력하세요: ");
        scanf("%d", &bytePosition);

        printf("해당 위치에 넣을 데이터 0~255를 입력하세요: ");
        scanf("%d", &insertByte);

        // bytePosition별로 위치가 다르다.
        if(bytePosition == 1)
        {
            // 첫 8비트 0으로 초기화-> 첫 8비트를 제외한 나머지 비트를 마스킹 하여 추출한다.
            num = num & ~(0xff);
            // insertByte를 num과 합친다.
            num = num | insertByte;
        }
        else if(bytePosition == 2)
        {
            // 2byet 위치의 비트를 0으로 초기화 한다. 
            num = num & ~(0xff00);
            // insertByte 위에 8bit의 0이 생성됨. 
            num = num | (insertByte << 8);
        }
        else if(bytePosition == 3)
        {
            // 3byte 위치의 비트를 0으로 초기화 한다. 
            num = num & ~(0xff0000);
            num = num | (insertByte << 16);
        }
        else if(bytePosition == 4)
        {
            // 4byte 위치의 비트를 0으로 초기화 한다. 
            num = num & ~(0xff000000);
            num = num | (insertByte << 24);
        }
        
        // 출력 
        printf("%x \n", num);
        printf("%p \n", num);
    }
}
