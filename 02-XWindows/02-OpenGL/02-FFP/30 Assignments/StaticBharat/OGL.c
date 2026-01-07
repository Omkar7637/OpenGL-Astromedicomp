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

GLfloat angleCircle = 0.0f;

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
  
    // Swap the buffers
    glXSwapBuffers(gpDisplay, window);
}


void bharat()
{
	void B();
	void H();
	void A();
	void R();
	void T();

	// B
	glPushMatrix();
	glTranslatef(-2.2, 0.5f, 0.0f);
	glScalef(0.8f, 0.8f, 0.8f);
	B();
	glPopMatrix();

	// H
	glPushMatrix();
	glTranslatef(-1.0, 0.15f, 0.0f);
	glScalef(0.65f, 0.72f, 0.8f);
	H();
	glPopMatrix();

	// A
	glPushMatrix();
	glTranslatef(-0.5, 0.5f, 0.0f);
	glScalef(0.65f, 0.72f, 0.8f);
	A();
	glPopMatrix();

	// R
	glPushMatrix();
	glTranslatef(0.25, 0.5f, 0.0f);
	glScalef(0.7f, 0.75f, 0.8f);
	R();
	glPopMatrix();

	// A
	glPushMatrix();
	glTranslatef(1.1, 0.5f, 0.0f);
	glScalef(0.65f, 0.72f, 0.8f);
	A();
	glPopMatrix();

	// T
	glPushMatrix();
	glTranslatef(1.9, 0.45f, 0.0f);
	glScalef(0.65f, 0.68f, 0.8f);
	T();
	glPopMatrix();
}


void B()
{
	void B1();
	void B2();
	void verticalH();

	B1();
	glPushMatrix();
	glTranslatef(0.0f, -0.89f, 0.0f);
	B2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glScalef(0.9, 0.9, 1.0f);
	verticalH();
	glPopMatrix();
}

void B1()
{
	void hollowCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat initialColor1[], GLfloat targetColor[], GLfloat lineWidth);
	GLfloat saffronColor[] = { 1.0f, 0.4f, 0.12f };
	GLfloat lowerColorGreen[] = { 0.01, 0.41, 0.21 };
	GLfloat whiteColor[] = { 1.0f, 1.0f, 1.0f };

	 //Upper Circle
	glPushMatrix();
	glRotatef(-100, 0.0f, 0.0f, 1.0f);
		hollowCircle(0.0f, 0.0f, 0.2, saffronColor, whiteColor, 30.0f);
	glPopMatrix(); 

}
void B2()
{
	void hollowCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat initialColor1[], GLfloat targetColor[], GLfloat lineWidth);
	GLfloat saffronColor[] = { 1.0f, 0.4f, 0.12f };
	GLfloat lowerColorGreen[] = { 0.01, 0.41, 0.21 };
	GLfloat whiteColor[] = { 1.0f, 1.0f, 1.0f };
	
	
	// Upper Circle
	glPushMatrix();
	glRotatef(-100, 0.0f, 0.0f, 1.0f);
		hollowCircle(0.0f, 0.0f, 0.2, whiteColor, lowerColorGreen, 50.0f);
	glPopMatrix();

}

void H()
{
	void verticalH();
	void horizontalH();

	glPushMatrix();

	glTranslatef(-1.0f, 0.5f, 0.0f);
	verticalH();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	verticalH();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5f, 0.0f, 0.0f);
	horizontalH();
	glPopMatrix();

}

void A()
{
	void verticalH();
	void horizontalA();

	// Horizontal line
	glPushMatrix();
	glTranslatef(0.15f, -0.5f, 0.0f);
	horizontalA();
	glPopMatrix();

	// Left slanting line
	glPushMatrix();
	glRotatef(-15, 0.0, 0.0, 1.0);
	verticalH();
	glPopMatrix();

	// Right Slanting line
	glPushMatrix();
	glTranslatef(0.3f, 0.0f, 0.0);
	glRotatef(15, 0.0, 0.0, 1.0);
	verticalH();
	glPopMatrix();


}

void R()
{
	void B1();
	void verticalH();

	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[]);

	// slanting line
	GLfloat height = 1.1f;
	GLfloat length = 0.15f;

	GLint lowerColorGreen[] = { 4, 106, 56 };
	GLint whiteColor[] = { 255, 255, 255 };

	glPushMatrix();
	glTranslatef(0.22f, -0.85f, 0.0f);
	glRotatef(35, 0.0f, 0.0f, 1.0f);
	quads(length, height, whiteColor, whiteColor, lowerColorGreen, lowerColorGreen);
	glPopMatrix();

	// Upper round
	B1();

	// Left bar
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glScalef(0.9, 0.9, 1.0f);
	verticalH();
	glPopMatrix();


	
}
void T()
{
	void verticalH();
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[]);


	verticalH();

	// Left vertical 
	GLfloat height = 0.2f;
	GLfloat length = 1.0f;

	GLint upperColorSaffron[] = { 255, 103, 31 };

	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	quads(length, height, upperColorSaffron, upperColorSaffron, upperColorSaffron, upperColorSaffron);
	glPopMatrix();
}



void horizontalA()
{
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[]);

	// Left vertical 
	GLfloat height = 0.2f;
	GLfloat length = 0.6f;

	GLint whiteColor[] = { 255, 255, 255 };

	quads(length, height, whiteColor, whiteColor, whiteColor, whiteColor);
}

