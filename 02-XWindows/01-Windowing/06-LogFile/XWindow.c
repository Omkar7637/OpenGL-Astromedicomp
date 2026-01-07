// Standard Header files
#include <stdio.h>   // for printf (later replaced with fprintf)
#include <stdlib.h>  // for exit()
#include <memory.h>  // for memset

// XLib related header files
#include <X11/Xlib.h>   // Core Xlib APIs (similar to <windows.h> in Win32)
#include <X11/Xutil.h>  // For XVisualInfo and related utility functions
#include <X11/XKBlib.h> // For keyboard-related Xlib APIs

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL; // Interface between XServer and XClient
XVisualInfo visualInfo;    // Holds visual configuration of the graphics hardware
Window window;             // Represents the XWindow object (like HWND in Win32)
Colormap colormap;         // Hardware-managed structure holding color mapping data
Bool bFullScreen = False;
FILE *gpFile = NULL;

// Entry point function
int main(void)
{
    // Function Declarations
    void uninitialize(void);
    void toggleFullScreen(void);

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

    // Create Log File
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

    // Open Display Connection
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay() failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Get Default Screen
    defaultScreen = XDefaultScreen(gpDisplay);

    // Get Default Depth
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // Get Visual Info
    memset((void *)&visualInfo, 0, sizeof(XVisualInfo));
    status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, &visualInfo);
    if (status == 0)
    {
        fprintf(gpFile, "XMatchVisualInfo failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Set Window Attributes
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo.screen);
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        visualInfo.visual,
        AllocNone);
    colormap = windowAttributes.colormap;

    // Set Event Mask
    windowAttributes.event_mask =
        KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // Create Window
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        0, 0,
        WIN_WIDTH, WIN_HEIGHT,
        0,
        visualInfo.depth,
        InputOutput,
        visualInfo.visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
        &windowAttributes);
    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // Create Atom for Window Manager to handle window close
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // Set Window Title
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | FullScreen");

    // Map Window to Display
    XMapWindow(gpDisplay, window);

    // Center Window on Screen
    screen = XScreenOfDisplay(gpDisplay, visualInfo.screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

    // Event Loop
    while (1)
    {
        XNextEvent(gpDisplay, &event);

        switch (event.type)
        {
        case MapNotify:
            break;

        case FocusIn:
            break;

        case FocusOut:
            break;

        case ConfigureNotify:
            break;

        case KeyPress:
            keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);

            // Handle ESC key
            switch (keySym)
            {
            case XK_Escape:
                fprintf(gpFile, "Program terminated successfully!\n");
                uninitialize();
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
            }

            // Handle 'F' key for Fullscreen toggle
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

        case ButtonPress:
            break;

        case Expose:
            break;

        case 33: // WM_DELETE_WINDOW event
            fprintf(gpFile, "Program terminated successfully!\n");
            fclose(gpFile);
            uninitialize();
            exit(EXIT_SUCCESS);
            break;

        default:
            break;
        }
    }

    return (0);
}

// Function to toggle between Fullscreen and Windowed mode
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

    // Send the event to XServer
    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        False,
        SubstructureNotifyMask,
        &event);
}

// Function to release all resources
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
