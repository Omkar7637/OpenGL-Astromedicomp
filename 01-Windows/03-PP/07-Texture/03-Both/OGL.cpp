// Win32 headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"vmath.h" // matrix and transformation related things
using namespace vmath;

// OpenGL related header Files 
#include<gl/glew.h> // This header files must be included before GL.h
#include<gl/GL.h>

// Custom header file
#include"OGL.h"

// OpenGL related Libraries
#pragma comment(lib, "glew32.lib")
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

// Shader related variables
GLuint shaderProgramObject = 0;
enum 
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
};

GLuint vao = 0;
GLuint vao_pyramid = 0;
GLuint vao_cube = 0;
GLuint vbo_position_pyramid = 0;
GLuint vbo_position_cube = 0;
GLuint vbo_position = 0;
GLuint vbo_TexCoord_pyramid = 0;
// GLuint vbo_TexCoord = 0;
GLuint vbo_TexCoord_cube = 0;
GLuint vbo_color = 0;

//Rotation Angles
GLfloat anglePyramid = 0.0f;
GLfloat angleCube = 0.0f;

GLuint mvpMatrixUniform = 0; // model view projection

mat4 presepectiveProjectionMatrix; // matrix 4x4

// For texture
GLuint textureStone = 0;
GLuint textureKundali = 0;
GLuint textureSamplerUniform = 0; // texture uniform



// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Local Function Decalartions
	int initialized(void);
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
	void uninitialized(void);
	BOOL loadGLTexture(GLuint*, TCHAR[]);

	// Variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	GLenum glewResult;

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

	// Initialize GLEW
	glewResult = glewInit();
	if(glewResult != GLEW_OK)
	{
		fprintf(gpFile, "glewInit Failed\n");
		return(-6);
	}

	// Print GL info
	printGLInfo();

	// Steps
	// 1] write the shader source code
	// 2] Create the shader object
	// 3] Give the shdaer source to the shader object
	// 4] Compile the shader programatically
	// 5] Do shader compiliation error checking

	// ========VERTEX SHADER=======
	const char* vertexShaderSourceCode = 
	"#version 460 core\n" \
	"in vec4 aPosition;\n" \
	"in vec2 aTexCoord;\n" \
	"out vec2 out_TexCoord;\n" \
	"uniform mat4 uMVPMatrix;\n" \
	"void main(void)\n" \
	"{\n" \
	"gl_Position = uMVPMatrix * aPosition;\n" \
	"out_TexCoord = aTexCoord;\n" \
	"}\n";
	// 2nd
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	// 3rd
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);
	// 4th
	glCompileShader(vertexShaderObject);
	// 5th
	GLint status = 0;
	GLint infoLogLenth = 0;
	GLchar* szinfoLog = NULL;
	
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	if(status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLenth);

		if(infoLogLenth > 0)
		{
			szinfoLog = (GLchar*) malloc(infoLogLenth * sizeof(GLchar));

			if(szinfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, infoLogLenth, NULL, szinfoLog);
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG = %s\n", szinfoLog);
				free(szinfoLog);
				szinfoLog = NULL;
			}
		}
		uninitialized();
	}

	// ===============FRAGMENT SHADER===============
	const GLchar* fragmentShaderSourceCode = 
	"#version 460 core\n" \
	"in vec2 out_TexCoord;\n" \
	"uniform sampler2D uTextureSampler;\n" \
	"out vec4 FragColor;\n" \
	"void main(void)\n" \
	"{\n" \
	"FragColor = texture(uTextureSampler, out_TexCoord);\n" \
	"}\n";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	status = 0;
	infoLogLenth = 0;
	szinfoLog = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if(status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLenth);

		if(infoLogLenth > 0)
		{
			szinfoLog = (GLchar*) malloc(infoLogLenth * sizeof(GLchar));

			if(szinfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, infoLogLenth, NULL, szinfoLog);
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG = %s\n", szinfoLog);
				free(szinfoLog);
				szinfoLog = NULL;
			}
		}
		uninitialized();
	}

	// 1] CREATE SHADER PROGRAM OBJECT
	// 2] Attach Shader object to shader program objects
	// 3] tell to link shader objects to shader program object
	// 4] Check for link error logs

	// Create, Attach, Link Shader Program Object
	shaderProgramObject = glCreateProgram();

	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// bind shader attribute at a certan index in shader to same index in host program
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
	// Link the shader program object
	glLinkProgram(shaderProgramObject);
	
	// Check for link error logs
	// 1] Get the link status
	// 2] Get the info log length	
	// 3] Allocate memory for info log
	// 4] Get the info log
	// 5] Print the info log
	// 6] Free the info log memory
	// 7] Uninitialize the program if link failed
	// 8] Unbind the shader program object
	// 9] Unbind the shader object

	status = 0;
	infoLogLenth = 0;
	szinfoLog = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	
	if(status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLenth);

		if(infoLogLenth > 0)
		{
			szinfoLog = (GLchar*) malloc(infoLogLenth * sizeof(GLchar));

			if(szinfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLenth, NULL, szinfoLog);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG = %s\n", szinfoLog);
				free(szinfoLog);
				szinfoLog = NULL;
			}
		}
		uninitialized();
	}

	// Get the uniform locations
	// uMVPMatrix is the uniform variable in vertex shader
	// uTextureSampler is the uniform variable in fragment shader
	// uMVPMatrix is the uniform variable in vertex shader	
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");	

	//Provide vertex position/vertices, color, normal, texcoord, etc. //in PP OpenGL no glVertex,Color,Texcoords,only given by array here
	const GLfloat pyramid_position[] =
	{
		// front
	 0.0f,  1.0f,  0.0f, // front-top
	-1.0f, -1.0f,  1.0f, // front-left
	 1.0f, -1.0f,  1.0f, // front-right

	 // right
	 0.0f,  1.0f,  0.0f, // right-top
	 1.0f, -1.0f,  1.0f, // right-left
	 1.0f, -1.0f, -1.0f, // right-right

	 // back
	 0.0f,  1.0f,  0.0f, // back-top
	 1.0f, -1.0f, -1.0f, // back-left
	-1.0f, -1.0f, -1.0f, // back-right

	// left
	0.0f,  1.0f,  0.0f, // left-top
   -1.0f, -1.0f, -1.0f, // left-left
   -1.0f, -1.0f,  1.0f, // left-right
	};
	const GLfloat pyramid_texcoords[] =
	{// front
	0.5, 1.0, // front-top
	0.0, 0.0, // front-left
	1.0, 0.0, // front-right

	// right
	0.5, 1.0, // right-top
	1.0, 0.0, // right-left
	0.0, 0.0, // right-right

	// back
	0.5, 1.0, // back-top
	0.0, 0.0, // back-left
	1.0, 0.0, // back-right

	// left
	0.5, 1.0, // left-top
	1.0, 0.0, // left-left
	0.0, 0.0, // left-right
	};
	const GLfloat cube_position[] =
	{
		// front
	 1.0f,  1.0f,  1.0f, // top-right of front
	-1.0f,  1.0f,  1.0f, // top-left of front
	-1.0f, -1.0f,  1.0f, // bottom-left of front
	 1.0f, -1.0f,  1.0f, // bottom-right of front

	 // right
	  1.0f,  1.0f, -1.0f, // top-right of right
	  1.0f,  1.0f,  1.0f, // top-left of right
	  1.0f, -1.0f,  1.0f, // bottom-left of right
	  1.0f, -1.0f, -1.0f, // bottom-right of right

	  // back
	   1.0f,  1.0f, -1.0f, // top-right of back
	  -1.0f,  1.0f, -1.0f, // top-left of back
	  -1.0f, -1.0f, -1.0f, // bottom-left of back
	   1.0f, -1.0f, -1.0f, // bottom-right of back

	   // left
	   -1.0f,  1.0f,  1.0f, // top-right of left
	   -1.0f,  1.0f, -1.0f, // top-left of left
	   -1.0f, -1.0f, -1.0f, // bottom-left of left
	   -1.0f, -1.0f,  1.0f, // bottom-right of left

	   // top
		1.0f,  1.0f, -1.0f, // top-right of top
	   -1.0f,  1.0f, -1.0f, // top-left of top
	   -1.0f,  1.0f,  1.0f, // bottom-left of top
		1.0f,  1.0f,  1.0f, // bottom-right of top

		// bottom
		 1.0f, -1.0f,  1.0f, // top-right of bottom
		-1.0f, -1.0f,  1.0f, // top-left of bottom
		-1.0f, -1.0f, -1.0f, // bottom-left of bottom
		 1.0f, -1.0f, -1.0f, // bottom-right of bottom
	};
	const GLfloat cube_texcoords[] =
	{
		// front
	1.0f, 1.0f, // top-right of front
	0.0f, 1.0f, // top-left of front
	0.0f, 0.0f, // bottom-left of front
	1.0f, 0.0f, // bottom-right of front

	// right
	1.0f, 1.0f, // top-right of right
	0.0f, 1.0f, // top-left of right
	0.0f, 0.0f, // bottom-left of right
	1.0f, 0.0f, // bottom-right of right

	// back
	1.0f, 1.0f, // top-right of back
	0.0f, 1.0f, // top-left of back
	0.0f, 0.0f, // bottom-left of back
	1.0f, 0.0f, // bottom-right of back

	// left
	1.0f, 1.0f, // top-right of left
	0.0f, 1.0f, // top-left of left
	0.0f, 0.0f, // bottom-left of left
	1.0f, 0.0f, // bottom-right of left

	// top
	1.0f, 1.0f, // top-right of top
	0.0f, 1.0f, // top-left of top
	0.0f, 0.0f, // bottom-left of top
	1.0f, 0.0f, // bottom-right of top

	// bottom
	1.0f, 1.0f, // top-right of bottom
	0.0f, 1.0f, // top-left of bottom
	0.0f, 0.0f, // bottom-left of bottom
	1.0f, 0.0f, // bottom-right of bottom
	};

	// ================pyramid=====================

	// Vertex array object for arrays of vertex arrtributes
	glGenVertexArrays(1, &vao_pyramid); // vao vertex array object
	glBindVertexArray(vao_pyramid);

	// POSITION
	glGenBuffers(1, &vbo_position_pyramid); // vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_position), pyramid_position, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// COLOR
	glGenBuffers(1, &vbo_TexCoord_pyramid); // vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord_pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_texcoords), pyramid_texcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // vao unbind


	// ================cube=====================

	// Vertex array object for arrays of vertex arrtributes
	glGenVertexArrays(1, &vao_cube); // vao vertex array object
	glBindVertexArray(vao_cube);

	// POSITION
	glGenBuffers(1, &vbo_position_cube); // vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// COLOR
	glGenBuffers(1, &vbo_TexCoord_cube); // vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_TexCoord_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);
	glBindVertexArray(0); // vao unbind

	// depth related calls
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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

	
	presepectiveProjectionMatrix = mat4::identity(); // this is analogas to glloadidentity

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

		// gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);
		

		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitmap);

		hBitmap = NULL;
	}

	return(bResult);

}

