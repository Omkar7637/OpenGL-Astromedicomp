#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "OGL.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Display *gpDisplay = NULL;
XVisualInfo *visualInfo;
Window window;
Colormap colormap;
Bool bFullScreen = False;
FILE *gpFile = NULL;
Bool bActiveWindow = False;
GLXContext glxContext = NULL;

float angleCube = 0.0f;
float identityMatrix[16];
float translationMatrix[16];
float scaleMatrix[16];
float rotationMatrixX[16];
float rotationMatrixY[16];
float rotationMatrixZ[16];

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
        fprintf(gpFile, "glXChooseVisual failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "glXChooseVisual Succedded.\n");
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
        0, 0,
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
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | Load MultMatrix - Cube ");
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
        XRootWindow(gpDisplay, visualInfo->screen),
        False,
        SubstructureNotifyMask,
        &event);
}

int initialize(void)
{
    void printGLInfo();
    void resize(int, int);
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext);
    printGLInfo();

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    identityMatrix[0] = 1.0f; identityMatrix[1] = 0.0f; identityMatrix[2] = 0.0f; identityMatrix[3] = 0.0f;
    identityMatrix[4] = 0.0f; identityMatrix[5] = 1.0f; identityMatrix[6] = 0.0f; identityMatrix[7] = 0.0f;
    identityMatrix[8] = 0.0f; identityMatrix[9] = 0.0f; identityMatrix[10] = 1.0f; identityMatrix[11] = 0.0f;
    identityMatrix[12] = 0.0f; identityMatrix[13] = 0.0f; identityMatrix[14] = 0.0f; identityMatrix[15] = 1.0f;

    translationMatrix[0] = 1.0f; translationMatrix[1] = 0.0f; translationMatrix[2] = 0.0f; translationMatrix[3] = 0.0f;
    translationMatrix[4] = 0.0f; translationMatrix[5] = 1.0f; translationMatrix[6] = 0.0f; translationMatrix[7] = 0.0f;
    translationMatrix[8] = 0.0f; translationMatrix[9] = 0.0f; translationMatrix[10] = 1.0f; translationMatrix[11] = 0.0f;
    translationMatrix[12] = 0.0f; translationMatrix[13] = 0.0f; translationMatrix[14] = -6.0f; translationMatrix[15] = 1.0f;

    scaleMatrix[0] = 0.75f; scaleMatrix[1] = 0.0f; scaleMatrix[2] = 0.0f; scaleMatrix[3] = 0.0f;
    scaleMatrix[4] = 0.0f; scaleMatrix[5] = 0.75f; scaleMatrix[6] = 0.0f; scaleMatrix[7] = 0.0f;
    scaleMatrix[8] = 0.0f; scaleMatrix[9] = 0.0f; scaleMatrix[10] = 0.75f; scaleMatrix[11] = 0.0f;
    scaleMatrix[12] = 0.0f; scaleMatrix[13] = 0.0f; scaleMatrix[14] = 0.0f; scaleMatrix[15] = 1.0f;

    resize(WIN_WIDTH, WIN_HEIGHT);
    return (0);
}

void printGLInfo(void)
{
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version: %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}
void resize(int width, int height)
{
    if (height <= 0)
        height = 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(translationMatrix);
    glMultMatrixf(scaleMatrix);

    rotationMatrixX[0] = 1.0f;
    rotationMatrixX[1] = 0.0f;
    rotationMatrixX[2] = 0.0f;
    rotationMatrixX[3] = 0.0f;
    rotationMatrixX[4] = 0.0f;
    rotationMatrixX[5] = cosf(angleCube);
    rotationMatrixX[6] = sinf(angleCube);
    rotationMatrixX[7] = 0.0f;
    rotationMatrixX[8] = 0.0f;
    rotationMatrixX[9] = -sinf(angleCube);
    rotationMatrixX[10] = cosf(angleCube);
    rotationMatrixX[11] = 0.0f;
    rotationMatrixX[12] = 0.0f;
    rotationMatrixX[13] = 0.0f;
    rotationMatrixX[14] = 0.0f;
    rotationMatrixX[15] = 1.0f;

    rotationMatrixY[0] = cosf(angleCube);
    rotationMatrixY[1] = 0.0f;
    rotationMatrixY[2] = -sinf(angleCube);
    rotationMatrixY[3] = 0.0f;
    rotationMatrixY[4] = 0.0f;
    rotationMatrixY[5] = 1.0f;
    rotationMatrixY[6] = 0.0f;
    rotationMatrixY[7] = 0.0f;
    rotationMatrixY[8] = sinf(angleCube);
    rotationMatrixY[9] = 0.0f;
    rotationMatrixY[10] = cosf(angleCube);
    rotationMatrixY[11] = 0.0f;
    rotationMatrixY[12] = 0.0f;
    rotationMatrixY[13] = 0.0f;
    rotationMatrixY[14] = 0.0f;
    rotationMatrixY[15] = 1.0f;

    rotationMatrixZ[0] = cosf(angleCube);
    rotationMatrixZ[1] = sinf(angleCube);
    rotationMatrixZ[2] = 0.0f;
    rotationMatrixZ[3] = 0.0f;
    rotationMatrixZ[4] = -sinf(angleCube);
    rotationMatrixZ[5] = cosf(angleCube);
    rotationMatrixZ[6] = 0.0f;
    rotationMatrixZ[7] = 0.0f;
    rotationMatrixZ[8] = 0.0f;
    rotationMatrixZ[9] = 0.0f;
    rotationMatrixZ[10] = 1.0f;
    rotationMatrixZ[11] = 0.0f;
    rotationMatrixZ[12] = 0.0f;
    rotationMatrixZ[13] = 0.0f;
    rotationMatrixZ[14] = 0.0f;
    rotationMatrixZ[15] = 1.0f;

    glMultMatrixf(rotationMatrixX);
    glMultMatrixf(rotationMatrixY);
    glMultMatrixf(rotationMatrixZ);

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glEnd();
    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    angleCube = angleCube + 0.01f;
    if (angleCube >= 360.0f)
        angleCube = angleCube - 360.0f;
}

void uninitialize(void)
{
    if (bFullScreen == True)
    {
        toggleFullScreen();
        bFullScreen = False;
    }

    if (glxContext)
    {
        if (glXGetCurrentContext() == glxContext)
            glXMakeCurrent(gpDisplay, 0, 0);
        glXDestroyContext(gpDisplay, glxContext);
        glxContext = NULL;
    }

    if (window)
    {
        XDestroyWindow(gpDisplay, window);
        window = 0;
    }

    if (colormap)
    {
        XFreeColormap(gpDisplay, colormap);
        colormap = 0;
    }

    if (visualInfo)
    {
        XFree(visualInfo);
        visualInfo = NULL;
    }

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    if (gpFile)
    {
        fprintf(gpFile, "Log File Closed Successfully\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
