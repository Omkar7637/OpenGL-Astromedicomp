#include <windows.h>
#include <gl/gl.h>
#include <stdbool.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HGLRC hRC; // Rendering context
HDC hDC;   // Device context

void EnableOpenGL(HWND hwnd) {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        24, 8, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };
    
    hDC = GetDC(hwnd);
    int format = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, format, &pfd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
}

void DisableOpenGL(HWND hwnd) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

void RenderScene() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.5f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.5f, -0.5f);
    glEnd();
    
    SwapBuffers(hDC);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_OWNDC, WndProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW),
                            (HBRUSH)(COLOR_WINDOW+1), NULL, TEXT("OpenGLWin"), NULL };
    RegisterClassEx(&wndclass);
    
    HWND hwnd = CreateWindow(TEXT("OpenGLWin"), TEXT("OpenGL in Win32"),
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                             NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    
    EnableOpenGL(hwnd);
    
    MSG msg;
    bool running = true;
    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Render scene in a loop
        RenderScene();
    }

    
    DisableOpenGL(hwnd);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch (iMsg) 
    {
        case WM_PAINT:
            RenderScene();
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
    return 0;
}
