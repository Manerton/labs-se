// test.cpp: определяет точку входа для приложения.

// -- системные либы -- //
#include "stdafx.h"
#include "resource.h"
#include "stdlib.h"

// -- мои классы -- //
#include "trenirovka.h"
#include "stats.h"
#define MAX_LOADSTRING 100
// Глобальные переменные:
static HINSTANCE hInst;								// -- текущий экземпляр приложения -- //
static WCHAR szTitle[MAX_LOADSTRING];				// -- текст строки заголовка -- //
static WCHAR szWindowClass[MAX_LOADSTRING];			// -- имя класса главного окна -- //
// -- дескрипторы окон HWND -- //
static HWND mainWindow; // -- дескриптор главного окна -- //
static HWND menuWindow; // -- дескриптор окна с главным меню -- //
static HWND trenirovkaWindow;   // -- дескриптор окна с тренажером -- //
static HWND statsWindow;    // -- дескриптор окна со статистикой -- //
// -- мои классы -- //
static Trenirovka trenirovka; // -- класс для тренировки -- //
static Stats stats; // -- класс, собирающий статистику -- //

// -- объявления функций, включенных в этот модуль кода -- //
ATOM				MyRegisterClass(HINSTANCE hInstance);   // -- регистрация класса главного окна -- //
BOOL				InitInstance(HINSTANCE, int);   // -- инициализация главного окна -- //
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);    // -- обработчик сообщений для главного окна -- //
LRESULT CALLBACK	MenuWndProc(HWND, UINT, WPARAM, LPARAM);    // -- обработчик сообщений для главного меню (дочернее окно в главном окне) -- //
LRESULT CALLBACK	TrenirovkaWndProc(HWND, UINT, WPARAM, LPARAM);  // -- обработчик сообщений для окна с тренажером (дочернее окно в главном окне) -- //
LRESULT CALLBACK	StatsWndProc(HWND, UINT, WPARAM, LPARAM);  // -- обработчик сообщений для окна со статистикой (дочернее окно в главном окне) -- //
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);  // -- обработчик сообщений для диалогового окна "О программе" -- //
LRESULT CALLBACK    getNameWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
   mainWindow = hWnd;   // -- сохраним дескриптор в глоб. переменной -- //
   if (!hWnd)
   {
      return FALSE;
   }

   // -- тут же создаем дочерние окна из ресурсов -- //
   menuWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MENUWINDOW),hWnd,DLGPROC(MenuWndProc));
   trenirovkaWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TRENIROVKAWINDOW),hWnd,DLGPROC(TrenirovkaWndProc));
   statsWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_STATS),hWnd,DLGPROC(StatsWndProc));

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// -- обрабатывает сообщения (сигналы, события) в главном окне -- //
//  WM_COMMAND	- обработка меню приложения -- //
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
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, DLGPROC(About));
            break;
        case IDM_GUIDE:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_GUIDE), hWnd, DLGPROC(About));
            break;
        case IDM_HINT:
            // -- если сейчас открыта тренировка и кнопка "подсказка" доступна для нажатия -- //
            if (IsWindowVisible(trenirovkaWindow) && IsWindowVisible(GetDlgItem(trenirovkaWindow,IDB_HINT)))
            {
                SendMessage(trenirovkaWindow,WM_COMMAND,WPARAM(IDB_HINT),FALSE);
            }
            break;
        case IDM_REPEAT:
            if (IsWindowVisible(trenirovkaWindow) && IsWindowVisible(GetDlgItem(trenirovkaWindow,IDB_REPEAT)))
            {
                SendMessage(trenirovkaWindow,WM_COMMAND,WPARAM(IDB_REPEAT),FALSE);
            }
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_SETFOCUS:
        SetFocus(GetNextDlgTabItem(mainWindow, HWND(wParam), FALSE));   // -- фокус на активном дочернем окне -- //
        break;
    case WM_DESTROY:
        if (!stats.checkName()) stats.statsToFile();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return FALSE;
}

