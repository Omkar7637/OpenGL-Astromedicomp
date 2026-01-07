// Win32 headers
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES 1
#include<math.h>
#pragma region OpenGL Traingle
// OpenGL related header Files 
// #include <GL/glew.h>
#include<gl/GL.h>
#include<gl/GLU.h>


// Custom header file
#include"OGL.h"

// OpenGL related Libraries
#pragma comment(lib, "opengl32.lib")
// #pragma comment(lib, "glew32.lib")
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
#define CHECKBOARD_WIDTH 16
#define CHECKBOARD_HEIGHT 16

// GLubyte checkerboard[CHECKBOARD_HEIGHT][CHECKBOARD_WIDTH][4];
// GLuint texture_checkerboard;

int count = 1;
int shapes = 0;
GLfloat z = 50.0f;
GLfloat x = 0.0f;
GLfloat y = 10.0f;
GLfloat lz = -1.0f;
GLfloat lx = 0.0f;
GLfloat ly = 0.0f;
GLfloat upX, upY = 1, upZ;
float speed = 2.0f; // For the walking Speed like camera movements


// Texture related variables
GLuint textureSmiley = 0;
GLuint textureTerrain = 0;

float yaw = -900.0f;   // looking down Z axis
float pitch = 0.0f;

static int lastX = -1;
static int lastY = -1;
float yCameraMovement;
float xCameramovement;
float zCameraMovement;
POINT lastMouse = { -1, -1 };
POINT center;
float cameraAngleSpeed = 2.0f; // it is used for controll the speed of camera movement like looking up down left right
int selectedObjectIndex = -1; // -1 = none selected


// Function declarations
void updateDirection(); // For updating camera position 



typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float u, v;
} TexCoord;

typedef struct {
    float nx, ny, nz;
} Normal;

typedef struct {
    int v[3];
    int vt[3];
    int vn[3];
} Face;

typedef struct {
	Vertex* vertices;
	TexCoord* texCoords;
	Normal* normals;
	Face* faces;
	GLuint textureID;
	int vertexCount, texCoordCount, normalCount, faceCount;
	// Transformations
    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float scaleX, scaleY, scaleZ;
} Model;

Model loadOBJModel(const char* filename) 
{
	Model model = {0};
	FILE* fp = fopen(filename, "r");
	
	if (!fp) 
	{
	perror("Failed to open OBJ file");
	exit(1);
	}
	
	char line[256];
	int vCount = 0, vtCount = 0, vnCount = 0, fCount = 0;

	// First pass: count lines
	while (fgets(line, sizeof(line), fp)) 
	{
		if (strncmp(line, "v ", 2) == 0) vCount++;
		else if (strncmp(line, "vt ", 3) == 0) vtCount++;
		else if (strncmp(line, "vn ", 3) == 0) vnCount++;
		else if (strncmp(line, "f ", 2) == 0) fCount++;
	}

	model.vertices = malloc(vCount * sizeof(Vertex));
	model.texCoords = malloc(vtCount * sizeof(TexCoord));
	model.normals  = malloc(vnCount * sizeof(Normal));
	model.faces    = malloc(fCount * sizeof(Face));
	model.vertexCount = vCount;
	model.texCoordCount = vtCount;
	model.normalCount = vnCount;
	model.faceCount = fCount;

	rewind(fp);

	int vi = 0, vti = 0, vni = 0, fi = 0;

	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "v ", 2) == 0) {
			sscanf(line, "v %f %f %f", &model.vertices[vi].x, &model.vertices[vi].y, &model.vertices[vi].z);
			vi++;
		} else if (strncmp(line, "vt ", 3) == 0) {
			sscanf(line, "vt %f %f", &model.texCoords[vti].u, &model.texCoords[vti].v);
			vti++;
		} else if (strncmp(line, "vn ", 3) == 0) {
			sscanf(line, "vn %f %f %f", &model.normals[vni].nx, &model.normals[vni].ny, &model.normals[vni].nz);
			vni++;
		} else if (strncmp(line, "f ", 2) == 0) {
			sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&model.faces[fi].v[0], &model.faces[fi].vt[0], &model.faces[fi].vn[0],
				&model.faces[fi].v[1], &model.faces[fi].vt[1], &model.faces[fi].vn[1],
				&model.faces[fi].v[2], &model.faces[fi].vt[2], &model.faces[fi].vn[2]);
			fi++;
		}
	}

	fclose(fp);
	return model;

}


// --------- Global Models ----------
#define MAX_MODELS 10
Model models[MAX_MODELS];
int modelCount = 0;
// -----------------------------------

