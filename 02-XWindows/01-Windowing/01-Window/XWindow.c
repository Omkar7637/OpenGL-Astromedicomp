// RTR2024 - 150 Omkar Kashid
// ------------------------------------------------------------
// XWindows (Xlib) Example Program
// This program demonstrates how to create a simple X11 window
// using low-level Xlib APIs in Linux environments.
// ------------------------------------------------------------

// Standard Header files
#include <stdio.h>   // For printf (used here for error messages)
#include <stdlib.h>  // For exit()
#include <memory.h>  // For memset()

// XLib related header files
#include <X11/Xlib.h>   // Contains all Xlib APIs (similar to <windows.h> in Win32)
#include <X11/Xutil.h>  // Utility functions for window handling and visuals

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// ------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------
Display *gpDisplay = NULL; // Pointer to the connection between XClient and XServer
XVisualInfo visualInfo;    // Describes display hardware properties (similar to HDC)
Window window;             // Represents an on-screen window object (like HWND)
Colormap colormap;         // Hardware-level color mapping table (RGB intensity data)

// ------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------
void uninitialize(void);

// ------------------------------------------------------------
// Entry Point - main()
// ------------------------------------------------------------
int main(void)
{
    // Local variable declarations
    int defaultScreen;
    int defaultDepth;
    Status status;
    XSetWindowAttributes windowAttributes; // Used for setting window parameters
    Atom windowManagerDeleteAtom;          // Atom used for graceful window close
    XEvent event;                          // Represents events like key press, close, etc.

    // ------------------------------------------------------------
    // Step 1: Open connection with XServer
    // ------------------------------------------------------------
    gpDisplay = XOpenDisplay(NULL); // NULL → use default display
    if (gpDisplay == NULL)
    {
        printf("XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------------
    // Step 2: Get default screen and depth
    // ------------------------------------------------------------
    defaultScreen = XDefaultScreen(gpDisplay); // Get the screen number
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen); // Get color depth (bits per pixel)

    // ------------------------------------------------------------
    // Step 3: Match visual information (similar to choosing pixel format)
    // ------------------------------------------------------------
    memset((void *)&visualInfo, 0, sizeof(XVisualInfo)); // Initialize struct to zero
    status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, &visualInfo);
    if (status == 0)
    {
        printf("XMatchVisualInfo failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------------
    // Step 4: Set window attributes
    // ------------------------------------------------------------
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0; // No border color
    windowAttributes.background_pixmap = 0; // No background image
    windowAttributes.background_pixel = XWhitePixel(gpDisplay, visualInfo.screen); // White background
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        visualInfo.visual,
        AllocNone); // Don't pre-allocate color cells

    colormap = windowAttributes.colormap; // Store for cleanup later

    // ------------------------------------------------------------
    // Step 5: Create the actual window
    // ------------------------------------------------------------
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo.screen),
        0, // X position
        0, // Y position
        WIN_WIDTH,
        WIN_HEIGHT,
        0,                // Border width
        visualInfo.depth, // Bit depth
        InputOutput,      // Type of window
        visualInfo.visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap, // Attributes mask
        &windowAttributes);

    if (!window)
    {
        printf("XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------------
    // Step 6: Setup communication for window close event
    // ------------------------------------------------------------
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

    // ------------------------------------------------------------
    // Step 7: Set window title
    // ------------------------------------------------------------
    XStoreName(gpDisplay, window, "RTR2024 - 150 Omkar Kashid | X Window");

    // ------------------------------------------------------------
    // Step 8: Map the window (make it visible)
    // ------------------------------------------------------------
    XMapWindow(gpDisplay, window);

    // ------------------------------------------------------------
    // Step 9: Event Loop (Message Pump)
    // ------------------------------------------------------------
    while (1)
    {
        XNextEvent(gpDisplay, &event); // Blocks until event is received

        switch (event.type)
        {
        case 33: // 33 corresponds to 'WM_DELETE_WINDOW'
            uninitialize();
            exit(EXIT_SUCCESS);
            break;

        default:
            // Other events like KeyPress, MouseMove can be handled here
            break;
        }
    }

    return 0;
}

// ------------------------------------------------------------
// Function: uninitialize()
// Purpose: Clean up allocated resources gracefully
// ------------------------------------------------------------
void uninitialize(void)
{
    // Destroy the window if it exists
    if (window)
    {
        XDestroyWindow(gpDisplay, window);
    }

    // Free the colormap (hardware resource)
    if (colormap)
    {
        XFreeColormap(gpDisplay, colormap);
    }

    // Close the display connection to XServer
    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }
}