inline void GetNewZadanie() // -- подготовка к выводу нового задания и последующий вывод -- //
{
    trenirovka.clear(); // -- очищаю тренировку (массивы внутри класса Trenirovka) -- //

    HWND hAnswer = GetDlgItem(trenirovkaWindow,IDC_ANSWER); // -- дескриптор поля для ввода ответа пользователя -- //
    HWND hZadanie = GetDlgItem(trenirovkaWindow,IDC_ZADANIE);   // -- дескриптор поля с текстом условия задания -- //
    HWND hHint = GetDlgItem(trenirovkaWindow,IDB_HINT); // -- дескриптор кнопки подсказки -- //

    SetWindowText(hZadanie,trenirovka.getZadanie().c_str());    // -- генерирую новое задание и вывожу его -- //
    SetFocus(hAnswer);  // -- фокус на поле ввода -- //
    ShowWindow(hHint, SW_SHOW); // -- показываю кнопку подсказки -- //
}

inline void GetPrevZadanie() // -- подготовка к выводу предыдущего задания и последующий вывод -- //
{
    trenirovka.clear(); // -- очищаю тренировку (массивы внутри класса Trenirovka) -- //

    HWND hAnswer = GetDlgItem(trenirovkaWindow,IDC_ANSWER); // -- дескриптор поля для ввода ответа пользователя -- //
    HWND hZadanie = GetDlgItem(trenirovkaWindow,IDC_ZADANIE);   // -- дескриптор поля с текстом условия задания -- //

    SetWindowText(hZadanie,trenirovka.getPrevZadanie().c_str());    // -- вывожу предыдущее задание -- //
    SetFocus(hAnswer);  // -- фокус на поле ввода -- //
}

// Обработчик сообщений для главного меню (menuWindow)
LRESULT CALLBACK MenuWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)
    // -- для шрифта -- //
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
            ShowWindow(GetDlgItem(trenirovkaWindow,IDB_REPEAT), SW_HIDE); // -- скрываю кнопку повтора -- //
            DialogBox(hInst, MAKEINTRESOURCE(IDD_GETNAME), trenirovkaWindow, DLGPROC(getNameWndProc));
            // -- начинаем тренировку -- //

            GetNewZadanie();
            break;
        case IDB_STATS:
            ShowWindow(hDlg, SW_HIDE);
            ShowWindow(statsWindow, SW_SHOW);
            // -- читаем стату из файла -- //
            std::wstring ws = stats.statsFromFile();
            if (ws == L"Файла со статистикой не существует.")
            {
                MessageBox(hDlg,ws.c_str(),L"Ошибка",MB_ICONERROR);
            }
            else
            {
                SetWindowText(GetDlgItem(statsWindow,IDC_TEXTEDIT),stats.statsFromFile().c_str());
            }
            break;
        }
        break;
    }

    return FALSE;
}

