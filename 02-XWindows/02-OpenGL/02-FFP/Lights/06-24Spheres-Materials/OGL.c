#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
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
GLUquadric *quadric = NULL;
Bool bLight = False;

GLfloat lightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPosition[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightModelAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat lightModelLocalViewer[] = {0.0f};
GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;
GLint keyPressed = -1;

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
        GLX_DEPTH_SIZE, 24, None};
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
        fprintf(gpFile, "glXChooseVisual Succeeded.\n");
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
        0, 0, WIN_WIDTH, WIN_HEIGHT,
        0, visualInfo->depth, InputOutput, visualInfo->visual,
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
    XStoreName(gpDisplay, window, "RTR2024-150 Omkar Kashid | 3 Moving Lights on Static Sphere ");
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
                case 'L':
                case 'l':
                    if (bLight == False)
                    {
                        bLight = True;
                        glEnable(GL_LIGHTING);
                    }
                    else
                    {
                        bLight = False;
                        glDisable(GL_LIGHTING);
                    }
                    break;
                case 'X':
                case 'x':
                    keyPressed = 1;
                    angleForXRotation = 0.0f;
                    break;
                case 'Y':
                case 'y':
                    keyPressed = 2;
                    angleForYRotation = 0.0f;
                    break;
                case 'Z':
                case 'z':
                    keyPressed = 3;
                    angleForZRotation = 0.0f;
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

    quadric = gluNewQuadric();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lightModelLocalViewer);
    resize(WIN_WIDTH, WIN_HEIGHT);
    return (0);
}

