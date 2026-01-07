// statndard header files
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// X-Lib related headers files 
#include <X11/Xlib.h>   // x-lib API
#include <X11/Xutil.h>  // visual info related API
#include <X11/XKBlib.h> // keyboard related file

// openGL related header files
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
// macros
#define WIN_WIDTH   800
#define WIN_HEIGHT  600

// window
char windowName[] = "Xwindows : Omkar Kashid";

// global variables
Display *gpDisplay = NULL;      // it structure of 77 member structures/members
XVisualInfo *visualInfo = NULL; // it struct->struct->struct .... it gives hardware information (ex. graphics card)
Window window;                  // it is window object
Colormap colormap;

Bool bFullscreen = False;

Bool bActiveWindow = False;

// openGL related variables
GLXContext glxContext = NULL;

// Variables related to File I/O
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

int main(void) {    
    // function declarations
    void toggleFullScreen(void);
    void uninitialized(void);
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);

    // local variables
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;

    Screen *screen = NULL;
    int screen_width, screen_height;

    KeySym keySym;
    char keys[52]; // although we need 0th index  -   --  [52]

    int frameBufferAttributes[] = 
    {
        GLX_DOUBLEBUFFER, True,
        GLX_RGBA,
        GLX_RED_SIZE,
        8,
        GLX_GREEN_SIZE,
        8,
        GLX_BLUE_SIZE,
        8,
        GLX_ALPHA_SIZE,
        8,
        None
    };

    Bool bDone = False;

    // Add logs to the file
    gpFile = fopen(gszLogFileName, "w");  // r :- read, w :- write, a :- append r+ a+ 
	if(gpFile == NULL)
	{
		printf("File I/O failed...\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(gpFile, "Log file created.\n");
	}

    gpDisplay = XOpenDisplay(NULL); // XOpen with x server
    if(gpDisplay == NULL) {
        fprintf(gpFile, "XOpenDisplay failed to connect with server..!\n");
        uninitialized();
        exit(EXIT_FAILURE);
    }

    // create the default screen object with 
    defaultScreen = XDefaultScreen(gpDisplay);

    // get default depth
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // get visual info
    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, frameBufferAttributes);
    if(visualInfo == NULL) {
        fprintf(gpFile, "glXChooseVisualInfo() failed..!\n");
        uninitialized();
        exit(EXIT_FAILURE);
    }

    // set window attributes/properties of window
    memset((void*)&windowAttributes, 0, sizeof(XSetWindowAttributes));

    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen);
    windowAttributes.colormap = XCreateColormap(gpDisplay,
    XRootWindow(gpDisplay, visualInfo->screen),
    visualInfo->visual,
    AllocNone);

    colormap = windowAttributes.colormap;

    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask; // PointerMotionMask is mouse move // VisibilityChangeMask

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
        uninitialized();
        exit(EXIT_FAILURE);
    }

    // create atom for window manager to distroy the window
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // set window title
    XStoreName(gpDisplay, window, windowName);

    // mapped window to show it
    XMapWindow(gpDisplay, window);

    // centering of window
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screen_width = XWidthOfScreen(screen);
    screen_height = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screen_width/2 - WIN_WIDTH/2, screen_height/2 - WIN_HEIGHT/2);

    // initialize
    int iResult = initialize();
    if(iResult == -1) {
        uninitialized();
        exit(EXIT_FAILURE);
    }
    else {
        fprintf(gpFile, "intialize() successfull\n");
    }

    // game loop
    while(bDone == False) {
        while(XPending(gpDisplay)) {
            XNextEvent(gpDisplay, &event);
            switch (event.type) {
            case 33:
                bDone = True;
                break;
            
            case MapNotify:
                break;

            case FocusIn:
                bActiveWindow = True;
                break;

            case FocusOut:
                bActiveWindow = False;
                break;

            case ConfigureNotify:
                resize(event.xconfigure.width, event.xconfigure.height);
                break;

            case KeyPress:
                // for escape key
                keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                switch(keySym) {
                case XK_Escape:
                    bDone = True;
                    break;

                default:
                    break;
                }

                // for alphabetic keypress
                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch(keys[0]) {
                case 'F':
                case 'f':
                    if(bFullscreen == False) {
                        toggleFullScreen();
                        bFullscreen = True;
                    }
                    else {
                        toggleFullScreen();
                        bFullscreen = False;
                    }
                    break;
                    
                default:
                    break;
                }
                break;

            case ButtonPress:
                switch(event.xbutton.button) {
                case 1:
                    break;

                case 2:
                    break;

                case 3:
                    break;

                default:
                    break;
                }
                break;

            case Expose:
                break;

            default:
                break;
            }
        }

        // rendering
        if(bActiveWindow == True) {
            // display
            display();

            // update
            update();
        }
    }

    uninitialized();

    return 0;
}

