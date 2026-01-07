// Header files
#include<stdio.h>
#include<stdlib.h>  // exit()
#include<memory.h>  // for memory

// Xlib header files
#include<X11/Xlib.h>    // Similar to windows.h. For all Xlib apis
#include<X11/Xutil.h>   // VisualInfo and related apis
#include<X11/XKBlib.h>  // keyboard function related apis

// OpenGL related header files
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

#include<math.h>

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global variables
Display *gpDisplay = NULL;
XVisualInfo *visualInfo = NULL;
Window window;
Colormap colorMap;
Bool bFullscreen = False;
FILE *gpFile = NULL;
Bool bActiveWindow = False;

// OpenGL related variables
GLXContext glxContext = NULL;


// Graph related variables
GLfloat bTransitionX = -3.0f;

GLfloat hTransitionX = -3.0f;
GLfloat hTransitionY = 3.0f;

GLfloat aTransitionX = -3.0f;
GLfloat aTransitionY = -3.0f;

GLfloat rTransitionX = 3.0f;
GLfloat rTransitionY = 3.0f;

GLfloat a2TransitionX = 3.0f;
GLfloat a2TransitionY = -3.0f;

GLfloat tTransitionX = 3.0f;

GLfloat xPlaneMovement = -3.3f;
GLfloat yPlaneMovement = 0.0f;

Bool gB = True;
Bool gH = False;
Bool gA = False;
Bool gR = False;
Bool gA2 = False;
Bool gT = False;

Bool gBColor = False;
Bool gHColor = False;
Bool gAColor = False;
Bool gRColor = False;
Bool gA2Color = False;
Bool gTColor = False;

Bool planeMovementBool = False;
GLfloat xPlaneMovementCenter = 1.0f;
GLfloat yPlaneMovementUpper = 0.0f;
GLfloat xPlaneMovementRight = -1.0f;

GLfloat xAxis1 = 0.0f;
GLfloat yAxis1 = 0.0f;

GLfloat xAxis2 = 0.0f;
GLfloat yAxis2 = 0.0f;

GLfloat radius = 1.5f;
GLfloat arcIncrement1 = 180.0f;
GLfloat arcIncrement2 = 180.0f;

GLfloat arcDecrement1 = 270.0f;
GLfloat arcDecrement2 = 90.0f;

GLfloat upperPlanStraight = -1.49f;

int main(void)
{
    // Function declarations
    void toggleFullscreen(void);
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);

    
    // Variable declarations
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];  // Although, we need only 0th index, conventionally, the array size is equal to the number of alphabet, for capital or small or both

    int frameBufferAttributes[] = 
    {
        GLX_DOUBLEBUFFER, True,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
        None
    };
    Bool bDone = False;

    // Add logs to the file
	gpFile = fopen("Log.txt", "w");
	if(gpFile == NULL)
	{
		printf("File I/O failed...\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(gpFile, "Log file created.\n");
	}

    // Code
    gpDisplay = XOpenDisplay(NULL);

    // Open the connection with XServer from XClient
    if(gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay failed to connect to XServer\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Create the default screen object
    defaultScreen = XDefaultScreen(gpDisplay);

    // Get default depth
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // Get Visual info
    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, frameBufferAttributes);

    if(visualInfo == NULL)
    {
        fprintf(gpFile, "glXChooseVisual() failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "glXChooseVisual() successful.\n");
    }

    // Set window attributes/properties
    memset((void*) &windowAttributes, 0, sizeof(XSetLineAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo -> screen);
    windowAttributes.colormap = XCreateColormap(
        gpDisplay, 
        XRootWindow(gpDisplay, visualInfo -> screen),
        visualInfo -> visual,
        AllocNone);
    
    colorMap = windowAttributes.colormap;

    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // Create window
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo -> screen),
        0,
        0,
        WIN_WIDTH,
        WIN_HEIGHT,
        0,
        visualInfo -> depth,
        InputOutput,
        visualInfo -> visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
        &windowAttributes
    );

    if(!window)
    {
        fprintf(gpFile, "Window creation failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Create atom for window manager to destroy the window
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set window title
    XStoreName(gpDisplay, window, "Omkar Kashid : XWindow");

    // Map the window to show it
    XMapWindow(gpDisplay, window);

    // Centering of window
    screen = XScreenOfDisplay(gpDisplay, visualInfo -> screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth/2 - WIN_WIDTH/2, screenHeight/2 - WIN_HEIGHT/2);

    // Initialize
    int iResult = initialize();

    if(iResult == -1)
    {
        fprintf(gpFile, "Intialize() failed. \n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
	{
		fprintf(gpFile, "Initialize() completed successfully.\n");
	}

    // Game loop
    while(bDone == False)
    {
        while(XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event);
            switch(event.type)
            {
                case MapNotify:
                break;

                case FocusIn:
                    bActiveWindow = True;
                break;

                case FocusOut:
                break;

                case ConfigureNotify:
                    resize(event.xconfigure.width, event.xconfigure.height);
                break;

                case KeyPress:
                    // For escape key
                    keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                    switch(keySym)
                    {
                        case XK_Escape:
                            bDone = True;
                        break;

                        default:
                        break;
                    }

                    // For alphabetic keypress
                    XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                    switch(keys[0])
                    {
                        case 'F':
                        case 'f':
                        if(bFullscreen == False)
                        {
                            toggleFullscreen();
                            bFullscreen = True;
                        }
                        else
                        {
                            toggleFullscreen();
                            bFullscreen = False;
                        }

                        break;

                        default:
                        break;
                    }
                break;

                case ButtonPress:
                break;

                case Expose:
                break;

                case 33:
                    bDone = True;
                break;

                default:
                break;
            }
        }

        // Rendering
        if(bActiveWindow == True)
        {
            display();

            // Update
            update();
        }
    }
   
    uninitialize();
    return(0);
}

void toggleFullscreen(void)
{
    // Code
    Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False); // Network compliant atom
    Atom windowManagerFullscreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent event;
    memset((void *)&event, 0, sizeof(XEvent)); // Event che 33 members known state la anlet

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerNormalStateAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullscreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom;
    
    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo -> screen),
        False,
        SubstructureNotifyMask,
        &event);
}

