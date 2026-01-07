// Win32 headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#pragma region OpenGL Traingle
// OpenGL related header Files 
#include<gl/GL.h>

// Custom header file
#include"OGL.h"

// OpenGL related Libraries
#pragma comment(lib, "opengl32.lib")

// Micros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Function decalrations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Gloabl variable declartions

// Variables related with fullscreen
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

// Variables related to File I/O
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

// Active window related variable
BOOL gbActiveWindow = FALSE;

// Exit key pressed related
BOOL gbEscKeyIsPressed = FALSE;

// OpenGL related global variables
HDC ghdc = NULL; // handle to device context
HGLRC ghrc = NULL; 

// Function Declaration (REMOVE default values)
void screenToGLVertex(float screenX, float screenY, float screenZ, 
	float* glX, float* glY, float* glZ,  
	float screenWidth, float screenHeight, float depthRange);

// Function Definition
void screenToGLVertex(float screenX, float screenY, float screenZ, 
	float* glX, float* glY, float* glZ,  
	float screenWidth, float screenHeight, float depthRange) {
*glX = (2.0f * screenX) / screenWidth - 1.0f;  
*glY = 1.0f - (2.0f * screenY) / screenHeight; 
*glZ = (2.0f * screenZ) / depthRange - 1.0f;
}

