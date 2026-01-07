// -----------------------------------------------------------------------------
// Program : XWindow OpenGL BlueScreen Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates how to create an X11 window, initialize an OpenGL
//           rendering context using GLX, handle basic input/events, toggle
//           fullscreen, and render a simple colored triangle. This file keeps
//           original structure and indentation while providing detailed,
 //          documentation-style comments above each major block and function.
// -----------------------------------------------------------------------------

// Standard Header files
#include <stdio.h>   // for printf / fprintf
#include <stdlib.h>  // for exit()
#include <memory.h>  // for memset()

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
// These are process-global handles/resources used throughout the program.
// Keep them global to simplify cleanup from any code path.
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
int main(void)
{
    // -------------------------------------------------------------------------
    // Function declarations (kept local to main for clarity of original file)
    // -------------------------------------------------------------------------
    int initialize(void);           // Create GLX context, set up OpenGL state
    void resize(int, int);          // Handle viewport changes
    void display(void);             // Render a single frame
    void update(void);              // Animation / per-frame updates (placeholder)
    void uninitialize(void);        // Cleanup resources before exit
    void toggleFullScreen(void);    // Ask window manager to toggle fullscreen

    // -------------------------------------------------------------------------
    // Variable declarations used in startup and event loop
    // -------------------------------------------------------------------------
    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];

    // Framebuffer attributes: request a double-buffered RGBA visual with 8-bit components.
    // This array is passed to glXChooseVisual. 'None' terminates the list.
    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        None
    };

    Bool bDone = False; // Main loop exit flag

    // -------------------------------------------------------------------------
    // Create a log file to capture runtime messages. This helps debugging when
    // running under different window managers or GPUs.
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
    // Open connection to the X server.
    // XOpenDisplay(NULL) uses the DISPLAY environment variable (e.g. ":0").
    // If this fails, no X server is available or DISPLAY is unset.
    // -------------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    // Query default screen and default depth for basic display information.
    // These values are used by GLX / XCreateWindow below.
    // -------------------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // -------------------------------------------------------------------------
    // Choose a Visual compatible with our requested framebuffer attributes.
    // glXChooseVisual returns an XVisualInfo* describing a visual that matches
    // our requirements (double-buffer, RGBA, 8-bit channels). If it returns
    // NULL, the system cannot satisfy the requested attributes.
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // Configure X window attributes.
    // - Create a colormap for the chosen visual (required for many visuals).
    // - Set background pixel, border, and the event mask (which events we want).
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

    // We want to receive keyboard, mouse, focus, configure (resize) and expose events.
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // -------------------------------------------------------------------------
    // Create the X window.
    // XCreateWindow requires the visual and depth that match the GLX visual.
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // Register the WM_DELETE_WINDOW protocol so the window manager can notify
    // us when the user clicks the close button. Without this, the close button
    // may be ignored by the client application.
    // -------------------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // -------------------------------------------------------------------------
    // Set the window title. Updated to include author/ID.
    // -------------------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | BlueScreen");

    // -------------------------------------------------------------------------
    // Map the window (make it visible) and center it on the default screen.
    // -------------------------------------------------------------------------
    XMapWindow(gpDisplay, window);

    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window,
                screenWidth / 2 - WIN_WIDTH / 2,
                screenHeight / 2 - WIN_HEIGHT / 2);

    // -------------------------------------------------------------------------
    // Initialize OpenGL/GLX state (create context, bind it, set clear color).
    // If initialize() fails, log and exit.
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
    // Main loop:
    // - Use XPending/XNextEvent to process pending X events without blocking
    //   forever. This pattern lets the app run continuous rendering/animation
    //   while still responding to events.
    // - When the window has focus (bActiveWindow == True) call display() and update().
    // -------------------------------------------------------------------------
    while (bDone == False)
    {
        while (XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event);
            switch (event.type)
            {
            case MapNotify:
                // Window has been mapped (made visible). Useful for delayed init.
                break;

            case FocusIn:
                // The window gained input focus. Resume rendering/updates.
                bActiveWindow = True;
                break;

            case FocusOut:
                // The window lost input focus. Pause rendering/updates if desired.
                bActiveWindow = False;
                break;

            case ConfigureNotify:
                // Window resized or moved. Update the OpenGL viewport.
                resize(event.xconfigure.width, event.xconfigure.height);
                break;

            case KeyPress:
                // Convert X keycode to KeySym for easier handling of named keys.
                keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                switch (keySym)
                {
                case XK_Escape:
                    // Escape key requested; exit main loop gracefully.
                    bDone = True;
                    break;
                default:
                    break;
                }

                // Also check for printable alphabetic keys using XLookupString.
                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch (keys[0])
                {
                case 'F':
                case 'f':
                    // Toggle fullscreen by asking the window manager to change _NET_WM_STATE.
                    toggleFullScreen();
                    bFullScreen = (bFullScreen == False) ? True : False;
                    break;
                default:
                    break;
                }
                break;

            case ButtonPress:
                // Mouse button pressed - can be used for interactions.
                break;

            case Expose:
                // Expose indicates regions need repainting. We handle full repaint in display().
                break;

            case 33: // WM_DELETE_WINDOW (client message mapped to 33 in many systems)
                // Window manager requested close; end loop.
                bDone = True;
                break;

            default:
                break;
            }
        }

        // Only render/update when window is active to avoid wasting GPU when not focused.
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }

    // Cleanup before exit.
    uninitialize();
    // Note: program intentionally does not return here because uninitialize handles closure.
}

