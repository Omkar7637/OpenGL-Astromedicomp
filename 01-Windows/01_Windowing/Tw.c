#include <windows.h>
#include <tchar.h>
#include <time.h>

// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBouncingBall(HDC hdc, int x, int y);
void DrawAnimatedText(HDC hdc, int x, int y, TCHAR* text);
void DrawGradientBackground(HDC hdc, RECT rect);

// Global Variables
TCHAR strText[255] = TEXT("Welcome to Advanced Win32 Animation!");
int ballX = 50, ballY = 50, ballDirectionX = 1, ballDirectionY = 1; // Bouncing ball position and direction
int textX = 50;  // Starting position of the animated text
int textSpeed = 5;  // Speed of the animated text movement

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // Variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("AdvancedWin32");

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

    // Register the window class
    RegisterClassEx(&wndclass);

    // Create the window
    hwnd = CreateWindow(szAppName, TEXT("Advanced Animation Example"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

    // Show and update the window
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    // Message Loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ((int)msg.wParam);
}

// Window Procedure: Handles messages sent to the window
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (iMsg)
    {
    case WM_PAINT:
        // Paint the window content
        hdc = BeginPaint(hwnd, &ps);

        // Get the client rectangle
        GetClientRect(hwnd, &rect);

        // Draw a gradient background
        DrawGradientBackground(hdc, rect);

        // Draw animated text
        DrawAnimatedText(hdc, textX, 50, strText);

        // Draw the bouncing ball
        DrawBouncingBall(hdc, ballX, ballY);

        EndPaint(hwnd, &ps);
        break;

    case WM_TIMER:
        // Handle the timer event to animate
        // Move the text
        textX += textSpeed;
        if (textX > 600 || textX < 0) {
            textSpeed = -textSpeed; // Reverse direction
        }

        // Move the bouncing ball
        ballX += ballDirectionX * 5;
        ballY += ballDirectionY * 5;

        // Check for ball collision with window boundaries
        if (ballX <= 0 || ballX >= 600) ballDirectionX = -ballDirectionX;
        if (ballY <= 0 || ballY >= 400) ballDirectionY = -ballDirectionY;

        // Force the window to repaint and display the updated content
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_CREATE:
        // Start a timer to trigger animation every 30 ms
        SetTimer(hwnd, 1, 30, NULL);
        break;

    case WM_DESTROY:
        // Handle window close event
        KillTimer(hwnd, 1);  // Stop the timer
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }

    return 0;
}

// Function to draw the bouncing ball
void DrawBouncingBall(HDC hdc, int x, int y)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red ball
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - 20, y - 20, x + 20, y + 20); // Draw ball at current position
    DeleteObject(hBrush);
}

// Function to draw animated text
void DrawAnimatedText(HDC hdc, int x, int y, TCHAR* text)
{
    SetTextColor(hdc, RGB(0, 255, 0)); // Green text
    SetBkMode(hdc, TRANSPARENT); // Transparent background
    TextOut(hdc, x, y, text, _tcslen(text)); // Draw animated text
}

// Function to draw a gradient background
void DrawGradientBackground(HDC hdc, RECT rect)
{
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Loop to create a vertical gradient effect
    for (int i = 0; i < height; ++i)
    {
        // Calculate the color blending based on the vertical position
        int red = (255 * (height - i)) / height;    // Red decreases as we go down
        int green = (255 * i) / height;             // Green increases as we go down
        int blue = 0;                               // Blue remains constant

        // Create a brush with the calculated color
        HBRUSH hBrush = CreateSolidBrush(RGB(red, green, blue));

        // Draw a line with the color
        RECT rowRect = {rect.left, rect.top + i, rect.right, rect.top + i + 1};
        FillRect(hdc, &rowRect, hBrush);

        // Clean up the brush
        DeleteObject(hBrush);
    }
}
