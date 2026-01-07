// -----------------------------------------------------------------------------
// Program : XWindow Message Handling Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates creation of an XWindow and handling of various messages
// -----------------------------------------------------------------------------

// Standard Header Files
#include <stdio.h>   // For printf()
#include <stdlib.h>  // For exit()
#include <memory.h>  // For memset()

// XLib Header Files
#include <X11/Xlib.h>   // Core X11 functions for window creation and event handling
#include <X11/Xutil.h>  // Utility functions related to window management

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL;  // Pointer to the connection between X client and X server
XVisualInfo visualInfo;     // Contains information about the visual (like pixel format)
Window window;              // Represents the window handle
Colormap colormap;          // Hardware resource that maps color values to screen pixels

// Function Declaration
void uninitialize(void);

int main(void)
{
    // Variable Declarations
    int defaultScreen;
    int defaultDepth;
    Status status;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;             // Used to receive events (e.g., keypress, resize)
    Screen *screen = NULL;
    int screenWidth, screenHeight;

    // -------------------------------------------------------------------------
    // Step 1: Establish Connection with X Server
    // -------------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL);  // NULL => use default display
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
    // Step 3: Match a TrueColor Visual (similar to Pixel Format in Win32)
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

    // Create a colormap associated with the visual
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        visualInfo.visual,
        AllocNone
    );
    colormap = windowAttributes.colormap;

    // Specify the events this window should listen for
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
    // Step 6: Setup Protocol for Window Manager (Close Button Handling)
    // -------------------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // -------------------------------------------------------------------------
    // Step 7: Set Window Title and Map (Display) It
    // -------------------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | Messages");
    XMapWindow(gpDisplay, window);

    // -------------------------------------------------------------------------
    // Step 8: Center the Window on the Screen
    // -------------------------------------------------------------------------
    screen = XScreenOfDisplay(gpDisplay, visualInfo.screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window,
                screenWidth / 2 - WIN_WIDTH / 2,
                screenHeight / 2 - WIN_HEIGHT / 2);

    // -------------------------------------------------------------------------
    // Step 9: Message Loop (Event Processing)
    // -------------------------------------------------------------------------
    while (1)
    {
        XNextEvent(gpDisplay, &event);  // Wait and fetch the next event

        switch (event.type)
        {
        case MapNotify:
            printf("Event: Window mapped (visible on screen).\n");
            break;

        case FocusIn:
            printf("Event: Window gained focus.\n");
            break;

        case FocusOut:
            printf("Event: Window lost focus.\n");
            break;

        case ConfigureNotify:
            printf("Event: Window resized.\n");
            break;

        case KeyPress:
            printf("Event: Key pressed.\n");
            break;

        case ButtonPress:
            switch (event.xbutton.button)
            {
            case 1:
                printf("Event: Left mouse button clicked.\n");
                break;
            case 2:
                printf("Event: Middle mouse button clicked.\n");
                break;
            case 3:
                printf("Event: Right mouse button clicked.\n");
                break;
            case 4:
                printf("Event: Mouse wheel up.\n");
                break;
            case 5:
                printf("Event: Mouse wheel down.\n");
                break;
            default:
                break;
            }
            break;

        case Expose:
            // Called when window needs to be redrawn (paint event)
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
// Function: uninitialize()
// Desc    : Releases all allocated resources before program termination
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