// -----------------------------------------------------------------------------
// toggleFullScreen()
// -----------------------------------------------------------------------------
// Send a ClientMessage to the root window requesting the window manager toggle
// the _NET_WM_STATE_FULLSCREEN hint for our window. Many modern WMs recognize
// this EWMH hint and will perform the fullscreen behavior.
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
    // l[0] = 1 to add, 0 to remove; we invert based on current bFullScreen
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
// - Create a GLX context suitable for the chosen visual.
// - Make the context current with the X window.
// - Initialize basic OpenGL state (clear color, depth, etc.).
// Returns 0 on success, -1 on failure.
int initialize(void)
{
    // Create GLX context. Parameters:
    //  - display, chosen visual, shared context (NULL), direct rendering (True)
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glXCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    // Make the created context current for the window so OpenGL calls affect it.
    glXMakeCurrent(gpDisplay, window, glxContext);

    // Set the default clear (background) color to black initially.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return (0);
}

// -----------------------------------------------------------------------------
// resize(width, height)
// -----------------------------------------------------------------------------
// Adjust the OpenGL viewport when the window size changes. Protect against
// height == 0 to avoid division by zero in perspective calculations.
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
// Clear the color buffer, render a simple colored triangle, and swap buffers.
// This uses the legacy fixed-function pipeline (glBegin/glEnd) for simplicity.
void display(void)
{
    // Clear color buffer to the clear color set in initialize().
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a simple triangle with three colored vertices.
    // NOTE: This is fixed-function OpenGL (immediate mode) for education/demo.
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

    // Swap front and back buffers to present the rendered image.
    glXSwapBuffers(gpDisplay, window);
}

// -----------------------------------------------------------------------------
// update()
// -----------------------------------------------------------------------------
// Placeholder for per-frame updates (animation, physics, timers). Keep this
// separate from display() to maintain single-responsibility functions.
void update(void)
{
    // Currently empty - add animations or state updates here.
}

// -----------------------------------------------------------------------------
// uninitialize()
// -----------------------------------------------------------------------------
// Release and destroy all acquired resources: GLX context, X resources, and log.
// Always safe to call even if initialization failed partway.
void uninitialize(void)
{
    // If the current context is ours, detach it from the thread and drawable.
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
        fprintf(gpFile, "Program terminated successfully.\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
