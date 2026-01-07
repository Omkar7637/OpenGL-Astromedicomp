// -----------------------------------------------------------------------------
// Program : XWindow OpenGL BlueScreen Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates how to create an X11 window, initialize an OpenGL
//           rendering context using GLX, handle basic input/events, toggle
//           fullscreen, and render a simple triangle. All comments are in
//           professional English. Code, indentation and logic are kept intact.
// -----------------------------------------------------------------------------

// Standard Header files
#include <stdio.h>  // to use printf / fprintf
#include <stdlib.h> // for exit()
#include <memory.h> // for memset

// XLib related header files
#include <X11/Xlib.h>   // Core X11 functions and types (Display, Window, XEvent, etc.)
#include <X11/Xutil.h>  // Utility structures and helpers (XVisualInfo, XSizeHints, ...)
#include <X11/XKBlib.h> // X Keyboard extension helpers (keycode -> keysym conversions)

// OpenGL related header files
#include <GL/gl.h>  // Core fixed-function OpenGL API
#include <GL/glx.h> // GLX: the X11 <-> OpenGL interface (context creation / buffer swap)

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------
// These are process-global handles/resources used throughout the program.
// Keep them global to simplify cleanup from any code path.
Display *gpDisplay = NULL;  // Connection handle to the X server (XOpenDisplay)
XVisualInfo *visualInfo;    // Visual information selected by glXChooseVisual
Window window;              // X11 window handle
Colormap colormap;          // Colormap created for the chosen visual
Bool bFullScreen = False;   // Fullscreen toggle state (True/False)
FILE *gpFile = NULL;        // Log file handle

Bool bActiveWindow = False; // Tracks whether our window has input focus (used to pause rendering)

// OpenGL / GLX state
GLXContext glxContext = NULL; // The GLX OpenGL context

int main(void)
{
    // ---------------------------------------------------------------------
    // Function declarations (kept local to main for clarity of original file)
    // ---------------------------------------------------------------------
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);
    void toggleFullScreen(void);

    // ---------------------------------------------------------------------
    // Variable declarations used in startup and event loop
    // ---------------------------------------------------------------------
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes; // X window attribute structure
    Atom windowManagerDeleteAtom;
    XEvent event; // XEvent structure for incoming X server events
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym; // KeySym used for named key handling
    char keys[26]; // Buffer for XLookupString (printable characters)

    // Framebuffer attributes: request a double-buffered RGBA visual with 8-bit components.
    // This array is passed to glXChooseVisual. 'None' terminates the list.
    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,   // 8 bits for red
        GLX_GREEN_SIZE, 8, // 8 bits for green
        GLX_BLUE_SIZE, 8,  // 8 bits for blue
        GLX_ALPHA_SIZE, 8, // 8 bits for alpha
        None
    };
    Bool bDone = False;

    // ---------------------------------------------------------------------
    // Create Log File
    // ---------------------------------------------------------------------
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

    // ---------------------------------------------------------------------
    // Open Display connection to the X server.
    // XOpenDisplay(NULL) uses the DISPLAY environment variable (e.g. ":0").
    // ---------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ---------------------------------------------------------------------
    // Create the default screen object and get default depth.
    // ---------------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // ---------------------------------------------------------------------
    // Choose a Visual compatible with the requested framebuffer attributes.
    // glXChooseVisual returns an XVisualInfo* describing a matching visual.
    // ---------------------------------------------------------------------
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

    // ---------------------------------------------------------------------
    // Configure X window attributes and create a colormap for the visual.
    // ---------------------------------------------------------------------
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

    // We want to receive keyboard, mouse, focus, configure (resize) and expose events.
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // ---------------------------------------------------------------------
    // Create the X window.
    // XCreateWindow requires the visual and depth that match the GLX visual.
    // ---------------------------------------------------------------------
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0, // X
        0, // Y
        WIN_WIDTH,
        WIN_HEIGHT,
        0,                 // border width
        visualInfo->depth, // depth
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

    // ---------------------------------------------------------------------
    // Register WM_DELETE_WINDOW so the window manager can request close.
    // ---------------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // ---------------------------------------------------------------------
    // Set Window title (includes author tag).
    // ---------------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | GL Ortho ");

    // ---------------------------------------------------------------------
    // Map the window (make visible) and center it on the default screen.
    // ---------------------------------------------------------------------
    XMapWindow(gpDisplay, window);

    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

    // ---------------------------------------------------------------------
    // Initialize OpenGL/GLX state (create context, bind it, set clear color).
    // ---------------------------------------------------------------------
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

    // ---------------------------------------------------------------------
    // Game / Main Loop:
    // - Use XPending/XNextEvent to process pending X events without blocking.
    // - When the window has focus (bActiveWindow == True) call display() and update().
    // ---------------------------------------------------------------------
    while (bDone == False)
    {
        while (XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event);
            switch (event.type)
            {
            case MapNotify:
                // Window mapped (visible).
                break;
            case FocusIn:
                // Window gained input focus -> resume rendering.
                bActiveWindow = True;
                break;
            case FocusOut:
                // Window lost focus -> pause rendering.
                bActiveWindow = False;
                break;
            case ConfigureNotify:
                // Window resized or moved. Update viewport / projection.
                resize(event.xconfigure.width, event.xconfigure.height);
                break;
            case KeyPress:
                // Convert X keycode to KeySym for named-key handling.
                keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                switch (keySym)
                {
                case XK_Escape:
                    // Escape requested: exit main loop.
                    bDone = True;
                    break;
                default:
                    break;
                }

                // Also check printable characters using XLookupString.
                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch (keys[0])
                {
                case 'F':
                case 'f':
                    // Toggle fullscreen via window manager hint.
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
                // Mouse button pressed - placeholder for interaction.
                break;
            case Expose:
                // Expose events indicate regions need repainting.
                break;
            case 33:
                // WM_DELETE_WINDOW client message -> exit requested by window manager.
                bDone = True;
                break;
            default:
                break;
            }
        }

        // Only render/update when window is active to avoid wasting GPU resources.
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }

    // Cleanup before exit.
    uninitialize();
    // Note: program intentionally does not return because uninitialize handles closure.
}

