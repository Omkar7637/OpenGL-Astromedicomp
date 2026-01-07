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

// Pakages for java native io
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.Matrix;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener 
{
    private Context context; 
    private GestureDetector gestureDetector;
    private int shaderProgramObject; // Shader Program Object
    private int mvpMatrixUniform; // Model View Projection Matrix Uniform
    private int vao[] = new int[1]; // Vertex Array Object
    private int vbo[] = new int[1]; // Vertex Buffer Object for Position
    private int vbo_color[] = new int[1]; // Vertex Buffer Object for Color
    private float perspectiveProjectionMatrix[] = new float[16]; // 4x4 matrix
    private float modelViewMatrix[] = new float[16];


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

        // Vertex Shader==========================
        final String vertexShaderSourceCode = String.format
        (
            "#version 320 es\n" +
            "in vec4 aPosition;\n" +
            "in vec4 aColor;\n" +
            "out vec4 aColorVS;\n" +
            "uniform mat4 uMVPMatrix;\n" +
            "void main(void)\n" +
            "{\n" +
            "gl_Position = uMVPMatrix * aPosition;\n" +
            "aColorVS = aColor;\n" +
            "}\n"
        );

        int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
        GLES32.glCompileShader(vertexShaderObject);

        int shaderCompileStatus[] = new int[1];
        int infoLogLength[] = new int[1];
        String szInfoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);
        if (shaderCompileStatus[0] == GLES32.GL_FALSE) 
        {
            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) 
            {
                szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
                System.out.println("AMC: Vertex Shader Compilation Log: " + szInfoLog + "\n");
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader========================
        final String fragmentShaderSourceCode = String.format
        (
            "#version 320 es\n" +
            "precision highp float;\n" +
            "in vec4 aColorVS;\n" +
            "out vec4 fragColor;\n" +
            "void main(void)\n" +
            "{\n" +
            "fragColor = aColorVS;\n" +
            "}\n"
        );


        int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);  
        GLES32.glCompileShader(fragmentShaderObject);

        shaderCompileStatus[0] = 0;
        infoLogLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);
        if (shaderCompileStatus[0] == GLES32.GL_FALSE) 
        {
            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) 
            {
                szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
                System.out.println("AMC: Fragment Shader Compilation Log: " + szInfoLog + "\n");
                uninitialize();
                System.exit(0);
            }
        }

        // Shader Program=========================
        shaderProgramObject = GLES32.glCreateProgram();
        // Attach Vertex Shader to Shader Program
        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);
        // Bind attributes
        GLES32.glBindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_POSITION, "aPosition");
        GLES32.glBindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_COLOR, "aColor");

        // Link Shader Program
        GLES32.glLinkProgram(shaderProgramObject);

        // Check for linking errors
        int shaderProgramLinkStatus[] = new int[1];
        infoLogLength[0] = 0;
        szInfoLog = null;
        // Check for linking errors
        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, shaderProgramLinkStatus, 0);
        // Check for linking errors
        if (shaderProgramLinkStatus[0] == GLES32.GL_FALSE) 
        {
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) 
            {
                szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("AMC: Shader Program Link Log: " + szInfoLog + "\n");
                uninitialize();
                System.exit(0);
            }
        }

        // Get MVP uniform location
        mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
        float[] mvp = new float[16];
        
        Matrix.multiplyMM(mvp, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, mvp, 0);

        final float triangle_position[] = new float[]
        {
            0.0f, 1.0f, 0.0f, 
            -1.0f, -1.0f, 0.0f, 
            1.0f, -1.0f, 0.0f
        };

        final float triangle_color[] = new float[]
        {
            1.0f, 0.0f, 0.0f, 1.0f, // RED
            0.0f, 1.0f, 0.0f, 1.0f, // GREEN
            0.0f, 0.0f, 1.0f, 1.0f  // BLUE
        };

        // Create VAO
        GLES32.glGenVertexArrays(1, vao, 0);
        GLES32.glBindVertexArray(vao[0]);
        // Create VBO for Position
        GLES32.glGenBuffers(1, vbo, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo[0]);
        // Fill VBO for Position
        // create a native buffer suitable for native io but for java float is 4 bytes
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(triangle_position.length * 4);
        // arrange the byte order of buffer in native byte order
        byteBuffer.order(ByteOrder.nativeOrder());
        // create float type buffer
        FloatBuffer trianglePositionBuffer = byteBuffer.asFloatBuffer();
        // put data into buffer
        trianglePositionBuffer.put(triangle_position);
        // set array at 0th position
        trianglePositionBuffer.position(0);
        // fill data into buffer
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, byteBuffer.capacity(), byteBuffer, GLES32.GL_STATIC_DRAW);
        // Set Vertex Attribute Pointer
        GLES32.glVertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_POSITION);
        // Unbind VBO
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


        // Create VBO for Color
        GLES32.glGenBuffers(1, vbo_color, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_color[0]);
        ByteBuffer colorBuffer = ByteBuffer.allocateDirect(triangle_color.length * 4);
        colorBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer triangleColorBuffer = colorBuffer.asFloatBuffer();
        triangleColorBuffer.put(triangle_color);
        triangleColorBuffer.position(0);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, colorBuffer.capacity(), colorBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_COLOR, 4, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_COLOR);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);



        GLES32.glBindVertexArray(0);

        // Depth initialization
        GLES32.glClearDepthf(1.0f); // Depth Buffer Setup
        GLES32.glEnable(GLES32.GL_DEPTH_TEST); // Enables Depth Testing
        GLES32.glDepthFunc(GLES32.GL_LEQUAL); // The Type Of Depth Testing To Do
        GLES32.glEnable(GLES32.GL_CULL_FACE); // Enable Cull Back Face

        // Set Clear Color (Blue)
        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Blue 

        // perspective matrix initialization
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)getWidth()/(float)getHeight(), 0.1f, 100.0f);
        // GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, getMVPMatrix, 0);
        
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
        
        GLES32.glUseProgram(shaderProgramObject); // Use Shader Program Object
        Matrix.setIdentityM(modelViewMatrix, 0);
        float translationMatrix[] = new float[16];
        Matrix.setIdentityM(translationMatrix, 0);
        Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -5.0f);
        modelViewMatrix = translationMatrix;

        float modelviewprojectionMatrix[] = new float[16];
        Matrix.setIdentityM(modelviewprojectionMatrix, 0);
        Matrix.multiplyMM(modelviewprojectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelviewprojectionMatrix, 0);
        GLES32.glBindVertexArray(vao[0]); // Bind VAO
        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 3); // Draw the Triangle
        GLES32.glBindVertexArray(0); // Unbind VAO  
        GLES32.glUseProgram(0); // Unuse Shader Program Object
        
        requestRender(); // Request to render the next frame
    }

    private void update()
    {
        // Code
    }

    private void uninitialize()
    {
        // Code
        if (vbo[0] != 0) 
        {
            GLES32.glDeleteBuffers(1, vbo, 0);
            vbo[0] = 0;
        }
        if (vao[0] != 0) 
        {
            GLES32.glDeleteVertexArrays(1, vao, 0);
            vao[0] = 0;
        }
        if (shaderProgramObject != 0) 
        {
            int shaderCount[] = new int[1];
            int shaderNumber[] = new int[1];
            GLES32.glUseProgram(shaderProgramObject);
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, shaderCount, 0);
            int count = shaderCount[0];
            if (count > 0) 
            {
                GLES32.glGetAttachedShaders(shaderProgramObject, count, null, 0, shaderNumber, 0);
                for (int i = 0; i < count; i++) 
                {
                    GLES32.glDetachShader(shaderProgramObject, shaderNumber[i]);
                    GLES32.glDeleteShader(shaderNumber[i]);
                    shaderNumber[i] = 0;
                }
            }
            GLES32.glUseProgram(0);
            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;
        }
        

    }


}