int initialize(void)
{
    void resize(int, int);
    // Code
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if(glxContext == NULL)
    {
        fprintf(gpFile, "glXCreateContext() failed\n");
        return(-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    // Clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);

    glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    resize(WIN_WIDTH, WIN_HEIGHT);
    return(0);
}

void resize(int width, int height)
{
    // Code
    if(height <= 0)
    {
        height = 1;
    }

    // Set view port
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// Set to identity matrix
	glLoadIdentity();

	// Set perspective projection
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}


void display(void)
{
    // code
    // Function declaration
	void bharat();
	void planeMovementUpper();
	void planeMovementLower();
	void planeMovementCenter();

	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the color buffer and depth buffer to prepare for new drawing with glClearColor's and glClearDepth's values

	// set matrix to model view mode
	glMatrixMode(GL_MODELVIEW);

	// TRIANGLE 

	// set it to idetity matrix
	glLoadIdentity();

	// Translet triangle backwords
	glTranslatef(0.0f, 0.0f, -4.0f);
	bharat();
	//planeMovementBool = True;
	if (planeMovementBool == True)
	{
		planeMovementUpper();
		planeMovementLower();
		planeMovementCenter();

		if (upperPlanStraight <= 1.7f)
		{
			xAxis1 = -1.5f + radius * cos(arcIncrement1 * M_PI / 180);
			yAxis1 = 2.0f + radius * sin(arcIncrement1 * M_PI / 180);

			xAxis2 = -1.5f + radius * cos(arcIncrement2 * M_PI / 180);
			yAxis2 = -2.0f + radius * sin(arcIncrement2 * M_PI / 180);
		}
		else
		{
			xAxis1 = -1.5 + radius * cos(arcDecrement1 * M_PI / 180);
			yAxis1 = 2.0f + radius * sin(arcDecrement1 * M_PI / 180);

			xAxis2 = 0.0 + radius * cos(arcDecrement2 * M_PI / 180);
			yAxis2 = -2.0f + radius * sin(arcDecrement2 * M_PI / 180);
		}
	}
  
    // Swap the buffers
    glXSwapBuffers(gpDisplay, window);
}



void planeMovementLower()
{
	void createPlane();

	glPushMatrix();
	glTranslatef(xAxis2, yAxis2, 0.0f);
	glScalef(0.3f, 0.3f, 0.3f);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	createPlane();
	glPopMatrix();
}

void planeMovementUpper()
{
	void createPlane();

	glPushMatrix();
	glTranslatef(xAxis1, yAxis1, 0.0f);
	glScalef(0.3f, 0.3f, 0.3f);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	createPlane();
	glPopMatrix();
}


void planeMovementCenter()
{
	void createPlane();

	glPushMatrix();
	glTranslatef(xPlaneMovement, yPlaneMovement, 0.0f);
	glScalef(0.3f, 0.3f, 0.3f);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	createPlane();
	glPopMatrix();
}

void createPlane()
{
	void triangle(GLfloat height, GLfloat length, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], Bool rightAngleTriangle);
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color);


	// Triangle
	GLfloat height = 1.0f;
	GLfloat length = 0.3f;

	GLint colorSaffron[] = { 255, 103, 31 };
	GLint colorGreen[] = { 4, 106, 56 };
	GLint whiteColor[] = { 255, 255, 255 };

	triangle(height, length, whiteColor, colorSaffron, whiteColor, True);
	glPushMatrix();
	//glTranslatef(0.2f, 0.0f, 0.0f);
	glRotatef(180, 0.0, 1.0, 0.0f);
	triangle(height, length, whiteColor, colorGreen, whiteColor, True);
	glPopMatrix();

	height = 0.5f;
	length = 0.2f;

	glPushMatrix();
	glTranslatef(-0.38f, 0.53f, 0.0f);
	glRotatef(180, 0.0, 1.0, 0.0f);
	glRotatef(180, 1.0, 0.0, 0.0f);
	glRotatef(5, 0.0, 0.0, 1.0f);
	triangle(height, length, colorSaffron, colorSaffron, colorSaffron, True);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.38f, 0.53f, 0.0f);
	glRotatef(180, 1.0, 0.0, 0.0f);
	//glRotatef(180, 1.0, 0.0, 0.0f);
	glRotatef(5, 0.0, 0.0, 1.0f);
	triangle(height, length, colorGreen, colorGreen, colorGreen, True);
	glPopMatrix();
}