// Обработчик сообщений для тренировки (trenirovkaWindow)
LRESULT CALLBACK TrenirovkaWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)

    // -- для шрифта -- //
    static LOGFONTW lf;
    HFONT hFont1;

    int wmId;
    // -- часто используемые дискрипторы окон -- //
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
        SendMessage(hZadanie, WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(hAnswer, WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hDlg,IDC_BADRESULT), WM_SETFONT, WPARAM(hFont1), TRUE );
        SendMessage(GetDlgItem(hDlg,IDC_GOODRESULT), WM_SETFONT, WPARAM(hFont1), TRUE );
        return TRUE;
    case WM_CTLCOLORSTATIC:
        // -- красный цвет у текста, если неправильно -- //
        if (HWND(lParam) == GetDlgItem(hDlg,IDC_BADRESULT))
        {
            SetTextColor(HDC(wParam), RGB(255, 20, 0));
            SetBkMode(HDC(wParam), TRANSPARENT);
            return INT_PTR(GetSysColorBrush(COLOR_3DFACE));
        }
        // -- зеленый, если правильно -- //
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
            stats.statsToFile();
            stats = Stats(); // -- сбрасываю стату -- //
            // -- скрываю результат (правильно,неправильно) -- //
            ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_HIDE);
            // -- скрываю само окно с тренажером -- //
            ShowWindow(hDlg, SW_HIDE);
            // -- включаю окно с меню -- //
            ShowWindow(menuWindow, SW_SHOW);
            SetFocus(menuWindow);
            break;
        case IDB_HINT:
            MessageBox(hDlg,trenirovka.getHint().c_str(),L"Подсказка",MB_ICONINFORMATION);

            stats.hint(); // -- собираем статистику -- //

            ShowWindow(GetDlgItem(hDlg,IDB_HINT), SW_HIDE); // -- скрываю кнопку -- //
            ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_HIDE); // -- скрываю результат -- //
            ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_HIDE); // -- скрываю результат -- //
            break;
        case IDB_REPEAT:
            ShowWindow(GetDlgItem(hDlg,IDC_ANSWER), SW_SHOW); // -- показываю поле для ввода -- //
            ShowWindow(GetDlgItem(hDlg,IDC_ANSWER_TITLE), SW_SHOW);
            SetWindowText(GetDlgItem(hDlg,IDB_GETZADANIE),L"Ответить и получить следующее задание");

            stats.repeat(); // -- собираем статистику -- //

            ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_HIDE); // -- скрываю результат -- //
            ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_HIDE); // -- скрываю результат -- //
            ShowWindow(GetDlgItem(hDlg,IDB_REPEAT), SW_HIDE); // -- скрываю кнопку повтора -- //

            GetPrevZadanie(); // -- вывод прошлого задания -- //
            break;
        case IDB_GETZADANIE:
            const uint8_t max_length = 50;
            wchar_t otvet[max_length];
            GetWindowText(hAnswer,otvet,max_length); // -- получаю ответ -- //
            SetWindowText(hAnswer,L""); // -- очищаю поле ввода -- //

            ShowWindow(GetDlgItem(hDlg,IDC_ANSWER), SW_SHOW); // -- показываю поле для ввода -- //
            ShowWindow(GetDlgItem(hDlg,IDC_ANSWER_TITLE), SW_SHOW);
            SetWindowText(GetDlgItem(hDlg,IDB_GETZADANIE),L"Ответить и получить следующее задание");

            stats.complete(); // -- собираем статистику -- //
            trenirovka.savePrevZadanie();
            if (trenirovka.checkAnswer(otvet))  // -- сообщаю о правильности ответа -- //
            {
                ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_HIDE);
                ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_SHOW);
                ShowWindow(GetDlgItem(hDlg,IDB_REPEAT), SW_HIDE); // -- скрываю кнопку повтора -- //
                stats.right(); // -- собираем статистику -- //
                GetNewZadanie(); // -- след. задание -- //
            }
            else
            {
                ShowWindow(GetDlgItem(hDlg,IDC_BADRESULT), SW_SHOW);
                ShowWindow(GetDlgItem(hDlg,IDC_GOODRESULT), SW_HIDE);

                if (stats.isRepeatActivated())  // -- если была нажата кнопка повтор, т.е если была совершена еще одна ошибка уже в режиме повтора -- //
                {
                    ShowWindow(GetDlgItem(hDlg,IDC_ANSWER), SW_HIDE);   // -- скрываем поле ввода -- //
                    ShowWindow(GetDlgItem(hDlg,IDC_ANSWER_TITLE), SW_HIDE);
                    // -- так как поле ввода скрыто, ответа тут нет, только получение след. задания -- //
                    // -- пользователь может или получить след. задание или повторить текущее неправильно решенное -- //
                    SetWindowText(GetDlgItem(hDlg,IDB_GETZADANIE),L"Получить следующее задание");
                }
                else GetNewZadanie();

                ShowWindow(GetDlgItem(hDlg,IDB_REPEAT), SW_SHOW); // -- показываю кнопку повтора -- //

                stats.wrong(); // -- собираем статистику -- //
            }
            stats.disableRepeatFlag(); // -- выходим из режима повтора -- //
            break;
        }
        break;
    }

    return FALSE;
}

// Обработчик сообщений для статистики (statsWindow)
LRESULT CALLBACK StatsWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)

    // -- для шрифта -- //
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
        case IDB_TOMENU:
            // -- скрываю окно со статистикой -- //
            ShowWindow(hDlg, SW_HIDE);
            // -- включаю окно с меню -- //
            ShowWindow(menuWindow, SW_SHOW);
            SetFocus(menuWindow);
            break;
        }
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
// -- Обработчик сообщений для окна getNameWnd (окно перед тренажером, которое получает имя пользователя) -- //
LRESULT CALLBACK getNameWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam)
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            wchar_t str[100];
            GetWindowText(GetDlgItem(hDlg,IDC_TEXTEDIT),str,100);
            stats.changeName(str);
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}
