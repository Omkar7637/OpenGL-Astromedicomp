// Win32 headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>

// OpenGL related header Files 
#include<gl/glew.h> // This header files must be included before GL.h
#include<gl/GL.h>

// Custom header file
#include"OGL.h"
#include"vmath.h" // matrix and transformation related things
using namespace vmath;
#include "Sphere.h" // Sphere

// OpenGL related Libraries
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Sphere.lib")

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
	AMC_ATTRIBUTE_NORMAL,

};

GLuint mvpMatrixUniform = 0; // model view projection

mat4 perspectiveProjectionMatrix; // matrix 4x4

// Sphere related variables
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

int gNumVertices = 0;
int gNumElements = 0;

GLuint gVao_sphere = 0;
GLuint gVbo_sphere_position = 0;
GLuint gVbo_sphere_normal = 0;
GLuint gVbo_sphere_element = 0;

GLuint modelViewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;
GLuint ldUniform = 0;
GLuint kdUniform = 0;
GLuint lightPositionUniform = 0;
GLuint LKeyPressedUniform = 0;
BOOL bLight = FALSE;

GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };		// sir changed last from 0.0 to 1.0 in this live code

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
				case 'L':
				case 'l':
						if (bLight == FALSE)
						{
							bLight = TRUE;		// GL_LIGHTING doesn't work in PP
						}
						else
						{
							bLight = FALSE;
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

	const GLchar* vertexShaderSourceCode =
		"#version 460 core \n" \
		"in vec4 aPosition;\n" \
		"in vec3 aNormal;\n" \
		"uniform mat4 uModelViewMatrix;\n" \
		"uniform mat4 uProjectionMatrix;\n" \
		"uniform vec3 uLd;\n" \
		"uniform vec3 uKd;\n" \
		"uniform vec4 uLightPosition;\n" \
		"uniform int uLKeyIsPressed;\n" \
		"out vec3 out_DiffuseLight;\n" \
		"void main(void)\n" \
		"{\n" \
		"gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;\n" \
		"if(uLKeyIsPressed == 1)\n" \
		"{\n" \
		"	vec4 eyeCoordinates = uModelViewMatrix * aPosition;\n" \
		"	mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));\n" \
		"	vec3 transformedNormal = normalize(normalMatrix * aNormal);\n" \
		"	vec3 lightSource = normalize(vec3(uLightPosition - eyeCoordinates));\n" \
		"	out_DiffuseLight = uLd * uKd * max(dot(lightSource, transformedNormal), 0.0);\n" \
		"}\n" \
		"else\n" \
		"{\n" \
		"	out_DiffuseLight = vec3(1.0, 1.0, 1.0);\n" \
		"}\n" \
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
		"in vec3 out_DiffuseLight;\n" \
		"out vec4 FragColor;\n" \
		"void main(void)\n" \
		"{\n" \
		"FragColor = vec4(out_DiffuseLight, 1.0);\n" \
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
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

	glLinkProgram(shaderProgramObject);

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

	// Get the required uniform location from the Shader
	modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelViewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
	ldUniform = glGetUniformLocation(shaderProgramObject, "uLd");
	kdUniform = glGetUniformLocation(shaderProgramObject, "uKd");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");
	LKeyPressedUniform = glGetUniformLocation(shaderProgramObject, "uLKeyIsPressed");

	// provide vertex, position, color, normal, texcoord etc.

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// Vertex array object for arrays of vertex attributes
	// Sphere
	// vao
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	// position vbo
	glGenBuffers(1, &gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// unbind vao	
	glBindVertexArray(0);

	// depth related calls
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// from here onword OpenGL codes starts
	// tell the opengl to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity(); // this is analogas to glloadidentity

	// Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);
	
	return(0);
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

	perspectiveProjectionMatrix = vmath :: perspective (45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}


void display(void)
{
	//code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the screen by setting the color iwth glclearclor() and glCleardepth call

	// Use Shader Program Object
	glUseProgram(shaderProgramObject);

	// transformations
	mat4 modelViewMatrix = mat4::identity(); // this is anloags to glloadidenty in display for modelview matrix
	
	// Translation like gltranslate
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
	modelViewMatrix = translationMatrix;

	glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLight == TRUE)
	{
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform4fv(lightPositionUniform, 1, lightPosition);
		glUniform1i(LKeyPressedUniform, 1);
	}
	else
	{
		glUniform1i(LKeyPressedUniform, 0);
	}

	//// Bind with vao
	// Sphere
	// *** bind vao ***
	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// Unused shader program object
	glUseProgram(0);

	// Swap the buffers
	SwapBuffers(ghdc);

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

	// Free vbo of position
	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	// Free vao
	if (gVao_sphere)
	{
		glDeleteVertexArrays(1, &gVao_sphere);
		gVao_sphere = 0;
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

