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

// Maths related header
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
	void drawPoint();
	void drawHVerticalGreenLine();
	void drawVerticalBlueLine();
	void drawHorizontalRedLine();
	void drawHorizontalBlueLine();
    void hollowCircle(float, float, float);

	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the color buffer and depth buffer to prepare for new drawing with glClearColor's and glClearDepth's values

	// set matrix to model view mode
	glMatrixMode(GL_MODELVIEW);

	// TRIANGLE 

	// set it to idetity matrix
	glLoadIdentity();

	// Translet triangle backwords
	glTranslatef(0.0f, 0.0f, -4.0f);

	drawPoint();
    drawVerticalBlueLine();
	drawHorizontalBlueLine();
	drawHVerticalGreenLine();
	drawHorizontalRedLine();
    hollowCircle(0.0f, 0.0f, 0.5f);
  
    // Swap the buffers
    glXSwapBuffers(gpDisplay, window);
}

void hollowCircle(float cx, float cy, float radius)
{
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < 360; i++) {
		float theta = i * 3.14159f / 180.0f;
		float x = cx + (radius * cos(theta));
		float y = cy + (radius * sin(theta));
		glVertex2f(x, y);
		fprintf(gpFile, "%f, %f\n", x, y);
	}
	glEnd();
}

void drawVerticalBlueLine()
{
	glLineWidth(3.0f);
	glColor3f(0.0f, 0.0f, 1.0f);

	glPushMatrix();

	GLfloat i = -2.1f;


	for (GLint count = -20; count <= 20; count++)
	{
		i = i + 0.1f;

		if (count == 0)
		{

		}
		else if (count % 5 == 0)
		{

			fprintf(gpFile, "Count = %d\n", count);
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex3f(i, -2.1f, 0.0f);
			glVertex3f(i, 2.1f, 0.0f);
			glEnd();
		}
		else
		{
			glLineWidth(3.0f);
			glBegin(GL_LINES);
			glVertex3f(i, -2.1f, 0.0f);
			glVertex3f(i, 2.1f, 0.0f);
			glEnd();
		}

	}
	glPopMatrix();
}

void drawHVerticalGreenLine()
{
	void drawLine(GLfloat color1[], GLfloat color2[], GLfloat vertex1[], GLfloat vertex2[], GLfloat lineWidth);

	GLfloat lineColor[] = {0.0f, 1.0f, 0.0f};
	GLfloat lineVertex1[] = { 0.0f, 2.1f, 0.0f };
	GLfloat lineVertex2[] = { 0.0f, -2.1f, 0.0f };

	GLfloat lineWidth = 5.0f;

	drawLine(lineColor, lineColor, lineVertex1, lineVertex2, lineWidth);
}

void drawHorizontalBlueLine()
{
	
	glColor3f(0.0f, 0.0f, 1.0f);

	glPushMatrix();

	GLfloat i = -2.1f;

	
	for (GLint count = -20; count <= 20; count++)
	{
		i = i + 0.1f;

		if (count == 0)
		{

		}
		else if (count % 5 == 0)
		{

			fprintf(gpFile, "Count = %d\n", count);
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex3f(-2.1f, i, 0.0f);
			glVertex3f(2.1f, i, 0.0f);
			glEnd();
		}
		else
		{
			glLineWidth(3.0f);
			glBegin(GL_LINES);
			glVertex3f(-2.1f, i, 0.0f);
			glVertex3f(2.1f, i, 0.0f);
			glEnd();
		}
		
	}
	glPopMatrix();
}

void drawHorizontalRedLine()
{
	void drawLine(GLfloat color1[], GLfloat color2[], GLfloat vertex1[], GLfloat vertex2[], GLfloat lineWidth);

	GLfloat lineColor[] = { 1.0f, 0.0f, 0.0f };
	GLfloat lineVertex1[] = { 2.1f, 0.0f, 0.0f };
	GLfloat lineVertex2[] = { -2.1f, 0.0f, 0.0f };

	GLfloat lineWidth = 5.0f;

	drawLine(lineColor, lineColor, lineVertex1, lineVertex2, lineWidth);
}

void drawLine(GLfloat color1[], GLfloat color2[], GLfloat vertex1[], GLfloat vertex2[], GLfloat lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glColor3f(color1[0], color1[1], color1[2]);
	glVertex3f(vertex1[0], vertex1[1], vertex1[2]);

	glColor3f(color2[0], color2[1], color2[2]);
	glVertex3f(vertex2[0], vertex2[1], vertex2[2]);

	glEnd();
}

void drawPoint()
{
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}

void update(void)
{
    // Code

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