void renderModel(Model* model) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < model->faceCount; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			int vi = model->faces[i].v[j] - 1;
			int vti = model->faces[i].vt[j] - 1;
			int vni = model->faces[i].vn[j] - 1;


					if (vni >= 0 && vni < model->normalCount)
						glNormal3f(model->normals[vni].nx, model->normals[vni].ny, model->normals[vni].nz);
					if (vti >= 0 && vti < model->texCoordCount)
						glTexCoord2f(model->texCoords[vti].u, model->texCoords[vti].v);
					if (vi >= 0 && vi < model->vertexCount)
						glVertex3f(model->vertices[vi].x, model->vertices[vi].y, model->vertices[vi].z);
			}
	}
	glEnd();
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

				case 'r':
				case 'R': count = 0; shapes = 0; break;
				case 'w': // Forward
						x += lx * speed;
						y += ly * speed;
						z += lz * speed;
						break;

				case 's': // Backward
						x -= lx * speed;
						y -= ly * speed;
						z -= lz * speed;
						break;

				case 'd': // Strafe Left
						x -= lz * speed;
						z += lx * speed;
						break;

				case 'a': // Strafe Right
						x += lz * speed;
						z -= lx * speed;
						break;

				case 'z': z -= speed; break;
				case 'Z': z += speed; break;
				case 'y': y -= speed; break;
				case 'Y': y += speed; break;
				// case 'w': xCameramovement += cameraAngleSpeed; break;
				// case 's': xCameramovement -= cameraAngleSpeed; break;
				// case 'a': yCameraMovement -= cameraAngleSpeed; break;
				// case 'd': yCameraMovement += cameraAngleSpeed; break;
				// case 'q': zCameraMovement -= cameraAngleSpeed; break;
				// case 'e': zCameraMovement += cameraAngleSpeed; break;
				        // case '1': models[0].posX -= 0.1f; break; // Move Rabbit left
						// case '!': models[0].posX += 0.1f; break; // Move Rabbit right
						// case '2': models[0].posY -= 0.1f; break; // Move Cube forward
						// case '@': models[0].posZ += 0.1f; break; // Move Cube backward
						case '1': selectedObjectIndex = 0; break; // Select first object
						case '2': selectedObjectIndex = 1; break; // Select second object
						case '3': selectedObjectIndex = 2; break;

				float moveSpeed = 0.05f;
				Model* m = (selectedObjectIndex >= 0 && selectedObjectIndex < modelCount) ? &models[selectedObjectIndex] : NULL;

				if (m != NULL) {
					switch (wParam) {
						case 'i': // move forward (along camera look direction)
							m->posX += lx * moveSpeed;
							m->posY += ly * moveSpeed;
							m->posZ += lz * moveSpeed;
							break;

						case 'k': // move backward
							m->posX -= lx * moveSpeed;
							m->posY -= ly * moveSpeed;
							m->posZ -= lz * moveSpeed;
							break;

						case 'j': // strafe left (cross product of up and look vector)
							m->posX -= lz * moveSpeed;
							m->posZ += lx * moveSpeed;
							break;

						case 'l': // strafe right
							m->posX += lz * moveSpeed;
							m->posZ -= lx * moveSpeed;
							break;

						case 'u': m->posY += moveSpeed; break; // up
						case 'o': m->posY -= moveSpeed; break; // down
					}
				}



				default:
					break;
			
			}
			break;

		case WM_DESTROY :
			PostQuitMessage(0);
			break;


		case WM_MOUSEMOVE:
        {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);

            if (lastMouse.x == -1) {
                lastMouse.x = mouseX;
                lastMouse.y = mouseY;
                break;
            }

            int deltaX = mouseX - lastMouse.x;
            int deltaY = mouseY - lastMouse.y;

            float sensitivity = 0.02f; // adjust as needed

            yCameraMovement += deltaX * sensitivity;  // yaw (left/right)
            xCameramovement -= deltaY * sensitivity;  // pitch (up/down)

            // Clamp pitch
            if (xCameramovement > 360.0f) xCameramovement = 360.0f;
            if (xCameramovement < -360.0f) xCameramovement = -360.0f;

            updateDirection();
			// Warp to center
			center.x = WIN_WIDTH / 2;
			center.y = WIN_HEIGHT / 2;
			ClientToScreen(hwnd, &center);
			SetCursorPos(center.x, center.y);

			// Reset lastMouse to center
			lastMouse.x = WIN_WIDTH / 2;
			lastMouse.y = WIN_HEIGHT / 2;

        }
        break;

		// case WM_MOUSEMOVE:
		// {
		// 	int currentX = LOWORD(lParam);
		// 	int currentY = HIWORD(lParam);

		// 	if (lastX == -1) lastX = currentX;
		// 	if (lastY == -1) lastY = currentY;

		// 	if (wParam & MK_RBUTTON)
		// 	{
		// 		float sensitivity = 0.1f;
		// 		float dx = (currentX - lastX) * sensitivity;
		// 		float dy = (currentY - lastY) * sensitivity;

		// 		yaw += dx;
		// 		pitch -= dy;

		// 		if (pitch > 89.0f) pitch = 89.0f;
		// 		if (pitch < -89.0f) pitch = -89.0f;

		// 		float radYaw = yaw * M_PI / 180.0f;
		// 		float radPitch = pitch * M_PI / 180.0f;

		// 		lx = cos(radYaw) * cos(radPitch);
		// 		ly = sin(radPitch);
		// 		lz = sin(radYaw) * cos(radPitch);
		// 	}

		// 	lastX = currentX;
		// 	lastY = currentY;
		// }
		// break;


		
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