void printGLInfo()
{
    GLint numExtensions = 0;
    fprintf(gpFile, "\nOpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version  : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version    : %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    for (int i = 0; i < numExtensions; i++)
        fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
}

void resize(int width, int height)
{
    if (height == 0)
        height = 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}
void display(void)
{
    //function declarations
	void draw24Spheres(void);

	//code

	//Clear OpenGL Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set Matrix to Model View Mode
	glMatrixMode(GL_MODELVIEW);

	//Set into Identity Matrix
	glLoadIdentity();
	
	if (keyPressed == 1)
	{
		glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotation;
	}
	else if (keyPressed == 2)
	{
		glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotation;
	}
	else if (keyPressed == 3)
	{
		glRotatef(angleForZRotation, 0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotation;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_AUTO_NORMAL); //jr tumhi surface la normal dilelya nastil tr toh automatically normal deto
	glEnable(GL_NORMALIZE);

	//draw24Spheres
	draw24Spheres();

	gluSphere(quadric, 0.90f, 500, 500);
    glXSwapBuffers(gpDisplay, window);
}

void draw24Spheres(void)
{
	//variable declarations
	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	//code
	
	//1st Sphere of 1st Column : Emerald
	materialAmbient[0] = 0.0215f;
	materialAmbient[1] = 0.1745f;
	materialAmbient[2] = 0.0215f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.07568f;
	materialDiffuse[1] = 0.61424f;
	materialDiffuse[2] = 0.07568f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.633f;
	materialSpecular[1] = 0.727811;
	materialSpecular[2] = 0.633;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//2nd Sphere of 1st Column : Jade
	materialAmbient[0] = 0.0135f;
	materialAmbient[1] = 0.2225f;
	materialAmbient[2] = 0.1575f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.54f;
	materialDiffuse[1] = 0.89f;
	materialDiffuse[2] = 0.63f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.316228f;
	materialSpecular[1] = 0.316228f;
	materialSpecular[2] = 0.316228f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f - (2.5f * 1), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	

	//3rd Sphere of 1st Column : Obsidian
	materialAmbient[0] = 0.0135f;
	materialAmbient[1] = 0.2225f;
	materialAmbient[2] = 0.1575f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.54f;
	materialDiffuse[1] = 0.89f;
	materialDiffuse[2] = 0.63f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.316228f;
	materialSpecular[1] = 0.316228f;
	materialSpecular[2] = 0.316228f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.3f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f - (2.5f * 2), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//4th Sphere of 1st Column : Pearl
	materialAmbient[0] = 0.25f;
	materialAmbient[1] = 0.20725f;
	materialAmbient[2] = 0.20725;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 1.0f;
	materialDiffuse[1] = 0.829f;
	materialDiffuse[2] = 0.829f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.296648f;
	materialSpecular[1] = 0.296648f;
	materialSpecular[2] = 0.296648f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.088f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//5th Sphere of 1st Column : Ruby
	materialAmbient[0] = 0.1745f;
	materialAmbient[1] = 0.01175f;
	materialAmbient[2] = 0.01175f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.61424f;
	materialDiffuse[1] = 0.04136f;
	materialDiffuse[2] = 0.04136f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.727811f;
	materialSpecular[1] = 0.626959f;
	materialSpecular[2] = 0.626959f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//6th Sphere of 1st Column : Turquoise
	materialAmbient[0] = 0.1f;
	materialAmbient[1] = 0.18725f;
	materialAmbient[2] = 0.1745f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.396f;
	materialDiffuse[1] = 0.74151f;
	materialDiffuse[2] = 0.69102f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.297254f;
	materialSpecular[1] = 0.30829f;
	materialSpecular[2] = 0.306678f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f - (2.5f * 4), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//*********************************************************
	//*********************************************************
	//1st Sphere of 2nd Column : Brass
	materialAmbient[0] = 0.329412f;
	materialAmbient[1] = 0.223529f;
	materialAmbient[2] = 0.027451f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.780392f;
	materialDiffuse[1] = 0.568627f;
	materialDiffuse[2] = 0.113725f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.992157f;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.21794872f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 1), 14.0f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//2nd Sphere of 1st Column : Bronze
	materialAmbient[0] = 0.2125f;
	materialAmbient[1] = 0.1275f;
	materialAmbient[2] = 0.054f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.714f;
	materialDiffuse[1] = 0.4284f;
	materialDiffuse[2] = 0.18144f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.393548f;
	materialSpecular[1] = 0.271906f;
	materialSpecular[2] = 0.166721f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.2f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 1), 14.0f - (2.5f * 1), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);


	//3rd Sphere of 1st Column : Chrome
	materialAmbient[0] = 0.25f;
	materialAmbient[1] = 0.25f;
	materialAmbient[2] = 0.25f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4f;
	materialDiffuse[1] = 0.4f;
	materialDiffuse[2] = 0.4f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.774597f;
	materialSpecular[1] = 0.774597f;
	materialSpecular[2] = 0.774597f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 1), 14.0f - (2.5f * 2), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//4th Sphere of 2nd Column : Copper
	materialAmbient[0] = 0.19125f;
	materialAmbient[1] = 0.0735f;
	materialAmbient[2] = 0.0225;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.7038f;
	materialDiffuse[1] = 0.27048f;
	materialDiffuse[2] = 0.0828f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.256777f;
	materialSpecular[1] = 0.137622f;
	materialSpecular[2] = 0.086014f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 1), 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//5th Sphere of 2nd Column : Gold
	materialAmbient[0] = 0.24725f;
	materialAmbient[1] = 0.1995f;
	materialAmbient[2] = 0.0745f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.75164f;
	materialDiffuse[1] = 0.60648f;
	materialDiffuse[2] = 0.22648f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.628281f;
	materialSpecular[1] = 0.555802f;
	materialSpecular[2] = 0.366065f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 1), 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//6th Sphere of 2nd Column : Silver
	materialAmbient[0] = 0.19225f;
	materialAmbient[1] = 0.19225f;
	materialAmbient[2] = 0.19225f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.50754f;
	materialDiffuse[1] = 0.50754f;
	materialDiffuse[2] = 0.50754f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.508273f;
	materialSpecular[1] = 0.508273f;
	materialSpecular[2] = 0.508273f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 1), 14.0f - (2.5f * 4), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//*********************************************************
	//*********************************************************
	//1st Sphere of 3rd Column : Black
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.01f;
	materialDiffuse[1] = 0.01f;
	materialDiffuse[2] = 0.01f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.50f;
	materialSpecular[1] = 0.50f;
	materialSpecular[2] = 0.50f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 2), 14.0f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//2nd Sphere of 3rd Column : Cyan
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.1f;
	materialAmbient[2] = 0.06f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.0f;
	materialDiffuse[1] = 0.50980392f;
	materialDiffuse[2] = 0.50980392f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.50196078f;
	materialSpecular[1] = 0.50196078f;
	materialSpecular[2] = 0.50196078f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 2), 14.0f - (2.5f * 1), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);


	//3rd Sphere of 3rd Column : Green
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.1f;
	materialDiffuse[1] = 0.35f;
	materialDiffuse[2] = 0.1f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.45f;
	materialSpecular[1] = 0.55f;
	materialSpecular[2] = 0.45f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 2), 14.0f - (2.5f * 2), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//4th Sphere of 3rd Column : Red
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5f;
	materialDiffuse[1] = 0.0f;
	materialDiffuse[2] = 0.0f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.6f;
	materialSpecular[2] = 0.6f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 2), 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//5th Sphere of 3rd Column : White
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.55f;
	materialDiffuse[1] = 0.55f;
	materialDiffuse[2] = 0.55f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.70f;
	materialSpecular[1] = 0.70f;
	materialSpecular[2] = 0.70f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 2), 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//6th Sphere of 3rd Column : Yellow
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5f;
	materialDiffuse[1] = 0.5f;
	materialDiffuse[2] = 0.0f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.60f;
	materialSpecular[1] = 0.60f;
	materialSpecular[2] = 0.50f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 2), 14.0f - (2.5f * 4), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//*********************************************************
	//*********************************************************
	//1st Sphere of 4th Column : Black
	materialAmbient[0] = 0.02f;
	materialAmbient[1] = 0.02f;
	materialAmbient[2] = 0.02f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.01f;
	materialDiffuse[1] = 0.01f;
	materialDiffuse[2] = 0.01f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.4f;
	materialSpecular[1] = 0.4f;
	materialSpecular[2] = 0.4f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 3), 14.0f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//2nd Sphere of 3rd Column : Cyan
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.05f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4f;
	materialDiffuse[1] = 0.5f;
	materialDiffuse[2] = 0.5f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.04f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.7f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 3), 14.0f - (2.5f * 1), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);


	//3rd Sphere of 4th Column : Green
	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.4f;
	materialDiffuse[1] = 0.5f;
	materialDiffuse[2] = 0.4f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.04f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.04f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 3), 14.0f - (2.5f * 2), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//4th Sphere of 4th Column : Red
	materialAmbient[0] = 0.05f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5f;
	materialDiffuse[1] = 0.4f;
	materialDiffuse[2] = 0.4f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.04f;
	materialSpecular[2] = 0.04f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 3), 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//5th Sphere of 4th Column : White
	materialAmbient[0] = 0.05f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.05f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5f;
	materialDiffuse[1] = 0.5f;
	materialDiffuse[2] = 0.5f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.7f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 3), 14.0f - (2.5f * 3), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	//6th Sphere of 3rd Column : Yellow
	materialAmbient[0] = 0.05f;
	materialAmbient[1] = 0.05f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.5f;
	materialDiffuse[1] = 0.5f;
	materialDiffuse[2] = 0.4f;
	materialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.7f;
	materialSpecular[1] = 0.7f;
	materialSpecular[2] = 0.04f;
	materialSpecular[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f + (8.0f * 3), 14.0f - (2.5f * 4), 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

}

void update(void)
{
    // code
    angleForXRotation = angleForXRotation + 1.0f;
	// if (angleForXRotation >= 360.0f)
	// {
	// 	angleForXRotation = angleForXRotation - 360.0f;
	// }

	angleForYRotation = angleForYRotation + 1.0f;
	// if (angleForYRotation >= 360.0f)
	// {
	// 	angleForYRotation = angleForYRotation - 360.0f;
	// }

	angleForZRotation = angleForZRotation + 1.0f;
	// if (angleForZRotation >= 360.0f)
	// {
	// 	angleForZRotation = angleForZRotation - 360.0f;
	// }
}

void uninitialize(void)
{
    // code
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

    if (colormap) // it is hardware resource so, colormap by convention should be declared globally and it should be freed though it's structure.
    {
        XFreeColormap(gpDisplay, colormap);
    }

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    // gpfile
    // Close the file
    if (gpFile)
    {
        fprintf(gpFile, "Program terminated Successfully !!!");
        fclose(gpFile);
        gpFile = NULL;
    }
}
