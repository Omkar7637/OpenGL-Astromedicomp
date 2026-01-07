// -----------------------------------------------------------------------------
// Program : XWindow Fullscreen Toggle Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates creating a window using XLib and toggling fullscreen
//           mode using keyboard input (F/f key).
// -----------------------------------------------------------------------------

// Standard Header Files
#include <stdio.h>   // For printf()
#include <stdlib.h>  // For exit()
#include <memory.h>  // For memset()

// XLib Header Files
#include <X11/Xlib.h>   // Core X11 functions for window creation and event handling
#include <X11/Xutil.h>  // For visual information and related APIs
#include <X11/XKBlib.h> // For keyboard handling functions

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL;  // Connection between X client and X server
XVisualInfo visualInfo;     // Describes pixel format and display properties
Window window;              // Window handle
Colormap colormap;          // Maps color values to screen pixels
Bool bFullScreen = False;   // Keeps track of fullscreen toggle state

// Function Declarations
void uninitialize(void);
void toggleFullScreen(void);

int main(void)
{
    // Variable Declarations
    int defaultScreen;
    int defaultDepth;
    Status status;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];

    // -------------------------------------------------------------------------
    // Step 1: Establish Connection with X Server
    // -------------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        printf("Error: Unable to connect to XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    // Step 2: Obtain Default Screen and Depth
    // -------------------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // -------------------------------------------------------------------------
    // Step 3: Match a TrueColor Visual
    // -------------------------------------------------------------------------
    memset((void *)&visualInfo, 0, sizeof(XVisualInfo));
    status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, &visualInfo);
    if (status == 0)
    {
        printf("Error: XMatchVisualInfo failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    // Step 4: Configure Window Attributes
    // -------------------------------------------------------------------------
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XWhitePixel(gpDisplay, visualInfo.screen);

    // Create colormap for the selected visual
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        visualInfo.visual,
        AllocNone
    );
    colormap = windowAttributes.colormap;

    // Define which events the window should handle
    windowAttributes.event_mask = KeyPressMask |
                                  ButtonPressMask |
                                  FocusChangeMask |
                                  StructureNotifyMask |
                                  ExposureMask;

    // -------------------------------------------------------------------------
    // Step 5: Create the Window
    // -------------------------------------------------------------------------
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        0, 0,
        WIN_WIDTH,
        WIN_HEIGHT,
        0,
        visualInfo.depth,
        InputOutput,
        visualInfo.visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
        &windowAttributes
    );

    if (!window)
    {
        printf("Error: XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------------
    // Step 6: Setup Protocol for Window Close (WM_DELETE_WINDOW)
    // -------------------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // -------------------------------------------------------------------------
    // Step 7: Set Window Title and Map (Display) It
    // -------------------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | FullScreen");
    XMapWindow(gpDisplay, window);

    // -------------------------------------------------------------------------
    // Step 8: Center the Window on Screen
    // -------------------------------------------------------------------------
    screen = XScreenOfDisplay(gpDisplay, visualInfo.screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window,
                screenWidth / 2 - WIN_WIDTH / 2,
                screenHeight / 2 - WIN_HEIGHT / 2);

    // -------------------------------------------------------------------------
    // Step 9: Message/Event Loop
    // -------------------------------------------------------------------------
    while (1)
    {
        XNextEvent(gpDisplay, &event);

        switch (event.type)
        {
        case KeyPress:
            keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);

            // Escape key exits the application
            if (keySym == XK_Escape)
            {
                uninitialize();
                exit(EXIT_SUCCESS);
            }

            // Check for alphabetic keypress
            XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
            switch (keys[0])
            {
            case 'F':
            case 'f':
                // Toggle fullscreen mode
                toggleFullScreen();
                bFullScreen = (bFullScreen == False) ? True : False;
                break;
            default:
                break;
            }
            break;

        case ButtonPress:
        case Expose:
        case FocusIn:
        case FocusOut:
        case ConfigureNotify:
            // Handle other events silently
            break;

        case 33: // WM_DELETE_WINDOW
            uninitialize();
            exit(EXIT_SUCCESS);
            break;

        default:
            break;
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------
// Function : toggleFullScreen()
// Desc     : Toggles between fullscreen and windowed mode by sending a
//            _NET_WM_STATE_FULLSCREEN message to the XServer.
// -----------------------------------------------------------------------------
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
    event.xclient.data.l[0] = bFullScreen ? 0 : 1; // 0 = disable, 1 = enable
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom;

    // Send event to XServer to toggle fullscreen
    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        False,
        SubstructureNotifyMask,
        &event
    );
}

// -----------------------------------------------------------------------------
// Function : uninitialize()
// Desc     : Releases all allocated resources before program termination.
// -----------------------------------------------------------------------------
void uninitialize(void)
{
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
}