int initialized(void)
{	
	// function declarations
	void printGLInfo(void);
	void resize(int, int);
	//void loadGLTexture(void);
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
	// if (glewInit() != GLEW_OK) {
    // fprintf(gpFile, "GLEW initialization failed\n");
    // return -6;
	// }


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
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);  // Sky Blue color

	// Load procedural Textures of chekarboard
	//loadGLTexture();

	// Load Textures
	if(loadGLTexture(&textureSmiley, MAKEINTRESOURCE(IDBITMAP_SMILEY)) == FALSE)
	{
		fprintf(gpFile, "Load texture Smiley failed!!\n");
		return(-6);
	}
	if (!loadGLTexture(&textureTerrain, MAKEINTRESOURCE(IDBITMAP_TERRAIN))) {
    fprintf(gpFile, "Terrain texture load failed\n");
    return -7;
	}

	Model rabbit  = loadOBJModel("Untitled.obj"); // or however you're loading
	loadGLTexture(&rabbit.textureID, MAKEINTRESOURCE(IDBITMAP_TERRAIN));
	rabbit.posX = 0.0f;  rabbit.posY = 0.0f; rabbit.posZ = 0.0f;
	rabbit.rotY = 0.0f;
	// rabbit.scaleX = rabbit.scaleY = rabbit.scaleZ = 0.0f;
	models[modelCount++] = rabbit;

	// Model cube = loadOBJModel("Cube.obj");
	// loadGLTexture(&cube.textureID, MAKEINTRESOURCE(IDBITMAP_CUBE));
	// cube.posX = -2.0f; cube.posY = 0.0f; cube.posZ = -1.0f;
	// cube.rotY = 0.0f;
	// cube.scaleX = cube.scaleY = cube.scaleZ = 1.0f;
	// models[modelCount++] = cube;

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
	gluPerspective(25.0f, (GLfloat)width / (GLfloat)height, 0.1f, 700.0f); // FOV-Y , Aspect Ratio, near, far
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        x, y, z,             // Camera position
        x + lx, y + ly, z + lz, // Look-at direction (use direction vector lx, ly, lz)
		upX, upY, upZ     // Up vector
    );

    glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, texture_checkerboard);
	glBindTexture(GL_TEXTURE_2D, textureSmiley);

	glColor3f(1.0f, 1.0f, 1.0f);

	// FOR THE REPETATIVE BASE/GROUND ===============================================================================
    // Repeat the ground in a grid
    float tileSize = 15.0f;       // Size of one tile
    int range = 500;              // Half the grid size (draw from -range to +range)

    glBegin(GL_QUADS);
    for (int i = -range; i < range; i++) {
        for (int j = -range; j < range; j++) {
            float x0 = i * tileSize;
            float z0 = j * tileSize;
            float x1 = x0 + tileSize;
            float z1 = z0 + tileSize;

            // Texture coordinates repeated (optional: scale UVs)
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, -1.0f, z0);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, -1.0f, z0);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, -1.0f, z1);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, -1.0f, z1);
        }
    }
    glEnd();
	
	
    glBindTexture(GL_TEXTURE_2D, 0);
	//====================================================================================================================
	glBindTexture(GL_TEXTURE_2D, textureTerrain);  // Enable texture
	
	for (int i = 0; i < modelCount; i++) {
		Model m = models[i];
		glPushMatrix();
		glTranslatef(m.posX, m.posY, m.posZ);
		glRotatef(m.rotX, 1, 0, 0);
		glRotatef(m.rotY, 0, 1, 0);
		glRotatef(m.rotZ, 0, 0, 1);
		glScalef(m.scaleX, m.scaleY, m.scaleZ);
		glBindTexture(GL_TEXTURE_2D, m.textureID);
		renderModel(&m);
		glPopMatrix();
	}



