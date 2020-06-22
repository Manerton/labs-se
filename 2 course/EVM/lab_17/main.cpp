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
static Five op1;    // наши операнды
static Five op2;
static time_t seed = time(nullptr);
static mt19937 mt(seed);
// -- объявления функций, включенных в этот модуль кода -- //
BOOL				InitInstance(HINSTANCE, int);   // -- инициализация главного окна -- //
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);    // -- обработчик сообщений для главного окна -- //
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);  // -- обработчик сообщений для диалогового окна "О программе" -- //

void get_rand_op() // Генерируем операнды
{     
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

    op1 = {op1_str};
    op2 = {op2_str};

    if (op1<op2) swap(op1,op2);
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

// -- создаем (инициализируем) главное окно программы -- //
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // -- сохранить дескриптор экземпляра в глобальной переменной -- //

   HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN),nullptr,DLGPROC(WndProc));
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

    static LOGFONTW lf;
    HFONT hFont1;
    Five res;
    switch (message)
    {
    case WM_INITDIALOG:
        // -- делаем шрифт побольше -- //
        lf.lfHeight = 24;
        lstrcpy(LPTSTR(&lf.lfFaceName), L"Microsoft Sans Serif");
        hFont1 = CreateFontIndirect(&lf);
        // -- устанавливаем шрифт для текста -- //
        SendMessage(GetDlgItem(hWnd,IDC_OP1_TEXT), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDC_OP2_TEXT), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,ID_OP1), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,ID_OP2), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDB_ADD), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDB_SUB), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDB_MUL), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDB_DIV), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDC_RES_TEXT), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,ID_RES), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDB_FORWARD), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hWnd,IDOK), WM_SETFONT, WPARAM(hFont1), TRUE );
        get_rand_op();
        SetDlgItemTextA(hWnd,ID_OP1,op1.toString().c_str());
        SetDlgItemTextA(hWnd,ID_OP2,op2.toString().c_str());
        return TRUE;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, DLGPROC(About));
            break;
        case IDM_EXIT:
        case IDCANCEL:
        case IDOK:
            DestroyWindow(hWnd);
            break;
        case IDB_FORWARD:
            get_rand_op();
            SetDlgItemTextA(hWnd,ID_OP1,op1.toString().c_str());
            SetDlgItemTextA(hWnd,ID_OP2,op2.toString().c_str());
            SetDlgItemTextA(hWnd,ID_RES,"");
            break;
        case IDB_ADD:
            res = op1+op2;
            SetDlgItemTextA(hWnd,ID_RES,res.toString().c_str());
            break;
        case IDB_SUB:
            res = op1-op2;
            SetDlgItemTextA(hWnd,ID_RES,res.toString().c_str());
            break;
        case IDB_MUL:
            res = op1*op2;
            SetDlgItemTextA(hWnd,ID_RES,res.toString().c_str());
            break;
        case IDB_DIV:
            if (size_t(std::count(op2.begin(),op2.end(),0)) != op2.get_size())
            {
                res = op1/op2;
                SetDlgItemTextA(hWnd,ID_RES,res.toString().c_str());
            }
            else
            {
                SetDlgItemTextA(hWnd,ID_RES,"zero divide");
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
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
