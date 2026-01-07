// -----------------------------------------------------------------------------
// Program : XWindow OpenGL Perspective Rectangle Example (Black and White)
// Author : RTR2024-150 Omkar Kashid
// Desc : Demonstrates creating an X11 window, initializing an OpenGL GLX
// context, setting up a perspective projection (gluPerspective), and
// rendering a simple white rectangle using immediate mode (fixed function
// pipeline). The file maintains the original structure and indentation
// with detailed, professional comments.
// -----------------------------------------------------------------------------

// Standard Header files
#include <stdio.h>  // for printf and fprintf
#include <stdlib.h> // for exit()
#include <memory.h> // for memset()

// XLib related header files
#include <X11/Xlib.h>   // Core X11 functions and types (Display, Window, XEvent, etc.)
#include <X11/Xutil.h>  // Utility structures and helpers (XVisualInfo, XSizeHints, ...)
#include <X11/XKBlib.h> // X Keyboard extension helpers (keycode -> keysym conversions)

// OpenGL related header files
#include <GL/gl.h>  // Core OpenGL API
#include <GL/glx.h> // GLX: The X11 <-> OpenGL interface (context creation / buffer swap)
#include <GL/glu.h> // GLU: Utility library for functions like gluPerspective()

// Custom Header Files
#include "OGL.h"

// OpenGL Related Libraries (Windows-specific, typically ignored on Linux/X11 but kept for completeness)
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"GLU32.lib")

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------
// XLib resources
Display *gpDisplay = NULL; // Connection handle to the X server.
XVisualInfo *visualInfo;   // Visual information selected by glXChooseVisual.
Window window;             // X11 window handle.
Colormap colormap;         // Colormap created for the chosen visual.
Bool bFullScreen = False;  // Fullscreen toggle state (True/False).
FILE *gpFile = NULL;        // Log file handle for debugging output.

Bool bActiveWindow = False; // Tracks if the window has input focus.

// OpenGL / GLX state
GLXContext glxContext = NULL; // The GLX OpenGL context.

// RTR2024-150 Omkar Kashid

