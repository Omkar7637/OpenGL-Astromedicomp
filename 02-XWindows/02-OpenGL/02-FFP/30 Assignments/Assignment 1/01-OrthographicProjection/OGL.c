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

float identityMatrix[16];
float orthographicMatrix[16];

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

    // Define matrix - Identity
	// Column 1
	identityMatrix[0] = 1.0f;
	identityMatrix[1] = 0.0f;
	identityMatrix[2] = 0.0f;
	identityMatrix[3] = 0.0f;

	// Column 2
	identityMatrix[4] = 0.0f;
	identityMatrix[5] = 1.0f;
	identityMatrix[6] = 0.0f;
	identityMatrix[7] = 0.0f;

	// Column 3
	identityMatrix[8] = 0.0f;
	identityMatrix[9] = 0.0f;
	identityMatrix[10] = 1.0f;
	identityMatrix[11] = 0.0f;

	// Column 4
	identityMatrix[12] = 0.0f;
	identityMatrix[13] = 0.0f;
	identityMatrix[14] = 0.0f;
	identityMatrix[15] = 1.0f;


	// Define matrix - Orthographic
	// Column 1
	orthographicMatrix[0] = 0.0f;
	orthographicMatrix[1] = 0.0f;
	orthographicMatrix[2] = 0.0f;
	orthographicMatrix[3] = 0.0f;

	// Column 2
	orthographicMatrix[4] = 0.0f;
	orthographicMatrix[5] = 0.0f;
	orthographicMatrix[6] = 0.0f;
	orthographicMatrix[7] = 0.0f;

	// Column 3
	orthographicMatrix[8] = 0.0f;
	orthographicMatrix[9] = 0.0f;
	orthographicMatrix[10] = 0.0f;
	orthographicMatrix[11] = 0.0f;

	// Column 4
	orthographicMatrix[12] = 0.0f;
	orthographicMatrix[13] = 0.0f;
	orthographicMatrix[14] = 0.0f;
	orthographicMatrix[15] = 0.0f;


    resize(WIN_WIDTH, WIN_HEIGHT);
    return(0);
}

void resize(int width, int height)
{
    // Code
	// If height by accident, then make height value to 1
	if (height <= 0)
	{
		height = 1;
	}

	// Set the view port
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// Set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// Set to identity matrix
	glLoadIdentity();

	// Do orthographic projection
	if (width <= height)
	{
		// Define matrix - Orthographics
	// Column 1
		orthographicMatrix[0] = 2 / (100.0f - (-100.0f));
		orthographicMatrix[1] = 0.0f;
		orthographicMatrix[2] = 0.0f;
		orthographicMatrix[3] = 0.0f;

		// Column 2
		orthographicMatrix[4] = 0.0f;
		orthographicMatrix[5] = 2 / (100.0f * ((GLfloat)height / (GLfloat)width)) - (-100.0f * ((GLfloat)height / (GLfloat)width));
		orthographicMatrix[6] = 0.0f;
		orthographicMatrix[7] = 0.0f;

		// Column 3
		orthographicMatrix[8] = 0.0f;
		orthographicMatrix[9] = 0.0f;
		orthographicMatrix[10] = -2 / (100.0f - (-100.0f));
		orthographicMatrix[11] = 0.0f;

		// Column 4
		orthographicMatrix[12] = -((100.0f + (-100.0f)) / (100.0f - (-100.0f)));
		orthographicMatrix[13] = -(((100.0f * ((GLfloat)height / (GLfloat)width)) + (-100.0f * ((GLfloat)height / (GLfloat)width))) / ((100.0f * ((GLfloat)height / (GLfloat)width)) - (-100.0f * ((GLfloat)height / (GLfloat)width))));
		orthographicMatrix[14] = -((100.0f + (-100.0f)) / (100.0f - (-100.0f)));
		orthographicMatrix[15] = 1.0f;

		glMultMatrixf(orthographicMatrix);
	}
	else
	{
		// Define matrix - Orthographic
	// Column 1
		orthographicMatrix[0] = 2/((-100.0f * ((GLfloat)width / (GLfloat)height)) - (100.0f * ((GLfloat)width / (GLfloat)height)));
		orthographicMatrix[1] = 0.0f;
		orthographicMatrix[2] = 0.0f;
		orthographicMatrix[3] = 0.0f;

		// Column 2
		orthographicMatrix[4] = 0.0f;
		orthographicMatrix[5] = 2/(100.0f - (-100.0f));
		orthographicMatrix[6] = 0.0f;
		orthographicMatrix[7] = 0.0f;

		// Column 3
		orthographicMatrix[8] = 0.0f;
		orthographicMatrix[9] = 0.0f;
		orthographicMatrix[10] = -2 / (100.0f - (-100.f));
		orthographicMatrix[11] = 0.0f;

		// Column 4
		orthographicMatrix[12] = -(
			(100.0f * ((GLfloat)width / (GLfloat)height)) + (-100.0f * ((GLfloat)width / (GLfloat)height))
			) / (
				(100.0f * ((GLfloat)width / (GLfloat)height)) - (-100.0f * ((GLfloat)width / (GLfloat)height))
				);
		orthographicMatrix[13] = -(100.0f + (-100.0f)) / (100.0f - (-100.0f));
		orthographicMatrix[14] = -(100.0f + (-100.0f)) / (100.0f - (-100.0f));
		orthographicMatrix[15] = 1.0f;

		glMultMatrixf(orthographicMatrix);
	}
}


void display(void)
{
    // Code
    glClear(GL_COLOR_BUFFER_BIT );

	// Draw triangle
	glBegin(GL_TRIANGLES);

	// Apex
	glVertex3f(0.0f, 50.0f, 0.0f);

	// Left bottom
	glVertex3f(-50.0f, -50.0f, 0.0f);

	// Right bottom
	glVertex3f(50.0f, -50.0f, 0.0f);

	glEnd();
    
    // Swap the buffers
    glXSwapBuffers(gpDisplay, window);
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
