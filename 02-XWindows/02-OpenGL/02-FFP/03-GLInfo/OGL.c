// -----------------------------------------------------------------------------
// Program : XWindow OpenGL GLInfo Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Creates an X11 window, initializes an OpenGL context using GLX,
//           prints OpenGL driver information (vendor, renderer, version) to a
//           log file, handles basic input/events (keyboard, resize, fullscreen),
//           and renders a simple colored triangle using the fixed-function pipeline.
// -----------------------------------------------------------------------------

// Standard Header files
#include <stdio.h>   // for printf / fprintf
#include <stdlib.h>  // for exit()
#include <memory.h>  // for memset

// XLib related header files
#include <X11/Xlib.h>   // Core X11 functions and types (Display, Window, XEvent, etc.)
#include <X11/Xutil.h>  // Utility structures and helpers (XVisualInfo, XSizeHints, ...)
#include <X11/XKBlib.h> // X Keyboard extension helpers (keycode -> keysym conversions)

// OpenGL related header files
#include <GL/gl.h>   // Core fixed-function OpenGL API
#include <GL/glx.h>  // GLX: the X11 <-> OpenGL interface (context creation / buffer swap)

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------
// Process-global handles/resources used throughout the program. Kept global to
// simplify cleanup from any code path.
Display *gpDisplay = NULL;   // Connection handle to the X server (XOpenDisplay)
XVisualInfo *visualInfo;     // Visual information selected by glXChooseVisual
Window window;               // X11 window handle
Colormap colormap;           // Colormap created for the chosen visual
Bool bFullScreen = False;    // Fullscreen toggle state (True/False)
FILE *gpFile = NULL;         // Log file handle

Bool bActiveWindow = False;  // Tracks whether our window has input focus (used to pause rendering)

// OpenGL / GLX state
GLXContext glxContext = NULL; // The GLX OpenGL context

// -----------------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------------
// Program entry point:
// - Creates a log file for runtime messages
// - Connects to the X server and selects a visual compatible with requested
//   framebuffer attributes (double-buffered RGBA, 8-bit components)
// - Creates an X window, registers WM_DELETE_WINDOW, maps and centers the window
// - Initializes OpenGL/GLX context and prints GL information
// - Processes X events in a loop while rendering when the window is active
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
    XSetWindowAttributes windowAttributes; // struct for X window attributes
    Atom windowManagerDeleteAtom;
    XEvent event; // X event union
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym; // KeySym for key identification
    char keys[26]; // Buffer for printable key lookup
    int framebufferAttributes[] = {
        // Attributes passed to glXChooseVisual. Terminated by None.
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8, None
    };
    Bool bDone = False;

    // -------------------------------------------------------------------------
    // Create Log File
    // - Useful for debugging across different window managers and GPUs.
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // Open Display connection
    // - Uses DISPLAY environment variable. If NULL, uses default display.
    // -------------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    // Get default screen and depth
    // - These values inform glX and XCreateWindow calls below.
    // -------------------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // -------------------------------------------------------------------------
    // Choose a visual compatible with the requested framebuffer attributes.
    // - glXChooseVisual returns an XVisualInfo pointer describing a visual that
    //   matches requested properties (doublebuffer, RGBA, 8-bit channels).
    // - If NULL, the system cannot satisfy the requested attributes.
    // -------------------------------------------------------------------------
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
    // Set window attributes and create colormap for the chosen visual.
    // - background_pixel / border_pixel / event_mask are configured here.
    // -------------------------------------------------------------------------
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

    // Event mask: which X events this window will receive
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // -------------------------------------------------------------------------
    // Create the X window using the chosen visual and attributes.
    // - The depth must match visualInfo->depth for correct display.
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // Register WM_DELETE_WINDOW protocol so window manager close button works.
    // -------------------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // -------------------------------------------------------------------------
    // Set window title (updated to include author/ID).
    // -------------------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | BlueScreen ");

    // -------------------------------------------------------------------------
    // Map and center the window on the default screen.
    // -------------------------------------------------------------------------
    XMapWindow(gpDisplay, window);

    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

    // -------------------------------------------------------------------------
    // Initialize OpenGL (create GLX context, bind it, and print GL info).
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
    // Main event/render loop:
    // - Use XPending/XNextEvent to process all events without blocking rendering.
    // - Render and update only when the window is active/focused.
    // -------------------------------------------------------------------------
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
        // rendering
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }
    uninitialize();
    // return (0);
}

