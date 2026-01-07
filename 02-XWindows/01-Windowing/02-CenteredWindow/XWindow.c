// RTR2024 - 150 Omkar Kashid
// ------------------------------------------------------------
// XWindows (Xlib) Program : Centered Window Example
// This program demonstrates how to create a simple X11 window,
// center it on the screen, and handle the close event.
// ------------------------------------------------------------

// Standard Header files
#include <stdio.h>   // For printf (later can be replaced by fprintf for logs)
#include <stdlib.h>  // For exit()
#include <memory.h>  // For memset()

// XLib related header files
#include <X11/Xlib.h>   // Equivalent to <windows.h> in Win32; contains Xlib APIs
#include <X11/Xutil.h>  // Utility functions and structures for XWindow handling

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// ------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------
Display *gpDisplay = NULL; // Interface between XClient (our app) and XServer
XVisualInfo visualInfo;    // Structure containing information about visual types
Window window;             // Represents a window on the screen (similar to HWND)
Colormap colormap;         // Hardware-level table defining how colors are stored

// ------------------------------------------------------------
// Function Declaration
// ------------------------------------------------------------
void uninitialize(void);

// ------------------------------------------------------------
// Entry point: main()
// ------------------------------------------------------------
int main(void)
{
    // Local variable declarations
    int defaultScreen;
    int defaultDepth;
    Status status;
    XSetWindowAttributes windowAttributes; // Used to set visual and color properties
    Atom windowManagerDeleteAtom;          // Used for handling window close event
    XEvent event;                          // Structure to hold event data
    Screen *screen = NULL;                 // Represents a screen of the display
    int screenWidth, screenHeight;

    // ------------------------------------------------------------
    // Step 1: Open connection to the XServer
    // ------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL); // NULL → use default display (like localhost:0)
    if (gpDisplay == NULL)
    {
        printf("XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------------
    // Step 2: Get default screen and depth
    // ------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay); // Retrieve screen number
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen); // Get bits per pixel

    // ------------------------------------------------------------
    // Step 3: Match visual info (similar to choosing pixel format in Win32)
    // ------------------------------------------------------------
    memset((void *)&visualInfo, 0, sizeof(XVisualInfo)); // Initialize struct
    status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, &visualInfo);
    if (status == 0)
    {
        printf("XMatchVisualInfo failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------------
    // Step 4: Set window attributes (background, border, colormap)
    // ------------------------------------------------------------
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0; // No border color
    windowAttributes.background_pixmap = 0; // No image background
    windowAttributes.background_pixel = XWhitePixel(gpDisplay, visualInfo.screen); // White background
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        visualInfo.visual,
        AllocNone); // Don't allocate cells initially

    colormap = windowAttributes.colormap; // Save for cleanup later

    // ------------------------------------------------------------
    // Step 5: Create the window
    // ------------------------------------------------------------
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        0, // Initial X position
        0, // Initial Y position
        WIN_WIDTH,
        WIN_HEIGHT,
        0,                // Border width
        visualInfo.depth, // Depth
        InputOutput,      // Type of window (input + output)
        visualInfo.visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap, // Attribute mask
        &windowAttributes);

    if (!window)
    {
        printf("XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------------
    // Step 6: Register "close window" protocol
    // ------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // ------------------------------------------------------------
    // Step 7: Set window title
    // ------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024 - 150 Omkar Kashid | Centering of Window");

    // ------------------------------------------------------------
    // Step 8: Map window (make visible)
    // ------------------------------------------------------------
    XMapWindow(gpDisplay, window);

    // ------------------------------------------------------------
    // Step 9: Center the window on the screen
    // ------------------------------------------------------------
    screen = XScreenOfDisplay(gpDisplay, visualInfo.screen); // Get screen pointer
    screenWidth = XWidthOfScreen(screen);   // Get screen width
    screenHeight = XHeightOfScreen(screen); // Get screen height

    // Move window to center position
    XMoveWindow(gpDisplay, window,
                (screenWidth / 2) - (WIN_WIDTH / 2),
                (screenHeight / 2) - (WIN_HEIGHT / 2));

    // ------------------------------------------------------------
    // Step 10: Event Loop (like GetMessage in Win32)
    // ------------------------------------------------------------
    while (1)
    {
        XNextEvent(gpDisplay, &event); // Wait for next event in queue

        switch (event.type)
        {
        case 33: // 33 is "WM_DELETE_WINDOW" event code
            uninitialize();
            exit(EXIT_SUCCESS);
            break;

        default:
            // Other events (e.g., keypress, resize) can be handled here
            break;
        }
    }

    return (0);
}

// ------------------------------------------------------------
// Function: uninitialize()
// Purpose : Release all allocated XWindow resources
// ------------------------------------------------------------
void uninitialize(void)
{
    // Destroy window if created
    if (window)
    {
        XDestroyWindow(gpDisplay, window);
    }

    // Free colormap (hardware resource)
    if (colormap)
    {
        XFreeColormap(gpDisplay, colormap);
    }

    // Close the display connection
    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }
}
