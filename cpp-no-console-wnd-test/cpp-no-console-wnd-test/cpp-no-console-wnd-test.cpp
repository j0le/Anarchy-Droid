// cpp-no-console-wnd-test.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "cpp-no-console-wnd-test.h"
#include <cstdlib>
#include <bit>
#include <cstring>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



void DoConsoleStuff() {
    bool allocating_console_succeeded = AllocConsole();

    if (!allocating_console_succeeded) {
        return;
    }

    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdout_handle == INVALID_HANDLE_VALUE) {
        std::exit(1);
        return;
    }

    if (!SetConsoleCP(CP_UTF8)) {
        std::exit(1);
        return;
    }
    if (!SetConsoleOutputCP(CP_UTF8)) {
        std::exit(1);
        return;
    }

    constexpr char8_t hello_msg[] = u8"Hallöle, was geht denn hier ab?\n";


    DWORD written = 0;
    if (!WriteFile(stdout_handle, hello_msg, sizeof(hello_msg) - 1, &written, nullptr)) {
        std::exit(1);
        return;
    }

    constexpr wchar_t hello_msg_utf16[] = L"Günter sagt: Jetzt nochmal mit UTF-16 U+10437 \U00010437.\n";
    if (!WriteConsoleW(stdout_handle, hello_msg_utf16, (sizeof(hello_msg_utf16) / sizeof(wchar_t)) - 1, nullptr, nullptr)) {
        std::exit(1);
        return;
    }

    


    {
        STARTUPINFO si{ .cb = sizeof(si) };
        PROCESS_INFORMATION pi{};

        constexpr wchar_t cmd_line[] = L"cmd.exe";
        wchar_t cmd_line_mutable[sizeof(cmd_line) / sizeof(wchar_t)];
        std::memcpy(cmd_line_mutable, cmd_line, sizeof(cmd_line_mutable));

        if (!CreateProcessW(
            /*[in, optional]      LPCWSTR               lpApplicationName,   */ nullptr, //L"cmd.exe",
            /*[in, out, optional] LPWSTR                lpCommandLine,       */ cmd_line_mutable,
            /*[in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes, */ nullptr,
            /*[in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,  */ nullptr,
            /*[in]                BOOL                  bInheritHandles,     */ true,
            /*[in]                DWORD                 dwCreationFlags,     */ 0,
            /*[in, optional]      LPVOID                lpEnvironment,       */ nullptr,
            /*[in, optional]      LPCWSTR               lpCurrentDirectory,  */ nullptr,
            /*[in]                LPSTARTUPINFOW        lpStartupInfo,       */ &si,
            /*[out]               LPPROCESS_INFORMATION lpProcessInformation */ &pi
        )) {
            std::exit(1);
            return;
        }
    }

    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(3000ms);
    }

    {
        HWND console_hwnd = GetConsoleWindow();

        if (console_hwnd == nullptr) {
            std::exit(1);
            return;
        }

        ShowWindow(console_hwnd, SW_HIDE);
    }
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DoConsoleStuff();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPPNOCONSOLEWNDTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPNOCONSOLEWNDTEST));

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

    
    if (HWND console_hwnd = GetConsoleWindow()) {
        ShowWindow(console_hwnd, SW_SHOW);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
        static_cast<void>(SendNotifyMessageW(console_hwnd, WM_CLOSE, 0, 0));
    }
    

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPNOCONSOLEWNDTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPPNOCONSOLEWNDTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_FILE_TEST:
            {
                HWND console_hwnd = GetConsoleWindow();
                if (console_hwnd != nullptr) {
                    if (!ShowWindow(console_hwnd, SW_HIDE)) {
                        ShowWindow(console_hwnd, SW_SHOW);
                    }
                }
                break;
            }
                

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
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
