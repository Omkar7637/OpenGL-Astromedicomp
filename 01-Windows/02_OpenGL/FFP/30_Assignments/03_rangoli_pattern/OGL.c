// Win32 headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
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

void lastpattern()
{


	glPushMatrix();
	glTranslatef(-1.7f, -0.8f, 0.0f);

	GLfloat tstartX = -0.45f;
	GLfloat tstartY = 0.45f;
	GLfloat tgap = 0.3f;

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			GLfloat x = tstartX + col * tgap;
			GLfloat y = tstartY - row * tgap;

			glBegin(GL_LINE_LOOP);
			glVertex3f(x, y - tgap, 0.0f);
			glVertex3f(x, y, 0.0f);
			glVertex3f(x + tgap, y, 0.0f);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(x + tgap, y, 0.0f);
			glVertex3f(x + tgap, y - tgap, 0.0f);
			glVertex3f(x, y - tgap, 0.0f);
			glVertex3f(x + tgap, y - tgap, 0.0f);
			glEnd();
		}
	}

	glPopMatrix();


}

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
	gluLookAt(0.0f, 0.0f, 8.0f,   // Eye
		0.0f, 0.0f, 0.0f,   // Center
		0.0f, 1.0f, 0.0f);  // Up

	// DOT MATRIX
	glPushMatrix();
	glTranslatef(-1.7f, 0.8f, 0.0f); 
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);


	GLfloat startx = -0.45f;
	GLfloat starty = 0.45f;
	GLfloat gap = 0.3f;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			GLfloat x = startx + col * gap;
			GLfloat y = starty - row * gap;
			glVertex3f(x, y, 0.0f);
		}
	}
	glEnd();
	glPopMatrix();

	// TRIANGLE MATRIX
	glPushMatrix();

	glTranslatef(0.0f, 0.8f, 0.0f); 
	GLfloat tstartX = -0.45f;
	GLfloat tstartY = 0.45f;
	GLfloat tgap = 0.3f; 

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			GLfloat x = tstartX + col * tgap;
			GLfloat y = tstartY - row * tgap;

			glBegin(GL_LINE_LOOP);
			glVertex3f(x, y - tgap, 0.0f);             
			glVertex3f(x, y, 0.0f);                   
			glVertex3f(x + tgap, y , 0.0f);       
			glEnd();
		}
	}
	glPopMatrix();


	// SQUARE MATRIX
	glPushMatrix();
	glTranslatef(1.7f, 0.8f, 0.0f); 

	GLfloat sstartX = -0.45f;
	GLfloat sstartY = 0.45f;
	GLfloat sgap = 0.3f; 

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			GLfloat x = sstartX + col * sgap;
			GLfloat y = sstartY - row * sgap;

			glBegin(GL_LINE_LOOP);
			glVertex3f(x, y - sgap, 0.0f);
			glVertex3f(x, y, 0.0f);                   
			glVertex3f(x + sgap, y, 0.0f);  
			glVertex3f(x + sgap, y - sgap, 0.0f);

			glEnd();
		}
	}
	glPopMatrix();

	// COLORED SQUARE MATRIX WITH WITE BOARDER
	glPushMatrix();
	glTranslatef(1.7f, -0.8f, 0.0f); 

	GLfloat cstartX = -0.45f;
	GLfloat cstartY = 0.45f;
	GLfloat cgap = 0.3f; 

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			GLfloat x = cstartX + col * cgap;
			GLfloat y = cstartY - row * cgap;

	
			if (col == 0)
				glColor3f(1.0f, 0.0f, 0.0f); 
			else if (col == 1)
				glColor3f(0.0f, 1.0f, 0.0f); 
			else if (col == 2)
				glColor3f(0.0f, 0.0f, 1.0f); 

			// Draw a square
			glBegin(GL_POLYGON);
			glVertex3f(x, y - cgap, 0.0f);        // Bottom-left
			glVertex3f(x, y, 0.0f);              // Top-left
			glVertex3f(x + cgap, y, 0.0f);       // Top-right
			glVertex3f(x + cgap, y - cgap, 0.0f);// Bottom-right
			glEnd();

			// --- WHITE BORDER LINES ---
			glColor3f(1.0f, 1.0f, 1.0f); // White color for grid lines
			glBegin(GL_LINE_LOOP);
			glVertex3f(x, y - cgap, 0.0f);        // Bottom-left
			glVertex3f(x, y, 0.0f);              // Top-left
			glVertex3f(x + cgap, y, 0.0f);       // Top-right
			glVertex3f(x + cgap, y - cgap, 0.0f);// Bottom-right
			glEnd();
		}
	}
	glPopMatrix();


	// ===========================================================================
	glPushMatrix();
	glTranslatef(0.0f, -0.8f, 0.0f); 

	// Square boundary (optional)
	GLfloat estartX = -0.5f;
	GLfloat estartY = 0.5f;
	GLfloat esize = 1.0f;

	// Draw square border
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(estartX, estartY, 0.0f);                  // Top-left
	glVertex3f(estartX + esize, estartY, 0.0f);           // Top-right
	glVertex3f(estartX + esize, estartY - esize, 0.0f);    // Bottom-right
	glVertex3f(estartX, estartY - esize, 0.0f);           // Bottom-left
	glEnd();

	// Draw radial lines from top-left corner
	int numDivisions = 6;
	for (int i = 1; i <= numDivisions; i++) {
		GLfloat t = (GLfloat)(i) / numDivisions;

		GLfloat targetX, targetY;

		if (i <= numDivisions / 2) {
			// First half of lines go to bottom edge
			targetX = estartX + t * esize * 2;
			targetY = estartY - esize;
		}
		else {
			// Second half of lines go to right edge
			targetX = estartX + esize;
			targetY = estartY - (t - 0.5f) * esize * 2;
		}

		glBegin(GL_LINES);
		glVertex3f(estartX, estartY, 0.0f); // top-left corner
		glVertex3f(targetX, targetY, 0.0f);
		glEnd();
	}

	glPopMatrix();

	// ==============================================================================================================

	lastpattern();

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



