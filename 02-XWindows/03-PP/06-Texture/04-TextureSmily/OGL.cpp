// Standard Header files
#include <stdio.h>  //to use printf later convert into fprintf
#include <stdlib.h> // for exit()
#include <iostream>
#include <string.h>
#include <filesystem>
#include <memory.h> //for memset

// XLib related header files
#include <X11/Xlib.h>   // Win32 chi windows.h ani hikadchi xlib.h for all xlib APIs
#include <X11/Xutil.h>  // For Visual info and related API
#include <X11/XKBlib.h> // For Keyboard related XLib APIs (not inluded something like this in windows)

// Texture library header
#include <SOIL/SOIL.h>

// OpenGL related header files
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h> // needed for gluPerspective (we keep original usage)
#include <GL/glx.h>

// Custom Header Files
#include "OGL.h"
#include "vmath.h"
using namespace vmath;

namespace fs = std::filesystem;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL;      // global pointer display, Display ha 77 members ch strucutre ahe jyat anek members swatah structure ahe, it is actually interface between XServer and XClient
XVisualInfo *visualInfo = NULL; // Xvisual info he HDC madhlua DC sarkha ahe Device Context, device chi avastha sangnara, device context bhetto, structure within structure ahe, hardware(graphics card cha visual capacity baddal sangnara) cha status baddal denara data structure
Window window;                  // window ha object ahe Xwindow cha, hwnd in win32, window la represent karto ha object
Colormap colormap;              // Graphic card madhe colors eka certain memory madhe arrange kele asta tya arrangement la xwindow colormap asa mhnta, ani hy map madhe every cell la color cell mhntat ani every cell RGB chi intensity sangte, he hardware data structre ahe mhnun he resource ahe
Bool bFullScreen = False;
FILE *gpFile = NULL;

// Shader related global variables
GLuint shaderProgramObject = 0;

Bool bActiveWindow = False; // in Win32

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;

// OpenGL related variables
GLXContext glxContext = NULL;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_TEXCOORD,
};

GLuint vao_smiley = 0;
GLuint vao_pyramid = 0;

GLuint vbo_position_smiley = 0;
GLuint vbo_position_pyramid = 0;

GLuint vbo_texcoord_smiley = 0;
GLuint vbo_texcoord_pyramid = 0;

GLuint mvpMatrixUniform = 0;
mat4 perspectiveProjectionMatrix; // matrix 4x4

GLuint textureSmiley = 0;

GLuint textureSamplerUniform = 0;

GLfloat angleCube = 0.0f;
GLfloat angleCubeCnt = 0.0f;

GLfloat anglePyramid = 0.0f;
GLfloat anglePyramidCnt = 0.0f;