// -----------------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------------
int main(void)
{
    // function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);
    void toggleFullScreen(void);

    // variable declarations
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes; // Structure to hold window properties.
    Atom windowManagerDeleteAtom;         // Atom for the WM_DELETE_WINDOW protocol.
    XEvent event;                        // Structure to receive X events.
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym; // XLib structure for key symbol representation.
    char keys[26]; // Buffer for XLookupString (for printable characters).
    // Framebuffer attributes: Request a double-buffered RGBA visual with 8-bit components.
    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        None // Terminates the attribute list.
    };
    Bool bDone = False; // Main loop exit flag.

    // -------------------------------------------------------------------------
    // Create Log File
    // -------------------------------------------------------------------------
    gpFile = fopen("Log File", "w");
    if (gpFile == NULL)
    {
        printf("Log File Creation Failed!!\n");
        // Exit successfully as console message was printed.
        exit(EXIT_SUCCESS);
    }
    else
    {
        fprintf(gpFile, "Program Started Successfully\n");
    }

    // -------------------------------------------------------------------------
    // Open Display connection
    // -------------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL); // Connects to the X server (using DISPLAY environment variable if NULL).
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Get default screen number.
    defaultScreen = XDefaultScreen(gpDisplay);

    // Get default depth (number of bits per pixel).
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // -------------------------------------------------------------------------
    // Choose Visual (Graphics Configuration)
    // -------------------------------------------------------------------------
    // Find a visual that supports our GLX framebuffer attributes.
    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, framebufferAttributes);
    if (visualInfo == NULL)
    {
        fprintf(gpFile, "glXChooseVisual failed. \n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "glXChooseVisual Succedded. \n");
    }

    // -------------------------------------------------------------------------
    // Set Window Attributes/Properties
    // -------------------------------------------------------------------------
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    // Use the black pixel value associated with the chosen screen.
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen);
    // Create a colormap for the window, necessary for color mapping.
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        visualInfo->visual,
        AllocNone // Do not allocate new colors.
    );
    colormap = windowAttributes.colormap;

    // Setting Event mask for the messages we want to receive.
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // -------------------------------------------------------------------------
    // Create the X Window
    // -------------------------------------------------------------------------
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0,                 // Initial X position.
        0,                 // Initial Y position.
        WIN_WIDTH,         // Width.
        WIN_HEIGHT,        // Height.
        0,                 // Border width.
        visualInfo->depth, // Depth must match the visual.
        InputOutput,       // Window class.
        visualInfo->visual, // Visual structure to use.
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap, // Value mask for which attributes are being set.
        &windowAttributes); // Pointer to the attributes structure.
    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    // Window Manager Protocols
    // -------------------------------------------------------------------------
    // Atom creation for the WM_DELETE_WINDOW protocol (close button).
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    // Register the protocol with the window manager.
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set Window title (updated with new tag).
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | Perspective Rectangle");

    // Map the window to make it visible on the screen.
    XMapWindow(gpDisplay, window);

    // Centering of Window
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen); // Get screen information.
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    // Calculate and set window position for centering.
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

    // -------------------------------------------------------------------------
    // Initialize OpenGL
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // Game Loop / Event Loop
    // -------------------------------------------------------------------------
    while (bDone == False)
    {
        // Process all pending X events without blocking (non-blocking loop).
        while (XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event); // Get the next event from the queue.
            switch (event.type)
            {
            case MapNotify:
                break;
            case FocusIn:
                // Window gained focus.
                bActiveWindow = True;
                break;
            case FocusOut:
                // Window lost focus.
                bActiveWindow = False;
                break;
            case ConfigureNotify:
                // Window resized or moved. Call resize with new dimensions.
                resize(event.xconfigure.width, event.xconfigure.height);
                break;
            case KeyPress:
                // Handle non-printable keys (like Escape) using KeySym.
                keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                switch (keySym)
                {
                case XK_Escape:
                    bDone = True;
                    break;
                default:
                    break;
                }

                // Handle printable keys (like 'F' or 'f') using XLookupString.
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
            case ButtonPress: // For mouse clicks.
                break;
            case Expose: // For repainting/WM_PAINT equivalent.
                break;
            case 33:          // ClientMessage for WM_DELETE_WINDOW protocol (user clicking close).
                bDone = True;
                break;
            default:
                break;
            }
        }
        // Rendering (Outside of event processing, for continuous animation).
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }
    uninitialize();
}

// -----------------------------------------------------------------------------
// toggleFullScreen()
// -----------------------------------------------------------------------------
// Sends an EWMH ClientMessage to the root window to request the window manager
// to toggle the _NET_WM_STATE_FULLSCREEN property for the window.
void toggleFullScreen(void)
{
    // Get the Atoms for the window manager protocol and the fullscreen state.
    Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom windowManagerFullscreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent event;
    memset((void *)&event, 0, sizeof(XEvent));

    // Set up the ClientMessage event structure.
    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerNormalStateAtom;
    event.xclient.format = 32;
    // Toggle action: 1 to set fullscreen (if bFullScreen is False), 0 to unset (if bFullScreen is True).
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom; // The property to toggle.
    
    // Send the event to the root window to be handled by the window manager.
    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        False,                  // Do not propagate to children.
        SubstructureNotifyMask, // Event mask for this type of structural change.
        &event);
}

// -----------------------------------------------------------------------------
// initialize()
// -----------------------------------------------------------------------------
// Creates the GLX context, makes it current, prints GL information, sets the
// clear color, and performs the initial resize/setup of the projection matrix.
// Returns 0 on success, -1 on failure.
int initialize(void)
{
    //function declarations (local scope)
    void printGLInfo(void);
    void resize(int,int);

    // code
    // Create GLX context with direct rendering (True) preferred.
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    // Make the created context the current context for the window.
    glXMakeCurrent(gpDisplay, window, glxContext);

    // Print OpenGL Info for debugging/verification.
    printGLInfo();

    // Set the default color for clearing the color buffer (black).
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initial call to resize to set up the viewport and projection matrix.
    resize(WIN_WIDTH,WIN_HEIGHT);

    return (0);
}

