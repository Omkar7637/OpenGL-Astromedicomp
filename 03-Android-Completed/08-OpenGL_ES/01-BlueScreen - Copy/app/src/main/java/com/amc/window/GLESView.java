package com.amc.window;

import android.content.Context;

// Events related Packages
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

// OpenGL_ES related Packages
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener 
{
    private Context context;
    private GestureDetector gestureDetector;

    public GLESView(Context _context)
    {
        super(_context);
        context = _context; // Recomended

        // Initialization of OpenGL ES
        setEGLContextClientVersion(3); // Set OpenGL ES 3.0
        setRenderer(this); // Set Renderer for drawing on the GLSurfaceView
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY); // Render only when there is a change
                
        // Initialize GestureDetector
        gestureDetector = new GestureDetector(context, this);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // Methods of GLSurfaceView.Renderer
    // 3 implemented methods of GLSurfaceView.Renderer
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) 
    {
        //Code
        // OpenGL ES initialization
        int iresult = initialize(gl);
        if (iresult != 0) 
        {
            // Log error
            System.out.println("AMC: Initialization Failed!");
            System.exit(0);
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) 
    {
        resize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) 
    {
        display();
        update();
    }

    // Let GestureDetector handle touch events
    @Override
    public boolean onTouchEvent(MotionEvent e) 
    {
        // Code
        // Let GestureDetector handle touch events
        if (!gestureDetector.onTouchEvent(e)) 
        {
            return super.onTouchEvent(e);
        }
        return true;
    }

    // DoubleTapListener methods
    @Override
    public boolean onDoubleTap(MotionEvent e) 
    {
        // Code
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) 
    {
        // Code
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) 
    {
        // Code
        return true;
    }

    // OnGestureListener methods
    @Override
    public boolean onDown(MotionEvent e) 
    {
        // Code
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) 
    {
        // Code
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) 
    {
        // Code
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) 
    {
        // Code
        // Uninitialize and close the application if user scrolls
        uninitialize();
        System.exit(0);
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) 
    {
        // Code
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) 
    {
        // Code
        return true;
    }

    // Our opengl custom methods
    private int initialize(GL10 gl)
    {
        // Code
        printGLESInfo(gl);

        // Depth initialization
        GLES32.glClearDepthf(1.0f); // Depth Buffer Setup
        GLES32.glEnable(GLES32.GL_DEPTH_TEST); // Enables Depth Testing
        GLES32.glDepthFunc(GLES32.GL_LEQUAL); // The Type Of Depth Testing To Do
        GLES32.glEnable(GLES32.GL_CULL_FACE); // Enable Cull Back Face

        // Set Clear Color (Blue)
        GLES32.glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue 
        return 0;
    }

    private void printGLESInfo(GL10 gl)
    {
        // Code
        String gles_vendor = gl.glGetString(GL10.GL_VENDOR); // e.g. Qualcomm, Intel, NVIDIA
        String gles_renderer = gl.glGetString(GL10.GL_RENDERER); // e.g. Adreno (TM) 640, Intel(R) HD Graphics 3000
        String gles_Version = gl.glGetString(GL10.GL_VERSION); // e.g. OpenGL ES 3.2 V@145.0 (GIT@I2f6eaa8d5c)
        String glsl_Version = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION); // e.g. OpenGL ES GLSL ES 3.20
        System.out.println("AMC: " + gles_vendor + "\n");
        System.out.println("AMC: " + gles_renderer + "\n");
        System.out.println("AMC: " + gles_Version + "\n");
        System.out.println("AMC: " + glsl_Version + "\n");
    }


    private void resize(int width, int height)
    {
        // Code
        if (height <= 0) // To prevent divide by zero
        {
            height = 1;
            GLES32.glViewport(0, 0, width, height);
        }
        GLES32.glViewport(0, 0, width, height);
    }

    private void display()
    {
        // Code
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        
        requestRender(); // Request to render the next frame
    }

    private void update()
    {
        // Code
    }

    private void uninitialize()
    {
        // Code
    }


}
