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

// Sphere
import java.nio.ShortBuffer;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	private int shaderProgramObject;


	private int[] vao_cube = new int[1];
	private int[] vbo_cube_position = new int[1];
	private int[] vbo_cube_normal = new int[1];

	private int modelViewMatrixUniform;
	private int projectionMatrixUniform;
	float[] perspectiveProjectionMatrix = new float[16];

	
	private int ldUniform = 0;
	private int kdUniform = 0;
	private int lightPositionUniform = 0;
	private int doubleTapUniform = 0;

	private float angleX_cube = 0.0f;
	private float angleY_cube = 0.0f;
	private float angleZ_cube = 0.0f;
	
	private boolean bLightingEnable = false;
	private boolean bAnimationEnable = false;

	private float[] materialDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };	// Gray Ambient light
	private float[] lightDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };	// White Diffuse light
	private float[] lightPosition = { 0.0f, 0.0f, 2.0f, 1.0f };

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
		if(bLightingEnable == false)
			bLightingEnable = true;
		else	
			bLightingEnable = false;
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
		if(bAnimationEnable == false)
			bAnimationEnable = true;
		else
			bAnimationEnable = false;
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
			"in vec3 aNormal;"+
			"uniform mat4 uModelViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec3 uLd;"+
			"uniform vec3 uKd;"+
			"uniform vec4 uLightPosition;"+
			"uniform mediump int uDoubleTap;"+
			"out vec3 oDiffuseLight;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
			"vec4 eyePosition  = uModelViewMatrix * aPosition;"+
			"mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));"+
			"vec3 n            = normalize(normalMatrix * aNormal);"+
			"vec3 s            = normalize(vec3(uLightPosition - eyePosition));"+
			"oDiffuseLight     = uLd * uKd * max(dot(s, n), 0.0f);"+
			"}"+
			"else"+
			"{"+
			"oDiffuseLight=vec3(0.0f,0.0f,0.0f);"+
			"}"+
			"gl_Position=uProjectionMatrix*uModelViewMatrix*aPosition;"+
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
			"in vec3 oDiffuseLight;"+
			"uniform mediump int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
			"FragColor = vec4(oDiffuseLight,1.0f);"+
			"}"+
			"else"+
			"{"+
			"FragColor = vec4(1.0f,1.0f,1.0f,1.0f);"+
			"}"+
			"}"
		);

		int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);
		GLES32.glCompileShader(fragmentShaderObject);

		shaderCompileStatus[0] = 0;
		infoLogLength[0] = 0;
		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
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
		GLES32.glBindAttribLocation(shaderProgramObject, VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

		GLES32.glLinkProgram(shaderProgramObject);

		int[] programLinkStatus = new int[1];
		infoLogLength[0] = 0;
		infoLog = null;

		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, programLinkStatus, 0);
		if(programLinkStatus[0] == GLES32.GL_FALSE)
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

		//mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
		modelViewMatrixUniform  = GLES32.glGetUniformLocation(shaderProgramObject, "uModelViewMatrix");
		projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
		ldUniform               = GLES32.glGetUniformLocation(shaderProgramObject, "uLd");
		kdUniform               = GLES32.glGetUniformLocation(shaderProgramObject, "uKd");
		lightPositionUniform    = GLES32.glGetUniformLocation(shaderProgramObject, "uLightPosition");
		doubleTapUniform        = GLES32.glGetUniformLocation(shaderProgramObject, "uDoubleTap");

		// Geomatry / shape

		final float[] cube_position =
		{
			// front
			 1.0f,  1.0f,  1.0f, // top-right of front
			-1.0f,  1.0f,  1.0f, // top-left of front
			-1.0f, -1.0f,  1.0f, // bottom-left of front
			 1.0f, -1.0f,  1.0f, // bottom-right of front

		   // right
			  1.0f,  1.0f, -1.0f, // top-right of right
			  1.0f,  1.0f,  1.0f, // top-left of right
			  1.0f, -1.0f,  1.0f, // bottom-left of right
			  1.0f, -1.0f, -1.0f, // bottom-right of right

			  // back
			   1.0f,  1.0f, -1.0f, // top-right of back
			  -1.0f,  1.0f, -1.0f, // top-left of back
			  -1.0f, -1.0f, -1.0f, // bottom-left of back
			   1.0f, -1.0f, -1.0f, // bottom-right of back

			   // left
			   -1.0f,  1.0f,  1.0f, // top-right of left
			   -1.0f,  1.0f, -1.0f, // top-left of left
			   -1.0f, -1.0f, -1.0f, // bottom-left of left
			   -1.0f, -1.0f,  1.0f, // bottom-right of left

			   // top
				1.0f,  1.0f, -1.0f, // top-right of top
			   -1.0f,  1.0f, -1.0f, // top-left of top
			   -1.0f,  1.0f,  1.0f, // bottom-left of top
				1.0f,  1.0f,  1.0f, // bottom-right of top

				// bottom
				 1.0f, -1.0f,  1.0f, // top-right of bottom
				-1.0f, -1.0f,  1.0f, // top-left of bottom
				-1.0f, -1.0f, -1.0f, // bottom-left of bottom
				 1.0f, -1.0f, -1.0f, // bottom-right of bottom

		};

		final float[] cube_normals =
		{
			// front surface
			0.0f,  0.0f,  1.0f, // top-right of front
			0.0f,  0.0f,  1.0f, // top-left of front
			0.0f,  0.0f,  1.0f, // bottom-left of front
			0.0f,  0.0f,  1.0f, // bottom-right of front

			// right surface
			1.0f,  0.0f,  0.0f, // top-right of right
			1.0f,  0.0f,  0.0f, // top-left of right
			1.0f,  0.0f,  0.0f, // bottom-left of right
			1.0f,  0.0f,  0.0f, // bottom-right of right

			// back surface
			0.0f,  0.0f, -1.0f, // top-right of back
			0.0f,  0.0f, -1.0f, // top-left of back
			0.0f,  0.0f, -1.0f, // bottom-left of back
			0.0f,  0.0f, -1.0f, // bottom-right of back

			// left surface
		   -1.0f,  0.0f,  0.0f, // top-right of left
		   -1.0f,  0.0f,  0.0f, // top-left of left
		   -1.0f,  0.0f,  0.0f, // bottom-left of left
		   -1.0f,  0.0f,  0.0f, // bottom-right of left

		   // top surface
		   0.0f,  1.0f,  0.0f, // top-right of top
		   0.0f,  1.0f,  0.0f, // top-left of top
		   0.0f,  1.0f,  0.0f, // bottom-left of top
		   0.0f,  1.0f,  0.0f, // bottom-right of top

		   // bottom surface
		   0.0f, -1.0f,  0.0f, // top-right of bottom
		   0.0f, -1.0f,  0.0f, // top-left of bottom
		   0.0f, -1.0f,  0.0f, // bottom-left of bottom
		   0.0f, -1.0f,  0.0f, // bottom-right of bottom
		};

		// vao
		GLES32.glGenVertexArrays(1, vao_cube, 0);

		// Bind
		GLES32.glBindVertexArray(vao_cube[0]);

		
		// VBO for position
		GLES32.glGenBuffers(1, vbo_cube_position, 0);
		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_cube_position[0]);

		// prepare triangle vertices array for GL Buffer Data 
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cube_position.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(cube_position);
		positionBuffer.position(0);

		GLES32.glBufferData(
			GLES32.GL_ARRAY_BUFFER, 
			cube_position.length * 4, 
			positionBuffer, 
			GLES32.GL_STATIC_DRAW
		);
		GLES32.glVertexAttribPointer(
			VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 
			3, 
			GLES32.GL_FLOAT, 
			false, 
			0, 
			0
		);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		// VBO for normal
		GLES32.glGenBuffers(1, vbo_cube_normal, 0);
		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_cube_normal[0]);

		// prepare triangle vertices array for GL Buffer Data 
		byteBuffer = ByteBuffer.allocateDirect(cube_normals.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer normalsBuffer = byteBuffer.asFloatBuffer();
		normalsBuffer.put(cube_normals);
		normalsBuffer.position(0);

		GLES32.glBufferData(
			GLES32.GL_ARRAY_BUFFER, 
			cube_normals.length * 4, 
			normalsBuffer, 
			GLES32.GL_STATIC_DRAW
		);
		GLES32.glVertexAttribPointer(
			VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL, 
			3, 
			GLES32.GL_FLOAT, 
			false, 
			0, 
			0
		);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL);
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
		//float[] projectionMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix, 0);
		//Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		float[] translationMatrix = new float[16];
		float[] rotationMatrix = new float[16];
		float[] rotationMatrix_X = new float[16];
		float[] rotationMatrix_Y = new float[16];
		float[] rotationMatrix_Z = new float[16];
		float[] rotationMatrix_XY = new float[16];

		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.setIdentityM(rotationMatrix, 0);
		Matrix.setIdentityM(rotationMatrix_X, 0);
		Matrix.setIdentityM(rotationMatrix_Y, 0);
		Matrix.setIdentityM(rotationMatrix_Z, 0);
		Matrix.setIdentityM(rotationMatrix_XY, 0);


		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -7.0f);
		Matrix.setRotateM(rotationMatrix_X, 0, angleX_cube, 1.0f, 0.0f, 0.0f);
		Matrix.setRotateM(rotationMatrix_Y, 0, angleY_cube, 0.0f, 1.0f, 0.0f);
		Matrix.setRotateM(rotationMatrix_Z, 0, angleZ_cube, 0.0f, 0.0f, 1.0f);
		
		Matrix.multiplyMM(rotationMatrix_XY, 0, rotationMatrix_X, 0, rotationMatrix_Y, 0);
		Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix_XY, 0, rotationMatrix_Z, 0);

		Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);


		GLES32.glUniformMatrix4fv(modelViewMatrixUniform, 1, false, modelViewMatrix, 0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);

		if (bLightingEnable == true)
		{
			GLES32.glUniform1i(doubleTapUniform, 1);
			//glUniform3fv(ldUniform, 1, lightDiffuse);
			//OR
			GLES32.glUniform3f(ldUniform, 1.0f, 1.0f, 1.0f);
			GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);

		}
		else
		{
			GLES32.glUniform1i(doubleTapUniform, 0);
		}

		
		GLES32.glBindVertexArray(vao_cube[0]);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);
        // unbind vao
        GLES32.glBindVertexArray(0);


		GLES32.glUseProgram(0);	
		
		if(bAnimationEnable == true)
			update();
		
		// Render
		requestRender();
	}

	private void update()
	{
		angleX_cube = angleX_cube - 0.21f;
		angleY_cube = angleY_cube - 0.28f;
		angleZ_cube = angleZ_cube - 0.2f;

		if (angleX_cube <= 0.0f)
		{
			angleX_cube = angleX_cube + 360.0f;
		}
		if (angleY_cube <= 0.0f)
		{
			angleY_cube = angleY_cube + 360.0f;
		}
		if (angleZ_cube <= 0.0f)
		{
			angleZ_cube = angleZ_cube + 360.0f;
		}
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

		if(vbo_cube_normal[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_cube_normal, 0);
			vbo_cube_normal[0] = 0;
		}
		if(vbo_cube_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_cube_position, 0);
			vbo_cube_position[0] = 0;
		}
		if(vao_cube[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_cube, 0);
			vao_cube[0] = 0;
		}
	}

}