void bharat()
{
	void drawB();
	void drawH();
	void drawA();
	void drawR();
	void drawA2();
	void drawT();



	glPushMatrix();
	glTranslatef(bTransitionX, 0.0f, 0.0f);
	drawB();
	glPopMatrix();

	if (gH)
	{
		glPushMatrix();
		glTranslatef(hTransitionX, hTransitionY, 0.0f);
		drawH();
		glPopMatrix();
	}
	if (gA)
	{
		glPushMatrix();
		glTranslatef(aTransitionX, aTransitionY, 0.0f);
		drawA();
		glPopMatrix();

	}
	if (gR)
	{

		glPushMatrix();
		glTranslatef(rTransitionX, rTransitionY, 0.0f);
		drawR();
		glPopMatrix();

	}
	if (gA2)
	{
		glPushMatrix();
		glTranslatef(a2TransitionX, a2TransitionY, 0.0f);
		drawA2();
		glPopMatrix();
	}
	if (gT)
	{
		glPushMatrix();
		glTranslatef(tTransitionX, 0.0f, 0.0f);
		drawT();
		glPopMatrix();
	}
}

void drawB()
{
	void B(Bool color);

	glPushMatrix();
	glTranslatef(-2.2, 0.5f, 0.0f);
	glScalef(0.8f, 0.8f, 0.8f);
	B(gBColor);
	glPopMatrix();
}

void drawH()
{
	void H(Bool color);

	glPushMatrix();
	glTranslatef(-1.0, 0.15f, 0.0f);
	glScalef(0.65f, 0.72f, 0.8f);
	H(gHColor);
	glPopMatrix();
}
void drawA()
{
	void A(Bool color);

	glPushMatrix();
	glTranslatef(-0.5, 0.5f, 0.0f);
	glScalef(0.65f, 0.72f, 0.8f);
	A(gAColor);
	glPopMatrix();
}

void drawR()
{
	void R(Bool color);

	glPushMatrix();
	glTranslatef(0.25, 0.5f, 0.0f);
	glScalef(0.7f, 0.75f, 0.8f);
	R(gRColor);
	glPopMatrix();
}

