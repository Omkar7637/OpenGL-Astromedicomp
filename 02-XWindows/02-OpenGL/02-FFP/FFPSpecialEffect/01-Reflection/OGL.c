#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>
#include "OGL.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MAX_LIMIT_CUBE_TRANSLATE_X 8.0f
#define MIN_LIMIT_CUBE_TRANSLATE_X -8.0f
#define MAX_LIMIT_CUBE_TRANSLATE_Y 4.0f
#define MIN_LIMIT_CUBE_TRANSLATE_Y -4.0f
#define CLOSEST_LIMIT_CUBE_TRANSLATE_Z 5.0f
#define FARTHEST_LIMIT_CUBE_TRANSLATE_Z -20.0f
#define INCREMENT 0.1f
#define DECREMENT -0.1f

Display *gpDisplay = NULL;
XVisualInfo *visualInfo;
Window window;
Colormap colormap;
Bool bFullScreen = False;
FILE *gpFile = NULL;
Bool bActiveWindow = False;
GLXContext glxContext = NULL;
float angleCube = 0.0f;
GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.5f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPosition[] = {0.0f, 5.0f, 10.0f, 1.0f};
GLfloat materialAmbient[] = {0.5f, 0.5f, 0.5f, 1.5f};
GLfloat materialDiffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialShininess = 128.0f;
Bool bLight = False;
GLuint texture_marble = 0;
GLfloat translateCubeX = 0.0f;
GLfloat translateCubeY = 1.0f;
GLfloat translateCubeZ = 0.0f;
GLfloat cubeScale = 0.25f;

int main(void)
{
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);
    void toggleFullScreen(void);

    int defaultScreen;
    int defaultDepth;
    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;
    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];
    int framebufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        None};
    Bool bDone = False;

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

    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

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

    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen);
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        visualInfo->visual,
        AllocNone);
    colormap = windowAttributes.colormap;
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0,
        0,
        WIN_WIDTH,
        WIN_HEIGHT,
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

    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | Texture - Smiley ");
    XMapWindow(gpDisplay, window);
    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

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
                case XK_Right:
                    if (translateCubeX < MAX_LIMIT_CUBE_TRANSLATE_X)
                        translateCubeX += INCREMENT;
                    break;
                case XK_Left:
                    if (translateCubeX > MIN_LIMIT_CUBE_TRANSLATE_X)
                        translateCubeX += DECREMENT;
                    break;
                case XK_Up:
                    if (translateCubeY < MAX_LIMIT_CUBE_TRANSLATE_Y)
                        translateCubeY += INCREMENT;
                    break;
                case XK_Down:
                    if (translateCubeY > MIN_LIMIT_CUBE_TRANSLATE_Y)
                        translateCubeY += DECREMENT;
                    break;
                case XK_KP_Add:
                    if (translateCubeZ < CLOSEST_LIMIT_CUBE_TRANSLATE_Z)
                        translateCubeZ += INCREMENT;
                    break;
                case XK_KP_Subtract:
                    if (translateCubeZ > FARTHEST_LIMIT_CUBE_TRANSLATE_Z)
                        translateCubeZ += DECREMENT;
                    break;
                default:
                    break;
                }

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
            case 33:
                bDone = True;
                break;
            default:
                break;
            }
        }
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }
    uninitialize();
}
