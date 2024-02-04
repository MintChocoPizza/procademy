// hw9_BMP.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int main()
{
    errno_t err;
    FILE* pFile;
    size_t llRWFileSize;
    long lFileSize;
    DWORD headerSize;
    DWORD bitmapFileSize;

    tagBITMAPFILEHEADER stFileHeader;
    tagBITMAPINFOHEADER *pInfoHeader;
    unsigned char* pHeader;


    unsigned char* pFileMemory = NULL;

    err = fopen_s(&pFile, "b.bmp", "rb");
    if (err != NULL)
    {
        printf_s("파일 오픈 실패 \n");
        return -1;
    }
    //=======================================================
    // 파일의 전체 크기를 구한다. 
    //=======================================================
    fseek(pFile, 0, SEEK_END);
    lFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    





    //=======================================================
    // BMP 파일이 맞는지 헤더를 열어 확인한다.
    //=======================================================
    llRWFileSize = fread_s(&stFileHeader, sizeof(tagBITMAPFILEHEADER), 1, sizeof(tagBITMAPFILEHEADER), pFile);
    if (llRWFileSize != sizeof(tagBITMAPFILEHEADER))
    {
        printf_s("파일을 읽는데 실패했습니다. \n");
        fclose(pFile);
        return -1;
    }
    if (stFileHeader.bfType != 0x4d42)
    {
        printf_s("비트맵 파일이 아닙니다. \n");
        fclose(pFile);
        return -1;
    }
    

    //=======================================================
    // 1. DWORD 만큼 파일 사이즈를 읽는다. 
    // 2. 파일 포인터를 읽은 만큼 뒤로 돌린다.
    // 3. 동적 할당으로 사이즈 만큼의 메모리에 데이터를 넣는다. 
    // 4. 헤더파일 사이즈에 맞는 헤더를 포인터로 가리킨다.
    // 
    // 만약 헤더파일의 크기가 커서 잘리더라도 밑의 데이터는 필요 없다.
    //=======================================================
    llRWFileSize = fread_s(&headerSize, sizeof(DWORD), 1, sizeof(DWORD), pFile);
    if (llRWFileSize != sizeof(DWORD))
    {
        printf_s("파일을 읽는데 실패했습니다. \n");
        fclose(pFile);
        return -1;
    }
    fseek(pFile,
        //-1 * sizeof(DWORD),
        -4,
        SEEK_CUR);
    pHeader = (unsigned char*)malloc(headerSize);
    if (pHeader == NULL)
    {
        printf_s("메모리 할당 실패 \n");
        fclose(pFile);
        return -1;
    }
    int size1 = _msize(pHeader);
    llRWFileSize = fread_s(pHeader, headerSize, 1, headerSize, pFile);
    if (llRWFileSize != headerSize)
    {
        printf_s("파일을 읽는데 실패했습니다. \n");
        fclose(pFile);
        return -1;
    }
    pInfoHeader = (tagBITMAPINFOHEADER*)pHeader;



    //=======================================================
    // 1. 비트맵 사이즈를 계산한다.
    //  1-1. 가로픽셀 수 * 높이픽셀 수 * 픽셀당 비트 수  => 비트맵 크기(bit의 갯수)
    //  1-2. 가로픽셀 수 * 높이픽셀 수 * (픽셀당 비트 수(32bit)/8(bit) == 4byte) => 비트맵 크기(byte의 갯수)
    //  1-3. 해당 파일은 32bit 파일이므로 1픽셀에 4byte를 사용한다.
    // 2. 사이즈 만큼 메모리를 동적할당한다. 
    // 3. 메모리에 데이터를 쓴다. 
    // 
    // 동적 할당 메모리가 너무 커서 한번에 할당이 안된다.
    // 작은 파일 24bit 기준으로 
    // 
    // 
    // 
    //=======================================================
    //bitmapFileSize = (*pInfoHeader).biWidth * (*pInfoHeader).biHeight * (*pInfoHeader).biBitCount / 8;
    bitmapFileSize = ftell(pFile);
    bitmapFileSize = lFileSize - bitmapFileSize;
    pFileMemory = (unsigned char*)malloc(bitmapFileSize);
    if (pFileMemory == NULL)
    {
        printf_s("메모리 할당 실패 \n");
        fclose(pFile);
        return -1;
    }
    int size2 = _msize(pFileMemory);
    llRWFileSize = fread_s(pFileMemory, bitmapFileSize, 1, bitmapFileSize, pFile);
    if (llRWFileSize != bitmapFileSize)
    {
        printf_s("파일을 읽는데 실패했습니다. \n");
        fclose(pFile);
        return -1;
    }
    fclose(pFile);







    //=======================================================
    // 변경한 비트맵을 저장하기 위해 쓰기로 파일을 오픈한다.
    //=======================================================
    err = fopen_s(&pFile, "b_copy.bmp", "wb");
    if (err != NULL)
    {
        printf_s("파일 오픈 실패 \n");
        return -1;
    }

    unsigned char* pPointer = pFileMemory;
    for (int y = 0; y < (*pInfoHeader).biHeight; ++y)
    {
        for (int x = 0; x < (*pInfoHeader).biWidth; ++x)
        {
            //unsigned char r;
            //unsigned char g;
            //unsigned char b;
            //// 0xAA RR GG BB
            //// 메모리상 BB GG RR AA
            //b = *(((unsigned char*)pPointer)+0);
            //g = *(((unsigned char*)pPointer)+1);
            //r = *(((unsigned char*)pPointer)+2);

            //*(((unsigned char*)pPointer) + 0) = b-30;
            //*(((unsigned char*)pPointer) + 1) = g-30;
            //*(((unsigned char*)pPointer) + 2) = r-30;

            //=======================================================
            // 여기는 한 픽셀 -> 3Byte
            // pPointer은 1Byte
            // 
            // 반복문을 3번 돌려 데이터를 변경한다.
            //=======================================================

            *pPointer += 50;
            ++pPointer;
        }
    }


    //=======================================================
    // 변경한 파일을 
    // 헤더 
    // 헤더인포
    // 데이터 
    // 순으로 쓴다.
    //=======================================================
    llRWFileSize = 0;
    llRWFileSize += fwrite(&stFileHeader, 1, sizeof(tagBITMAPFILEHEADER), pFile);
    llRWFileSize += fwrite(pHeader, 1,headerSize, pFile);
    llRWFileSize += fwrite(pFileMemory, 1, bitmapFileSize, pFile);
    fclose(pFile);

    if (lFileSize != llRWFileSize)
    {
        printf_s("읽은 파일의 크기와 저장한 파일의 크기가 다릅니다. \n");
    }

    printf_s("비트맵 변환 완료!!! \n");
    printf_s("아무키나 누르세요 \n");
    _getch();

    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
