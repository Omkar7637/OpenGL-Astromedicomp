// RTR2024-150 Omkar Kashid
// ****************************************************
// OpenGL Fixed Function Pipeline Example using Xlib
// ****************************************************
// Description :
// This program demonstrates the initialization of an X11 window
// integrated with OpenGL context creation using GLX. It renders
// a simple colored triangle and displays basic OpenGL information.
//
// Platform    : Linux (X11 + GLX)
// Libraries   : Xlib, OpenGL, GLU
// ****************************************************

#include <stdio.h>      // For printf, fprintf
#include <stdlib.h>     // For exit()
#include <memory.h>     // For memset

// XLib related header files
#include <X11/Xlib.h>   
#include <X11/Xutil.h>  
#include <X11/XKBlib.h> 

// OpenGL related header files
#include <GL/gl.h>      
#include <GL/glx.h>     
#include <GL/glu.h>     

// Custom Header
#include "OGL.h"

// OpenGL libraries
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"GLU32.lib")

// Window Dimensions
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL;     
XVisualInfo *visualInfo;       
Window window;                 
Colormap colormap;             
Bool bFullScreen = False;      
FILE *gpFile = NULL;           
Bool bActiveWindow = False;    
GLXContext glxContext = NULL;  

int main(void)
{
    // Function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);
    void toggleFullScreen(void);

    // Variable declarations
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];
    Bool bDone = False;

    // Framebuffer attributes (similar to PixelFormat in Win32)
    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        None
    };

    // Create Log File
    gpFile = fopen("Log File", "w");
    if (gpFile == NULL)
    {
        printf("Log File Creation Failed!!\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        fprintf(gpFile, "Program Started Successfully\n");
    }

    // Step 1: Open connection with X Server
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay failed to connect with XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Obtain default screen and depth information
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // Choose a visual compatible with our framebuffer attributes
    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, framebufferAttributes);
    if (visualInfo == NULL)
    {
        fprintf(gpFile, "glXChooseVisual failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "glXChooseVisual succeeded.\n");
    }

    // Configure window attributes
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen);
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        visualInfo->visual,
        AllocNone
    );
    colormap = windowAttributes.colormap;

    // Register required event masks
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // Create window
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0, 0,
        WIN_WIDTH, WIN_HEIGHT,
        0,
        visualInfo->depth,
        InputOutput,
        visualInfo->visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
        &windowAttributes
    );

    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Create atom for window close event
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set Window Title
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | OpenGL - Triangle");

    // Map window to display
    XMapWindow(gpDisplay, window);

    // Center window on screen
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, (screenWidth / 2 - WIN_WIDTH / 2), (screenHeight / 2 - WIN_HEIGHT / 2));

    // Initialize OpenGL
    int iResult = initialize();
    if (iResult == -1)
    {
        fprintf(gpFile, "initialize() Failed\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "initialize() Completed Successfully\n");
    }

    // Main Event Loop
    while (bDone == False)
    {
        while (XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event);
            switch (event.type)
            {
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
                keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                switch (keySym)
                {
                case XK_Escape:
                    bDone = True;
                    break;
                default:
                    break;
                }

                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch (keys[0])
                {
                case 'F':
                case 'f':
                    toggleFullScreen();
                    bFullScreen = !bFullScreen;
                    break;
                default:
                    break;
                }
                break;

            case 33:
                bDone = True;
                break;

            default:
                break;
            }
        }

        // Render frame if active
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }

    uninitialize();
}

// ****************************************************
// Toggle Fullscreen Mode
// ****************************************************
void toggleFullScreen(void)
{
    Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom windowManagerFullscreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent event;
    memset((void *)&event, 0, sizeof(XEvent));

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerNormalStateAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom;

    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        False,
        SubstructureNotifyMask,
        &event
    );
}

// ****************************************************
// Initialize OpenGL
// ****************************************************
int initialize(void)
{
    void printGLInfo();
    void resize(int, int);

    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    // Print OpenGL details
    printGLInfo();

    // Set clear color (background)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initial Resize
    resize(WIN_WIDTH, WIN_HEIGHT);

    return (0);
}

// ****************************************************
// Print OpenGL Vendor, Renderer, and Version Info
// ****************************************************
void printGLInfo(void)
{
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version  : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}

// ****************************************************
// Resize Callback - Adjusts viewport and projection
// ****************************************************
void resize(int width, int height)
{
    if (height <= 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

// ****************************************************
// Display Function - Renders the Triangle
// ****************************************************
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -4.0f);

    glBegin(GL_TRIANGLES);
    // Apex
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    // Left bottom
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    // Right bottom
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glXSwapBuffers(gpDisplay, window);
}

// ****************************************************
// Update Function (Animation Logic Placeholder)
// ****************************************************
void update(void)
{
    // Currently empty
}

// ****************************************************
// Cleanup Resources and Close Application
// ****************************************************
void uninitialize(void)
{
    GLXContext currentContext = glXGetCurrentContext();
    if (currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    if (glxContext)
    {
        glXDestroyContext(gpDisplay, glxContext);
        glxContext = NULL;
    }

    if (visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    if (window)
    {
        XDestroyWindow(gpDisplay, window);
    }

    if (colormap)
    {
        XFreeColormap(gpDisplay, colormap);
    }

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    if (gpFile)
    {
        fprintf(gpFile, "Program terminated Successfully!!!");
        fclose(gpFile);
        gpFile = NULL;
    }
}