void drawA2()
{
	void A2(Bool color);

	glPushMatrix();
	glTranslatef(1.1, 0.5f, 0.0f);
	glScalef(0.65f, 0.72f, 0.8f);
	A2(gA2Color);
	glPopMatrix();
}

void drawT()
{
	void T(Bool color);

	glPushMatrix();
	glTranslatef(1.9, 0.45f, 0.0f);
	glScalef(0.65f, 0.68f, 0.8f);
	T(gTColor);
	glPopMatrix();
}

void B(Bool gBColor)
{
	void B1(Bool color);
	void B2(Bool color);
	void verticalH(Bool color);

	B1(gBColor);
	glPushMatrix();
	glTranslatef(0.0f, -0.89f, 0.0f);
	B2(gBColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glScalef(0.9, 0.9, 1.0f);
	verticalH(gBColor);
	glPopMatrix();
}

void B1(Bool color)
{
	void hollowCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat initialColor[], GLfloat targetColor[], GLfloat lineWidth, Bool color);
	GLfloat saffronColor[] = { 1.0f, 0.4f, 0.12f };
	GLfloat lowerColorGreen[] = { 0.01, 0.41, 0.21 };
	GLfloat whiteColor[] = { 1.0f, 1.0f, 1.0f };

	//Upper Circle
	glPushMatrix();
	glRotatef(-100, 0.0f, 0.0f, 1.0f);
	hollowCircle(0.0f, 0.0f, 0.2, saffronColor, whiteColor, 30.0f, color);
	glPopMatrix();

}
void B2(Bool color)
{
	void hollowCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat initialColor[], GLfloat targetColor[], GLfloat lineWidth, Bool color);
	GLfloat saffronColor[] = { 1.0f, 0.4f, 0.12f };
	GLfloat lowerColorGreen[] = { 0.01, 0.41, 0.21 };
	GLfloat whiteColor[] = { 1.0f, 1.0f, 1.0f };


	// Upper Circle
	glPushMatrix();
	glRotatef(-100, 0.0f, 0.0f, 1.0f);
	hollowCircle(0.0f, 0.0f, 0.2, whiteColor, lowerColorGreen, 50.0f, color);
	glPopMatrix();

}

void H(Bool color)
{
	void verticalH(Bool color);
	void horizontalH(Bool color);

	glPushMatrix();

	glTranslatef(-1.0f, 0.5f, 0.0f);
	verticalH(gHColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	verticalH(gHColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5f, 0.0f, 0.0f);
	horizontalH(gHColor);
	glPopMatrix();

}

void A2(Bool color)
{
	void verticalH(Bool color);
	void horizontalA(Bool color);

	// Horizontal line
	glPushMatrix();
	glTranslatef(0.15f, -0.5f, 0.0f);
	horizontalA(gA2Color);
	glPopMatrix();

	// Left slanting line
	glPushMatrix();
	glRotatef(-15, 0.0, 0.0, 1.0);
	verticalH(gA2Color);
	glPopMatrix();

	// Right Slanting line
	glPushMatrix();
	glTranslatef(0.3f, 0.0f, 0.0);
	glRotatef(15, 0.0, 0.0, 1.0);
	verticalH(gA2Color);
	glPopMatrix();
}

void A(Bool color)
{
	void verticalH(Bool color);
	void horizontalA(Bool color);

	// Horizontal line
	glPushMatrix();
	glTranslatef(0.15f, -0.5f, 0.0f);
	horizontalA(gAColor);
	glPopMatrix();

	// Left slanting line
	glPushMatrix();
	glRotatef(-15, 0.0, 0.0, 1.0);
	verticalH(gAColor);
	glPopMatrix();

	// Right Slanting line
	glPushMatrix();
	glTranslatef(0.3f, 0.0f, 0.0);
	glRotatef(15, 0.0, 0.0, 1.0);
	verticalH(gAColor);
	glPopMatrix();
}

void R(Bool color)
{
	void B1(Bool color);
	void verticalH(Bool color);

	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color);

	// slanting line
	GLfloat height = 1.1f;
	GLfloat length = 0.15f;

	GLint lowerColorGreen[] = { 4, 106, 56 };
	GLint whiteColor[] = { 255, 255, 255 };

	glPushMatrix();
	glTranslatef(0.22f, -0.85f, 0.0f);
	glRotatef(35, 0.0f, 0.0f, 1.0f);
	quads(length, height, whiteColor, whiteColor, lowerColorGreen, lowerColorGreen, gRColor);
	glPopMatrix();

	// Upper round
	B1(gRColor);

	// Left bar
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glScalef(0.9, 0.9, 1.0f);
	verticalH(gRColor);
	glPopMatrix();



}
void T(Bool color)
{
	void verticalH(Bool color);
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color);


	verticalH(color);

	// Left vertical 
	GLfloat height = 0.2f;
	GLfloat length = 1.0f;

	GLint upperColorSaffron[] = { 255, 103, 31 };

	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	quads(length, height, upperColorSaffron, upperColorSaffron, upperColorSaffron, upperColorSaffron, color);
	glPopMatrix();
}