void hexToRGB(const char* hex, float* r, float* g, float* b) {
    int ir, ig, ib;
    sscanf(hex, "#%02x%02x%02x", &ir, &ig, &ib);
    *r = ir / 255.0f;
    *g = ig / 255.0f;
    *b = ib / 255.0f;
}

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Local Function Decalartions
	int initialize(int);
	void display(void);
	void update(void);
	void uninitialized(void);

	// Variable declarations
	WNDCLASSEX wndclass; // WNDCLASS is a struct
	HWND hwnd;
	MSG msg;// MSG is a struct
	TCHAR szAppName[] = TEXT("RTR6_WIN");
	BOOL bDone = FALSE;

	// Code
	// Create Log File // fopen_s // fprint_s s for secured
	gpFile = fopen(gszLogFileName, "w");  // r :- read, w :- write, a :- append r+ a+ 
	if(gpFile == NULL)
	{
		MessageBox(NULL, TEXT("LogFile creation Failed"), TEXT("File I/O ERROR"), MB_OK);// NULL
		exit(0);
	}
	else
	{
		fprintf(gpFile, "program started sucessfully\n");
	}


	// Window Class Initilization 
	wndclass.cbSize = sizeof(WNDCLASSEX); 
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ; 
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registration Of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd =	CreateWindowEx(WS_EX_APPWINDOW,
			szAppName,
		 	TEXT("Omkar Ankush Kashid"),
	  		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        	(GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2,  // Center X
        	(GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2, // Center Y
		 	WIN_WIDTH,
		  	WIN_HEIGHT,
		   	NULL,
		    NULL,
			hInstance,
			NULL);
		
	ghwnd = hwnd;

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Paint Background Of The Window
	UpdateWindow(hwnd);

	// Initilized
	int result = initialized();
	if(result != 0)
	{
		fprintf(gpFile, "initilized() failed\n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "initilized() Complited Sucessfully");
	}

	// Set this window as foreground and active window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// GameLoop
	while(bDone == FALSE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(gbActiveWindow == TRUE)
			{
				if(gbEscKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}

				// Render
				display();

				// Update
				update();
				
			}
		}
	}

	// Uninitilized
	uninitialized();

	return((int)msg.wParam);
}

// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void togglefullscreen(void);
	void resize(int, int);
	void uninitialized(void);

	// Code
	switch (iMsg)
	{
		
		case WM_CREATE:
			ZeroMemory((void *)&wpPrev, sizeof(WINDOWPLACEMENT));
			wpPrev.length = sizeof(WINDOWPLACEMENT);
			break;

		case WM_SETFOCUS:
			gbActiveWindow = TRUE;
			break;

		case WM_KILLFOCUS:
			gbActiveWindow = FALSE;
			break;

		case WM_ERASEBKGND:
			return(0);

		case WM_SIZE:
			resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					gbEscKeyIsPressed = TRUE;
					break;
				default:
					break;
			}
			break;
		

		case WM_CHAR:
			switch(wParam)
			{
				case 'F':
				case 'f':
					if(gbFullScreen == FALSE)
					{
						togglefullscreen();
						gbFullScreen = TRUE;
					}
					else
					{
						togglefullscreen();
						gbFullScreen = FALSE;
					}
					break;
				default:
					break;
			
			}
			break;

		case WM_DESTROY :
			PostQuitMessage(0);
			break;

		
		case WM_CLOSE:
			uninitialized();
			break;
			
		default:
			break;
			
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void togglefullscreen(void)
{
	// variable declarations
	MONITORINFO mi;
	

	// Code
	if(gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			ZeroMemory((void *)&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);
			if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right-mi.rcMonitor.left, mi.rcMonitor.bottom-mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		
		ShowCursor(TRUE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

}

int initialized(void)
{
	// function declarations
	void printGLInfo(void);

	// Variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	// code

	//Pixel format discripter initilization
	ZeroMemory((void *)&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	
	// getdc
	ghdc = GetDC(ghwnd);
	if(ghdc == NULL)
	{
		fprintf(gpFile, "GetDC Function failed\n");
		return(-1);
	}

	// get matching pixel format index using hdc and pfd
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if(iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "choose pixel format failed\n");
		return(-2);
	}
	
	// Select The pixel format of found index
	if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "setPixel format failed");
		return(-3);
	}

	// Create rendering context using hdc pfd and pixel format index and choosen pixel format index
	ghrc = wglCreateContext(ghdc);
	if(ghrc == NULL)
	{
		fprintf(gpFile, "wgl context failed\n");
		return(-4);
	}

	// make this rendering context as current context
	if(wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglmake failed\n");
		return(-5);
	}

	// Print GL info
	printGLInfo();

	// from here onword OpenGL codes starts
	// tale the opengl to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	return(0);
}

void printGLInfo(void) {
	// code
	// print openGL Information
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "---------x--------\n");
	fprintf(gpFile, "openGL vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n",glGetString(GL_RENDERER));
	fprintf(gpFile, "openGL version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "openGL EXTENSIONS : %s\n", glGetString(GL_EXTENSIONS));
	fprintf(gpFile, "---------x--------\n");
	

}
void resize(int width, int height)
{
	// code
	// if height by accedent become 0 or less that 0 then make height 1
	if(height <= 0)
	{
		height = 1;
	}
	// set the view port
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{
	// code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT); //Clears the color buffer to prepare for new drawing.
	glLoadIdentity();
    
// 
	float glX1, glY1, glZ1;
	float glX2, glY2, glZ2;
	float glX3, glY3, glZ3;
	float glX4, glY4, glZ4;
	float glX5, glY5, glZ5;

	// ff7f2aff :- orange
	// deaa87ff :- Skin dark
	// ffccaaff :- Skin light 

	float r, g, b;
	hexToRGB("#ff7f2aff", &r, &g, &b); // Example: Orange Color

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(60, 540, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(60, 550, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(450, 550, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(450, 540, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();

	hexToRGB("#deaa87ff", &r, &g, &b); // Example: Orange Color

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(65, 525, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(60, 540, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(450, 540, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(448, 525, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();

	hexToRGB("#e9c6afff", &r, &g, &b); // Example: Orange Color

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(65, 505, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(65, 525, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(448, 525, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(450, 505, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();


	hexToRGB("#d38d5fff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(65, 500, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(65, 505, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(448, 505, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(448, 500, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();

	hexToRGB("#e1a171ff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(65, 495, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(65, 500, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(448, 500, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(448, 495, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();



	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(35, 540, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(35, 545, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(60, 550, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(60, 540, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(20, 531, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(20, 531, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(35, 545, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(35, 540, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();	
	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(450, 540, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(450, 550, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(470, 562, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(470, 555, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();


	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(470, 555, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(470, 562, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(687, 562, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(687, 555, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(740, 525, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(740, 530, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(784, 530, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(784, 525, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();


	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(700, 534, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(700, 539, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(757, 539, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(757, 534, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	
	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(666, 542, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(666, 550, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(713, 550, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(713, 542, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	
	hexToRGB("#ff7f2aff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(116, 555, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(116, 563, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(398, 563, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(398, 555, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	
	hexToRGB("#deaa87ff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(123, 532, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(123, 555, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(390, 555, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(390, 532, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	
	hexToRGB("#ffccaaff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(125, 510, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(123, 532, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(390, 532, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(388, 510, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	
	hexToRGB("#c87137ff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(125, 495, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(121, 510, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(390, 510, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(388, 495, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	hexToRGB("#a05a2cff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(125, 487, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(125, 495, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // 1
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(388, 495, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); // 4
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(388, 487, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	

	hexToRGB("#deaa87ff", &r, &g, &b); 

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	screenToGLVertex(34, 522, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); // 4
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(35, 540, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // 3
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(60, 540, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(65, 525, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	hexToRGB("#deaa87ff", &r, &g, &b); 

	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	screenToGLVertex(20, 515, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(20, 531, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); 
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(35, 540, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(34, 522, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	hexToRGB("#e6c0a6ff", &r, &g, &b); 

	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	screenToGLVertex(20, 505, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(20, 515, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // done
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(34, 522, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(34, 505, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	

	hexToRGB("#e6c0a6ff", &r, &g, &b); 
	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	screenToGLVertex(34, 504, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(34, 522, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); 
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(65, 526, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(65, 506, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	hexToRGB("#d38d5fff", &r, &g, &b); 
	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	screenToGLVertex(19, 501, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(20, 505, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); 
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(65, 506, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(65, 500, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	
	hexToRGB("#ff9955ff", &r, &g, &b); 
	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	screenToGLVertex(19, 495, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
	glVertex3f(glX1, glY1, glZ1);
	screenToGLVertex(19, 501, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // done
	glVertex3f(glX2, glY2, glZ2);
	screenToGLVertex(65, 500, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); // done
	glVertex3f(glX3, glY3, glZ3);
	screenToGLVertex(65, 495, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
	glVertex3f(glX4, glY4, glZ4);
	glEnd();
	

	//Swap the buffers
	SwapBuffers(ghdc); // win32 function // Swaps the back buffer with the front buffer to display the rendered image (Double Buffering).

}

void update(void)
{
	// code
}

void uninitialized(void)
{
	// function declarations
	void togglefullscreen(void);

	// code
	// if user is exiting in full screen then restored back to noraml
	if(gbFullScreen == TRUE)
	{
		togglefullscreen();
		gbFullScreen = FALSE;
	}


	// make hdc as current context by relesing  rendering context as current context
	if(wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// delet the rendering context
	if(ghrc) 
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// relse the dc
	if(ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// distroy winode
	if(ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	// Close the file
	if(gpFile)
	{
		fprintf(gpFile, "Program terminated sucessfully");
		fclose(gpFile);
		gpFile = NULL;
	}
}


