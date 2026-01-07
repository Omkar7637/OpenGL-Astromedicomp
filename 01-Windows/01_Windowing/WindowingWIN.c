// Win32 headers
#include <windows.h>

// Global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Constants for menu item identifiers
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_EXIT 4
#define IDM_EDIT_UNDO 5
#define IDM_EDIT_REDO 6
#define IDM_EDIT_CUT 7
#define IDM_EDIT_COPY 8
#define IDM_EDIT_PASTE 9
#define IDM_VIEW_ZOOMIN 10
#define IDM_VIEW_ZOOMOUT 11
#define IDM_VIEW_FULLSCREEN 12
#define IDM_HELP_ABOUT 13

// Function prototypes
HMENU CreateMainMenu();
HMENU CreateContextMenu();

// Global variables
HMENU hContextMenu; // Handle for the context menu

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // Variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("RTR_WIN");

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
    wndclass.lpszMenuName = NULL; // Menu will be set programmatically
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Registering the window class
    RegisterClassEx(&wndclass);

    // Creating the window
    hwnd = CreateWindow(szAppName,
                        TEXT("Omkar Ankush Kashid - Advanced Menu"),
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    // Displaying the window
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ((int)msg.wParam);
}

// Callback function for window events
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_CREATE:
    {
        // Create the menu and attach it to the window
        HMENU hMenu = CreateMainMenu();
        SetMenu(hwnd, hMenu);

        // Create the context menu
        hContextMenu = CreateContextMenu();
        break;
    }

    case WM_RBUTTONDOWN:
    {
        // Show context menu on right-click
        POINT pt;
        GetCursorPos(&pt);
        TrackPopupMenu(hContextMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
        break;
    }

    case WM_COMMAND:
    {
        // Handle menu commands
        switch (LOWORD(wParam))
        {
        case IDM_FILE_NEW:
            MessageBox(hwnd, TEXT("New File selected"), TEXT("File Menu"), MB_OK);
            break;
        case IDM_FILE_OPEN:
            MessageBox(hwnd, TEXT("Open File selected"), TEXT("File Menu"), MB_OK);
            break;
        case IDM_FILE_SAVE:
            MessageBox(hwnd, TEXT("Save File selected"), TEXT("File Menu"), MB_OK);
            break;
        case IDM_FILE_EXIT:
            DestroyWindow(hwnd);
            break;
        case IDM_EDIT_UNDO:
            MessageBox(hwnd, TEXT("Undo selected"), TEXT("Edit Menu"), MB_OK);
            break;
        case IDM_EDIT_REDO:
            MessageBox(hwnd, TEXT("Redo selected"), TEXT("Edit Menu"), MB_OK);
            break;
        case IDM_EDIT_CUT:
            MessageBox(hwnd, TEXT("Cut selected"), TEXT("Edit Menu"), MB_OK);
            break;
        case IDM_EDIT_COPY:
            MessageBox(hwnd, TEXT("Copy selected"), TEXT("Edit Menu"), MB_OK);
            break;
        case IDM_EDIT_PASTE:
            MessageBox(hwnd, TEXT("Paste selected"), TEXT("Edit Menu"), MB_OK);
            break;
        case IDM_VIEW_ZOOMIN:
            MessageBox(hwnd, TEXT("Zoom In selected"), TEXT("View Menu"), MB_OK);
            break;
        case IDM_VIEW_ZOOMOUT:
            MessageBox(hwnd, TEXT("Zoom Out selected"), TEXT("View Menu"), MB_OK);
            break;
        case IDM_VIEW_FULLSCREEN:
            MessageBox(hwnd, TEXT("Full Screen selected"), TEXT("View Menu"), MB_OK);
            break;
        case IDM_HELP_ABOUT:
            MessageBox(hwnd, TEXT("About selected"), TEXT("Help Menu"), MB_OK | MB_ICONINFORMATION);
            break;
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }

    return 0;
}

// Function to create the main menu
HMENU CreateMainMenu()
{
    HMENU hMenu = CreateMenu(); // Main menu handle

    // File menu
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, TEXT("New\tCtrl+N"));
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, TEXT("Open\tCtrl+O"));
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, TEXT("Save\tCtrl+S"));
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, TEXT("Exit\tAlt+F4"));

    // Edit menu
    HMENU hEditMenu = CreateMenu();
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_UNDO, TEXT("Undo\tCtrl+Z"));
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_REDO, TEXT("Redo\tCtrl+Y"));
    AppendMenu(hEditMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_CUT, TEXT("Cut\tCtrl+X"));
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_COPY, TEXT("Copy\tCtrl+C"));
    AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_PASTE, TEXT("Paste\tCtrl+V"));

    // View menu
    HMENU hViewMenu = CreateMenu();
    AppendMenu(hViewMenu, MF_STRING, IDM_VIEW_ZOOMIN, TEXT("Zoom In\tCtrl++"));
    AppendMenu(hViewMenu, MF_STRING, IDM_VIEW_ZOOMOUT, TEXT("Zoom Out\tCtrl+-"));
    AppendMenu(hViewMenu, MF_STRING, IDM_VIEW_FULLSCREEN, TEXT("Full Screen\tF11"));

    // Help menu
    HMENU hHelpMenu = CreateMenu();
    AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, TEXT("About\tF1"));

    // Add submenus to the main menu
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("File"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, TEXT("Edit"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, TEXT("View"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, TEXT("Help"));

    return hMenu;
}

// Function to create the context menu
HMENU CreateContextMenu()
{
    HMENU hContext = CreatePopupMenu(); // Context menu handle

    AppendMenu(hContext, MF_STRING, IDM_EDIT_CUT, TEXT("Cut"));
    AppendMenu(hContext, MF_STRING, IDM_EDIT_COPY, TEXT("Copy"));
    AppendMenu(hContext, MF_STRING, IDM_EDIT_PASTE, TEXT("Paste"));
    AppendMenu(hContext, MF_SEPARATOR, 0, NULL);
    AppendMenu(hContext, MF_STRING, IDM_FILE_EXIT, TEXT("Exit"));

    return hContext;
}
