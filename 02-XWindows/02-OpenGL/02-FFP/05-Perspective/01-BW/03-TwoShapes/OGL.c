// RTR2024-150 Omkar Kashid
// ============================================================================
// OpenGL Perspective Rectangle and Triangle Rendering using XLib and GLX
// ============================================================================
// Description:
// This program demonstrates how to create an X11 window and render simple
// geometric objects (a triangle and a rectangle) using OpenGL. It sets up
// the GLX context, handles window events, manages fullscreen toggling,
// and performs basic rendering in a 3D perspective projection.
//
// Author: Omkar Ankush Kashid
// Roll No: RTR2024-150
// ============================================================================

// Standard Header Files
#include <stdio.h>   // For printf and fprintf
#include <stdlib.h>  // For exit()
#include <memory.h>  // For memset()

// XLib Header Files (for X Window System programming)
#include <X11/Xlib.h>    // Core X11 APIs for window creation and event handling
#include <X11/Xutil.h>   // For XVisualInfo and related structures
#include <X11/XKBlib.h>  // For keyboard handling using XKB extension

// OpenGL Header Files
#include <GL/gl.h>   // Core OpenGL functions
#include <GL/glx.h>  // GLX functions for bridging X Window with OpenGL
#include <GL/glu.h>  // GLU library for higher-level utility functions

// Custom Header File
#include "OGL.h"

// OpenGL Libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

// ============================================================================
// Macros
// ============================================================================
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// ============================================================================
// Global Variables
// ============================================================================
Display *gpDisplay = NULL;    // Connection to the X server
XVisualInfo *visualInfo;      // Stores visual information for OpenGL context
Window window;                // The actual X11 window handle
Colormap colormap;            // Colormap for managing colors in X11
Bool bFullScreen = False;     // Fullscreen toggle flag
FILE *gpFile = NULL;          // Log file pointer
Bool bActiveWindow = False;   // Indicates whether window is in focus
GLXContext glxContext = NULL; // OpenGL rendering context

// ============================================================================
// Entry Point
// ============================================================================
int main(void)
{
    // Function Declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);
    void toggleFullScreen(void);

    // Variable Declarations
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];
    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        None
    };
    Bool bDone = False;

    // ========================================================================
    // Log File Creation
    // ========================================================================
    gpFile = fopen("Log File", "w");
    if (gpFile == NULL)
    {
        printf("Log File Creation Failed!\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        fprintf(gpFile, "Program Started Successfully\n");
    }

    // ========================================================================
    // Open Connection to X Server
    // ========================================================================
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "Failed to connect to X Server.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ========================================================================
    // Get Default Screen and Depth
    // ========================================================================
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // ========================================================================
    // Choose Visual Info for OpenGL Rendering
    // ========================================================================
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

    // ========================================================================
    // Set Window Attributes
    // ========================================================================
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
    windowAttributes.event_mask =
        KeyPressMask | ButtonPressMask | FocusChangeMask |
        StructureNotifyMask | ExposureMask;

    // ========================================================================
    // Create X11 Window
    // ========================================================================
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0, 0,
        WIN_WIDTH,
        WIN_HEIGHT,
        0,
        visualInfo->depth,
        InputOutput,
        visualInfo->visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
        &windowAttributes);

    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ========================================================================
    // Window Manager Setup
    // ========================================================================
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set window title
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | BW - Rectangle");

    // Map window to screen
    XMapWindow(gpDisplay, window);

    // Center the window on screen
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

    // ========================================================================
    // Initialize OpenGL
    // ========================================================================
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

    // ========================================================================
    // Game Loop (Event Handling and Rendering)
    // ========================================================================
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
                    if (bFullScreen == False)
                    {
                        toggleFullScreen();
                        bFullScreen = True;
                    }
                    else
                    {
                        toggleFullScreen();
                        bFullScreen = False;
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

        // Render if window is active
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }

    uninitialize();
}

// ============================================================================
// Function: toggleFullScreen
// Purpose : Toggles the window between fullscreen and windowed mode.
// ============================================================================
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
        &event);
}

// ============================================================================
// Function: initialize
// Purpose : Creates OpenGL context and initializes basic rendering settings.
// ============================================================================
int initialize(void)
{
    void printGLInfo();
    void resize(int, int);

    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed.\n");
        return (-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    // Print OpenGL information
    printGLInfo();

    // Set clear color (black background)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Perform warm-up resize
    resize(WIN_WIDTH, WIN_HEIGHT);

    return (0);
}

// ============================================================================
// Function: printGLInfo
// Purpose : Prints OpenGL environment and renderer details to log file.
// ============================================================================
void printGLInfo(void)
{
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version  : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}

// ============================================================================
// Function: resize
// Purpose : Handles window resizing and maintains proper aspect ratio.
// ============================================================================
void resize(int width, int height)
{
    if (height <= 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

// ============================================================================
// Function: display
// Purpose : Renders a triangle and rectangle with proper transformations.
// ============================================================================
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Render Triangle
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);

    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    // Render Rectangle
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -6.0f);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    // Swap buffers to display rendered image
    glXSwapBuffers(gpDisplay, window);
}

// ============================================================================
// Function: update
// Purpose : Reserved for animation or dynamic transformations.
// ============================================================================
void update(void)
{
    // No animation implemented for this demo
}

// ============================================================================
// Function: uninitialize
// Purpose : Frees resources and gracefully shuts down the OpenGL application.
// ============================================================================
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
        fprintf(gpFile, "Program Terminated Successfully!\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