void horizontalH()
{
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[]);

	// Left vertical 
	GLfloat height = 0.2f;
	GLfloat length = 0.8f;

	GLint whiteColor[] = { 255, 255, 255 };

	quads(length, height, whiteColor, whiteColor, whiteColor, whiteColor);
}

void verticalH()
{
	void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[]);

	// Left vertical 
	GLfloat height = 1.0f;
	GLfloat length = 0.2f;

	GLint upperColorSaffron[] = { 255, 103, 31 };
	GLint lowerColorGreen[] = { 4, 106, 56 };
	GLint whiteColor[] = { 255, 255, 255 };


	quads(length, height, upperColorSaffron, upperColorSaffron, whiteColor, whiteColor);
	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f);
	quads(length, height, whiteColor, whiteColor, lowerColorGreen, lowerColorGreen);
	glPopMatrix();

}


void quads(GLfloat length, GLfloat height, GLint colorVertex1[], GLint colorVertex2[], GLint colorVertex3[], GLint colorVertex4[])
{
	glBegin(GL_QUADS);

	GLfloat x = length / 2;
	GLfloat y = height / 2;

	// Front face

	glColor3f((float)colorVertex1[0] / 255, (float)colorVertex1[1] / 255, (float)colorVertex1[2] / 255);
	glVertex3f(x, y, 0.0f);

	glColor3f((float)colorVertex2[0] / 255, (float)colorVertex2[1] / 255, (float)colorVertex2[2] / 255);
	glVertex3f(-x, y, 0.0f);

	glColor3f((float)colorVertex3[0] / 255, (float)colorVertex3[1] / 255, (float)colorVertex3[2] / 255);
	glVertex3f(-x, -y, 0.0f);

	glColor3f((float)colorVertex4[0] / 255, (float)colorVertex4[1] / 255, (float)colorVertex4[2] / 255);
	glVertex3f(x, -y, 0.0f);

	glEnd();
}

void hollowCircle1(GLfloat cx, GLfloat cy, GLfloat radius, Bool col, GLfloat initialColor[], GLfloat targetColor[], GLfloat lineWidth)
{
	GLint loopCount = 200;
	GLfloat increment0 = ((initialColor[0] - targetColor[0]) / loopCount);
	GLfloat increment1 = ((initialColor[1] - targetColor[2]) / loopCount);
	GLfloat increment2 = ((initialColor[2] - targetColor[2]) / loopCount);

	fprintf(gpFile, "increment1=%f\n", increment1);
	//fprintf(gpFile, "increment2=%f\n", increment2);*/

	glLineWidth(lineWidth);
	glEnable(GL_LINE_SMOOTH);
	//glColor3f(initialColor[0], initialColor[1], initialColor[2]);
	glBegin(GL_LINE_LOOP);
	for (GLint i = 0; i < loopCount; i++)
	{
		GLfloat theta = 3.14159f * i / 180;
		GLfloat x = cx + (1.2 * radius * cos(theta));
		GLfloat y = cy + (1.8 * radius * sin(theta));

		/*if ((GLint)initialColor[0] == (GLint)targetColor[0])
		{
			initialColor[0] = targetColor[0];
		}
		if ((GLint)initialColor[1] >= (GLint)targetColor[1])
		{
			initialColor[1] = targetColor[1];
		}
		if ((GLint)initialColor[2] >= (GLint)targetColor[2])
		{
			initialColor[2] = targetColor[2];
		}*/


		if (initialColor[0] < targetColor[0])
		{
			targetColor[0] = targetColor[0] + increment0;

		}
		/*else
		{
			initialColor[0] = initialColor[0] - increment0;
		}*/

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
	}
	glEnd();
	glDisable(GL_LINE_SMOOTH);
}



void hollowCircle(GLfloat cx, GLfloat cy, GLfloat radius, GLfloat initialColor[], GLfloat targetColor[], GLfloat lineWidth)
{
	GLint loopCount = 200;
	GLfloat increment0 = ((initialColor[0] - targetColor[0]) / loopCount);
	GLfloat increment1 = ((initialColor[1] - targetColor[2]) / loopCount);
	GLfloat increment2 = ((initialColor[2] - targetColor[2]) / loopCount);

	fprintf(gpFile, "increment1=%f\n", increment2);
	//fprintf(gpFile, "increment2=%f\n", increment2);*/

	glLineWidth(lineWidth);
	glEnable(GL_LINE_SMOOTH);
	//glColor3f(initialColor[0], initialColor[1], initialColor[2]);
	glBegin(GL_LINE_LOOP);
	for (GLint i = 0; i < loopCount; i++)
	{
		GLfloat theta = 3.14159f * i / 180;
		GLfloat x = cx + (2 * radius * cos(theta));
		GLfloat y = cy + (2 * radius * sin(theta));


		//if (initialColor[0] <= targetColor[0])
		{
			targetColor[0] = targetColor[0] + increment0;

		}

		//if (initialColor[1] <= targetColor[1])
		{
			targetColor[1] = targetColor[1] + increment1;
		}
		
		//if (initialColor[2] <= targetColor[2])
		{
			targetColor[2] = targetColor[2] + increment2;
		}
		
		glColor3f(targetColor[0], targetColor[1], targetColor[2]);
		glVertex2f(x, y);
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
	GLfloat spinningSpeed = 0.05f;

	// Angle Circle
	angleCircle = angleCircle + spinningSpeed;

	if (angleCircle >= 360.0f)
	{
		angleCircle = angleCircle - 360.0f;
	}


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
