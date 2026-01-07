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

Display *gpDisplay = NULL;
XVisualInfo *visualInfo;
Window window;
Colormap colormap;
Bool bFullScreen = False;
FILE *gpFile = NULL;
Bool bActiveWindow = False;
GLXContext glxContext = NULL;
GLuint texture_smiley;
float angleCube = 0.0f;

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
        GLX_DEPTH_SIZE, 24, None
    };
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
        AllocNone
    );
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
    // variable declaration
    Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False); // XProtocol cha _NET mhnje network trasnsparent vapartoy mi
    Atom windowManagerFullscreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent event;

    memset((void *)&event, 0, sizeof(XEvent));
    // code

    event.type = ClientMessage;
    event.xclient.window = window; // tuzya event cha type konta client kadun janara msg, mg tyaala window pass kr
    event.xclient.message_type = windowManagerNormalStateAtom;
    event.xclient.format = 32;                                  // message 32 bit format cha ahe
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;              // data navacha union madhla long type cha data type, ani toh pn array ahe, if bFullscreen is true then l[0] is 0 else l[0] is 0
    event.xclient.data.l[1] = windowManagerFullscreenStateAtom; // apan tuala sangtoy mi asa ek event tayar kela jo 32 bit ahe, jo mazya state baddal sanganar ahe, ani jyacha union cha long variable madhe jr fullscreen asel tr toh l0 asel or l1 asel ani tyanusar toggle karycha ahe tula
    // send above event to XServer
    XSendEvent(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        False,                  // jr true asta tr message hyacha child event la propagate houde
        SubstructureNotifyMask, // not normal resize but special resize fullscreen, mhnun special mask ahe hite
        &event);
}

int initialize(void)
{
    // function declarations
    void printGLInfo();
    void resize(int, int);
    Bool loadGLTexture(GLuint *, const char *);

    // code
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True); // 3rd parameter asking ki tumhi konti context vapartay, share karatay ka individiual ahe, 4th ahe tumhi softeare rendering kartay ka hardware rendering, emulator madhe(software, rudimentary driver vr ) ashi context tayar karun de ji hardware rendering capability chi asel
    if (glxContext == NULL)
    {
        fprintf(gpFile, "glxCreateContext failed to create OpenGL context.\n");
        return (-1);
    }

    glXMakeCurrent(gpDisplay, window, glxContext); //

    // print GL Info
    printGLInfo();

    // depth cha lines hite yetil
    // Depth related Code : prerequisite of depth ani he kartan tumhala PixelFormatDescriptor enabled lagto mhun tumhi pfd la cDepthBits =32 mhnlela ahe
    glShadeModel(GL_SMOOTH); // 3D madhe color kartana mhnjch fragment cha v4 kartana smoothness cha v4 kr
    glClearDepth(1.0f);      // display madhe glClear la gelyavrti mazya depth buffer la ja ani saglyanichi value 1 kar asa sangto but ajun te zhala nahi
    glEnable(GL_DEPTH_TEST); // Depth test enable keli apan framebuffer cha 8 paiki
    glDepthFunc(GL_LEQUAL);  // L EQUAL : Code madhun apan konti test ahe te detoy, L EQUAL= tya tya fragment la pass kar jyanchi value mi vr dilelya 1.0 peksha Less than or EQUAL to 1 asel (LEQUAL:<=)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // load textures
    if (loadGLTexture(&texture_smiley, "Smiley.bmp") == False)
    {
        fprintf(gpFile, "loadGLTexture() failed to create Smiley image in Initialize\n");
        return (-6);
    }

    // Enable Texturing
    glEnable(GL_TEXTURE_2D);

    // Warmup resize
    resize(WIN_WIDTH, WIN_HEIGHT);

    return (0);
}

void printGLInfo(void)
{
    // code
    // Print OpenGL Information
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "******************\n");
    fprintf(gpFile, "OpenGL Vendor		: %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer	: %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version		: %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "******************\n");
}