void printGLInfo(void) {

	// Varible declarations
	GLint numExtensions, i;

	// code
	// print openGL Information
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "---------x--------\n");
	fprintf(gpFile, "openGL vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n",glGetString(GL_RENDERER));
	fprintf(gpFile, "openGL version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	// Get number of extentions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions); 
	fprintf(gpFile, "Extention Countes : %d\n", numExtensions); 	

	// Print opengl extensions
	// for(i = 0; i < numExtensions; i++)
	// {
	// 	fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	// }
	
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

	presepectiveProjectionMatrix = vmath :: perspective (45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}


void display(void)
{
	//code

	//Clear OpenGL Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use Shader Program Object
	glUseProgram(shaderProgramObject);

	//=======================PYRAMID====================================
	mat4 modelViewMatrix = mat4::identity(); //analogus to glLoadIdentity() from FFP for ModelViewMatrix
	mat4 modelViewProjectionMatrix = mat4::identity();

	//Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	translationMatrix = vmath::translate(-1.5f, 0.0f, -6.0f);
	rotationMatrix = vmath::rotate(anglePyramid, 0.0f, 1.0f, 0.0f);

	modelViewMatrix = translationMatrix * rotationMatrix; //order is very important
	modelViewProjectionMatrix = presepectiveProjectionMatrix * modelViewMatrix; //order is important, aadhi projection matrix * modelview matrix anycha

	//send this matrix to the shader in uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
	//1. shader madhla uniform tumhi tumcha kade kontya uniform madhe gheun thevlay
	//2. kiti matrices patvtay
	//3. toh matrices transpose karychay ka
	//4. jo matrix patvycha ahe to dya

	//For texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureStone);
	glUniform1i(textureSamplerUniform, 0);	//sampler 2d internally unsigned int ahe, gpu madhe sampler ch array asto

	//bind with vao
	glBindVertexArray(vao_pyramid);

	//draw the vertex arrays
	glDrawArrays(GL_TRIANGLES, 0, 12); //saying go to gpu to start drawing
	//1. primitive dya
	//2. array madhe survat kutun karu
	//3. triangle la kiti vertices ahet

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind with vao
	glBindVertexArray(0);

	//===================================CUBE=================================================
	//Transformations
	modelViewMatrix = mat4::identity(); //analogus to glLoadIdentity() from FFP for ModelViewMatrix
	modelViewProjectionMatrix = mat4::identity();

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(1.5f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	mat4 rotationMatrixX = mat4::identity();
	rotationMatrixX = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);

	mat4 rotationMatrixY = mat4::identity();
	rotationMatrixY = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);

	mat4 rotationMatrixZ = mat4::identity();
	rotationMatrixZ = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

	rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	
	modelViewProjectionMatrix = presepectiveProjectionMatrix * modelViewMatrix; //order is important, aadhi projection matrix * modelview matrix anycha

	//send this matrix to the shader in uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	//For texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKundali);
	glUniform1i(textureSamplerUniform, 0);

	//bind with vao
	glBindVertexArray(vao_cube);

	//draw the vertex arrays
	/*glDrawArrays(GL_TRIANGLE_FAN, 0,4);
	glDrawArrays(GL_TRIANGLE_FAN, 4,4);
	glDrawArrays(GL_TRIANGLE_FAN, 8,4);
	glDrawArrays(GL_TRIANGLE_FAN, 12,4);
	glDrawArrays(GL_TRIANGLE_FAN, 16,4);
	glDrawArrays(GL_TRIANGLE_FAN, 20,4);*/

	// or below both works

	for (int i = 0; i <= 20; i += 4)
	{
		glDrawArrays(GL_TRIANGLE_FAN, i, 4);
	}

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind with vao
	glBindVertexArray(0);

	//Unuse Shader Program Object
	glUseProgram(0);

	//Swap the buffers
	SwapBuffers(ghdc); //Win32(), ghdc ha specialist OS cha ahe, ghrc ha OpenGL

}

