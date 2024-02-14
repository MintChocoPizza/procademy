// FPS_test_fromCopilot.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <time.h>


// 이동할 속도를 상수로 정의합니다.
#define SPEED 10



int main() {
    // 현재 위치를 저장하는 변수입니다.
    float position = 0;

    // 이전 프레임의 시간을 저장하는 변수입니다.
    long previousTime = clock();

    // 무한 루프로 게임 루프를 구현합니다.
    while (1) {
        // 현재 프레임의 시간을 구합니다.
        long currentTime = clock();

        // 이전 프레임과 현재 프레임 사이의 시간 간격을 구합니다.
        float deltaTime = (currentTime - previousTime) / 1000.0f;

        // 이전 프레임의 시간을 현재 프레임의 시간으로 갱신합니다.
        previousTime = currentTime;

        // 이동량을 계산합니다. 속도에 시간 간격을 곱해줍니다.
        float movement = SPEED * deltaTime;

        // 현재 위치에 이동량을 더해줍니다.
        position += movement;

        // 현재 위치를 출력합니다.
        printf("Current position: %f\n", position);
    }

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