// -----------------------------------------------------------------------------
// printGLInfo(void)
// -----------------------------------------------------------------------------
// Queries and logs important OpenGL driver and version information.
void printGLInfo(void)
{
    //Print OpenGL Information
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor      : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer    : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version     : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}

// -----------------------------------------------------------------------------
// resize(width, height)
// -----------------------------------------------------------------------------
// Handles window resize events by adjusting the viewport and the projection matrix.
void resize(int width, int height)
{
    // Protection against zero height to prevent division by zero in aspect ratio calculation.
    if (height <= 0)
    {
        height = 1;
    }

    // Set the viewport to cover the entire new window area.
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Set the current matrix to the Projection matrix mode.
    glMatrixMode(GL_PROJECTION);

    // Reset the Projection matrix to the identity matrix.
    glLoadIdentity();

    // Set up a perspective projection.
    // Parameters: FOV-Y (45 deg), Aspect Ratio (width/height), Near Plane (0.1f), Far Plane (100.0f).
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

// -----------------------------------------------------------------------------
// display()
// -----------------------------------------------------------------------------
// Clears buffers, sets up the ModelView matrix, and draws the scene geometry.
void display(void)
{
    // Clear the color buffer to the clear color (black).
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the current matrix to the ModelView matrix mode.
    glMatrixMode(GL_MODELVIEW);

    // Reset the ModelView matrix to the identity matrix.
    glLoadIdentity();

    // Translate the geometry backwards along the Z-axis to be visible in the frustum.
    glTranslatef(0.0f, 0.0f, -3.0f);

    // Rectangle Drawing Code (Immediate Mode)
    // The color is implicitly white (1.0f, 1.0f, 1.0f) as no glColor is called.
    glBegin(GL_QUADS);
    
    // Right top - (1, 1)
    glVertex3f(1.0f, 1.0f, 0.0f);

    // Left top - (-1, 1)
    glVertex3f(-1.0f, 1.0f, 0.0f);
    
    // Left bottom - (-1, -1)
    glVertex3f(-1.0f, -1.0f, 0.0f);

    // Right bottom - (1, -1)
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    // Swap front and back buffers to display the rendered frame.
    glXSwapBuffers(gpDisplay, window);
}

// -----------------------------------------------------------------------------
// update()
// -----------------------------------------------------------------------------
// Placeholder for per-frame updates (animation logic).
void update(void)
{
    // Currently empty - Add animation or state updates here.
}

// -----------------------------------------------------------------------------
// uninitialize()
// -----------------------------------------------------------------------------
// Releases all acquired XLib and GLX resources. Safe to call at any time.
void uninitialize(void)
{
    // Detach the current OpenGL rendering context if it is ours.
    GLXContext currentContext = glXGetCurrentContext();
    if (currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    // Destroy the GLX context.
    if (glxContext)
    {
        glXDestroyContext(gpDisplay, glxContext);
        glxContext = NULL;
    }

    // Free the XVisualInfo structure.
    if (visualInfo)
    {
        // XFree must be used for XLib structures allocated by functions like glXChooseVisual.
        XFree(visualInfo);
        visualInfo = NULL;
    }

    // Destroy the X window.
    if (window)
    {
        XDestroyWindow(gpDisplay, window);
    }

    // Free the colormap resource.
    if (colormap)
    {
        XFreeColormap(gpDisplay, colormap);
    }

    // Close the connection to the X server.
    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    // Close the log file.
    if (gpFile)
    {
        fprintf(gpFile, "Program terminated Successfully !!!");
        fclose(gpFile);
        gpFile = NULL;
    }    
}