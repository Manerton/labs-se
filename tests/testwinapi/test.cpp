// test.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "test.h"

#define MAX_LOADSTRING 100
// Глобальные переменные:
static HINSTANCE hInst;								// -- текущий экземпляр -- //
static WCHAR szTitle[MAX_LOADSTRING];				// -- текст строки заголовка -- //
static WCHAR szWindowClass[MAX_LOADSTRING];			// -- имя класса главного окна -- //
static HWND menuWindow;
static HWND testWindow;

// -- объявления функций, включенных в этот модуль кода -- //
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// -- главная функция -- //
int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance)
    UNREFERENCED_PARAMETER(lpCmdLine)

    MSG msg;

    // -- инициализация глобальных строк из файла ресурсов -- //
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance); // -- регистрируем класс окна -- //

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST)); // -- для горячих клавиш -- //

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return int(msg.wParam);
}
// -- регистрируем класс окна, перед его созданием -- //
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
    wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground	= HBRUSH(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEST);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

// -- создаем (инициализируем) главное окно программы -- //
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // -- сохранить дескриптор экземпляра в глобальной переменной -- //

   HWND hWnd = CreateWindow(szWindowClass,  // -- имя зарегистрированного класса -- //
                            szTitle,    // -- имя окна -- //
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,    // -- стиль окна -- //
                            CW_USEDEFAULT,  // -- гориз. позиция -- //
                            0,              // -- вертикальная позиция -- //
                            800,  // -- ширина окна -- //
                            600,              // -- высота окна -- //
                            nullptr,        // -- дескриптор родительского окна -- //
                            nullptr,        // -- дескриптор меню окна -- //
                            hInstance,      // -- дескриптор экземпляра приложения -- //
                            nullptr            // -- указатель на данные, передаваемые в сообщении -- //
                            ); // -- создаем окно -- //
   menuWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MENUWINDOW),hWnd,DLGPROC(WndProc));
   testWindow = CreateWindow(szWindowClass, szTitle, WS_CHILD, 0, 300,
                800, 300, hWnd, nullptr, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"menu", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 200,
                   50, 50, menuWindow, HMENU(555), hInstance, nullptr);
   CreateWindow(L"BUTTON", L"test", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 100,
                   50, 50, testWindow, HMENU(556), hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   ShowWindow(menuWindow,nCmdShow);
   //ShowWindow(testWindow,nCmdShow);

   UpdateWindow(hWnd);

   return TRUE;
}

// -- обрабатывает сообщения (сигналы, события) в главном окне -- //
//  WM_COMMAND	- обработка меню приложения -- //
//  WM_PAINT	- закрасить главное окно -- //
//  WM_DESTROY	- ввести сообщение о выходе и вернуться -- //
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    //PAINTSTRUCT ps;
    //HDC hdc;

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case 555:
            ShowWindow(menuWindow, SW_HIDE);
            ShowWindow(testWindow, SW_SHOW);
            break;
        case 556:
            ShowWindow(testWindow, SW_HIDE);
            ShowWindow(menuWindow, SW_SHOW);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
//    case WM_PAINT:
//        hdc = BeginPaint(hWnd, &ps);
//        EndPaint(hWnd, &ps);
//        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}