int main(void)
{
    // function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void update(void);
    void uninitialize(void);
    void toggleFullScreen(void);

    // variable declarations
    int defaultScreen;
    int defaultDepth;

    XSetWindowAttributes windowAttributes; // struct
    Atom windowManagerDeleteAtom;
    XEvent event; // 33 member data structure

    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym; // KeySymbol sathi ch structure
    char keys[26]; // although we need only 0th index convenentionally the array size should be no. of alphabets(Capital or small or both)

    GLXFBConfig *pGLXConfigs = NULL;
    GLXFBConfig bestGLXFBConfig;
    XVisualInfo *pXVisualInfo = NULL;
    int iNumFBConfigs;

    int framebufferAttributes[] = {
        // Win32 madhe pfd ahe, asha prakar cha property denara array nehhmi 0 ne sampaavycha, tyala sequence samjat nahi,
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_DOUBLEBUFFER, True,
        // GLX_RGBA,
        GLX_RED_SIZE, 8, // PFD MADHE REDBITS
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
		None // None means 0 in XLib
    };
    Bool bDone = False;

    // code
    // Create Log File
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

    // Open Display connection
    gpDisplay = XOpenDisplay(NULL); // Step 1 : Client Server shi connection start karto ani null he display ch naav ahe
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // create the default screen object
    defaultScreen = XDefaultScreen(gpDisplay); // tuzya kade ji default screen ahe ti return kar ani tyacha number mala return kr defaultScreen madhe

    // get Default Depth
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen); // every api starts with X ani 1st param is gpDisplay

    pGLXConfigs = glXChooseFBConfig(gpDisplay, defaultScreen, framebufferAttributes, &iNumFBConfigs);
    if (pGLXConfigs == NULL)
    {
        fprintf(gpFile, "glXChooseFBConfig() failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "Found no. of FBConfig = %d\n", iNumFBConfigs);
    }

    int indexOfBestFBConfig = -1, indexOfWorstFBConfig = -1;
    int bestNumberOfSamples = -1, worstNumberOfSamples = 999;

    for (int i = 0; i < iNumFBConfigs; i++)
    {
        pXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXConfigs[i]);
        if (pXVisualInfo)
        {
            int sampleBuffer = 0, samples = 0;
            glXGetFBConfigAttrib(gpDisplay, pGLXConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
            glXGetFBConfigAttrib(gpDisplay, pGLXConfigs[i], GLX_SAMPLES, &samples);

            if (indexOfBestFBConfig < 0 || (sampleBuffer && samples > bestNumberOfSamples))
            {
                indexOfBestFBConfig = i;
                bestNumberOfSamples = samples;
            }

            if (indexOfWorstFBConfig < 0 || sampleBuffer || samples < worstNumberOfSamples)
            {
                indexOfWorstFBConfig = i;
                worstNumberOfSamples = samples;
            }
            XFree(pXVisualInfo);
        }
    }

    bestGLXFBConfig = pGLXConfigs[indexOfBestFBConfig];

    // Set the global glxFBConfig step 3
    glxFBConfig = bestGLXFBConfig;

    XFree(pGLXConfigs);

    visualInfo = glXGetVisualFromFBConfig(gpDisplay, glxFBConfig);

    // set window attributes/properties
    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));

    // defining the structure
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;                                         // PPM image is PortablePixMap in textures
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen); // we have 1 screen represnted by 2 data structure: 1 by default screen and 2 by visualInfo.screen
    windowAttributes.colormap = XCreateColormap(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        visualInfo->visual,
        AllocNone // whether to give memory to visual info, allocnone means no
    );
    colormap = windowAttributes.colormap;

    // Setting Event mask for the messages
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    // create the window
    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0, // X
        0, // Y
        WIN_WIDTH,
        WIN_HEIGHT,
        0,                 // border width mhnje mi det nahi tu de
        visualInfo->depth, // depth
        InputOutput,
        visualInfo->visual,
        CWBorderPixel | CWBackPixel | CWEventMask | CWColormap, // CW:CreateWindow, eventmask event yenar ahet window la
        &windowAttributes);

    if (!window)
    {
        fprintf(gpFile, "XCreateWindow failed.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    // create atom for window manager to destroy the window
    windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True); // WM_DELETE_WINDOW ha macro Xprotocol madhe ahe, True mhnje
    XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);            // window manager sathi protocol tayar kar

    // Set Window title
    fs::path current = fs::current_path();
    std::string foldernameStr = current.filename().string();
    std::string combinedStr = "Omkar Kashid | Xwindows : " + foldernameStr;
    const char *windowname = combinedStr.c_str();
    
    XStoreName(gpDisplay, window, windowname);

    // map the window to show it
    XMapWindow(gpDisplay, window);

    // Centering of Window
    screen       = XScreenOfDisplay(gpDisplay, visualInfo->screen); // kontya display sathhi data structure pahije, tyatle konta screen number sathi pahije
    screenWidth  = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2); // centering of window

    // initialize
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

    // Game Loop
    while (bDone == False)
    {
        while (XPending(gpDisplay)) // jo paryant bDOne Null ahe to paryant aat yet, aat alyvr jo Paryant XPending kahitari non-zero return kartay, XPending he Xlib mfunction message queue madhe asnare no. of event swhich are not yet processed jevha 0 hoil tevha kutla hi message pending nahi, jo paryant return karat nahi toparyant aat yenar, mhnje queue madhe event yenar, ani next event cha ch call ahe, ani mg pudhe apli switch case run honar, ani win32 madhe getmessage kade message nahi tevha tumcha animation hoila pahije,  tasa hikde XPending kade message nahi tevha tumcha animation hoila pahije, mhnjech XPending cha while che closing curly brace nantr, ani bDone cha closing brace cha andhi hite aplaya display ani update cha block yenar ahe, hite WndProc nahi na, hikde sagla asach ahe
        {
            XNextEvent(gpDisplay, &event); // GetMessage, queue rikhama asel tr gap basun rahta
            switch (event.type)
            {
            case MapNotify:
                break;
            case FocusIn:
                bActiveWindow = True; // Win32 madhe SetFocus hitla
                break;
            case FocusOut:
                bActiveWindow = False; // Win32 madhe KillFocus hitla
                break;
            case ConfigureNotify:
                resize(event.xconfigure.width, event.xconfigure.height); // win32 madhe loword of wparam,
                break;
            case KeyPress:
                // for escape key
                keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0); // he function aplya user ne press kelelya key mule alelya event madhlya Xkeystructure madhlya keycode la jata ani key sym madhe convert karta, ani last params key combinations dabto tevha 3rd param ani jr keycode combi madhe jr shift asel tr 4th param use kela jato
                switch (keySym)
                {
                case XK_Escape: // VK_ESC
                    bDone = True;
                    break;
                default:
                    break;
                }

                // for alphabetic keypress
                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL); // apan sangoty X windows la keypress cha aatmadhe kahi string yenar ahet tr tyat bg, event madhlya xkey navacha structure cha address detoy lookup string la, ani sangotyk ki hya structure madhlya strings madhe bag ani mala keys navacha array bharun de jyacha size ahe sizeof keys, ani NULL tevha lagto jevha lookup stirng multiple messages madhe vaprla tr tya lookup string chi state save karyala, ani mg NULL vaparto jevha hyaccha state jr ka tumhala pudhca event cha propagation sathi vaprayaccha aselt tr XcomposeStatus asa mhnta ani tyacha parameter pointer hite asta.
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
                case 'A':
                    angleCubeCnt += 0.5f;
                    anglePyramidCnt += 0.5f;
                    break;
                case 'a':
                    angleCubeCnt -= 0.5f;
                    anglePyramidCnt -= 0.5f;
                    break;
                default:
                    break;
                }
                break;
            case ButtonPress: // for mouse
                break;
            case Expose: // for WM_PAINT
                break;

            case 33:          // ha 33 WM_DESTROY ahe ha, ani ha WindowManager neutral ahe saglikade
                bDone = True; //
                break;
            default:
                break;
            }
        }
        // rendering
        if (bActiveWindow == True)
        {
            display();
            update();
        }
    }
    uninitialize();
    return (0);
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