// Do NOT call glBindTexture(GL_TEXTURE_2D, 0) here unless switching textures



	// //==================

	// glPushMatrix();
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, textureTerrain);

	// glTranslatef(-TERRAIN_SIZE / 2.0f, -5.0f, -TERRAIN_SIZE / 2.0f); // Center and lower terrain

	// glBegin(GL_QUADS);
	// for (int z = 0; z < TERRAIN_SIZE - 1; z++) {
	// 	for (int x = 0; x < TERRAIN_SIZE - 1; x++) {
	// 		float h1 = terrain[x][z];
	// 		float h2 = terrain[x+1][z];
	// 		float h3 = terrain[x+1][z+1];
	// 		float h4 = terrain[x][z+1];

	// 		// Texture scaling factor (to control repetition)
	// 		float scale = 0.1f;

	// 		glTexCoord2f(x * scale, z * scale);     glVertex3f(x,   h1, z);
	// 		glTexCoord2f((x+1) * scale, z * scale); glVertex3f(x+1, h2, z);
	// 		glTexCoord2f((x+1) * scale, (z+1)*scale); glVertex3f(x+1, h3, z+1);
	// 		glTexCoord2f(x * scale, (z+1)*scale);   glVertex3f(x,   h4, z+1);
	// 	}
	// }
	// glEnd();
	// glBindTexture(GL_TEXTURE_2D, 0);
    // glDisable(GL_TEXTURE_2D);

	// glPopMatrix();
	
	// //===========================


	// glPushMatrix();
	// glTranslatef(100.0f, 0.0f, 0.0f);
	// // glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, textureTerrain);

	// glTranslatef(-TERRAIN_SIZE / 2.0f, -5.0f, -TERRAIN_SIZE / 2.0f); // Center and lower terrain

	// glBegin(GL_QUADS);
	// for (int z = 0; z < TERRAIN_SIZE - 1; z++) {
	// 	for (int x = 0; x < TERRAIN_SIZE - 1; x++) {
	// 		float h1 = terrain[x][z];
	// 		float h2 = terrain[x+1][z];
	// 		float h3 = terrain[x+1][z+1];
	// 		float h4 = terrain[x][z+1];

	// 		// Texture scaling factor (to control repetition)
	// 		float scale = 0.1f;

	// 		glTexCoord2f(x * scale, z * scale);     glVertex3f(x,   h1, z);
	// 		glTexCoord2f((x+1) * scale, z * scale); glVertex3f(x+1, h2, z);
	// 		glTexCoord2f((x+1) * scale, (z+1)*scale); glVertex3f(x+1, h3, z+1);
	// 		glTexCoord2f(x * scale, (z+1)*scale);   glVertex3f(x,   h4, z+1);
	// 	}
	// }
	// glEnd();
	// glBindTexture(GL_TEXTURE_2D, 0);
    // glDisable(GL_TEXTURE_2D);

	// glPopMatrix();
	// updateDirection();

	glDisable(GL_TEXTURE_2D);
    SwapBuffers(ghdc);
}



GLfloat animateTexture;

void update(void)
{
	// code
	// UpdateTexture(animateTexture);
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
	// Cleanup (optional)
for (int i = 0; i < modelCount; i++) {
    free(models[i].vertices);
    free(models[i].texCoords);
    free(models[i].normals);
    free(models[i].faces);
}

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

	// if (texture_checkerboard) {
	// 	glDeleteTextures(1, &texture_checkerboard);
	// 	texture_checkerboard = 0;
	// 	}

	if (textureSmiley) 
	{
	glDeleteTextures(1, &textureSmiley);
	textureSmiley = 0;
	}

	// Close the file
	if(gpFile)
	{
		fprintf(gpFile, "Program terminated sucessfully");
		fclose(gpFile);
		gpFile = NULL;
	}
}

void updateDirection()
{
	// pitch(up or down)
    float radYDirectionRoll = yCameraMovement * M_PI / 180.0f;
	// yaw (left or right)
    float radXDirectionRoll = xCameramovement * M_PI / 180.0f;
	// tilt sideways
	float radZDirectionRoll = zCameraMovement * M_PI / 180.0f;

	// only for up down left right 
    float base_lx = cos(radXDirectionRoll) * sin(radYDirectionRoll);
    float base_ly = sin(radXDirectionRoll);
    float base_lz = -cos(radXDirectionRoll) * cos(radYDirectionRoll);

	// tilt rotation 
	lx = base_lx * cos(radZDirectionRoll) - base_ly * sin(radZDirectionRoll);
	ly = base_lx * sin(radZDirectionRoll) + base_ly * cos(radZDirectionRoll);
	lz = base_lz;
	
	// Up vector after roll
	upX = -sin(radZDirectionRoll);
	upY =  cos(radZDirectionRoll);
	upZ = 0.0f;

}