// fullscreen function
void toggleFullScreen(void) {
    // code
    Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom windowManagerFullScreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent event;
    memset((void*)&event, 0, sizeof(XEvent));

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerNormalStateAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullscreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullScreenStateAtom;

    // send above event to xserver
    XSendEvent(gpDisplay, 
        XRootWindow(gpDisplay, visualInfo->screen), 
        False, 
        SubstructureNotifyMask, 
        &event);
}

int initialize(void) {
    // function declarations
	void printGLInfo(void);

    // code
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if(glxContext == NULL) {
        fprintf(gpFile, "glxCreateContext failed to create graphics context..\n");
    }
    else {
        fprintf(gpFile, "glxCreate context done");
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    // Print GL info
	printGLInfo();

    // clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return (0);
}

void printGLInfo(void) {
    // code
    // print openGL Information
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "---------x--------\n");
    fprintf(gpFile, "openGL vendor : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer : %s\n",glGetString(GL_RENDERER));
    fprintf(gpFile, "openGL version : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "openGL EXTENSIONS : %s\n", glGetString(GL_EXTENSIONS));
    fprintf(gpFile, "---------x--------\n");
}

void resize(int width, int height) {
    // code;
    if(height <= 0) {
        height = 1;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void) {
    // code
    glClear(GL_COLOR_BUFFER_BIT);

   // set matrix to model view mode
	glMatrixMode(GL_MODELVIEW);
	
	// TRIANGLE 

	// set it to idetity matrix
	glLoadIdentity();
	
	// Translet triangle backwords
	glTranslatef(-1.5f, 0.0f, -8.0f);

	//glRotatef(angleTriangle, 0.0f, 1.0f, 0.0f);

	//Triangle Drawing Code
	glBegin(GL_TRIANGLES); // Tells OpenGL that we are drawing triangles.
	
	//apex
	glColor3f(1.0f, 0.0f, 0.0f); //Sets color to Red.
	glVertex3f(0.0f, 1.0f, 0.0f); // Places a vertex at the top.
	
	//left bottom
	glColor3f(0.0f, 1.0f, 0.0f); // Sets color to Green.
	glVertex3f(-1.0f, -1.0f, 0.0f); // Places a vertex at the bottom-left.
	
	//right bottom
	glColor3f(0.0f, 0.0f, 1.0f); // Sets color to Blue.
	glVertex3f(1.0f, -1.0f, 0.0f); //Places a vertex at the bottom-right.
	
	glEnd(); // Tells OpenGL that we have finished defining the triangle.
	
	// set matrix to model view mode
	//glMatrixMode(GL_MODELVIEW);

	// RECTANGLE	

	// set it to idetity matrix
	glLoadIdentity();
	
	// Translet triangle backwords
	glTranslatef(1.5f, 0.0f, -8.0f);

	
	//glRotatef(angleRectangle, 1.0f, 0.0f, 0.0f);

	//Rectangle Drawing Code
	glBegin(GL_QUADS); // Tells OpenGL that we are drawing triangles.
	
	//apex
	glColor3f(0.0f, 0.0f, 1.0f); // Sets color to Blue.
	glVertex3f(1.0f, 1.0f, 0.0f); // Places a vertex at the top.

	glVertex3f(-1.0f, 1.0f, 0.0f); // Places a vertex at the top.
	
	//left bottom
	glVertex3f(-1.0f, -1.0f, 0.0f); // Places a vertex at the bottom-left.
	
	//right bottom
	glVertex3f(1.0f, -1.0f, 0.0f); //Places a vertex at the bottom-right.
	
	glEnd(); // Tells OpenGL that we have finished defining the triangle.
	
    
    // Swap the buffers
    glXSwapBuffers(gpDisplay, window);
}

void update(void) {
    // code
}

// uninitialized
void uninitialized(void) {
    // code
    GLXContext currentContext = glXGetCurrentContext();
    if(currentContext && currentContext == glxContext) {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    if(glxContext) {
        glXDestroyContext(gpDisplay, glxContext);
    }
    glxContext = NULL;

    if(visualInfo) {
        free(visualInfo);
        visualInfo = NULL;
    }

    if(window) {
        XDestroyWindow(gpDisplay, window);
    }

    if(colormap) {
        XFreeColormap(gpDisplay, colormap);
    }

    if(gpDisplay) {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    // Close the file
    if(gpFile) {
        fprintf(gpFile, "program terminated successfully.!\n\n");
        fclose(gpFile);
    }
}