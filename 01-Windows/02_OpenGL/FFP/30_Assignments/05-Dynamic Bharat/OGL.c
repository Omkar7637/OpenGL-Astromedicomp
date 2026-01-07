// Win32 headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES 1
#include<math.h>
#pragma region OpenGL Traingle
// OpenGL related header Files 
#include<gl/GL.h>
#include<gl/GLU.h>

// Custom header file
#include"OGL.h"

// OpenGL related Libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

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
FILE* gpFile = NULL;

// Active window related variable
BOOL gbActiveWindow = FALSE;

// Exit key pressed related
BOOL gbEscKeyIsPressed = FALSE;

// OpenGL related global variables
HDC ghdc = NULL; // handle to device context
HGLRC ghrc = NULL;




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
	if (gpFile == NULL)
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
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
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
	if (result != 0)
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
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
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
			if (gbActiveWindow == TRUE)
			{
				if (gbEscKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}

				// Update
				update();

				// Render
				display();


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
		ZeroMemory((void*)&wpPrev, sizeof(WINDOWPLACEMENT));
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
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscKeyIsPressed = TRUE;
			break;
		default:
			break;
		}
		break;


	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			if (gbFullScreen == FALSE)
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

	case WM_DESTROY:
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
	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			ZeroMemory((void*)&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
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
	void resize(int, int);

	// Variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	// code

	//Pixel format discripter initilization
	ZeroMemory((void*)&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;


	// getdc
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDC Function failed\n");
		return(-1);
	}

	// get matching pixel format index using hdc and pfd
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "choose pixel format failed\n");
		return(-2);
	}

	// Select The pixel format of found index
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "setPixel format failed");
		return(-3);
	}

	// Create rendering context using hdc pfd and pixel format index and choosen pixel format index
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, "wgl context failed\n");
		return(-4);
	}

	// make this rendering context as current context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglmake failed\n");
		return(-5);
	}

	// Print GL info
	printGLInfo();
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// from here onword OpenGL codes starts
	// tel the opengl to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



	// Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void printGLInfo(void) {
	// code
	// print openGL Information
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "---------x--------\n");
	fprintf(gpFile, "openGL vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "openGL version : %s\n", glGetString(GL_VERSION));
	//fprintf(gpFile, "openGL EXTENSIONS : %s\n", glGetString(GL_EXTENSIONS));
	fprintf(gpFile, "---------x--------\n");


}
void resize(int width, int height)
{
	// code
	// if height by accedent become 0 or less that 0 then make height 1
	if (height <= 0)
	{
		height = 1;
	}
	// set the view port
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// set to identity matrix
	glLoadIdentity();

	// do prespective projection
	gluPerspective(25.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // FOV-Y , Aspect Ratio, near, far
}

// B
GLfloat moveBOnAxis = -5.0f;

// A
GLfloat moveAOnXAxis = -5.0f;
GLfloat moveAOnYAxis = -5.0f;

//A2
GLfloat moveA2OnXAxis = 6.9f;
GLfloat moveA2OnYAxis = -5.0f;

// H
GLfloat moveHOnXAxis = -5.0f;
GLfloat moveHOnYAxis = 4.7f;
// T
GLfloat moveTOnAxis = 5.0f;

// R
GLfloat moveROnXAxis = 5.0f;
GLfloat moveROnYAxis = 5.0f;

// Jet
GLfloat moveJetOnXAxis = -5.0f;
GLfloat moveJetTopX = -5.0f;
GLfloat moveJetTopY = 2.0f;
GLfloat moveJetBottomX = -5.0f;
GLfloat moveJetBottomY = -2.0f;
GLfloat moveJetTopAngle = 180.0f;


BOOL flagForB = FALSE;
BOOL flagForA = FALSE;
BOOL flagForA2 = FALSE;
BOOL flagForT = FALSE;
BOOL flagForR = FALSE;

BOOL flagForH = FALSE;
BOOL flagForHX = FALSE;
BOOL flagForHY = FALSE;

BOOL flagColorb = FALSE;
BOOL flagColora = FALSE;
BOOL flagColora2 = FALSE;
BOOL flagColorr = FALSE;
BOOL flagColort = FALSE;
BOOL flagColorh = FALSE;

GLfloat posB = -2.0f + 1.5f;
GLfloat posA = -1.6f + 1.5f;
GLfloat posA2 = 0.3f + 1.5f;
GLfloat posR = -1.4f + 1.5f;
GLfloat posT = -1.0f + 1.5f;
GLfloat posH = -1.8f + 1.5f;

GLfloat jetr;

struct Point3D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct Point3D planePosOffset[3];
BOOL start = FALSE;


void gray(GLfloat x, GLfloat y, GLfloat z, int shades)
{
	if(shades  == 1)
	{
		glColor3f(0.2f, 0.4f, 0.6f);   // Slightly lighter bluish-gray
	}
	else if(shades == 2)
	{
		glColor3f(0.3f, 0.5f, 0.7f);   // Mid-level metallic blue-gray
	}
	else if (shades == 3)
	{
		glColor3f(0.4f, 0.6f, 0.8f);   // Lighter bluish-metallic silver
	}
	else if (shades == 4)
	{
		glColor3f(0.1f, 0.2f, 0.3f);   // Very dark bluish-gray
	}
	glVertex3f((GLfloat)x,(GLfloat)y,(GLfloat)z );
}



void drawB(BOOL a)
{
	glPushMatrix();

	glTranslatef(moveBOnAxis, 0.0f, 0.0f);
	// Orange :- glColor3f(1.0f, 0.5f, 0.0f);
	// Dark Orange :- glColor3f(1.0f, 0.549f, 0.0f);

	//====================================================================
	// B
	//====================================================================
	
	//-----------------------------------
	if(a)
	{
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.25, 0.02, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-2.25, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-2.48, 0.69, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.48, 0.01, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-2.48, -0.72, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-2.25, -0.72, 0.0);

		glEnd();


		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-2, 0.68, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-2.25, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-2.25, 0.47, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-1.99, 0.41, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-1.8, 0.51, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-2, 0.68, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-1.99, 0.41, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2, 0.2, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.8, 0.14, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.8, 0.14, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2, 0.2, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.25, 0.12, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.25, -0.04, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.92, 0.02, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.8, -0.1, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.92, 0.02, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.25, -0.04, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.99, -0.15, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.8, -0.1, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.99, -0.15, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.99, -0.43, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.8, -0.5, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);


		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.8, -0.5, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.99, -0.43, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-2.25, -0.51, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-2.25, -0.72, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-2.03, -0.72, 0.0);

		glEnd();
	}
	else
	{
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.25, 0.02, 0.0);
		glVertex3f(-2.25, 0.69, 0.0);
		glVertex3f(-2.48, 0.69, 0.0);
		glVertex3f(-2.48, 0.01, 0.0);
		glVertex3f(-2.48, -0.72, 0.0);
		glVertex3f(-2.25, -0.72, 0.0);

		glEnd();


		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(-2, 0.68, 0.0);
		glVertex3f(-2.25, 0.69, 0.0);
		glVertex3f(-2.25, 0.47, 0.0);
		glVertex3f(-1.99, 0.41, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(-1.8, 0.51, 0.0);
		glVertex3f(-2, 0.68, 0.0);
		glVertex3f(-1.99, 0.41, 0.0);
		glVertex3f(-2, 0.2, 0.0);
		glVertex3f(-1.8, 0.14, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(-1.8, 0.14, 0.0);
		glVertex3f(-2, 0.2, 0.0);
		glVertex3f(-2.25, 0.12, 0.0);
		glVertex3f(-2.25, -0.04, 0.0);
		glVertex3f(-1.92, 0.02, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(-1.8, -0.1, 0.0);
		glVertex3f(-1.92, 0.02, 0.0);
		glVertex3f(-2.25, -0.04, 0.0);
		glVertex3f(-1.99, -0.15, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(-1.8, -0.1, 0.0);
		glVertex3f(-1.99, -0.15, 0.0);
		glVertex3f(-1.99, -0.43, 0.0);
		glVertex3f(-1.8, -0.5, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);


		glVertex3f(-1.8, -0.5, 0.0);
		glVertex3f(-1.99, -0.43, 0.0);
		glVertex3f(-2.25, -0.51, 0.0);
		glVertex3f(-2.25, -0.72, 0.0);
		glVertex3f(-2.03, -0.72, 0.0);

		glEnd();
	}

	glPopMatrix();

}
void drawH(BOOL a)
{
	glPushMatrix();

	glTranslatef(moveHOnXAxis, moveHOnYAxis, 0.0f);

	if(a)
	{
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.43, -0.03, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-1.43, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-1.67, 0.69, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.67, 0.04, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.67, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.43, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.17, 0.08, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.43, 0.08, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.43, -0.16, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.17, -0.16, 0.0);
	
		glEnd();
	
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.94, -0.05, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.94, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-1.17, 0.69, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.17, -0.04, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-1.17, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.94, -0.71, 0.0);
	
		glEnd();
	}
	else
	{
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.43, -0.03, 0.0);
		glVertex3f(-1.43, 0.69, 0.0);
		glVertex3f(-1.67, 0.69, 0.0);
		glVertex3f(-1.67, 0.04, 0.0);
		glVertex3f(-1.67, -0.71, 0.0);
		glVertex3f(-1.43, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-1.17, 0.08, 0.0);
		glVertex3f(-1.43, 0.08, 0.0);
		glVertex3f(-1.43, -0.16, 0.0);
		glVertex3f(-1.17, -0.16, 0.0);
	
		glEnd();
	
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.94, -0.05, 0.0);
		glVertex3f(-0.94, 0.69, 0.0);
		glVertex3f(-1.17, 0.69, 0.0);
		glVertex3f(-1.17, -0.04, 0.0);
		glVertex3f(-1.17, -0.71, 0.0);
		glVertex3f(-0.94, -0.71, 0.0);
	
		glEnd();
	}

	glPopMatrix();

}
void drawA(BOOL a)
{
	glPushMatrix();

	glTranslatef(moveAOnXAxis, moveAOnYAxis, 0.0f);

	if(a)
	{

		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.53, -0.05, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.46, 0.33, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.59, 0.69, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.75, -0.05, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.9, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.67, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.46, 0.33, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.34, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.59, 0.69, 0.0);
	
		glEnd();
	
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.17, -0.05, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.34, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.46, 0.33, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.38, -0.06, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.26, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.03, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.36, -0.15, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.56, -0.15, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.6, -0.38, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.32, -0.38, 0.0);
	
	
		glEnd();
	}
	else
	{
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.53, -0.05, 0.0);
		glVertex3f(-0.46, 0.33, 0.0);
		glVertex3f(-0.59, 0.69, 0.0);
		glVertex3f(-0.75, -0.05, 0.0);
		glVertex3f(-0.9, -0.71, 0.0);
		glVertex3f(-0.67, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.46, 0.33, 0.0);
		glVertex3f(-0.34, 0.69, 0.0);
		glVertex3f(-0.59, 0.69, 0.0);
	
		glEnd();
	
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.17, -0.05, 0.0);
		glVertex3f(-0.34, 0.69, 0.0);
		glVertex3f(-0.46, 0.33, 0.0);
		glVertex3f(-0.38, -0.06, 0.0);
		glVertex3f(-0.26, -0.71, 0.0);
		glVertex3f(-0.03, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.36, -0.15, 0.0);
		glVertex3f(-0.56, -0.15, 0.0);
		glVertex3f(-0.6, -0.38, 0.0);
		glVertex3f(-0.32, -0.38, 0.0);
	
	
		glEnd();
	}

	glPopMatrix();

}
void drawA2(BOOL a)
{
	glPushMatrix();
	glTranslatef(moveA2OnXAxis, moveA2OnYAxis, 0.0f);

	if(a)
	{

		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.53, -0.05, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.46, 0.33, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.59, 0.69, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.75, -0.05, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.9, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.67, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.46, 0.33, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.34, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.59, 0.69, 0.0);
	
		glEnd();
	
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.17, -0.05, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.34, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(-0.46, 0.33, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.38, -0.06, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.26, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.03, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.36, -0.15, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.56, -0.15, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.6, -0.38, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(-0.32, -0.38, 0.0);
	
	
		glEnd();
	}
	else
	{
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.53, -0.05, 0.0);
		glVertex3f(-0.46, 0.33, 0.0);
		glVertex3f(-0.59, 0.69, 0.0);
		glVertex3f(-0.75, -0.05, 0.0);
		glVertex3f(-0.9, -0.71, 0.0);
		glVertex3f(-0.67, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.46, 0.33, 0.0);
		glVertex3f(-0.34, 0.69, 0.0);
		glVertex3f(-0.59, 0.69, 0.0);
	
		glEnd();
	
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.17, -0.05, 0.0);
		glVertex3f(-0.34, 0.69, 0.0);
		glVertex3f(-0.46, 0.33, 0.0);
		glVertex3f(-0.38, -0.06, 0.0);
		glVertex3f(-0.26, -0.71, 0.0);
		glVertex3f(-0.03, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(-0.36, -0.15, 0.0);
		glVertex3f(-0.56, -0.15, 0.0);
		glVertex3f(-0.6, -0.38, 0.0);
		glVertex3f(-0.32, -0.38, 0.0);
	
	
		glEnd();
	}

	glPopMatrix();

}
void drawR(BOOL a)
{
	glPushMatrix();

	glTranslatef(moveROnXAxis, moveROnYAxis, 0.0f);
	if(a)
	{

		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.22, -0.09, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.22, 0.68, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.0, 0.68, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.0, -0.09, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(0.0, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(0.22, -0.71, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.4, 0.68, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.22, 0.68, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.22, 0.5, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.42, 0.43, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.62, 0.53, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.4, 0.68, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(0.42, 0.43, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.42, 0.14, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.64, 0.08, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.64, 0.08, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.42, 0.14, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.22, 0.02, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.22, -0.19, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.28, -0.18, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.46, -0.06, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.46, -0.06, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.28, -0.18, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(0.46, -0.71, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(0.67, -0.71, 0.0);
		glEnd();
	}
	else
	{
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.22, -0.09, 0.0);
		glVertex3f(0.22, 0.68, 0.0);
		glVertex3f(0.0, 0.68, 0.0);
		glVertex3f(0.0, -0.09, 0.0);
		glVertex3f(0.0, -0.71, 0.0);
		glVertex3f(0.22, -0.71, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(0.4, 0.68, 0.0);
		glVertex3f(0.22, 0.68, 0.0);
		glVertex3f(0.22, 0.5, 0.0);
		glVertex3f(0.42, 0.43, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(0.62, 0.53, 0.0);
		glVertex3f(0.4, 0.68, 0.0);
		glVertex3f(0.42, 0.43, 0.0);
		glVertex3f(0.42, 0.14, 0.0);
		glVertex3f(0.64, 0.08, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(0.64, 0.08, 0.0);
		glVertex3f(0.42, 0.14, 0.0);
		glVertex3f(0.22, 0.02, 0.0);
		glVertex3f(0.22, -0.19, 0.0);
		glVertex3f(0.28, -0.18, 0.0);
		glVertex3f(0.46, -0.06, 0.0);

		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);

		glVertex3f(0.46, -0.06, 0.0);
		glVertex3f(0.28, -0.18, 0.0);
		glVertex3f(0.46, -0.71, 0.0);
		glVertex3f(0.67, -0.71, 0.0);
		glEnd();

	}

	glPopMatrix();

}
void drawT(BOOL a)
{
	glPushMatrix();

	glTranslatef(moveTOnAxis, 0.0f, 0.0f);
	if(a)
	{

		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(2.24, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(1.2, 0.69, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(1.2, 0.43, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(2.24, 0.43, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.82, -0.1, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(1.82, 0.43, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(1.6, 0.43, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.6, -0.1, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(1.6, -0.7, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
		glVertex3f(1.82, -0.7, 0.0);
	
		glEnd();
	}
	else
	{
		//-----------------------------------
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(2.24, 0.69, 0.0);
		glVertex3f(1.2, 0.69, 0.0);
		glVertex3f(1.2, 0.43, 0.0);
		glVertex3f(2.24, 0.43, 0.0);
	
		glEnd();
		
		//-----------------------------------
		glBegin(GL_POLYGON);
	
		glVertex3f(1.82, -0.1, 0.0);
		glVertex3f(1.82, 0.43, 0.0);
		glVertex3f(1.6, 0.43, 0.0);
		glVertex3f(1.6, -0.1, 0.0);
		glVertex3f(1.6, -0.7, 0.0);
		glVertex3f(1.82, -0.7, 0.0);
	
		glEnd();
	}

	glPopMatrix();

}

void drawIAFInsignia(float cx, float cy, float radius) {
    int numSegments = 100;

    // Green Outer Circle
    glColor3f(0.0f, 1.0f, 0.0f);  // Green
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();

    // White Middle Circle
    glColor3f(1.0f, 1.0f, 1.0f);  // White
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(cx + (radius * 0.66f) * cos(angle), cy + (radius * 0.66f) * sin(angle));
    }
    glEnd();

    // Orange Inner Circle
    glColor3f(1.0f, 0.5f, 0.0f);  // Orange
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        glVertex2f(cx + (radius * 0.33f) * cos(angle), cy + (radius * 0.33f) * sin(angle));
    }
    glEnd();
}
void drawJet(int a, GLfloat middle)
{
	if( middle == 1)
	{
		glTranslatef((moveJetTopX + planePosOffset[0].x), ( moveJetTopY + planePosOffset[0].y), (0.0f + planePosOffset[0].z));
		glRotatef(moveJetTopAngle, 0.0f, 0.0f, 1.0f);
	}
	if( middle == 2)
	{
		glTranslatef((moveJetOnXAxis + planePosOffset[1].x), (0.0f + planePosOffset[1].y), (0.0f + planePosOffset[1].z));
		glRotatef(-90, 0.0f, 0.0f, 1.0f);
	}
	if( middle == 3)
	{
		glTranslatef((moveJetBottomX + planePosOffset[2].x), (moveJetBottomY + planePosOffset[2].y), (0.0f + planePosOffset[2].z));
	}
	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);

	
	//glRotatef(-90, 0.0f, 0.0f, 1.0f);
	// if( middle == 1)
	// {
	// 	glRotatef(moveJetTopAngle, 0.0f, 0.0f, 1.0f);
	// }

	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(0.1, 0.81, 0.0, 2);
	gray(0, 1.03, 0.0, 2);
	gray(-0.11, 0.81, 0.0, 2);

	glEnd();
	
	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(0.11, -0.44, 0.0, 1);
	gray(0.19, 0.35, 0.0, 1);
	gray(0.1, 0.81, 0.0, 1);
	gray(-0.11, 0.81, 0.0, 1);
	gray(-0.2, 0.34, 0.0, 1);
	gray(-0.13, -0.44, 0.0, 1);
	gray(-0.09, -0.79, 0.0, 1);
	gray(0.08, -0.79, 0.0, 1);

	glEnd();

	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(0.07, 0.26, 0.0, 3);
	gray(0.07, 0.55, 0.0, 3);
	gray(-0.01, 0.65, 0.0, 3);
	gray(-0.09, 0.55, 0.0, 3);
	gray(-0.09, 0.25, 0.0, 3);
	gray(-0.01, 0.14, 0.0, 3);

	glEnd();
	
	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(0.84, -0.33, 0.0, 4);
	gray(0.19, 0.35, 0.0, 4);
	gray(0.11, -0.4, 0.0, 4);
	gray(0.84, -0.54, 0.0, 4);


	glEnd();
	
	
	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(-0.2, 0.34, 0.0, 4);
	gray(-0.85, -0.34, 0.0, 4);
	gray(-0.85, -0.54, 0.0, 4);
	gray(-0.13, -0.44, 0.0, 4);

	glEnd();
	
	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(-0.13, -0.44, 0.0, 4);
	gray(-0.21, -0.45, 0.0, 4);
	gray(-0.44, -0.79, 0.0, 4);
	gray(-0.39, -0.93, 0.0, 4);
	gray(-0.09, -0.79, 0.0, 4);

	glEnd();
	
	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(0.44, -0.78, 0.0, 4);
	gray(0.2, -0.40, 0.0, 4);
	gray(0.11, -0.40, 0.0, 4);
	gray(0.08, -0.79, 0.0, 4);
	gray(0.37, -0.93, 0.0, 4);

	glEnd();
	
	//-----------------------------------
	glBegin(GL_POLYGON);

	gray(0.08, -0.79, 0.0, 4);
	gray(-0.09, -0.79, 0.0, 4);
	gray(-0.05, -0.88, 0.0, 4);
	gray(0.04, -0.88, 0.0, 4);

	glEnd();

	// Flame
	//-----------------------------------

	if(a == 1)
	{
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.08, -1, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.04, -0.88, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(-0.05, -0.88, 0.0);
		glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(-0.09, -1, 0.0);
			glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.01, -1.28, 0.0);
	
	
		glEnd();
	}
	if(a == 2)
	{
		glBegin(GL_POLYGON);
	
		glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08, -1, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.04, -0.88, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.05, -0.88, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.09, -1, 0.0);
			glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.01, -1.28, 0.0);
	
	
		glEnd();
	}
	if(a == 3)
	{
		glBegin(GL_POLYGON);
	
		glColor3f(0.2f, 0.8f, 0.2f);
	glVertex3f(0.08, -1, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
	glVertex3f(0.04, -0.88, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
	glVertex3f(-0.05, -0.88, 0.0);
		glColor3f(0.2f, 0.8f, 0.2f);
	glVertex3f(-0.09, -1, 0.0);
			glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.01, -1.28, 0.0);
	
	
		glEnd();
	}

	drawIAFInsignia(-0.4f, -0.22f, 0.15f);  // Adjust position/size as needed
	drawIAFInsignia(0.4f, -0.22f, 0.15f);  // Adjust position/size as needed

	glPopMatrix();

}

float thetaDeg = 180.0f; // in degrees
float thetaRad;
float radius = 3.0f;

void display(void)
{
	// code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the color buffer to prepare for new drawing.

	// set matrix to model view mode
	glMatrixMode(GL_MODELVIEW);

	// set it to idetity matrix
	glLoadIdentity();

	// Common view setup — only once
	gluLookAt(0.0f, 0.0f, 27.0f,   // Eye
		0.0f, 0.0f, 0.0f,   // Center
		0.0f, 1.0f, 0.0f);  // Up
	glPushMatrix();
	drawB(flagColorb);
	drawH(flagColorh);
	drawA(flagColora);
	drawR(flagColorr);
	drawA2(flagColora2);
	drawT(flagColort);


	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.0f);
	
	thetaRad = thetaDeg * (M_PI / 180.0f); // convert degree to radian

	moveJetTopX = radius  * cosf(thetaRad); // calculate x
	moveJetTopY = radius  * sinf(thetaRad); // calculate y
	glPopMatrix();

	glPushMatrix();
	drawJet(1,1);
	glPopMatrix();

	glPushMatrix();
	drawJet(2,2);
	glPopMatrix();

	glPushMatrix();
	drawJet(3,3);
	glPopMatrix();

	glPopMatrix();

	//Swap the buffers
	SwapBuffers(ghdc); // win32 function // Swaps the back buffer with the front buffer to display the rendered image (Double Buffering).

}

// Angle for the arc (0 to pi for 180-degree movement)
float angleTop = 0.0f;      // For the top jet
float angleBottom = 0.0f;   // For the bottom jet
// Parameters for the curve
   // Radius of the arc (distance from the center)
float centerY_top = 30.0f;  // Top jet curve (center is above the path)
float angularSpeed = 0.05f; // Adjust this value to control speed
int num_segments = 100; // More segments = smoother circle

int segments = 512; 
GLfloat angle = 3.14159f;
float theta = 0.0f;


void update(void)
{
	// code
	if(!start)
	{
		planePosOffset[0].x = -3.0f;		planePosOffset[0].y = 3.0f;		planePosOffset[0].z = 0.0f;
		planePosOffset[1].x = -2.0f;		planePosOffset[1].y = 0.0f;		planePosOffset[1].z = 0.0f;
		planePosOffset[2].x = -3.0f;		planePosOffset[2].y = -3.0f;	planePosOffset[2].z = 0.0f;
		start = TRUE;
	}


	// if(!flagForB)
	// {
	// 	moveBOnAxis += 0.02;
	// 	if(moveBOnAxis >= -0.5f)
	// 	{
	// 		flagForB = TRUE;
	// 	}
	// }
	// else if (moveHOnXAxis < -0.3f || moveHOnYAxis > 0.0f)
	// {
		
	// 	if(moveHOnXAxis < -0.3f) moveHOnXAxis += 0.02f;
	// 	if(moveHOnYAxis > 0.0f ) moveHOnYAxis -= 0.02f;
		
	// }
	// else if(moveAOnXAxis < -0.1f || moveAOnYAxis < 0.0f)
	// {
	// 	if(moveAOnXAxis < -0.1f) moveAOnXAxis += 0.02f;
	// 	if(moveAOnYAxis < 0.0f) moveAOnYAxis += 0.02f;
	// }
	// else if(moveROnXAxis > 0.1f || moveROnYAxis > 0.0f)
	// {
	// 	if(moveROnXAxis > 0.1f) moveROnXAxis -= 0.02f;
	// 	if(moveROnYAxis > 0.0f) moveROnYAxis -= 0.02f;
	// }
	// else if(moveA2OnXAxis > 1.8f || moveA2OnYAxis < 0.0f)
	// {
	// 	if(moveA2OnXAxis > 1.8f) moveA2OnXAxis -= 0.02f;
	// 	if(moveA2OnYAxis < 0.0f) moveA2OnYAxis += 0.02f;
	// }
	// else if(moveTOnAxis > 0.5f)
	// {
	// 	moveTOnAxis -= 0.02f;
	// }
	// else 
	if(moveJetOnXAxis < 10.0f)
	{
		
		moveJetOnXAxis += 0.005f;
		if(moveJetTopAngle <= 270)
		{
			moveJetTopAngle += 0.2;;
		}
		moveJetTopX ;//+= 0.005f;
		//moveJetTopY -= 0.005f;
		//moveJetBottomX += 0.005f;
		if(thetaDeg <= 270)
		{
			thetaDeg += 0.2f;
		}

		

		// TOP JET===========================================================
		// if (thetaDeg >= 90.0f)
		// {
		// 	thetaDeg -= 0.5f; // or some smooth value
		// }
		// else
		// {
		// 	thetaDeg = 360.0f;
		// }




		
	
		// /============================================================================
		

		if(moveJetOnXAxis >= -1.8f)
		{
			flagColorb = TRUE;
		}
		if(moveJetOnXAxis >= -0.92f)
		{
			flagColorh = TRUE;
		}
		if(moveJetOnXAxis >= -0.0f)
		{
			flagColora = TRUE;
		}
		if(moveJetOnXAxis >= 0.85f)
		{
			flagColorr = TRUE;
		}
		if(moveJetOnXAxis >= 2.0f)
		{
			flagColora2 = TRUE;
		}
		if(moveJetOnXAxis >= 2.7f)
		{
			flagColort = TRUE;
		}
	}

}

void uninitialized(void)
{
	// function declarations
	void togglefullscreen(void);

	// code
	// if user is exiting in full screen then restored back to noraml
	if (gbFullScreen == TRUE)
	{
		togglefullscreen();
		gbFullScreen = FALSE;
	}


	// make hdc as current context by relesing  rendering context as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// delet the rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// relse the dc
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// distroy winode
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}



	// Close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program terminated sucessfully");
		fclose(gpFile);
		gpFile = NULL;
	}
}



