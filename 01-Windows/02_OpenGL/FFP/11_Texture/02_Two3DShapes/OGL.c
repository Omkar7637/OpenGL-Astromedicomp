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
FILE *gpFile = NULL;

// Active window related variable
BOOL gbActiveWindow = FALSE;

// Exit key pressed related
BOOL gbEscKeyIsPressed = FALSE;

// OpenGL related global variables
HDC ghdc = NULL; // handle to device context
HGLRC ghrc = NULL; 

// Rotation angles
float anglecube = 0.0f;
float anglepyramid = 0.0f;

// Texture related variables
GLuint textureStone = 0;
GLuint textureKundali = 0;


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
	void resize(int, int);
	BOOL loadGLTexture(GLuint* ,TCHAR[]);

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
	pfd.cDepthBits = 32;// depth
	
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



	// depth related calls
	glShadeModel(GL_SMOOTH); 
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// from here onword OpenGL codes starts
	// tell the opengl to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Load Textures
	if(loadGLTexture(&textureStone, MAKEINTRESOURCE(IDBITMAP_STONE)) == FALSE)
	{
		fprintf(gpFile, "Load texture Stone failed!!\n");
		return(-6);
	}

	if(loadGLTexture(&textureKundali, MAKEINTRESOURCE(IDBITMAP_KUNDALI)) == FALSE)
	{
		fprintf(gpFile, "Load texture Kundali failed!!\n");
		return(-7);
	}

	// Enable Texturing 
	glEnable(GL_TEXTURE_2D);

	// Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);
	
	return(0);
}



BOOL loadGLTexture(GLuint* texture,TCHAR imageResourceID[])
{
	// variable declarations
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	// load the bitmap as image
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if(hBitmap)
	{
		bResult = TRUE;

		// get bitmap sturucture from the loaded bitmap image
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		// Genrate OpenGL Texture object
		glGenTextures(1, texture);

		// bind to the newly created empty structured oabject
		glBindTexture(GL_TEXTURE_2D, *texture);

		// unpack the image into memory for faster loading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitmap);

		hBitmap = NULL;
	}

	return(bResult);

}



void printGLInfo(void) {
	// code
	// print openGL Information
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "---------x--------\n");
	fprintf(gpFile, "openGL vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n",glGetString(GL_RENDERER));
	fprintf(gpFile, "openGL version : %s\n", glGetString(GL_VERSION));
	//fprintf(gpFile, "openGL EXTENSIONS : %s\n", glGetString(GL_EXTENSIONS));
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

	// set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// set to identity matrix
	glLoadIdentity();

	// do prespective projection
	gluPerspective(25.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // FOV-Y , Aspect Ratio, near, far
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
	glRotatef(anglepyramid, 0.0f, 1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, textureStone);

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
	glRotatef(anglecube, 1.0f, 0.0f, 0.0f);
	//glRotatef(angleCube, 0.0f, 1.0f, 0.0f);
	//glRotatef(angleCube, 0.0f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, textureKundali);


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
	// code
	anglecube = anglecube + 1.0f;

	if(anglecube >= 360.0f)
	{
		anglecube = anglecube - 360.0f;
	}

	anglepyramid = anglepyramid + 1.0f;

	if(anglepyramid >= 360.0f)
	{
		anglepyramid = anglepyramid - 360.0f;
	}

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

	
	if (textureKundali) {
		glDeleteTextures(1, &textureKundali);
		textureKundali = 0;
	}

	if (textureStone) {
		glDeleteTextures(1, &textureStone);
		textureStone = 0;
	}

	// Close the file
	if(gpFile)
	{
		fprintf(gpFile, "Program terminated sucessfully");
		fclose(gpFile);
		gpFile = NULL;
	}
}

