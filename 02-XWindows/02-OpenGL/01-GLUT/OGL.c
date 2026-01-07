// -----------------------------------------------------------------------------
// Program : Basic OpenGL Triangle Rendering using FreeGLUT on X11
// Author  : RTR2024-150 Omkar Kashid
// Desc    : Demonstrates window creation, basic OpenGL rendering, and handling
//           keyboard/mouse inputs with fullscreen toggle functionality.
// -----------------------------------------------------------------------------

// Header Files
#include <X11/Xlib.h>
#include <GL/freeglut.h>

// Global Variables
Bool bFullScreen = False;

// Entry-Point Function
int main(int argc, char *argv[])
{
    // Local Function Declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void mouse(int, int, int, int);
    void uninitialize(void);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // Set initial window position and size
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    // Create OpenGL window
    glutCreateWindow("RTR2024-150 Omkar Kashid | OpenGL Triangle");

    // Initialize OpenGL state
    initialize();

    // Register callback functions
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutCloseFunc(uninitialize);

    // Start the main event-processing loop
    glutMainLoop();

    return (0);
}

// Function to initialize OpenGL state
int initialize(void)
{
    // Set background (clear) color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return (0);
}

// Callback function to handle window resizing
void resize(int width, int height)
{
    // Define viewport dimensions
    glViewport(0, 0, width, height);
}

// Display callback - renders a simple triangle
void display(void)
{
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Begin drawing a colored triangle
    glBegin(GL_TRIANGLES);

    // Red vertex at the top
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    // Green vertex at the bottom left
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

    // Blue vertex at the bottom right
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

    // Swap buffers (double buffering)
    glutSwapBuffers();
}

// Keyboard input callback
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC key
        glutLeaveMainLoop();
        break;

    case 'F':
    case 'f': // Toggle fullscreen
        if (bFullScreen == False)
        {
            glutFullScreen();
            bFullScreen = True;
        }
        else
        {
            glutLeaveFullScreen();
            bFullScreen = False;
        }
        break;

    default:
        break;
    }
}

// Mouse input callback
void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON: // Exit on left mouse click
        glutLeaveMainLoop();
        break;

    default:
        break;
    }
}

// Cleanup function
void uninitialize(void)
{
    // Free any resources if allocated (currently none)
}