// Initialize GL, shaders, buffers, textures
int initialize(void)
{
    // function declarations
    void printGLInfo();
    void resize(int, int);
    void uninitialize(void);
    Bool loadGLTexture(GLuint * texture,  char * fileImagePath);

    GLenum glewResult;

    // code
    // get function pointer
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");

    // Declare attribs array
    GLint attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 5,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None};

    glxContext = glXCreateContextAttribsARB(gpDisplay, glxFBConfig, 0, True, attribs);
    if (!glxContext)
    {
        GLint attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
                GLX_CONTEXT_MINOR_VERSION_ARB, 5,
                GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
                None};

        glxContext = glXCreateContextAttribsARB(gpDisplay, glxFBConfig, 0, True, attribs);
        fprintf(gpFile, "Cannot get OpenGL 4.5, but received lesser context\n");
    }
    else
    {
        fprintf(gpFile, "Received OpenGL Context 4.5\n");
    }

    if (!glXMakeCurrent(gpDisplay, window, glxContext))
    {
        fprintf(gpFile, "glXMakeCurrent failed\n");
        return -1;
    }

    // Initialize GLEW
    glewResult = glewInit();
    if (glewResult != GLEW_OK)
    {
        fprintf(gpFile, "glewInit() failed: %s\n", glewGetErrorString(glewResult));
        return (-6);
    }
	
	// Print GL info
	printGLInfo();

	// Steps
	// 1] Write the shader source code
	// 2] Create the shader object
	// 3] Give the shader source to the shader object
	// 4] Compile the shader programatically
	// 5] Do shader compilation error checking

    const GLchar *vertexShaderSourceCode =
        "#version 460 core\n" \
        "in vec4 aPosition;\n" \
        "in vec2 aTexCoord;\n" \
        "out vec2 out_TexCoord;\n" \
        "uniform mat4 uMVPMatrix;\n" \
        "void main(void)\n" \
        "{\n" \
        "gl_Position = uMVPMatrix*aPosition;\n" \
        "out_TexCoord = aTexCoord;\n" \
        "}\n";

    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);
    glCompileShader(vertexShaderObject);

    GLint status = 0;
    GLint infoLogLength = 0;
    GLchar *szInfoLog = NULL;

    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar *)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
                fprintf(gpFile, "Vertex Shader Compilation Log = %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;
            }
        }
        uninitialize();
        return -1;
    }

    // FRAGMENT SHADER
    /*
    1. Write the shader source code.
    2. Create the shader object.
    3. Give the shader source code to the shader object.
    4. Compile the shader.
    5. Do Shader compiler error checking.
    */

	// ================ START OF FRAGMENT SHADER ================
    const GLchar *fragmentShaderSourceCode =
        "#version 460 core\n" \
        "in vec2 out_TexCoord;\n" \
		"uniform sampler2D uTextureSampler;\n" \
        "out vec4 FragColor;\n" \
        "void main(void)\n" \
        "{\n" \
        "FragColor = texture(uTextureSampler, out_TexCoord);\n" \
        "}\n";

    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
    glCompileShader(fragmentShaderObject);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
            if (szInfoLog != NULL)
            {
                fprintf(gpFile, "Fragment Shader Compilation = %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;
            }
        }
        uninitialize();
        return -1;
    }

    /*
    CREATE, ATTACH, LINK SHADER Program Object.
    1. Create Shader Program Object.
    2. Attach Shader Object to Shader Program Object.
    3. Tell to Link Shader Object to Shader Program Object.
    4. Check for Link Shader Logs.
    */

    shaderProgramObject = glCreateProgram();
    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, fragmentShaderObject);

    // Bind shader attribute at certain index in shader to same index in host program
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

    glLinkProgram(shaderProgramObject);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar *)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
                fprintf(gpFile, "Shader Program link log = %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;
            }
        }
        uninitialize();
    }

	// get the location from uniform shader
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

	// Provide vartex position, color, normal, texcod, etc.
	const GLfloat smiley_position[] =
	{
		// front
		1.0f,  1.0f, 0.0f, // top-right of top
		-1.0f,  1.0f, 0.0f, // top-left of top
		-1.0f,  -1.0f,  0.0f, // bottom-left of top
		1.0f,  -1.0f,  0.0f, // bottom-right of top
	};

	const GLfloat smiley_texcoords[] =
	{
		// front
		1.0f, 0.0f, // bottom-right of front
		0.0f, 0.0f, // bottom-left of front
		0.0f, 1.0f, // top-left of front
		1.0f, 1.0f, // top-right of front
	};

    // Vertex array object for array of vertex attributes
    glGenVertexArrays(1, &vao_smiley);
    glBindVertexArray(vao_smiley);

    // Position
    glGenBuffers(1, &vbo_position_smiley);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_smiley);

    glBufferData(GL_ARRAY_BUFFER, sizeof(smiley_position), smiley_position, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Texture
    glGenBuffers(1, &vbo_texcoord_smiley);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_smiley);

    glBufferData(GL_ARRAY_BUFFER, sizeof(smiley_texcoords), smiley_texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // print GL Info
    printGLInfo();

    // Depth related Code : prerequisite of depth
    glClearDepth(1.0f);      // display madhe glClear la gelyavrti mazya depth buffer la ja ani saglyanichi value 1 kar asa sangto but ajun te zhala nahi
    glEnable(GL_DEPTH_TEST); // Depth test enable keli apan framebuffer cha 8 paiki
    glDepthFunc(GL_LEQUAL);  // L EQUAL : Code madhun apan konti test ahe te detoy, L EQUAL= tya tya fragment la pass kar jyanchi value mi vr dilelya 1.0 peksha Less than or EQUAL to 1 asel (LEQUAL:<=)

	// from here onword OpenGL codes starts
	// tell the opengl to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Load Textures

    char textureFileStone[] = "Smiley.bmp";
	if(loadGLTexture(&textureSmiley, textureFileStone) == False)
	{
		fprintf(gpFile, "Load texture Stone failed!!\n");
		return(-6);
	}
    else
    {
        fprintf(gpFile, "Loaded texture %s -> id %u\n", textureFileStone, textureSmiley);
    }

	perspectiveProjectionMatrix = mat4::identity(); // this is analogous to glLoadIdentity()

    // Warmup resize
    resize(WIN_WIDTH, WIN_HEIGHT);

    return (0);
}

