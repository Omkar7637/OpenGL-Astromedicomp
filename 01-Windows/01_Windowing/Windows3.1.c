#include <windows.h>

// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // Variable declarations
    WNDCLASSEX wndclass;  // WNDCLASS is a struct
    HWND hwnd;
    MSG msg;  // MSG is a struct
    TCHAR szAppName[] = TEXT("RTR6_WIN");

    // Window Class Initialization
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Registration Of Window Class
    if (!RegisterClassEx(&wndclass)) {
        MessageBox(NULL, TEXT("Window class registration failed!"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Create Window
    hwnd = CreateWindow(szAppName,
                        TEXT("Omkar Ankush Kashid"),
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,  // Adjusted style for compatibility
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    if (!hwnd) {
        MessageBox(NULL, TEXT("Window creation failed!"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 0;
    }

    // Show Window
    ShowWindow(hwnd, iCmdShow);

    // Paint Background of the Window
    UpdateWindow(hwnd);

    // Message Loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
    case WM_CREATE:
        // Add a simple debug output here (will show in DebugView if you're using it)
        OutputDebugString(TEXT("Window Created\n"));
        break;

    case WM_DESTROY:
        // Introduce a small delay before exiting (5 seconds)
        Sleep(5000);  // 5000ms = 5 seconds
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    // Default window procedure to handle other messages
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
