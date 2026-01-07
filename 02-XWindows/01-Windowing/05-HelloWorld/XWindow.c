// -----------------------------------------------------------------------------
// Program : XWindow Hello World Example
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates creation of a basic XWindow and rendering text using XLib
// -----------------------------------------------------------------------------

// Standard Header Files
#include <stdio.h>   // for printf (later can be replaced with fprintf)
#include <stdlib.h>  // for exit()
#include <memory.h>  // for memset()

// XLib related Header Files
#include <X11/Xlib.h>   // Similar to windows.h in Win32, provides all XLib APIs
#include <X11/Xutil.h>  // For XVisualInfo and related structures
#include <X11/XKBlib.h> // For keyboard-related XLib APIs

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL; // Represents the connection between XClient and XServer
XVisualInfo visualInfo;    // Describes visual capabilities (similar to Device Context in Win32)
Window window;             // Represents the actual XWindow (like HWND in Win32)
Colormap colormap;         // Represents color mapping from memory to hardware
Bool bFullScreen = False;  // Fullscreen toggle flag

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

    // Variables for Hello World text rendering
    static XFontStruct *pFontStruct = NULL;
    static int winWidth, winHeight;
    static GC gc; // Graphics Context (similar to HDC in Win32)
    XGCValues gcValues;
    XColor color;
    int strLen;
    int strWidth;
    int fontHeight;
    char str[] = "Hello World!!!";

    // -----------------------------------------------------------------------------
    // Step 1 : Open Display Connection (connect XClient with XServer)
    // -----------------------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        printf("XOpenDisplay() failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -----------------------------------------------------------------------------
    // Step 2 : Get Default Screen and Depth Information
    // -----------------------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

    // -----------------------------------------------------------------------------
    // Step 3 : Match Visual Info for TrueColor Display
    // -----------------------------------------------------------------------------
    memset((void *)&visualInfo, 0, sizeof(XVisualInfo));
    status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, &visualInfo);
    if (status == 0)
    {
        printf("XMatchVisualInfo() failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -----------------------------------------------------------------------------
    // Step 4 : Set Window Attributes
    // -----------------------------------------------------------------------------
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
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // -----------------------------------------------------------------------------
    // Step 5 : Create Window
    // -----------------------------------------------------------------------------
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
        &windowAttributes);

    if (!window)
    {
        printf("XCreateWindow() failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // -----------------------------------------------------------------------------
    // Step 6 : Set Protocols and Title
    // -----------------------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | HelloWorld");

    // -----------------------------------------------------------------------------
    // Step 7 : Map Window and Center on Screen
    // -----------------------------------------------------------------------------
    XMapWindow(gpDisplay, window);
    screen = XScreenOfDisplay(gpDisplay, visualInfo.screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

    // -----------------------------------------------------------------------------
    // Step 8 : Message Loop
    // -----------------------------------------------------------------------------
    while (1)
    {
        XNextEvent(gpDisplay, &event);

        switch (event.type)
        {
        case MapNotify:
            // Load default fixed-pitch font when the window is mapped
            pFontStruct = XLoadQueryFont(gpDisplay, "fixed");
            break;

        case ConfigureNotify:
            // Get the new width and height when window is resized
            winWidth = event.xconfigure.width;
            winHeight = event.xconfigure.height;
            break;

        case KeyPress:
            // Handle key press events
            keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
            switch (keySym)
            {
            case XK_Escape:
                // Exit application on ESC key
                XUnloadFont(gpDisplay, pFontStruct->fid);
                XFreeGC(gpDisplay, gc);
                uninitialize();
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
            }

            // Handle Fullscreen toggle on 'F' or 'f'
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

        case Expose:
            // Create Graphics Context and render text
            gc = XCreateGC(gpDisplay, window, 0, &gcValues);
            XSetFont(gpDisplay, gc, pFontStruct->fid);
            XAllocNamedColor(gpDisplay, colormap, "green", &color, &color);
            XSetForeground(gpDisplay, gc, color.pixel);

            // Calculate text width and height for centering
            strLen = strlen(str);
            strWidth = XTextWidth(pFontStruct, str, strLen);
            fontHeight = pFontStruct->ascent + pFontStruct->descent;

            // Draw centered text
            XDrawString(gpDisplay, window, gc, winWidth / 2 - strWidth / 2, winHeight / 2 - fontHeight / 2, str, strLen);
            break;

        case 33: // WM_DESTROY message
            XUnloadFont(gpDisplay, pFontStruct->fid);
            XFreeGC(gpDisplay, gc);
            uninitialize();
            exit(EXIT_SUCCESS);
            break;

        default:
            break;
        }
    }

    return (0);
}

// -----------------------------------------------------------------------------
// Function: toggleFullScreen
// Desc    : Toggles the window between normal and fullscreen state
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
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom;

    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        False,
        SubstructureNotifyMask,
        &event);
}

// -----------------------------------------------------------------------------
// Function: uninitialize
// Desc    : Releases all allocated resources before program exit
// -----------------------------------------------------------------------------
void uninitialize(void)
{
    if (window)
        XDestroyWindow(gpDisplay, window);

    if (colormap)
        XFreeColormap(gpDisplay, colormap);

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }
}