// function for texture implementation
Bool loadGLTexture(GLuint *texture, const char *imageFilePath)
{
    // variable declarations
    int width, height;
    unsigned char *imageData = NULL;

    // code

    // Load the bitmap as image
    imageData = SOIL_load_image(imageFilePath, &width, &height, NULL, SOIL_LOAD_RGB);

    if (imageData == NULL)
    {
        fprintf(gpFile, "SOIL_load_image() is failed to get image data!!!");
    }

    // Generate OpenGL texture object
    glGenTextures(1, texture); // creates empty texture

    // bind to the empt structure object
    glBindTexture(GL_TEXTURE_2D, *texture);

    // unpack the image in memory for faster loading
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 1.tuza texture kut bnind kelay
    // 2.konta parameter dyacha ahe tula, MAG_FILTER: jevha scene madhla object magnified asel, jevha object motha hoil tevha high-quality texture pahije
    // 3.ata to Parameter GL_NEAREST or GL_LINEAR depend upon user need of performance or quality

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // 2.konta parameter dyacha ahe tula, MIN_FILTER: jevha scene madhla object minified asel, jevha object chota hoil tevha low-quality texture chalel
    // 3.object vr cha texture mipmap kr

    // glu Build kr 2d mipmaps, cannot use in PP
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, (const void *)imageData);
    // glTexImage2D & glMipmap used in PP
    // 1. image kute bind kelay
    // 2. kiti colors ahet image madhe RGB-3 RGBA-4
    // 3. width of img, which is in bmp.bmWidth struct
    // 4. ata height by bmp.bmHeight
    // 5. tumhi ji image denar ahe tyacha format kay ahe, GL_BGR_EXT ha microsoft cha Bitmap cha format ahe
    // 6. kutla type ahe image cha
    // 7. ata

    glBindTexture(GL_TEXTURE_2D, 0); // 0 dila ki tyala kalta unbind kela

    SOIL_free_image_data(imageData);

    return True;
}

void resize(int width, int height)
{
    /// If height by accident becomes 0 or <0 then make height 1
    if (height <= 0)
    {
        height = 1;
    }

    // Set the Viewport
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    // Window pahilyanda yetana resize hoitech, hite tumcha binoculor asa adjust kara ki to tumcha window cha barobr dimension la basel
    // Viewport match te window to left bottom of OpenGL, tuza 0,0 openglcha viewport shi map kar same for width, height
    // ekda tr call honarch window yetana

    // Set Matrix Projection Mode
    glMatrixMode(GL_PROJECTION); // MATRIX cha mode projection la thev

    // Set to identity Matrix
    glLoadIdentity(); // jya matrix cha mode tu atat set kela projection la tya matrix la identity matrix kr

    // Do Perspective Projection
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // we can use glFrustum here
    // gluPerspective(45.0f, FOV-Y Field of View Y, standard value of FOV-Y
    //  (GLfloat)width / (GLfloat)height, = Aspect Ratio
    //  0.1f, near wall
    //  100.0f); far wall
    // glFrustum has 6 parameters
}

void display(void)
{
    // code

    // Clear OpenGL Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // maza fakt color buffer clear kar jo initiazile madhe color varpla ahe tithun

    // Set Matrix to Model View Mode
    glMatrixMode(GL_MODELVIEW); // works for both triangle & rectangle

    // Triangle
    // Set into Identity Matrix
    glLoadIdentity(); // for triangle

    //Translate Triangle Backwards by Z 
	glTranslatef(0.0f, 0.0f, -3.0f);

	//binding the texture 
	glBindTexture(GL_TEXTURE_2D, texture_smiley);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	
	//unbinding the texture
	glBindTexture(GL_TEXTURE_2D, 0);
    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    // code
    angleCube = angleCube + 1.0f;
    if (angleCube >= 360.0f)
    {
        angleCube = angleCube - 360.0f;
    }
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

    if (texture_smiley)
	{
		glDeleteTextures(1, &texture_smiley);
		texture_smiley = 0;
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
