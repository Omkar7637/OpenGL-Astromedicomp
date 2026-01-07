// OGL.cpp
// Corrected: keeps original libraries, fixes SOIL + GL core profile issues

// Standard Header files
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <filesystem>
#include <memory.h> // for memset

// XLib related header files
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

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

// NOTE: removed #pragma comment(lib, ...) because those are Windows-only.
// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *gpDisplay = NULL;
XVisualInfo *visualInfo = NULL;
Window window;
Colormap colormap;
Bool bFullScreen = False;
FILE *gpFile = NULL;

// Shader related global variables
GLuint shaderProgramObject = 0;

Bool bActiveWindow = False;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;

// OpenGL related variables
GLXContext glxContext = NULL;

// attribute locations: keep position=0, texcoord=1
enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_TEXCOORD = 1,
};

GLuint vao = 0;
GLuint vbo_position = 0;
GLuint vbo_texcoord = 0;
GLuint mvpMatrixUniform = 0;
mat4 perspectiveProjectionMatrix; // matrix 4x4

GLuint textureStone = 0;
GLuint textureSamplerUniform = 0;

GLfloat angleTriangle = 0.0f;

Bool bDone = False;

// Forward declarations
int initialize(void);
void resize(int, int);
void display(void);
void update(void);
void uninitialize(void);
void toggleFullScreen(void);
void printGLInfo(void);
Bool loadGLTexture(GLuint *texture, char *fileImagePath);

