// Win32 headers
#include<Windows.h>
#include<stdio.h>
// #include<stdlib.h>   <-- Is this required ?

// OpenGL related header files
#include<gl/GL.h>
#include<gl/GLU.h>

// Custom header files
#include "OGL.h"

// OpenGL related libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

// macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
// variables related with full screen
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

// variables related to FILE I/O
char gszLogFileName[] = "Log.txt";
FILE* gpFile = NULL;

// active window related variable
BOOL gbActiveWindow = FALSE;

// exit key press related
BOOL gbEscapeKeyIsPressed = FALSE;

// OpenGL related global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

//Rotation angles
float anglePyramid = 0.0f;
float angleCube = 0.0f;

// Texture related global variables
GLuint texture_stone;
GLuint texture_kundali;

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//local function declaration
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);
	
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("RTR6");
	BOOL bDone = FALSE;

	// code
	// Create log file
	gpFile = fopen(gszLogFileName, "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log file creation failed"), TEXT("FILE I/O ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program Started Successfully\n");
	}

	// Window Class Initialization
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

	// Registration of Window Class
	RegisterClassEx(&wndclass);

	// Code for Centering of Window
	// I first get the Desktop Window's handle using GetDesktopWindow()
	// Then get the Desktop coordinates using GetWindowRect()
	// Once I have that I compute the X,Y coordinates for CreateWindow() using
	// WIN_X = ( DesktopWindow Right coordinate - DesktopWindow Left Cordinate)/ 2 - WIN_WIDTH / 2
	// WIN_Y = ( DesktopWindow Bottom coordinate - DesktopWindow Top Cordinate)/ 2 - WIN_HEIGHT / 2


	HWND dhwnd = NULL;
	
	dhwnd = GetDesktopWindow();
	if(dhwnd == NULL)
	{
		fprintf(gpFile, "Failed to received Desktop Window Handle\n");
	}
	else
	{
		fprintf(gpFile, "Received Desktop Window Handle\n");
	}

	RECT dRect;

	if (GetWindowRect(dhwnd, &dRect))
	{
		fprintf(gpFile, "Sucessfully executed GetWindowRect()\n");
		fprintf(gpFile, "Destop Co-ordinates are :\n");
		fprintf(gpFile, "Left = %ld\n", dRect.left);
		fprintf(gpFile, "Top = %ld\n", dRect.top);
		fprintf(gpFile, "Right = %ld\n", dRect.right);
		fprintf(gpFile, "Bottom = %ld\n", dRect.bottom);
	}
	else
	{
		fprintf(gpFile, "Error: Cannot execute GetWindowRect() for Desktop Window\n");
		fprintf(gpFile, "Destop Co-ordinates are :\n");
	}
	
	int WIN_X = (dRect.right - dRect.left) / 2 - WIN_WIDTH / 2;
	int WIN_Y = (dRect.bottom - dRect.top) / 2 - WIN_HEIGHT / 2;
	fprintf(gpFile, "Printing value of WIN_X = %d\n", WIN_X);
	fprintf(gpFile, "Printing value of WIN_Y = %d\n", WIN_Y);



	// Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Ameya Patil"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE ,
		WIN_X,
		WIN_Y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Paint background of the window
	UpdateWindow(hwnd);


	/*
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	*/

	// initialize
	int result = initialize();
	if (result != 0)
	{
		fprintf(gpFile, "initialize() failed\n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "initialize() completed successfully\n");
	}

	// Set this Window as foreground and active window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


	// Game loop
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
				if (gbEscapeKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}
				//render
				display();

				//update
				update();
			}
		}
	}

	// uninitialize
	uninitialize();

	return((int)msg.wParam);  // Paranthesis are not necessary for return, we can also use return<space><value>, Sir prefers using paranthesis
}

// Callback function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void toggleFullScreen(void);
	void resize(int, int);
	void uninitialize(void);

	// code
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
		return (0);
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = TRUE;
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
				toggleFullScreen();
				gbFullScreen = TRUE;
			}
			else
			{
				toggleFullScreen();
				gbFullScreen = FALSE;
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		uninitialize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void toggleFullScreen(void)
{
	// Variable declarations
	MONITORINFO mi;


	// code
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
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

}

int initialize(void)
{
	//function declarations
	void printGLInfo(void);
	void resize(int, int);
	BOOL loadGLTexture(GLuint*, TCHAR[]);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	//code
	// pixel format descriptor initialization
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

	// GetDC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDC() failed\n");
		return (-1);
	}

	// Get machining pixel format index using hdc and pfd
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "ChoosePixelFormat() failed\n");
		return (-2);
	}

	// Select the Pixel Format of the found index.
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat() failed\n");
		return (-3);
	}

	// Create rendering context using hdc, pfd and choosen iPixelFormatIndex
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext() failed\n");
		return (-4);
	}

	// Make this rendering context as current context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent() failed\n");
		return (-5);
	}

	// Print GLInfo
	printGLInfo();

	// Depth related code
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);            // Set the depth buffer and change value of all bits to 1.0f
	glEnable(GL_DEPTH_TEST);       // Enabled Depth test
	glDepthFunc(GL_LEQUAL);        // Pass all framents which are having depth component less than equal to 1.0f
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Perpective correction for getting the nicest value

	// From here onwards OpenGL code starts
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Load Textures
	if (loadGLTexture(&texture_stone, MAKEINTRESOURCE(IDBITMAP_STONE)) == FALSE)
	{
		fprintf(gpFile, "The function loadGLTexture() failed for IDBITMAP_STONE\n");
		return (-6);
	}

	if (loadGLTexture(&texture_kundali, MAKEINTRESOURCE(IDBITMAP_KUNDALI)) == FALSE)
	{
		fprintf(gpFile, "The function loadGLTexture() failed for IDBITMAP_KUNDALI\n");
		return (-7);
	}

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// warmup resize()
	resize(WIN_WIDTH, WIN_HEIGHT);

	return 0;
}