// Replacement: use glTexStorage2D + glTexSubImage2D + glGenerateMipmap
static int compute_mipmap_levels(int w, int h)
{
    int m = 1;
    int maxdim = (w > h) ? w : h;
    while ((1 << m) <= maxdim) m++;
    return m; // number of levels (includes base)
}

Bool loadGLTexture(GLuint *texture, char *fileImagePath)
{
    int width = 0, height = 0;
    unsigned char *imageData = SOIL_load_image(fileImagePath, &width, &height, NULL, SOIL_LOAD_RGBA);
    if (imageData == NULL)
    {
        fprintf(gpFile, "SOIL_load_image failed for %s\n", fileImagePath);
        return False;
    }

	// Genrate OpenGL Texture object
	glGenTextures(1, texture);
	// bind to the newly created empty structured oabject
	glBindTexture(GL_TEXTURE_2D, *texture);

    // set parameters
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 1 for tight RGBA
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // T1 mapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // allocate immutable storage
    int levels = compute_mipmap_levels(width, height);
    if (levels < 1) levels = 1;
    // internal format RGBA8
    glTexStorage2D(GL_TEXTURE_2D, levels, GL_RGBA8, width, height);
    // upload base level
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    // generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(imageData);
    return True;
}

void printGLInfo(void)
{
    GLint numExtensions = 0;
    fprintf(gpFile, "OPENGL INFORMATION\n");
    fprintf(gpFile, "--------- x -------\n");
    fprintf(gpFile, "OpenGL Vendor    : %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer  : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version   : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version     : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    fprintf(gpFile, "Extension Count  : %d\n", numExtensions);
    for (GLint i = 0; i < numExtensions; i++)
    {
        fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
    }

	fprintf(gpFile, "--------- x -------\n");
}

void resize(int width, int height)
{
    // code
    if (height <= 0)
    {
        height = 1;
    }

    // Set view port
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
}

void display(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use Shader Program Object
    glUseProgram(shaderProgramObject);

    // ModelViewProjection: translate back and rotate Y
    mat4 translationMatrix         = vmath::translate(0.0f, 0.0f, -6.0f);
    mat4 rotationMatrix            = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
    mat4 modelViewMatrix           = translationMatrix * rotationMatrix;
    mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    // send uniform - cast to GLfloat*
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, (const GLfloat *)modelViewProjectionMatrix);

    // for texture
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, textureSmiley);
    glUniform1i(textureSamplerUniform, 0);

    // Bind with vao_smiley
    glBindVertexArray(vao_smiley);

    // Draw the vertex arrays
    for(int i = 0; i <= 20; i += 4) {
        glDrawArrays(GL_TRIANGLE_FAN, i, 4);
    }

    glBindVertexArray(0);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Unuse shader program object
    glUseProgram(0);

    // Swap the buffers
    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    //angleCube += 0.5f;
    angleCube += angleCubeCnt;
    
    if (angleCube >= 360.0f) {
        angleCube -= 360.0f;
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
        XFree(visualInfo);
        visualInfo = NULL;
    }
	
	if (textureSmiley)
	{
		glDeleteTextures(1, &textureSmiley);
		textureSmiley = 0;
	}

    if (vbo_texcoord_smiley)
    {
        glDeleteBuffers(1, &vbo_texcoord_smiley);
        vbo_texcoord_smiley = 0;
    }

    if (vbo_position_smiley)
    {
        glDeleteBuffers(1, &vbo_position_smiley);
        vbo_position_smiley = 0;
    }

    if (vao_smiley)
    {
        glDeleteVertexArrays(1, &vao_smiley);
        vao_smiley = 0;
    }
    /*
    DETACH, DELETE SHADER OBJECTS AND DELETE SHADER PROGRAM OBJECTS
    1. Check whether shader program object is still present.
    2. Get no of shaders and continue only if no. of shaders > 0
    3. Create a buffer/array to hold shader objects of obtained numbers
    4. Get shader objects into this buffer/array and continue only if malloc() is succeed
    5. Start a loop for obtained no. of shaders and inside this loop detach and delete every shader object from the buffer/array
    6. Free the buffer/array
    7. Delete the shader program object
    */
    if (shaderProgramObject)
    {
        glUseProgram(shaderProgramObject);
        GLint numShaders = 0;
        glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
        if (numShaders > 0)
        {
            GLuint *pShaders = (GLuint *)malloc(numShaders * sizeof(GLuint));
            if (pShaders != NULL)
            {
                glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject, pShaders[i]);
                    glDeleteShader(pShaders[i]);
                    pShaders[i] = 0;
                }
            }
            free(pShaders);
        }
        glUseProgram(0);
        glDeleteProgram(shaderProgramObject);
        shaderProgramObject = 0;
    }

    if (window)
    {
        XDestroyWindow(gpDisplay, window);
        window = 0;
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
        fprintf(gpFile, "Program terminated Successfully !!!\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
