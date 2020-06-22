// -- системные либы -- //
#include <windows.h>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "resource.h"

// -- мои классы -- //
#include "bitstring.h"
#include "five.h"

#define MAX_LOADSTRING 100

using namespace std;

// Глобальные переменные:
static HINSTANCE hInst;								// -- текущий экземпляр приложения -- //
static WCHAR szTitle[MAX_LOADSTRING];				// -- текст строки заголовка -- //
static WCHAR szWindowClass[MAX_LOADSTRING];			// -- имя класса главного окна -- //
// -- объявления функций, включенных в этот модуль кода -- //
ATOM				MyRegisterClass(HINSTANCE hInstance);   // -- регистрация класса главного окна -- //
BOOL				InitInstance(HINSTANCE, int);   // -- инициализация главного окна -- //
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);    // -- обработчик сообщений для главного окна -- //
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);  // -- обработчик сообщений для диалогового окна "О программе" -- //
#include <iostream>
vector<string> get_str_result() // получаем рассчеты в виде строк
{
    vector<string> res;
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_int_distribution<int> dist_for_len(1, 5);
    uniform_int_distribution<int> dist_for_digit(0, 4);

    size_t len1 = dist_for_len(mt);
    size_t len2 = dist_for_len(mt);

    string op1_str = "";
    for (size_t i = 0; i < len1; ++i)
    {
        op1_str += to_string(dist_for_digit(mt));
    }

    string op2_str = "";
    for (size_t i = 0; i < len2; ++i)
    {
        op2_str += to_string(dist_for_digit(mt));
    }

    Five op1(op1_str);
    Five op2(op2_str);

    if (op1<op2) swap(op1,op2);

    res.push_back(op1.toString());
    res.push_back(op2.toString());

    Five res_of_op = op1 + op2;
    res.push_back(res_of_op.toString());

    res_of_op = op1 - op2;
    res.push_back(res_of_op.toString());

    res_of_op = op1 * op2;
    res.push_back(res_of_op.toString());

    if (size_t(std::count(op2.begin(),op2.end(),0)) != op2.get_size())
    {
        res_of_op = op1 / op2;
        res.push_back(res_of_op.toString());
    }
    else
    {
        res.push_back("zero divide");
    }

    return res;
}

// -- главная функция -- //
int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance)
    UNREFERENCED_PARAMETER(lpCmdLine)
    setlocale( LC_ALL, "rus" );
    // -- инициализация глобальных строк из файла ресурсов -- //
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MAIN, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance); // -- регистрируем класс окна -- //

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAIN)); // -- для горячих клавиш -- //

    MSG msg;
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
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
    wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground	= HBRUSH(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MAIN);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

// -- создаем (инициализируем) главное окно программы -- //
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // -- сохранить дескриптор экземпляра в глобальной переменной -- //

   HWND hWnd = CreateWindowEx(
                            WS_EX_CONTROLPARENT,
                            szWindowClass,  // -- имя зарегистрированного класса -- //
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
                            );
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// -- обрабатывает сообщения (сигналы, события) в главном окне -- //
//  WM_COMMAND	- обработка меню приложения -- //
//  WM_DESTROY	- ввести сообщение о выходе и вернуться -- //
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId;

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, DLGPROC(About));
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // устанавливаем шрифт и размеры
        static LOGFONTW lf;
        HFONT hFont1;
        lf.lfHeight = 30;
        lstrcpy(LPTSTR(&lf.lfFaceName), L"Microsoft Sans Serif");
        hFont1 = CreateFontIndirect(&lf);
        SelectObject(hdc, hFont1);

        vector<string> res = get_str_result(); // результаты вычислений

        TextOutA(hdc, 30, 20, res[0].c_str(), res[0].length());
        TextOutA(hdc, 30, 60, res[1].c_str(), res[1].length());
        TextOutA(hdc, 10, 100, "+ : ", 1);
        TextOutA(hdc, 10, 140, "- : ", 1);
        TextOutA(hdc, 10, 180, "* : ", 1);
        TextOutA(hdc, 10, 220, "/ : ", 1);
        SetTextColor(hdc, RGB(0, 128, 0)); // зелёный
        TextOutA(hdc, 30, 100, res[2].c_str(), res[2].length()); // сумма
        TextOutA(hdc, 30, 140, res[3].c_str(), res[3].length()); // вычитание
        TextOutA(hdc, 30, 180, res[4].c_str(), res[4].length()); // умножение
        TextOutA(hdc, 30, 220, res[5].c_str(), res[5].length()); // деление

        EndPaint(hWnd, &ps);
      }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return FALSE;
}

// Обработчик сообщений для окна "О программе".
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