void update(void)
{
	// code
	anglePyramid = anglePyramid + 2.0f;


	angleCube = angleCube + 1.5f;


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

	if (textureKundali) 
	{
		glDeleteTextures(1, &textureKundali);
		textureKundali = 0;
	}

	if (textureStone) 
	{
		glDeleteTextures(1, &textureStone);
		textureStone = 0;
	}

	

	//free vbo_position
	if (vbo_position_cube)
	{
		glDeleteBuffers(1, &vbo_position_cube);
		vbo_position_cube = 0;
	}

	//free vao
	if (vao_cube)
	{
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	

	//free vbo_position
	if (vbo_position_pyramid)
	{
		glDeleteBuffers(1, &vbo_position_pyramid);
		vbo_position_pyramid = 0;
	}

	//fre vao
	if (vao_pyramid)
	{
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
	}

	// Dettach , delete shaderobject and shader program object
	// how Dettach/Delete Shader program object of any number and any type of shaders
	// 1] Check if shader programm object is still there
	// 2] Get number of shaders and continue only if number of shader is greater than 0
	// 3] Create a buffer/Array to hold shader Object of obtained numbers of shaders
	// 4] Get Shader objects into this buffer/Array and continue if malloc is succedded
	// 5] Start a loop for obtained number of shaders and inside this loop dettached and delete every shader from the buffer/Array
	// 6] Free the buffer/Array
	// 7] Delete the Shader program object
	if(shaderProgramObject) 
	{
		glUseProgram(shaderProgramObject);
		GLint numShaders;
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
		if(numShaders > 0)
		{
			GLuint *pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
			if(pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
				for(GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
			}
			free(pShaders);
			pShaders = NULL;
		}
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
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

