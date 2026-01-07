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

// chekerboard related variables
#define CHECKBOARD_WIDTH 512
#define CHECKBOARD_HEIGHT 512

GLubyte checkerboard[CHECKBOARD_HEIGHT][CHECKBOARD_WIDTH][4];
GLuint texture_checkerboard;

int count = 0;
int shapes = 0;
GLfloat z = 5.0f;
GLfloat x = 0.0f;
GLfloat y = 0.0f;

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
				case '1':
					count++;
					break;
				case '2':
					shapes++;
					break;

				case 'r':
				case 'R':
					count = 0;
					shapes = 0;
					break;
					
				case 'z':
					z -= 0.1f;
					break;
				case 'Z':
					z +=0.1f;
					break;
				case 'x':
					x -= 0.1f;
					break;
				case 'X':
					x +=0.1f;
					break;
				case 'y':
					y -= 0.1f;
					break;
				case 'Y':
					y +=0.1f;
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
	void loadGLTexture(void);

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// 0.75/0.25/0.50

	// Load procedural Textures of chekarboard
	loadGLTexture();

	// Enable Texturing 
	glEnable(GL_TEXTURE_2D);

	// Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);
	
	return(0);
}


void loadGLTexture(void)
{
	// function declarations
	void makeChekarboard(void);

	// code
	makeChekarboard();

	// Genrate OpenGL Texture object
	glGenTextures(1, &texture_checkerboard);

	// bind to the newly created empty structured oabject
	glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

	// unpack the image into memory for faster loading
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 1:- Don't do unpacking by rowcolomn method by any byte size, 2:- do unpaking by a number divisible by 2, 4:- do unpaking by RGBA row coloumn methode

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKBOARD_WIDTH, CHECKBOARD_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void makeChekarboard(float time)
{
	switch (count)
	{
		case 1 :
			int i, j;

			for(i = 0; i < CHECKBOARD_HEIGHT; i++)
			{
				for(j = 0; j < CHECKBOARD_WIDTH; j++)
				{
					// Calculate radial distance from center
					float dx = j - (CHECKBOARD_WIDTH / 2);
					float dy = i - (CHECKBOARD_HEIGHT / 2);
					float dist = sqrtf(dx * dx + dy * dy);

					// Time-dependent wave distortions
					float wave = sinf(i * 0.1f + time) + cosf(j * 0.1f + time) + sinf(dist * 0.05f + time * 1.5f);

					// Normalize wave to range 0-1
					float norm = (wave + 3.0f) / 6.0f;

					// Bitwise interaction with time for more variation
					int xorVal = (i ^ j) & 0x3F;  // 0-63
					int andVal = (i & j) & 0x3F;  // 0-63
					int orVal  = (i | j) & 0x3F;  // 0-63

					// Blend bitwise and waveform
					int red   = (int)(norm * 255.0f * ((xorVal % 8) / 7.0f));
					int green = (int)((1.0f - norm) * 200.0f * ((orVal % 16) / 15.0f));
					int blue  = (int)(fabs(sinf(dist * 0.05f - time)) * 255.0f * ((andVal % 8) / 7.0f));

					// Clamp
					red   = red > 255 ? 255 : red;
					green = green > 255 ? 255 : green;
					blue  = blue > 255 ? 255 : blue;

					checkerboard[i][j][0] = (GLubyte)red;
					checkerboard[i][j][1] = (GLubyte)green;
					checkerboard[i][j][2] = (GLubyte)blue;
					checkerboard[i][j][3] = (GLubyte)255;
				}
			}
		break;

		case 2:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float dx = j - (CHECKBOARD_WIDTH / 2);
				float dy = i - (CHECKBOARD_HEIGHT / 2);
				float dist = sqrtf(dx * dx + dy * dy);
	
				float wave = sinf(dist * 0.1f - time * 2.0f);
				float norm = (wave + 1.0f) / 2.0f;
	
				int red   = (int)(norm * 255.0f);
				int green = (int)((1.0f - norm) * 255.0f);
				int blue  = 128;
	
				checkerboard[i][j][0] = (GLubyte)red;
				checkerboard[i][j][1] = (GLubyte)green;
				checkerboard[i][j][2] = (GLubyte)blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
		case 3:
			for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
			{
				for (int j = 0; j < CHECKBOARD_WIDTH; j++)
				{
					float u = (float)j / CHECKBOARD_WIDTH;
					float v = (float)i / CHECKBOARD_HEIGHT;
		
					float angle = atan2f(i - CHECKBOARD_HEIGHT/2, j - CHECKBOARD_WIDTH/2) + time;
					float radius = hypotf(j - CHECKBOARD_WIDTH/2, i - CHECKBOARD_HEIGHT/2);
		
					float val = fabs(sinf(angle * 6.0f + radius * 0.1f - time * 2.0f));
		
					int red   = (int)(val * 255);
					int green = (int)((1.0f - val) * 200);
					int blue  = (int)((sinf(time + radius * 0.05f) + 1.0f) * 127.5f);
		
					checkerboard[i][j][0] = red;
					checkerboard[i][j][1] = green;
					checkerboard[i][j][2] = blue;
					checkerboard[i][j][3] = 255;
				}
			}
		break;	
		case 4:
			for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
			{
				for (int j = 0; j < CHECKBOARD_WIDTH; j++)
				{
					float dx = j - CHECKBOARD_WIDTH / 2.0f;
					float dy = i - CHECKBOARD_HEIGHT / 2.0f;
		
					float angle = atan2f(dy, dx) + time * 0.5f;
					float radius = sqrtf(dx * dx + dy * dy);
		
					float wave = sinf(radius * 0.1f - time * 1.5f + angle * 3.0f);
		
					float norm = (wave + 1.0f) / 2.0f;
		
					int red   = (int)(norm * 255);
					int green = (int)((1.0f - norm) * 255);
					int blue  = (int)(fabs(sinf(angle * 4.0f)) * 255);
		
					checkerboard[i][j][0] = red;
					checkerboard[i][j][1] = green;
					checkerboard[i][j][2] = blue;
					checkerboard[i][j][3] = 255;
				}
			}	
		break;
		case 5:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float glitch = fmodf(i + j + sinf(time * 10.0f + j) * 10.0f, 32.0f) / 32.0f;
	
				int red   = (int)(glitch * 255);
				int green = (int)(fabs(sinf(time + i * 0.05f)) * 255);
				int blue  = (int)(fabs(cosf(time + j * 0.03f)) * 255);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
		case 6:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float v = sinf(i * 0.15f + time) + cosf(j * 0.15f - time);
				float pulse = sinf(sqrtf(i*i + j*j) * 0.05f - time);
	
				float norm = (v + pulse + 2.0f) / 4.0f;
	
				int red   = (int)(norm * 180 + 75);
				int green = (int)(norm * 255);
				int blue  = (int)((1.0f - norm) * 255);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
		case 7:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float dx = j - CHECKBOARD_WIDTH / 2.0f;
				float dy = i - CHECKBOARD_HEIGHT / 2.0f;
	
				float angle = atan2f(dy, dx);
				float radius = sqrtf(dx*dx + dy*dy);
	
				float wave = sinf(radius * 0.05f + angle * 4.0f - time * 3.0f);
				float norm = (wave + 1.0f) / 2.0f;
	
				int red   = (int)(norm * 255);
				int green = (int)(fabs(cosf(angle + time)) * 255);
				int blue  = (int)(fabs(sinf(angle - time)) * 255);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
		case 8:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float waveX = sinf(j * 0.3f + time);
				float waveY = cosf(i * 0.3f + time);
	
				float mix = (waveX + waveY) * 0.5f;
	
				int red   = (int)((mix + 1.0f) * 127.5f);
				int green = (int)(fabs(sinf(j * 0.05f + time)) * 255);
				int blue  = (int)(fabs(cosf(i * 0.05f - time)) * 255);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break ;
		case 9:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float x = j - CHECKBOARD_WIDTH / 2.0f;
				float y = i - CHECKBOARD_HEIGHT / 2.0f;
	
				float r = sqrtf(x * x + y * y);
				float angle = atan2f(y, x) + time;
	
				float wave = sinf(r * 0.15f - angle * 5.0f + time * 2.0f);
	
				float norm = (wave + 1.0f) / 2.0f;
	
				int red   = (int)(norm * 255);
				int green = (int)(fabs(sinf(angle)) * 255);
				int blue  = (int)(fabs(cosf(angle)) * 255);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
		case 10:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float dx = j - CHECKBOARD_WIDTH / 2.0f;
				float dy = i - CHECKBOARD_HEIGHT / 2.0f;
				float dist = sqrtf(dx * dx + dy * dy);
	
				float fade = expf(-dist * 0.03f) * sinf(time + dist * 0.1f);
	
				int red   = (int)(fade * 255);
				int green = (int)(fade * 200 + 55);
				int blue  = (int)(fade * 150 + 105);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
		case 11:
		for (int i = 0; i < CHECKBOARD_HEIGHT; i++)
		{
			for (int j = 0; j < CHECKBOARD_WIDTH; j++)
			{
				float dx = fabs((float)j - CHECKBOARD_WIDTH / 2);
				float dy = fabs((float)i - CHECKBOARD_HEIGHT / 2);
				float radial = sqrtf(dx * dx + dy * dy);
	
				float pattern = cosf(radial * 0.2f - time * 2.0f) * sinf(time + dx * 0.1f);
	
				float norm = (pattern + 1.0f) / 2.0f;
	
				int red   = (int)(norm * 180);
				int green = (int)(norm * 200 + 55);
				int blue  = (int)(norm * 255);
	
				checkerboard[i][j][0] = red;
				checkerboard[i][j][1] = green;
				checkerboard[i][j][2] = blue;
				checkerboard[i][j][3] = 255;
			}
		}
		break;
	}
	
}

void UpdateTexture(float time)
{
    makeChekarboard(time);
    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CHECKBOARD_WIDTH, CHECKBOARD_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
    glBindTexture(GL_TEXTURE_2D, 0);
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

void displaySphere(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glRotatef(anglecube, 1.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluSphere(quad, 1.5f, 64, 64);

    gluDeleteQuadric(quad);
    glBindTexture(GL_TEXTURE_2D, 0);
    SwapBuffers(ghdc);
}

void displayCylinder(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    glRotatef(anglecube, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluCylinder(quad, 1.0f, 1.0f, 2.5f, 48, 48);

    gluDeleteQuadric(quad);
    glBindTexture(GL_TEXTURE_2D, 0);
    SwapBuffers(ghdc);
}

void displayDisk(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    glRotatef(anglecube, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluDisk(quad, 0.5f, 1.5f, 64, 1);

    gluDeleteQuadric(quad);
    glBindTexture(GL_TEXTURE_2D, 0);
    SwapBuffers(ghdc);
}

void displayCone(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    glRotatef(anglecube, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluCylinder(quad, 1.0f, 0.0f, 2.5f, 48, 48);

    gluDeleteQuadric(quad);
    glBindTexture(GL_TEXTURE_2D, 0);
    SwapBuffers(ghdc);
}

void cube(void)
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// glTranslatef(0.0f, 0.0f, -10.0f);  // Move back to see cube
	gluLookAt(
		x, y, z,   // Eye/camera position
		0.0f, 0.0f, 0.0f,   // Target (center of the scene)
		1.0f, 1.0f, 1.0f    // Up vector (which direction is 'up')
	);
	glRotatef(anglecube, 1.0f, 0.0f, 0.0f);
	glRotatef(anglecube, 0.0f, 1.0f, 0.0f);
	glRotatef(anglecube, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

	glBegin(GL_QUADS);
	// Front face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

	// Back face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

	// Top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

	// Bottom face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

	// Right face
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

	// Left face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    SwapBuffers(ghdc);

}


void display(void)
{

	switch (shapes)
	{
		case 1:
		cube();
		break;
		case 2:
		displaySphere();
		break;
		case 3:
		displayCylinder();
		break;
		case 4:
		displayDisk();
		break;
		case 5:
		displayCone();
		break;

	}

}


GLfloat animateTexture;

void update(void)
{
	// code

	

	UpdateTexture(animateTexture);
    animateTexture += 0.01f;

	anglecube = anglecube + 0.1f;

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

	if (texture_checkerboard) {
		glDeleteTextures(1, &texture_checkerboard);
		texture_checkerboard = 0;
	}

	// Close the file
	if(gpFile)
	{
		fprintf(gpFile, "Program terminated sucessfully");
		fclose(gpFile);
		gpFile = NULL;
	}
}