void horizontalA(Bool color)
{
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color);

	// Left vertical 
	GLfloat height = 0.2f;
	GLfloat length = 0.6f;

	GLint whiteColor[] = { 255, 255, 255 };

	quads(length, height, whiteColor, whiteColor, whiteColor, whiteColor, gHColor);
}

void horizontalH(Bool color)
{
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color);

	// Left vertical 
	GLfloat height = 0.2f;
	GLfloat length = 0.8f;

	GLint whiteColor[] = { 255, 255, 255 };

	quads(length, height, whiteColor, whiteColor, whiteColor, whiteColor, gHColor);
}

void verticalH(Bool color)
{
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color);

	// Left vertical 
	GLfloat height = 1.0f;
	GLfloat length = 0.2f;

	GLint upperColorSaffron[] = { 255, 103, 31 };
	GLint lowerColorGreen[] = { 4, 106, 56 };
	GLint whiteColor[] = { 255, 255, 255 };


	quads(length, height, upperColorSaffron, upperColorSaffron, whiteColor, whiteColor, color);
	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f);
	quads(length, height, whiteColor, whiteColor, lowerColorGreen, lowerColorGreen, color);
	glPopMatrix();

}

void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[], Bool color)
{
	glBegin(GL_QUADS);

	GLfloat x = length / 2;
	GLfloat y = height / 2;

	if (color)
	{
		glColor3f((float)colorVertex1[0] / 255, (float)colorVertex1[1] / 255, (float)colorVertex1[2] / 255);
		glVertex3f(x, y, 0.0f);

		glColor3f((float)colorVertex2[0] / 255, (float)colorVertex2[1] / 255, (float)colorVertex2[2] / 255);
		glVertex3f(-x, y, 0.0f);

		glColor3f((float)colorVertex3[0] / 255, (float)colorVertex3[1] / 255, (float)colorVertex3[2] / 255);
		glVertex3f(-x, -y, 0.0f);

		glColor3f((float)colorVertex4[0] / 255, (float)colorVertex4[1] / 255, (float)colorVertex4[2] / 255);
		glVertex3f(x, -y, 0.0f);
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-x, y, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-x, -y, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, -y, 0.0f);
	}

	glEnd();
}

void triangle(GLfloat height, GLfloat length, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], Bool rightAngleTriangle)
{
	GLfloat x = length / 2;
	GLfloat y = height;

	if (rightAngleTriangle)
	{
		x = length;
		glBegin(GL_TRIANGLES);

		glColor3f((float)colorVertex1[0] / 255, (float)colorVertex1[1] / 255, (float)colorVertex1[2] / 255);
		glVertex3f(0.0f, y, 0.0f);

		glColor3f((float)colorVertex2[0] / 255, (float)colorVertex2[1] / 255, (float)colorVertex2[2] / 255);
		glVertex3f(-x, 0.0f, 0.0f);

		glColor3f((float)colorVertex3[0] / 255, (float)colorVertex3[1] / 255, (float)colorVertex3[2] / 255);
		glVertex3f(0.0, 0.0f, 0.0f);

		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);

		glColor3f((float)colorVertex1[0] / 255, (float)colorVertex1[1] / 255, (float)colorVertex1[2] / 255);
		glVertex3f(x, y, 0.0f);

		glColor3f((float)colorVertex2[0] / 255, (float)colorVertex2[1] / 255, (float)colorVertex2[2] / 255);
		glVertex3f(-x, -y, 0.0f);

		glColor3f((float)colorVertex3[0] / 255, (float)colorVertex3[1] / 255, (float)colorVertex3[2] / 255);
		glVertex3f(x, -y, 0.0f);

		glEnd();
	}
}