// -----------------------------------------------------------------------------
// toggleFullScreen()
// -----------------------------------------------------------------------------
// Sends a client message (_NET_WM_STATE) to the root window requesting the
// window manager toggle the fullscreen state for our window. Many modern WMs
// understand and apply this EWMH hint.
// -----------------------------------------------------------------------------
void toggleFullScreen(void)
{
    // variable declaration
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

// -----------------------------------------------------------------------------
// initialize()
// -----------------------------------------------------------------------------
// - Creates a GLX context for the chosen visual.
// - Makes that context current for the window.
// - Calls printGLInfo() to record GL vendor/renderer/version in the log.
// - Sets an initial clear color.
// Returns 0 on success, -1 on failure.
// -----------------------------------------------------------------------------
int initialize(void)
{
    //function declarations
    void printGLInfo();

    // Create GLX context (direct rendering if possible). Third parameter is share list (NULL).
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    // Make the created context current for the window so OpenGL calls affect it.
    glXMakeCurrent(gpDisplay, window, glxContext);

    // Print driver/OpenGL information to the log file for diagnostics.
    printGLInfo();

    // Set the default clear (background) color to black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return (0);
}

// -----------------------------------------------------------------------------
// printGLInfo()
// -----------------------------------------------------------------------------
// Queries and logs basic OpenGL driver information:
// - GL_VENDOR: GPU vendor (e.g., NVIDIA Corporation)
// - GL_RENDERER: Renderer string (GPU name/driver)
// - GL_VERSION: OpenGL version string
// -----------------------------------------------------------------------------
void printGLInfo(void)
{
    // Print OpenGL information to the log file for debugging and verification.
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor        : %s\n", (const char *)glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer      : %s\n", (const char *)glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version       : %s\n", (const char *)glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}

// -----------------------------------------------------------------------------
// resize(width, height)
// -----------------------------------------------------------------------------
// Updates the OpenGL viewport to the new window size. Protects against height==0.
// -----------------------------------------------------------------------------
void resize(int width, int height)
{
    if (height <= 0)
    {
        height = 1;
    }
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

// -----------------------------------------------------------------------------
// display()
// -----------------------------------------------------------------------------
// Clears the color buffer and renders a simple colored triangle using immediate
// mode (glBegin/glEnd). Swaps buffers using glXSwapBuffers for double buffering.
// -----------------------------------------------------------------------------
void display(void)
{
    // Clear color buffer to the clear color set in initialize().
    glClear(GL_COLOR_BUFFER_BIT);

    // Triangle Drawing Code (fixed-function pipeline / immediate mode)
    glBegin(GL_TRIANGLES);

    // Apex (top) vertex - red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    // Left bottom vertex - green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

    // Right bottom vertex - magenta
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

    // Present rendered image
    glXSwapBuffers(gpDisplay, window);
}

// -----------------------------------------------------------------------------
// update()
// -----------------------------------------------------------------------------
// Placeholder for per-frame updates (animations, physics, timers). Kept separate
// from display() for clarity and single responsibility.
// -----------------------------------------------------------------------------
void update(void)
{
    // Currently empty - add animations or time-based updates here as needed.
}

// -----------------------------------------------------------------------------
// uninitialize()
// -----------------------------------------------------------------------------
// Releases all acquired resources in reverse order: GLX context, visual info,
// X window resources, colormap, display connection, and closes the log file.
// Safe to call even if initialization failed partially.
// -----------------------------------------------------------------------------
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
        fprintf(gpFile, "Program terminated Successfully !!!\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
