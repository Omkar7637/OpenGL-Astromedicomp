//=============================================================================
// Project : OpenGL X11 Window - Basic Triangle Rendering
// File    : GLInfo.c
// Author  : RTR2024-150 Omkar Kashid
// Details : Demonstrates initialization of an OpenGL rendering context on
//           X11 (Linux) using GLX, with event handling and fullscreen toggling.
//=============================================================================

// Standard Header Files
#include <stdio.h>   // For printf, fprintf, file operations
#include <stdlib.h>  // For exit(), memory management
#include <memory.h>  // For memset()

// XLib Header Files (Linux Windowing System)
#include <X11/Xlib.h>    // Core X11 library for display, window management
#include <X11/Xutil.h>   // For Visual Info and utility functions
#include <X11/XKBlib.h>  // For keyboard input and key symbol mapping

// OpenGL Header Files
#include <GL/gl.h>   // Core OpenGL header
#include <GL/glx.h>  // GLX bridge between OpenGL and X11
#include <GL/glu.h>  // GLU utilities like gluPerspective()

// Custom Header File
#include "OGL.h"

// Link OpenGL Libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

//-----------------------------------------------------------------------------
// MACROS
//-----------------------------------------------------------------------------
#define WIN_WIDTH  800
#define WIN_HEIGHT 600

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
Display* gpDisplay = NULL;    // Connection to X Server
XVisualInfo* visualInfo;      // Stores information about visual format
Window window;                // X11 Window handle
Colormap colormap;            // Color mapping structure for X Window
Bool bFullScreen = False;     // Fullscreen toggle flag
Bool bActiveWindow = False;   // Tracks if window is in focus
FILE* gpFile = NULL;          // Log file pointer
GLXContext glxContext = NULL; // OpenGL rendering context

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
int initialize(void);
void resize(int, int);
void display(void);
void update(void);
void uninitialize(void);
void toggleFullScreen(void);

//=============================================================================
// Entry Point - main()
//=============================================================================
int main(void)
{
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen* screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];
    Bool bDone = False;

    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        None
    };

    // Create Log File --------------------------------------------------------
    gpFile = fopen("LogFile.txt", "w");
    if (gpFile == NULL)
    {
        printf("Log File Creation Failed!\n");
        exit(EXIT_SUCCESS);
    }
    fprintf(gpFile, "Program Started Successfully\n");

    // Open Connection to X Server --------------------------------------------
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay failed to connect with XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Get Default Screen and Depth -------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth  = XDefaultDepth(gpDisplay, defaultScreen);

    // Choose Visual Info -----------------------------------------------------
    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, framebufferAttributes);
    if (visualInfo == NULL)
    {
        fprintf(gpFile, "glXChooseVisual failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    fprintf(gpFile, "glXChooseVisual succeeded.\n");

    // Set Window Attributes --------------------------------------------------
    memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
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

    // Event Mask: defines which events the program listens for
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask |
                                  FocusChangeMask | StructureNotifyMask | ExposureMask;

    // Create Window ----------------------------------------------------------
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0, 0, WIN_WIDTH, WIN_HEIGHT,
        0, visualInfo->depth, InputOutput, visualInfo->visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
        &windowAttributes
    );

    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Handle window close event from Window Manager
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set Window Title -------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | OpenGL Triangle");

    // Map and Center Window --------------------------------------------------
    XMapWindow(gpDisplay, window);
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth  = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, (screenWidth / 2) - (WIN_WIDTH / 2), (screenHeight / 2) - (WIN_HEIGHT / 2));

    // Initialize OpenGL ------------------------------------------------------
    int iResult = initialize();
    if (iResult == -1)
    {
        fprintf(gpFile, "initialize() Failed\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    fprintf(gpFile, "initialize() Completed Successfully\n");

    // Main Event Loop --------------------------------------------------------
    while (bDone == False)
    {
        while (XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event);
            switch (event.type)
            {
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

                case 33: // Window Close (WM_DELETE_WINDOW)
                    bDone = True;
                    break;
            }
        }

        // Render if window active
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }

    // Cleanup ---------------------------------------------------------------
    uninitialize();
}

//=============================================================================
// toggleFullScreen() - Toggles Fullscreen Mode
//=============================================================================
void toggleFullScreen(void)
{
    Atom wmStateAtom  = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom fsStateAtom  = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent event;

    memset(&event, 0, sizeof(XEvent));
    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = wmStateAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = fsStateAtom;

    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        False,
        SubstructureNotifyMask,
        &event
    );
}

//=============================================================================
// initialize() - Creates GL Context and initializes state
//=============================================================================
int initialize(void)
{
    void printGLInfo(void);
    void resize(int, int);

    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glXCreateContext failed.\n");
        return -1;
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    printGLInfo();

    // Set default background color (black)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initial resize setup
    resize(WIN_WIDTH, WIN_HEIGHT);

    return 0;
}

//=============================================================================
// printGLInfo() - Displays GPU and OpenGL driver information
//=============================================================================
void printGLInfo(void)
{
    fprintf(gpFile, "==================== OpenGL Information ====================\n");
    fprintf(gpFile, "Vendor   : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "Version  : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "============================================================\n");
}

//=============================================================================
// resize() - Handles viewport and perspective setup
//=============================================================================
void resize(int width, int height)
{
    if (height <= 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

//=============================================================================
// display() - Renders a single colored triangle
//=============================================================================
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -4.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f,  1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glXSwapBuffers(gpDisplay, window);
}

//=============================================================================
// update() - Placeholder for animation or per-frame logic
//=============================================================================
void update(void)
{
    // Add animation logic if required
}

//=============================================================================
// uninitialize() - Cleanup all allocated resources
//=============================================================================
void uninitialize(void)
{
    GLXContext currentContext = glXGetCurrentContext();

    if (currentContext && currentContext == glxContext)
        glXMakeCurrent(gpDisplay, 0, 0);

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
        XDestroyWindow(gpDisplay, window);

    if (colormap)
        XFreeColormap(gpDisplay, colormap);

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    if (gpFile)
    {
        fprintf(gpFile, "Program terminated successfully.\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