// ---------------- main ----------------
int main(void)
{
    // function declarations already above
    int defaultScreen;
    int defaultDepth;

    XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;

    Screen *screen = NULL;
    int screenWidth, screenHeight;
    KeySym keySym;
    char keys[26];

    GLXFBConfig *pGLXConfigs = NULL;
    XVisualInfo *pXVisualInfo = NULL;
    int iNumFBConfigs;

    int framebufferAttributes[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        None
    };

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
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        fprintf(gpFile, "XOpenDisplay(XClient) failed to connect with the XServer.\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }

    defaultScreen = XDefaultScreen(gpDisplay);
    defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

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

    int indexOfBestFBConfig = -1;
    int bestNumberOfSamples = -1;

    for (int i = 0; i < iNumFBConfigs; i++)
    {
        pXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXConfigs[i]);
        if (pXVisualInfo)
        {
            int sampleBuffer = 0, samples = 0;
            glXGetFBConfigAttrib(gpDisplay, pGLXConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
            // correct enum for attribute: GLX_SAMPLES
            glXGetFBConfigAttrib(gpDisplay, pGLXConfigs[i], GLX_SAMPLES, &samples);

            if (indexOfBestFBConfig < 0 || (sampleBuffer && samples > bestNumberOfSamples))
            {
                indexOfBestFBConfig = i;
                bestNumberOfSamples = samples;
            }
            XFree(pXVisualInfo);
        }
    }

    if (indexOfBestFBConfig < 0)
    {
        fprintf(gpFile, "No suitable FBConfig found.\n");
        XFree(pGLXConfigs);
        uninitialize();
        exit(EXIT_FAILURE);
    }

    glxFBConfig = pGLXConfigs[indexOfBestFBConfig];
    XFree(pGLXConfigs);

    visualInfo = glXGetVisualFromFBConfig(gpDisplay, glxFBConfig);

    memset((void *)&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;
    windowAttributes.background_pixmap = 0;
    windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen);
    windowAttributes.colormap = XCreateColormap(gpDisplay, XRootWindow(gpDisplay, visualInfo->screen), visualInfo->visual, AllocNone);
    colormap = windowAttributes.colormap;
    windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;

    window = XCreateWindow(
        gpDisplay,
        XRootWindow(gpDisplay, visualInfo->screen),
        0, 0, WIN_WIDTH, WIN_HEIGHT, 0,
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

    fs::path current = fs::current_path();
    std::string foldernameStr = current.filename().string();
    std::string combinedStr = "Omkar Kashid | Xwindows : " + foldernameStr;
    const char *windowname = combinedStr.c_str();
    XStoreName(gpDisplay, window, windowname);

    XMapWindow(gpDisplay, window);

    screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
    screenWidth = XWidthOfScreen(screen);
    screenHeight = XHeightOfScreen(screen);
    XMoveWindow(gpDisplay, window, screenWidth / 2 - WIN_WIDTH / 2, screenHeight / 2 - WIN_HEIGHT / 2);

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
            case ClientMessage:
                // Map WM_DELETE_WINDOW messages to exit
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
    return 0;
}

// Toggle fullscreen
void toggleFullScreen(void)
{
    Atom wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = bFullScreen ? 0 : 1;
    xev.xclient.data.l[1] = fullscreen;
    XSendEvent(gpDisplay, XRootWindow(gpDisplay, visualInfo->screen), False, SubstructureNotifyMask, &xev);
}

// Initialize GL, shaders, buffers, textures
int initialize(void)
{
    void printGLInfo();
    void resize(int, int);
    void uninitialize(void);
    Bool loadGLTexture(GLuint *texture, char *fileImagePath);

    GLenum glewResult;

    // get function pointer for glXCreateContextAttribsARB
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");

    if (glXCreateContextAttribsARB)
    {
        GLint attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
                GLX_CONTEXT_MINOR_VERSION_ARB, 5,
                GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
                None};
        glxContext = glXCreateContextAttribsARB(gpDisplay, glxFBConfig, 0, True, attribs);
        if (!glxContext)
        {
            fprintf(gpFile, "glXCreateContextAttribsARB returned NULL; will try fallback.\n");
        }
        else
        {
            fprintf(gpFile, "Received OpenGL Context (requested 4.5 core)\n");
        }
    }

    if (!glxContext)
    {
        // Fallback: create a legacy context
        glxContext = glXCreateNewContext(gpDisplay, glxFBConfig, GLX_RGBA_TYPE, 0, True);
        if (!glxContext)
        {
            fprintf(gpFile, "Failed to create GL context\n");
            return -1;
        }
        else
        {
            fprintf(gpFile, "Created fallback GL context\n");
        }
    }

    if (!glXMakeCurrent(gpDisplay, window, glxContext))
    {
        fprintf(gpFile, "glXMakeCurrent failed\n");
        return -1;
    }

    // Initialize GLEW AFTER making context current
    glewExperimental = GL_TRUE;
    glewResult = glewInit();
    if (glewResult != GLEW_OK)
    {
        fprintf(gpFile, "glewInit() failed: %s\n", glewGetErrorString(glewResult));
        return (-6);
    }

    // Print GL info
    printGLInfo();

    // Vertex Shader
    const GLchar *vertexShaderSourceCode =
        "#version 460 core\n"
        "in vec4 aPosition;\n"
        "in vec2 aTexCoord;\n"
        "out vec2 out_TexCoord;\n"
        "uniform mat4 uMVPMatrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = uMVPMatrix * aPosition;\n"
        "    out_TexCoord = aTexCoord;\n"
        "}\n";

    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderObject, 1, &vertexShaderSourceCode, NULL);
    glCompileShader(vertexShaderObject);

    GLint status = 0;
    GLint infoLogLength = 0;

    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            char *szInfoLog = (char *)malloc(infoLogLength);
            glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
            fprintf(gpFile, "Vertex Shader Compilation Log = %s\n", szInfoLog);
            free(szInfoLog);
        }
        uninitialize();
        return -1;
    }

    // Fragment Shader
    const GLchar *fragmentShaderSourceCode =
        "#version 460 core\n"
        "in vec2 out_TexCoord;\n"
        "uniform sampler2D uTextureSampler;\n"
        "out vec4 FragColor;\n"
        "void main(void)\n"
        "{\n"
        "    FragColor = texture(uTextureSampler, out_TexCoord);\n"
        "}\n";

    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObject, 1, &fragmentShaderSourceCode, NULL);
    glCompileShader(fragmentShaderObject);

    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            char *szInfoLog = (char *)malloc(infoLogLength);
            glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
            fprintf(gpFile, "Fragment Shader Compilation Log = %s\n", szInfoLog);
            free(szInfoLog);
        }
        uninitialize();
        return -1;
    }

    // Create Program, attach, bind attributes and link
    shaderProgramObject = glCreateProgram();
    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, fragmentShaderObject);

    // Bind attribute locations BEFORE linking
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

    glLinkProgram(shaderProgramObject);

    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            char *szInfoLog = (char *)malloc(infoLogLength);
            glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
            fprintf(gpFile, "Shader Program link log = %s\n", szInfoLog);
            free(szInfoLog);
        }
        uninitialize();
        return -1;
    }

    // Uniform locations
    mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
    textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

    // Geometry: pyramid (4 faces * 3 vertices)
    const GLfloat triangle_position[] =
    {
        // front
         0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,

         // right
         0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,

         // back
         0.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // left
         0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
    };

    // T1 mapping (top: 0.5,1.0; left:0,0; right:1,0) - same per face
    const GLfloat triangle_texcord[] = {
        // front
        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // right
        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // back
        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // left
        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };

    // Create VAO + VBOs
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // positions - attribute 0
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_position), triangle_position, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // texcoords - attribute 1
    glGenBuffers(1, &vbo_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_texcord), triangle_texcord, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Print GL Info
    printGLInfo();

    // Depth and clear
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Load Textures using SOIL and glTexStorage2D + glTexSubImage2D approach
    char textureFile[] = "Stone.bmp"; // keep original filename
    if (loadGLTexture(&textureStone, textureFile) == False)
    {
        fprintf(gpFile, "Load texture Stone failed!!\n");
        // continue but object will be untextured
    }
    else
    {
        fprintf(gpFile, "Loaded texture %s -> id %u\n", textureFile, textureStone);
    }

    perspectiveProjectionMatrix = mat4::identity();

    // Warmup resize
    resize(WIN_WIDTH, WIN_HEIGHT);

    return 0;
}