void toggleFullScreen(void)
{
    // ---------------------------------------------------------------------
    // Send a ClientMessage to the root window requesting the window manager
    // to toggle the _NET_WM_STATE_FULLSCREEN hint for our window. Many WMs
    // recognize this EWMH hint and will perform the fullscreen action.
    // ---------------------------------------------------------------------
    Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom windowManagerFullscreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent event;
    memset((void *)&event, 0, sizeof(XEvent));

    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerNormalStateAtom;
    event.xclient.format = 32; // 32-bit data format
    // l[0] = 1 to add, 0 to remove; invert based on current state
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom;

    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        False,
        SubstructureNotifyMask,
        &event);
}

int initialize(void)
{
    // ---------------------------------------------------------------------
    // Create a GLX context suitable for the chosen visual and make it current.
    // ---------------------------------------------------------------------
    void printGLInfo();

    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext);

    // Print GL vendor/renderer/version information to the log file.
    printGLInfo();

    // Set the default clear (background) color (black).
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return (0);
}

void printGLInfo(void)
{
    // ---------------------------------------------------------------------
    // Print OpenGL information to the log file for debugging and verification.
    // ---------------------------------------------------------------------
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor    : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer  : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version   : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}

void resize(int width, int height)
{
    // Protect against a zero or negative height to avoid division by zero.
    if (height <= 0)
    {
        height = 1;
    }

    // Set the OpenGL viewport to match the new window dimensions.
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Set projection matrix mode and load identity before setting projection.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Maintain an orthographic projection that preserves aspect ratio.
    if (width <= height)
    {
        glOrtho(
            -100.0f, // left
            100.0f,  // right
            (-100.0f * ((GLfloat)WIN_HEIGHT / (GLfloat)WIN_WIDTH)), // bottom
            (100.0f * ((GLfloat)WIN_HEIGHT / (GLfloat)WIN_WIDTH)),  // top
            -100.0f, // near
            100.0f   // far
        );
    }
    else
    {
        glOrtho(
            (-100.0f * (GLfloat)height / (GLfloat)width), // left
            (100.0f * (GLfloat)height / (GLfloat)width),  // right
            -100.0f, // bottom
            100.0f,  // top
            -100.0f, // near
            100.0f   // far
        );
    }
}

void display(void)
{
    // ---------------------------------------------------------------------
    // Render a simple triangle using the fixed-function pipeline.
    // This example uses immediate mode (glBegin/glEnd) for simplicity.
    // ---------------------------------------------------------------------
    // Clear the color buffer using the clear color set in initialize().
    glClear(GL_COLOR_BUFFER_BIT);

    // Set matrix mode to modelview and reset to identity.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Triangle drawing using immediate mode.
    glBegin(GL_TRIANGLES);

    // Apex vertex
    glVertex3f(0.0f, 50.0f, 0.0f);

    // Left bottom vertex
    glVertex3f(-50.0f, -50.0f, 0.0f);

    // Right bottom vertex
    glVertex3f(50.0f, -50.0f, 0.0f);

    glEnd();

    // Swap the front and back buffers to present the rendered frame.
    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    // Placeholder for per-frame updates (animations, timers, state changes).
}

void uninitialize(void)
{
    // ---------------------------------------------------------------------
    // Release and destroy all acquired resources: GLX context, X resources,
    // and the log file. This function is safe to call even if initialization
    // failed partway through.
    // ---------------------------------------------------------------------

    // If the current GLX context is ours, detach it.
    GLXContext currentContext = glXGetCurrentContext();
    if (currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    // Destroy GLX context if created.
    if (glxContext)
    {
        glXDestroyContext(gpDisplay, glxContext);
        glxContext = NULL;
    }

    // Free allocated visual info returned by glXChooseVisual.
    if (visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    // Destroy the X window if created.
    if (window)
    {
        XDestroyWindow(gpDisplay, window);
    }

    // Free the colormap resource if created.
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

    // Finalize log file with a termination message and close it.
    if (gpFile)
    {
        fprintf(gpFile, "Program terminated Successfully !!!\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