void hollowCircle1(GLfloat cx, GLfloat cy, GLfloat radius, Bool col, GLfloat initialColor[], GLfloat targetColor[], GLfloat lineWidth)
{
	GLint loopCount = 200;
	GLfloat increment0 = ((initialColor[0] - targetColor[0]) / loopCount);
	GLfloat increment1 = ((initialColor[1] - targetColor[2]) / loopCount);
	GLfloat increment2 = ((initialColor[2] - targetColor[2]) / loopCount);

	glLineWidth(lineWidth);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
	for (GLint i = 0; i < loopCount; i++)
	{
		GLfloat theta = 3.14159f * i / 180;
		GLfloat x = cx + (1.4 * radius * cos(theta));
		GLfloat y = cy + (2.0 * radius * sin(theta));
		GLfloat x2 = cx + (1.0 * radius * cos(theta));
		GLfloat y2 = cy + (1.6 * radius * sin(theta));

		if (initialColor[0] < targetColor[0])
		{
			targetColor[0] = targetColor[0] + increment0;

		}

		if (initialColor[1] < targetColor[1])
		{
			targetColor[1] = targetColor[1] + increment1;
			//fprintf(gpFile, "initCo= %f\n", initialColor[1]);
		}
		/*else
		{
			initialColor[1] = initialColor[1] - increment1;
		}
		*/
		if (initialColor[2] < targetColor[2])
		{
			targetColor[2] = targetColor[2] + increment2;
		}
		/*else
		{
			initialColor[2] = initialColor[2] - increment2;
		}*/
		//if (col && i >= 180)
		//{
		glColor3f(targetColor[0], targetColor[1], targetColor[2]);
		//}
		glVertex2f(x, y);
		glVertex2f(x2, y2);
	}
	glEnd();
	glDisable(GL_LINE_SMOOTH);
}

void hollowCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat initialColor[], GLfloat targetColor[], GLfloat lineWidth, Bool color)
{
	GLint loopCount = 200;
	GLfloat increment0 = ((initialColor[0] - targetColor[0]) / loopCount);
	GLfloat increment1 = ((initialColor[1] - targetColor[2]) / loopCount);
	GLfloat increment2 = ((initialColor[2] - targetColor[2]) / loopCount);

	glLineWidth(lineWidth);
	glEnable(GL_LINE_SMOOTH);
	//glColor3f(initialColor[0], initialColor[1], initialColor[2]);
	glBegin(GL_LINES);
	for (GLint i = 0; i < loopCount; i++)
	{
		GLfloat theta = 3.14159f * i / 180;
		GLfloat x = cx + (2 * (radius + 0.04) * cos(theta));
		GLfloat y = cy + (2 * (radius + 0.04) * sin(theta));
		GLfloat x2 = cx + (2 * (radius - 0.04) * cos(theta));
		GLfloat y2 = cy + (2 * (radius - 0.04) * sin(theta));

		targetColor[0] = targetColor[0] + increment0;
		targetColor[1] = targetColor[1] + increment1;
		targetColor[2] = targetColor[2] + increment2;

		if (color)
		{
			glColor3f(targetColor[0], targetColor[1], targetColor[2]);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(x, y);
		glVertex2f(x2, y2);

	}
	glEnd();
	glDisable(GL_LINE_SMOOTH);
}

void drawLine(GLfloat color1[], GLfloat color2[], GLfloat vertex1[], GLfloat vertex2[], GLfloat lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	glColor3f(color1[0], color1[1], color1[2]);
	glVertex3f(vertex1[0], vertex1[1], vertex1[2]);

	glColor3f(color2[0], color2[1], color2[2]);
	glVertex3f(vertex2[0], vertex2[1], vertex2[2]);

	glEnd();
}

void update(void)
{
	// code
	GLfloat transitionSpeed = 0.007f;
	GLfloat arcTransitionSpeed = 0.1;

	// B transition
	bTransitionX = bTransitionX + transitionSpeed;

	if (bTransitionX >= 0.0)
	{
		bTransitionX = 0.0f;
		gH = True;
	}

	// H transition
	if (gH == True)
	{
		hTransitionX = hTransitionX + transitionSpeed;
		hTransitionY = hTransitionY - transitionSpeed;

		if (hTransitionX >= 0.0 && hTransitionY <= 0.0)
		{
			hTransitionX = 0.0f;
			hTransitionY = 0.0f;
			gA = True;
		}
	}

	// A transition
	if (gA == True)
	{
		aTransitionX = aTransitionX + transitionSpeed;
		aTransitionY = aTransitionY + transitionSpeed;
		if (aTransitionX >= 0.0 && aTransitionY >= 0.0)
		{
			aTransitionX = 0.0f;
			aTransitionY = 0.0f;
			gR = True;
		}
	}

	// R transition
	if (gR == True)
	{
		rTransitionX = rTransitionX - transitionSpeed;
		rTransitionY = rTransitionY - transitionSpeed;
		if (rTransitionX <= 0.0 && rTransitionY <= 0.0)
		{
			rTransitionX = 0.0f;
			rTransitionY = 0.0f;
			gA2 = True;
		}
	}

	// A2 transition
	if (gA2 == True)
	{
		a2TransitionX = a2TransitionX - transitionSpeed;
		a2TransitionY = a2TransitionY + transitionSpeed;
		if (a2TransitionX <= 0.0 && a2TransitionY >= 0.0)
		{
			a2TransitionX = 0.0f;
			a2TransitionY = 0.0f;
			gT = True;
		}
	}

	// T transition
	if (gT == True)
	{
		tTransitionX = tTransitionX - transitionSpeed;
		if (tTransitionX <= 0.0)
		{
			tTransitionX = 0.0f;
			planeMovementBool = True;
		}
	}

	if (planeMovementBool == True)
	{
		xPlaneMovement = xPlaneMovement + 0.002;

		// First round
		if (arcIncrement1 <= 270.0)
		{
			arcIncrement1 = arcIncrement1 + arcTransitionSpeed;
		}
		if (arcIncrement2 >= 90.0)
		{
			//fprintf(gpFile, "%f\n", arcIncrement);
			arcIncrement2 = arcIncrement2 - arcTransitionSpeed;
		}

		// Straight
		if (arcIncrement1 >= 270.0)
		{
			upperPlanStraight = upperPlanStraight + 0.002;
			xAxis1 = upperPlanStraight;
			xAxis2 = upperPlanStraight;

		}

		// Second round
		if (upperPlanStraight >= 1.7f && arcDecrement1 <= 360.0)
		{
			arcDecrement1 = arcDecrement1 + arcTransitionSpeed;
			//fprintf(gpFile, "%f\n", arcDecrement1);
		}
		if (upperPlanStraight >= 1.7f && arcDecrement2 >= 0.0)
		{
			arcDecrement2 = arcDecrement2 - arcTransitionSpeed;
		}


		// Color changing logic
		//fprintf(gpFile, "xPlaneMovement= %f\n", xPlaneMovement);
		if (xPlaneMovement >= -2.0 && xPlaneMovement <= -1.0)
		{
			gBColor = True;
		}

		if (xPlaneMovement >= -1.0 && xPlaneMovement <= 0.0)
		{
			gHColor = True;
		}

		if (xPlaneMovement >= 0.0 && xPlaneMovement <= 0.6)
		{
			gAColor = True;
		}

		if (xPlaneMovement >= 0.6 && xPlaneMovement <= 1.2)
		{
			gRColor = True;
		}

		if (xPlaneMovement >= 1.2 && xPlaneMovement <= 1.8)
		{
			gA2Color = True;
		}

		if (xPlaneMovement >= 1.8 && xPlaneMovement <= 2.4)
		{
			gTColor = True;
		}
	}


	//	/*if (xPlaneMovementUpper >= 7.0)
	//	{
	//		yPlaneMovementUpper = -6 / xPlaneMovementUpper;
	//		fprintf(gpFile, "yPlaneMovementUpper=%f\n", yPlaneMovementUpper);
	//	}*/
	//}
}


void uninitialize(void)
{
    // Code
    GLXContext currentContext = glXGetCurrentContext();

    if(currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    if(glxContext)
    {
        glXDestroyContext(gpDisplay, glxContext);
        glxContext = NULL;
    }

    if(visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    if(window)
    {
        XDestroyWindow(gpDisplay, window);
    }

    if(colorMap)
    {
        XFreeColormap(gpDisplay, colorMap);
    }
    

    if(gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

}