// Replacement: use glTexStorage2D + glTexSubImage2D + glGenerateMipmap
// This is option B you selected
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

    glGenTextures(1, texture);
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
        const char *ext = (const char *)glGetStringi(GL_EXTENSIONS, i);
        fprintf(gpFile, "%s\n", ext);
    }
    fprintf(gpFile, "--------- x -------\n");
}

void resize(int width, int height)
{
    if (height <= 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // keep original gluPerspective usage
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    // Also update our shader-side projection matrix (vmath)
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramObject);

    // ModelViewProjection: translate back and rotate Y
    mat4 translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
    mat4 rotationMatrix = vmath::rotate(angleTriangle, 0.0f, 1.0f, 0.0f);
    mat4 modelViewMatrix = translationMatrix * rotationMatrix;
    mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    // send uniform - cast to GLfloat*
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, (const GLfloat *)modelViewProjectionMatrix);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureStone);
    glUniform1i(textureSamplerUniform, 0);

    // Draw
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);

    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    angleTriangle += 0.5f;
    if (angleTriangle >= 360.0f)
        angleTriangle -= 360.0f;
}

void uninitialize(void)
{
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

    if (textureStone)
    {
        glDeleteTextures(1, &textureStone);
        textureStone = 0;
    }

    if (vbo_texcoord)
    {
        glDeleteBuffers(1, &vbo_texcoord);
        vbo_texcoord = 0;
    }

    if (vbo_position)
    {
        glDeleteBuffers(1, &vbo_position);
        vbo_position = 0;
    }

    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

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
                free(pShaders);
            }
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

    if (colormap)
    {
        XFreeColormap(gpDisplay, colormap);
    }

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }

    if (gpFile)
    {
        fprintf(gpFile, "Program terminated Successfully !!!\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}