void printGLInfo(void)
{
	//code
	// Print OpenGL information
	fprintf(gpFile, "OpenGL Information\n");
	fprintf(gpFile, "\n");
	fprintf(gpFile, "OpenGL Vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGl Version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "\n");

}

BOOL loadGLTexture(GLuint* texture, TCHAR imageResourceID[])
{
	// variable declarations
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	// Load the Bitmap as image
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		bResult = TRUE;
		// Get Bitmap structure from the loaded Bitmap image
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		// Generate OpenGL texture object
		glGenTextures(1, texture);

		// Bind to the newly created empty stucture object
		glBindTexture(GL_TEXTURE_2D, *texture);

		// Unpack the image in memory for faster loading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (const void*)bmp.bmBits);

		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitmap);
		hBitmap = NULL;

	}

	return bResult;
}

void resize(int width, int height)
{
	//code
	// if height by accident is 0 or becomes less than 0 then set it to 1
	if (height <= 0)
	{
		height = 1;
	}

	// Set the viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// Set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// Set to identity matrix
	glLoadIdentity();

	// Do perspective projection
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//glFrustrum
	// H = tan((fovy /2 ) / 180 * PI) * near
	// W = H * Aspect Ratio
	// glFrustrum(-W, W, -H, H, near, far)
	
	// There is also ortho function for GLU
	// gluOrth2D(l ,r ,b ,t);
}

void display(void)
{
	//code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the screen by setting the color iwth glclearclor() and glCleardepth call

	// PYRAMID
	// Set matrix to ModelView mode
	glMatrixMode(GL_MODELVIEW);

	// Set it to identity matrix
	glLoadIdentity();

	// Translate triangle backwards by Z
	glTranslatef(-1.5f, 0.0f, -5.0f);
	glRotatef(anglePyramid, 0.0f, 1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, texture_stone);

	// Triangle drawing code
	glBegin(GL_TRIANGLES);


	// Front phase
	// Apex
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	// Left bottom
	//glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right bottom
	//glColor3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);


	// Right phase
	// Apex
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	// Left bottom
	//glColor3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right bottom
	//glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);


	//Back phase
	// Apex
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	// Left bottom
	//glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Right bottom
	//glColor3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);


	// Left phase
	// Apex
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	// Left bottom
	//glColor3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Right bottom
	//glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	
	// CUBE
	// Set matrix to ModelView mode
	glMatrixMode(GL_MODELVIEW);

	// Set it to identity matrix
	glLoadIdentity();

	// Translate triangle backwards by Z
	glTranslatef(1.5f, 0.0f, -6.0f);
	glScalef(0.75f, 0.75f, 0.75f);
	glRotatef(angleCube, 1.0f, 0.0f, 0.0f);
	//glRotatef(angleCube, 0.0f, 1.0f, 0.0f);
	//glRotatef(angleCube, 0.0f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, texture_kundali);


	// Triangle drawing code
	glBegin(GL_QUADS);


	//Front face
	//glColor3f(1.0f, 0.0f, 0.0f);

	// Right top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//Left top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	// Left bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right bottom
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);


	// Right phase
	//glColor3f(0.0f, 1.0f, 0.0f);

	//Right top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	//Left top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Left bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right bottom
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);


	//Back phase
	//glColor3f(0.0f, 0.0f, 1.0f);

	// Right top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	// Left top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Right bottom
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);


	// Left phase
	//glColor3f(0.0f, 1.0f, 1.0f);

	// Right top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	// Left top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	// Left bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Right bottom
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);


	// Top phase
	//glColor3f(1.0f, 0.0f, 1.0f);

	// Right top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	// Left bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	// Right bottom
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);


	// Bottom phase
	//glColor3f(1.0f, 1.0f, 0.0f);

	// Right top
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Left top
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Left bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Right bottom
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);


	// Swap the buffers
	SwapBuffers(ghdc);

}

void update(void)
{
	//code
	anglePyramid = anglePyramid + 2.0f;
	if (anglePyramid >= 360.0f)
	{
		anglePyramid = anglePyramid - 360.0f;
	}

	angleCube = angleCube + 2.0f;
	if (angleCube >= 360.0f)
	{
		angleCube = angleCube - 360.0f;
	}

}

void uninitialize(void)
{
	// function declarations
	void toggleFullScreen(void);

	//code
	// if user is exiting in full screen then restore the screen back to normal.
	if (gbFullScreen == TRUE)
	{
		toggleFullScreen();
		gbFullScreen = FALSE;
	}

	if (texture_kundali)
	{
		glDeleteTextures(1, &texture_kundali);
		texture_kundali = 0;
	}

	if (texture_stone)
	{
		glDeleteTextures(1, &texture_stone);
		texture_kundali = 0;
	}

	// make hdc as current context by release rendering context as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// Delete the rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// Release the dc
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// Destroy Window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
	}

	// close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program Terminated Successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}

}