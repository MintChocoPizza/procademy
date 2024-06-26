#pragma once



#define MAX_LOADSTRING 100

// 전역 변수:
extern HINSTANCE hInst;                                // 현재 인스턴스입니다.
extern WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
extern WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
extern HACCEL hAccelTable;
extern MSG msg;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
