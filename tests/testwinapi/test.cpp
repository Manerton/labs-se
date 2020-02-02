// test.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "test.h"
#include "trenirovka.h"
#include "stdlib.h"

#define MAX_LOADSTRING 100
// Глобальные переменные:
static HINSTANCE hInst;								// -- текущий экземпляр -- //
static WCHAR szTitle[MAX_LOADSTRING];				// -- текст строки заголовка -- //
static WCHAR szWindowClass[MAX_LOADSTRING];			// -- имя класса главного окна -- //
static HWND mainWindow;
static HWND menuWindow;
static HWND trenirovkaWindow;
static Trenirovka trenirovka; // -- класс для тренировки -- //

// -- объявления функций, включенных в этот модуль кода -- //
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MenuWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	TrenirovkaWndProc(HWND, UINT, WPARAM, LPARAM);
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
        if (!TranslateAccelerator(mainWindow, hAccelTable, &msg))
        {
            if (!IsDialogMessage(menuWindow, &msg) && !IsDialogMessage(trenirovkaWindow, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
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
                            ); // -- создаем окно -- //
   menuWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MENUWINDOW),hWnd,DLGPROC(MenuWndProc));
   trenirovkaWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TRENIROVKAWINDOW),hWnd,DLGPROC(TrenirovkaWndProc));

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   mainWindow = hWnd;

   return TRUE;
}

// -- обрабатывает сообщения (сигналы, события) в главном окне -- //
//  WM_COMMAND	- обработка меню приложения -- //
//  WM_PAINT	- закрасить главное окно -- //
//  WM_DESTROY	- ввести сообщение о выходе и вернуться -- //
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

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
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_SETFOCUS:
        SetFocus(GetNextDlgTabItem(mainWindow, HWND(wParam), FALSE));
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



// Обработчик сообщений для главного меню (menuWindow)
INT_PTR CALLBACK MenuWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)
    static LOGFONTW lf;
    HFONT hFont1;
    int wmId;
    switch (message)
    {
    case WM_INITDIALOG:
        // -- делаем шрифт побольше -- //
        lf.lfHeight = 26;
        lstrcpy(LPTSTR(&lf.lfFaceName), L"Microsoft Sans Serif");
        hFont1 = CreateFontIndirect(&lf);
        // -- устанавливаем шрифт для текста -- //
        SendMessage(GetDlgItem(hDlg,IDC_TITLE), WM_SETFONT, WPARAM(hFont1), TRUE );
        return TRUE;

    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDB_TRENIROVKA:
            ShowWindow(hDlg, SW_HIDE);
            ShowWindow(trenirovkaWindow, SW_SHOW);
            SetFocus(trenirovkaWindow);
            SetFocus(GetDlgItem(trenirovkaWindow,IDC_ANSWER));
            break;
        }
        break;
    }

    return FALSE;
}

// Обработчик сообщений для тренировки (trenirovkaWindow)
INT_PTR CALLBACK TrenirovkaWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)
    static LOGFONTW lf;
    HFONT hFont1;
    int wmId;
    HWND hAnswer = GetDlgItem(hDlg,IDC_ANSWER); // -- дескриптор поля для ввода ответа пользователя -- //
    HWND hZadanie = GetDlgItem(hDlg,IDC_ZADANIE);   // -- дескриптор поля с текстом условия задания -- //
    switch (message)
    {
    case WM_INITDIALOG:
        // -- делаем шрифт побольше -- //
        lf.lfHeight = 26;
        lstrcpy(LPTSTR(&lf.lfFaceName), L"Microsoft Sans Serif");
        hFont1 = CreateFontIndirect(&lf);
        // -- устанавливаем шрифт для текста -- //
        SendMessage(GetDlgItem(hDlg,IDC_TITLE), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hDlg,IDC_ZADANIE), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hDlg,IDC_ANSWER), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hDlg,IDC_BADRESULT), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hDlg,IDC_GOODRESULT), WM_SETFONT, WPARAM(hFont1), TRUE );
        SetWindowText(hZadanie,trenirovka.getZadanie().c_str());    // -- при старте окна уже генерируется задание -- //
        return TRUE;
    case WM_CTLCOLORSTATIC:
        if (HWND(lParam) == GetDlgItem(hDlg,IDC_BADRESULT))
        {
            SetTextColor(HDC(wParam), RGB(255, 20, 0));
            SetBkMode(HDC(wParam), TRANSPARENT);
            return INT_PTR(GetSysColorBrush(COLOR_3DFACE));
        }
        if (HWND(lParam) == GetDlgItem(hDlg,IDC_GOODRESULT))
        {
            SetTextColor(HDC(wParam), RGB(0, 155, 0));
            SetBkMode(HDC(wParam), TRANSPARENT);
            return INT_PTR(GetSysColorBrush(COLOR_3DFACE));
        }
        break;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDB_TOMENU:
            //trenirovka.clear();
            ShowWindow(hDlg, SW_HIDE);
            ShowWindow(menuWindow, SW_SHOW);
            SetFocus(menuWindow);
            break;
        case IDB_HINT:
            MessageBox(hDlg,trenirovka.getHint().c_str(),L"Подсказка",MB_ICONINFORMATION);
            ShowWindow(GetDlgItem(hDlg,IDB_HINT), SW_HIDE); // -- скрываю кнопку -- //
            break;
        case IDB_GETZADANIE:
            wchar_t otvet[100];
            GetWindowText(hAnswer,otvet,100); // -- получаю ответ -- //
            SetWindowText(hAnswer,L""); // -- очищаю поле ввода -- //
            if (trenirovka.checkAnswer(otvet))  // -- сообщаю о правильности ответа -- //
            {
                ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_HIDE);
                ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_SHOW);
            }
            else
            {
                ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_SHOW);
                ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_HIDE);
            }
            trenirovka.clear(); // -- очищаю тренировку -- //
            SetWindowText(hZadanie,trenirovka.getZadanie().c_str());    // -- генерирую новое задание -- //
            SetFocus(GetDlgItem(hDlg,IDC_ANSWER));  // -- фокус на поле ввода -- //
            ShowWindow(GetDlgItem(hDlg,IDB_HINT), SW_SHOW); // -- показываю кнопку подсказки -- //
            break;
        }
        break;
    }

    return FALSE;
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
