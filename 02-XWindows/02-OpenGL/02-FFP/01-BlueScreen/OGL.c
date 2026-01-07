// -----------------------------------------------------------------------------
// Program : XWindow OpenGL BlueScreen Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates OpenGL context creation and blue background rendering 
//           using XLib and GLX on Linux.
// -----------------------------------------------------------------------------

// Standard Header files
#include <stdio.h>   // for printf / fprintf
#include <stdlib.h>  // for exit()
#include <memory.h>  // for memset()

// XLib related header files
#include <X11/Xlib.h>    // For basic XWindow APIs
#include <X11/Xutil.h>   // For XVisualInfo and related utilities
#include <X11/XKBlib.h>  // For keyboard-related functions

// OpenGL related header files
#include <GL/gl.h>   // Core OpenGL functions
#include <GL/glx.h>  // GLX: bridge between X11 and OpenGL

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL;    // Represents connection between XClient and XServer
XVisualInfo *visualInfo;      // Holds information about visual (pixel format, depth, etc.)
Window window;                // Equivalent to HWND in Win32; represents XWindow
Colormap colormap;            // Mapping of colors in XServer
Bool bFullScreen = False;     
FILE *gpFile = NULL;          
Bool bActiveWindow = False;   // To check window focus state

// OpenGL related variables
GLXContext glxContext = NULL; // Represents OpenGL rendering context

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

    // Open connection to XServer
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Get default screen and depth
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // Choose suitable visual info for OpenGL rendering
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

    // Set window attributes
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

    // Enable required events
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
        &windowAttributes);
    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Handle window close event
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set window title
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | BlueScreen");

    // Display the window on screen
    XMapWindow(gpDisplay, window);

    // Center window on screen
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, (screenWidth / 2) - (WIN_WIDTH / 2), (screenHeight / 2) - (WIN_HEIGHT / 2));

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

    // Main message loop
    while (bDone == False)
    {
        while (XPending(gpDisplay)) // Check for pending X events
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
                case XK_Escape: // Exit on Escape key
                    bDone = True;
                    break;
                default:
                    break;
                }

                // Handle alphabet keys
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

            case 33: // Window close event
                bDone = True;
                break;

            default:
                break;
            }
        }

        // Render only when window is active
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }

    uninitialize();
}

void toggleFullScreen(void)
{
    // Toggle between fullscreen and windowed mode
    Atom wmState = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom wmFullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent event;
    memset(&event, 0, sizeof(XEvent));

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = wmState;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = wmFullscreen;

    // Send toggle event to XServer
    XSendEvent(gpDisplay,
               XRootWindow(gpDisplay, visualInfo->screen),
               False,
               SubstructureNotifyMask,
               &event);
}

int initialize(void)
{
    // Create OpenGL context and set background color
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glXCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    // Set blue background
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    return (0);
}

void resize(int width, int height)
{
    // Adjust viewport when window is resized
    if (height <= 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{
    // Clear window with current clear color (blue)
    glClear(GL_COLOR_BUFFER_BIT);
    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    // Placeholder for future animations or updates
}

void uninitialize(void)
{
    // Cleanup and resource release
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
        fprintf(gpFile, "Program terminated successfully.\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
