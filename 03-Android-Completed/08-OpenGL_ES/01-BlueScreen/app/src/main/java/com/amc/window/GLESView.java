package com.amc.window;

import android.content.Context;  // To get the context of the activity, needed for initializing views

// Event-related Packages
import android.view.MotionEvent;             // Represents touch screen motion events
import android.view.GestureDetector;         // Detects common gestures like taps, scrolls, flings
import android.view.GestureDetector.OnGestureListener;    // Interface for basic gestures
import android.view.GestureDetector.OnDoubleTapListener;  // Interface specifically for double-tap events

// OpenGL ES related Packages
import android.opengl.GLSurfaceView;         // Provides a surface where OpenGL ES can render
import javax.microedition.khronos.opengles.GL10;  // OpenGL ES 1.0 interface (required by GLSurfaceView.Renderer)
import javax.microedition.khronos.egl.EGLConfig; // EGL configuration info for OpenGL context
import android.opengl.GLES32;                // OpenGL ES 3.2 specific functions

// GLSurfaceView class that handles OpenGL ES rendering and touch gestures
public class GLESView extends GLSurfaceView 
        implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener 
{
    private Context context;                  // To store the activity context
    private GestureDetector gestureDetector;  // Gesture detector to handle touch gestures

    // Constructor for GLESView
    public GLESView(Context _context)
    {
        super(_context);                       // Call the parent GLSurfaceView constructor
        context = _context;                    // Save the context (recommended for future use)

        // ================= OPENGL ES SETUP =================
        setEGLContextClientVersion(3);         // Use OpenGL ES 3.0 context
        setRenderer(this);                     // Set this class as the renderer
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY); 
        // Render only when requested (via requestRender) instead of continuously

        // ================= GESTURE DETECTOR =================
        gestureDetector = new GestureDetector(context, this); // Initialize with context and listener
        gestureDetector.setOnDoubleTapListener(this);        // Listen for double-tap gestures
    }

    // ================= GLSurfaceView.Renderer METHODS =================
    
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) 
    {
        // Called once when the surface is created
        // Initialize OpenGL ES resources, shaders, buffers, etc.
        int iresult = initialize(gl);        // Call custom initialization function
        if (iresult != 0) 
        {
            System.out.println("AMC: Initialization Failed!"); // Log error
            System.exit(0);                                    // Exit app if initialization fails
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) 
    {
        // Called when surface size changes (e.g., rotation or resizing)
        resize(width, height);                 // Adjust viewport and projection
    }

    @Override
    public void onDrawFrame(GL10 gl) 
    {
        // Called every time the view is redrawn
        display();                             // Render the frame
        update();                              // Update any animations or logic
    }

    // ================= TOUCH EVENTS =================
    @Override
    public boolean onTouchEvent(MotionEvent e) 
    {
        // Delegate the touch event to the gesture detector
        if (!gestureDetector.onTouchEvent(e)) 
        {
            return super.onTouchEvent(e);     // If not handled, let superclass handle it
        }
        return true;                           // Event handled
    }

    // ================= DOUBLE TAP LISTENER METHODS =================
    @Override
    public boolean onDoubleTap(MotionEvent e) 
    {
        // Called when a double tap occurs
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) 
    {
        // Called for each event within a double-tap gesture
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) 
    {
        // Called when a single tap is confirmed (not part of double-tap)
        return true;
    }

    // ================= GESTURE LISTENER METHODS =================
    @Override
    public boolean onDown(MotionEvent e) 
    {
        // Called when the user first touches the screen
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) 
    {
        // Called when the user presses down and holds (visual feedback possible)
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) 
    {
        // Called when the user lifts their finger after a tap
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) 
    {
        // Called when the user scrolls (drags finger)
        // Here, scrolling is used to exit the app
        uninitialize();                        // Release OpenGL resources
        System.exit(0);                        // Exit application
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) 
    {
        // Called when the user long-presses
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) 
    {
        // Called when a fling (fast swipe) occurs
        return true;
    }

    // ================= CUSTOM OPENGL METHODS =================

    private int initialize(GL10 gl)
    {
        // Print OpenGL info
        printGLESInfo(gl);

        // ================= DEPTH BUFFER =================
        GLES32.glClearDepthf(1.0f);           // Set depth buffer clear value (max depth)
        GLES32.glEnable(GLES32.GL_DEPTH_TEST); // Enable depth testing
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);  // Accept fragment if depth <= current depth

        // ================= CULLING =================
        GLES32.glEnable(GLES32.GL_CULL_FACE);  // Enable back-face culling (improves performance)

        // ================= CLEAR COLOR =================
        GLES32.glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue background

        return 0;                              // Return 0 on success
    }

    private void printGLESInfo(GL10 gl)
    {
        // Print OpenGL ES related info for debugging
        String gles_vendor = gl.glGetString(GL10.GL_VENDOR);   // GPU Vendor (e.g., Qualcomm)
        String gles_renderer = gl.glGetString(GL10.GL_RENDERER); // Renderer (e.g., Adreno 640)
        String gles_Version = gl.glGetString(GL10.GL_VERSION);   // OpenGL ES version
        String glsl_Version = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION); // GLSL version

        System.out.println("AMC: " + gles_vendor + "\n");
        System.out.println("AMC: " + gles_renderer + "\n");
        System.out.println("AMC: " + gles_Version + "\n");
        System.out.println("AMC: " + glsl_Version + "\n");
    }

    private void resize(int width, int height)
    {
        // Handle window resizing
        if (height <= 0) height = 1;           // Prevent divide by zero
        GLES32.glViewport(0, 0, width, height); // Set the viewport to cover the new size
    }

    private void display()
    {
        // Clear color and depth buffers before rendering
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        requestRender();                        // Request the next frame render
    }

    private void update()
    {
        // Update animations, logic, physics, etc.
    }

    private void uninitialize()
    {
        // Release OpenGL resources (shaders, buffers, textures, etc.)
    }
}
