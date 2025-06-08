#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    // HGLOBAL hs = LoadResource(hInstance, MAKEINTRESOURCE(IDR_HTML));
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    // wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    RegisterClass(&wc);
    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                           // Optional window styles.
        CLASS_NAME,                  // Window class
        L"Learn to Program Windows", // Window text
        WS_OVERLAPPEDWINDOW,         // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, // Parent window
        LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MYMENU)),
        // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );
    ;

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_FILE_OPEN:
            MessageBox(NULL, L"OPEN", L"tip", MB_OK);
            break;
        case ID_FILE_EXIT:
            PostQuitMessage(0);
            break;

        default:
            break;
        }
    }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        // case WM_LBUTTONDOWN:
        //     MessageBox(hwnd, L"Ok", L"tip", MB_CANCELTRYCONTINUE);
        //     return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}