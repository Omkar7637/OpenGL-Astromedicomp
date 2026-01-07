package com.amc.window;

import android.content.Context;

// OpenGL-ES related packages
import android.opengl.GLSurfaceView;
import android.opengl.GLES32;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

//event 
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;

// Java IO related packages
import java.nio.ByteBuffer;		// nio => native i/p o/p
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	private int shaderProgramObject;
	private int[] vao_triangle = new int[1];
	private int[] vbo_position_triangle = new int[1];
	private int[] vbo_color_triangle = new int[1];

	private int[] vao_rectangle = new int[1];
	private int[] vbo_position_rectangle = new int[1];
	private int[] vbo_color_rectangle = new int[1];


	private int mvpMatrixUniform;
	float[] perspectiveProjectionMatrix = new float[16];

	public GLESView(Context context)
	{
		super(context);

		// event related
		

		// OpenGL-ES related
		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		
		// create and set gestureDetector object
		gestureDetector = new GestureDetector(context, this, null, false);

		gestureDetector.setOnDoubleTapListener(this);
	}

	// implementation of 3 methods of GLSurfaceView interface
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		//code
		initialize(gl);
	}
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		// code
		resize(width, height);
	}
	@Override
	public void onDrawFrame(GL10 gl)
	{
		// code
		display();
	}

	// implementation of onTouch event method of View Class
	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
		//code
		if(!gestureDetector.onTouchEvent(e))
		{
			super.onTouchEvent(e); // if gesuteDetector cha fail zala tr bapach call kr
		}
		return true;
	}

	// implementation of 3 methods of OnDoubleTapListner interface
	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		//code
		return true;
	}
	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return true;
	}
	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		return true;
	}

	// implementation of 6 methods of OnGestureListner interface
	@Override
	public boolean onDown(MotionEvent e)
	{
		return true;
	}
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) // jhidkarna
	{
		return true;
	}
	@Override
	public void onLongPress(MotionEvent e)
	{
	}
	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
	{
		// code
		uninitialize();
		System.exit(0);

		return true;
	}
	@Override
	public void onShowPress(MotionEvent e)
	{
		//code
	}
	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return true;
	}

	// implementation of private methods
	
	private void initialize(GL10 gl)
	{
		// code
		// print openGL-ES info
		printGLInfo(gl);

		// Vertex shader 
		final String vertexShaderSourceCode = String.format
		(
			"#version 320 es"+
			"\n"+
			"in vec4 aPosition;"+
			"in vec4 aColor;"+
			"uniform mat4 uMVPMatrix;"+
			"out vec4 oColor;"+
			"void main(void)"+
			"{"+
			"gl_Position= uMVPMatrix*aPosition;"+
			"oColor = aColor;"+
			"}"
		);
		int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

		GLES32.glCompileShader(vertexShaderObject);

		int[] shaderCompileStatus = new int[1];

		int[] infoLogLength = new int[1];

		String infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("SAG: Vertex Shader Compilation Error Log : " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// Fragment Shader
		final String fragmentShaderSourceCode = String.format(
			"#version 320 es"+
			"\n"+
			"precision highp float;"+
			"in vec4 oColor;"+
			"out vec4 FragColor;"+
			"void main(void)"+
			"{"+
			"FragColor=oColor;"+  
			"}"
		);

		int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);
		GLES32.glCompileShader(fragmentShaderObject);

		shaderCompileStatus[0] = 0;
		infoLogLength[0] = 0;
		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);

		if(shaderCompileStatus[0] != GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("SAG: Fragment Shader Compilation Error Log : " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// Shader program
		shaderProgramObject = GLES32.glCreateProgram();
	
		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject, VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, "aPosition");
		GLES32.glBindAttribLocation(shaderProgramObject, VertexAttributesEnum.AMC_ATTRIBUTE_COLOR, "aColor");

		GLES32.glLinkProgram(shaderProgramObject);

		int[] programLinkStatus = new int[1];
		infoLogLength[0] = 0;
		infoLog = null;

		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, programLinkStatus, 0);
		if(programLinkStatus[0] != GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
				System.out.println("SAG: Shader Program Linking Error Log : " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

		// Geomatry / shape

		final float[] triangle_position = 
		{
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 
			1.0f, -1.0f, 0.0f
		};

		final float[] triangle_color =
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 
			0.0f, 0.0f, 1.0f,
		};



		final float[] rectangle_position = 
		{
			-1.0f, -1.0f, 0.0f, 
			-1.0f, 1.0f, 0.0f, 
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};


		final float[] rectangle_color = 
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 
			1.0f, 1.0f, 0.0f
		};

		// For triangle

		// vao
		GLES32.glGenVertexArrays(1, vao_triangle, 0);

		// Bind
		GLES32.glBindVertexArray(vao_triangle[0]);

		
		// VBO for position
		GLES32.glGenBuffers(1, vbo_position_triangle, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_triangle[0]);


		// prepare triangle vertices array for GL Buffer Data 
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(triangle_position.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(triangle_position);
		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangle_position.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		// VBO for Color
		GLES32.glGenBuffers(1, vbo_color_triangle, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_color_triangle[0]);


		// prepare triangle vertices array for GL Buffer Data 
		byteBuffer = ByteBuffer.allocateDirect(triangle_color.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer colorBuffer = byteBuffer.asFloatBuffer();
		colorBuffer.put(triangle_color);
		colorBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangle_color.length * 4, colorBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);

		// For Rectangle

		// vao
		GLES32.glGenVertexArrays(1, vao_rectangle, 0);

		// Bind
		GLES32.glBindVertexArray(vao_rectangle[0]);

		
		// VBO for position
		GLES32.glGenBuffers(1, vbo_position_rectangle, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_rectangle[0]);

		// prepare triangle vertices array for GL Buffer Data 
		byteBuffer = ByteBuffer.allocateDirect(rectangle_position.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(rectangle_position);
		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, rectangle_position.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		// VBO for Color
		GLES32.glGenBuffers(1, vbo_color_rectangle, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_color_rectangle[0]);


		// prepare triangle vertices array for GL Buffer Data 
		byteBuffer = ByteBuffer.allocateDirect(rectangle_color.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		colorBuffer = byteBuffer.asFloatBuffer();
		colorBuffer.put(rectangle_color);
		colorBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, rectangle_color.length * 4, colorBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);




		Matrix.setIdentityM(perspectiveProjectionMatrix, 0);


		// Depth enable setting
		GLES32.glClearDepthf(1.0f);
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		// enable culling for back faces
		//GLES32.glEnable(GLES32.GL_CULL_FACE);

		// clear color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	}

	private void printGLInfo(GL10 gl)
	{
		// code
		System.out.println("SAG: OpenGL-ES Renderer : " + gl.glGetString(GL10.GL_RENDERER));
		System.out.println("SAG: OpenGL-ES Version  : " + gl.glGetString(GL10.GL_VERSION));
		System.out.println("SAG: OpenGL-ES Shading Language Version : " + gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
	}

	private void resize(int width, int height)
	{
		// code
		if(height <= 0)
		{
			height = 1;
		}
		GLES32.glViewport(0, 0, width, height);

		Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
	}

	private void display()
	{
		// code 
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);

		// Transformations
		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		Matrix.translateM(modelViewMatrix, 0, -1.5f, 0.0f, -6.0f);
		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);
		
		GLES32.glBindVertexArray(vao_triangle[0]);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 3);

		GLES32.glBindVertexArray(0);



		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		Matrix.translateM(modelViewMatrix, 0, 1.5f, 0.0f, -6.0f);
		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);


		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);
		
		GLES32.glBindVertexArray(vao_rectangle[0]);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);		
		
		// Render
		requestRender();
	}

	private void uninitialize()
	{
		// code
		if(shaderProgramObject > 0)
		{
			GLES32.glUseProgram(shaderProgramObject);
			int[] retVal = new int[1];
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];
				int[] shaderObjets = new int[numAttachedShaders];
				GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShaders, retVal, 0, shaderObjets, 0);
				for(int i = 0; i < numAttachedShaders; i++)
				{
					GLES32.glDetachShader(shaderProgramObject, shaderObjets[i]);
					GLES32.glDeleteShader(shaderObjets[i]);
					shaderObjets[i] = 0;
				}
			}
			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;

		}
		if(vbo_color_rectangle[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_color_rectangle, 0);
			vbo_color_rectangle[0] = 0;
		}
		if(vbo_position_rectangle[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_position_rectangle, 0);
			vbo_position_rectangle[0] = 0;
		}
		if(vbo_color_triangle[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_color_triangle, 0);
			vbo_color_triangle[0] = 0;
		}
		if(vbo_position_triangle[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_position_triangle, 0);
			vbo_position_triangle[0] = 0;
		}
		if(vao_rectangle[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_rectangle, 0);
			vao_rectangle[0] = 0;
		}
		if(vao_triangle[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_triangle, 0);
			vao_triangle[0] = 0;
		}
	}

}