#include "framework.h"
#include "WindowsProject7.h"
#include <set>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND hEdit;  
HWND hButton;  
HWND hOutput;
HWND hEdit2;
HFONT hFontOutput;


bool isValidInput(const wstring& input) {
    if (input.length() % 4 != 0) {
        MessageBoxW(NULL, L"Строка должна быть кратна 4 символам", L"Ошибка", MB_OK | MB_ICONERROR);
        return false;
    }

    for (size_t i = 0; i < input.length(); ++i) {
        wchar_t ch = input[i];
        size_t pos = i % 4;

        if (pos == 0 && !iswdigit(ch)) {
            wstring msg = L"Ошибка: ожидается цифра на позиции " + to_wstring(i);
            MessageBoxW(NULL, msg.c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
            return false;
        }
        else if (pos == 1 && (!iswdigit(ch) || ((ch - L'0') % 2 != 0))) {
            wstring msg = L"Ошибка: ожидается чётная цифра на позиции " + to_wstring(i);
            MessageBoxW(NULL, msg.c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
            return false;
        }
        else if (pos == 2 && (!iswdigit(ch) || ((ch - L'0') % 2 == 0))) {
            wstring msg = L"Ошибка: ожидается нечётная цифра на позиции " + to_wstring(i);
            MessageBoxW(NULL, msg.c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
            return false;
        }
        else if (pos == 3 && !iswalpha(ch)) {
            wstring msg = L"Ошибка: ожидается буква на позиции " + to_wstring(i);
            MessageBoxW(NULL, msg.c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
            return false;
        }
    }

    return true;
}

set<string> inputSet(const wstring& input) {
    set<string> mySet;

    if (!isValidInput(input)) {
        MessageBox(NULL, L"Недопустимый формат ввода!", L"Error", MB_OK | MB_ICONERROR);
        return mySet;
    }
    for (size_t i = 0; i < input.length(); i += 4) {
        string chunk(input.begin() + i, input.begin() + i + 4);  
        mySet.insert(chunk);
    }
    return mySet;
}


set<string> unionSets(const set<string>& setA, const set<string>& setB) {
    set<string> result = setA;
    result.insert(setB.begin(), setB.end());
    return result;
}

set<string> intersectionSets(const set<string>& setA, const set<string>& setB) {
    set<string> result;
    for (const auto& element : setA) {
        if (setB.find(element) != setB.end()) {
            result.insert(element);
        }
    }
    return result;
}

set<string> differenceA_B(const set<string>& setA, const set<string>& setB) {
    set<string> result;
    for (const auto& element : setA) {
        if (setB.find(element) == setB.end()) {
            result.insert(element);
        }
    }
    return result;
}

set<string> differenceB_A(const set<string>& setA, const set<string>& setB) {
    set<string> result;
    for (const auto& element : setB) {
        if (setA.find(element) == setA.end()) {
            result.insert(element);
        }
    }
    return result;
}

set<string> symmetricDifference(const set<string>& setA, const set<string>& setB) {
    set<string> result;
    for (const auto& element : setA) {
        if (setB.find(element) == setB.end()) {
            result.insert(element);
        }
    }
    for (const auto& element : setB) {
        if (setA.find(element) == setA.end()) {
            result.insert(element);
        }
    }
    return result;
}

void printSet(HWND hWnd, const set<string>& mySet) {
    wstring output = L"{ ";
    for (const auto& elem : mySet) {
        output += wstring(elem.begin(), elem.end()) + L" ";
    }
    output += L"}";

    SetWindowTextW(hOutput, output.c_str());
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT7));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT7));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT7);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    hFontOutput = CreateFontW(
        26,                
        0,                 
        0,                 
        0,                 
        FW_NORMAL,         
        FALSE,             
        FALSE,             
        FALSE,             
        DEFAULT_CHARSET,   
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"        
    );


    hEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
        50, 10, 700, 30, hWnd, NULL, hInstance, NULL);

    hEdit2 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
        50, 50, 700, 30, hWnd, NULL, hInstance, NULL);

    hButton = CreateWindowW(L"BUTTON", L"Вычислить", WS_CHILD | WS_VISIBLE,
        50, 90, 100, 30, hWnd, (HMENU)1, hInstance, NULL);


    hOutput = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        50, 130, 1500, 400, hWnd, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SendMessage(hOutput, WM_SETFONT, (WPARAM)hFontOutput, TRUE);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int wmId = LOWORD(wParam);
        if (wmId == 1) {  
            WCHAR text1[256], text2[256];
            GetWindowTextW(hEdit, text1, 256);
            GetWindowTextW(hEdit2, text2, 256);

            set<string> setA = inputSet(text1);
            set<string> setB = inputSet(text2);

            if (setA.empty() || setB.empty()) {
                SetWindowTextW(hOutput, L"Одно из множеств не введено или формат неверен!");
                return 0;
            }

            set<string> unionResult = unionSets(setA, setB);
            set<string> intersectionResult = intersectionSets(setA, setB);
            set<string> diffA_B = differenceA_B(setA, setB);
            set<string> diffB_A = differenceB_A(setA, setB);
            set<string> symmetricResult = symmetricDifference(setA, setB);

            wstring result;

            auto appendSet = [&](const wstring& label, const set<string>& s) {
                result += label + L": { ";
                bool first = true;
                for (const auto& elem : s) {
                    if (!first) result += L", ";
                    result += wstring(elem.begin(), elem.end());
                    first = false;
                }
                result += L" }\r\n";
            };

            appendSet(L"A ∪ B (объединение)", unionResult);
            appendSet(L"A ∩ B (пересечение)", intersectionResult);
            appendSet(L"A \\ B (дополнение)", diffA_B);
            appendSet(L"B \\ A (дополнение)", diffB_A);
            appendSet(L"A △ B (симметрическая разность)", symmetricResult);

            SetWindowTextW(hOutput, result.c_str());
        }
        break;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}