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

// Texture related
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private final Context context;

	private GestureDetector gestureDetector = null;

	private int shaderProgramObject;
	private int[] vao = new int[1];
	private int[] vbo_position = new int[1];
	private int[] vbo_texture = new int[1];

	private int mvpMatrixUniform;
	float[] perspectiveProjectionMatrix = new float[16];

	private int[] smileyTexture = new int[1];
	private int textureSamplerUniform;
	private int keyPressedUniform;

	private int singleTap;

	public GLESView(Context _context)
	{
		super(_context);

		context = _context;

		// event related
		

		// OpenGL-ES related
		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		
		// create and set gestureDetector object
		gestureDetector = new GestureDetector(_context, this, null, false);

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
		singleTap++;
		if(singleTap > 4)
			singleTap = 0;
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
			"in vec2 aTexCoord;"+
			"uniform mat4 uMVPMatrix;"+
			"out vec2 oTexCoord;"+
			"void main(void)"+
			"{"+
			"gl_Position= uMVPMatrix*aPosition;"+
			"oTexCoord=aTexCoord;"+
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
				System.out.println("AMC : Vertex Shader Compilation Error Log : " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// Fragment Shader
		final String fragmentShaderSourceCode = String.format(
			"#version 320 es"+
			"\n"+
			"precision highp float;"+
			"in vec2 oTexCoord;"+
			"out vec4 FragColor;"+
			"uniform highp sampler2D uTextureSampler;"+
			"uniform int uKeyPressed;"+
			"void main(void)"+
			"{"+
			"if(uKeyPressed == 0)"+
			"{"+
			"FragColor=vec4(1.0f, 1.0f, 1.0f, 1.0f);"+
			"}"+
			"else"+
			"{"+
			"FragColor=texture(uTextureSampler, oTexCoord);"+  
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

		if(shaderCompileStatus[0] != GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("AMC : Fragment Shader Compilation Error Log : " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// Shader program
		shaderProgramObject = GLES32.glCreateProgram();
	
		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject, VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, "aPosition");
		GLES32.glBindAttribLocation(shaderProgramObject, VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORDS, "aTexCoord");

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
				System.out.println("AMC :Shader Program Linking Error Log : " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
		textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uTextureSampler");
		keyPressedUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uKeyPressed");

		// Geomatry / shape

		final float[] sqaure_position = {
			-1.0f, -1.0f, 0.0f, 
			-1.0f, 1.0f, 0.0f, 
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};


		// vao
		GLES32.glGenVertexArrays(1, vao, 0);

		// Bind
		GLES32.glBindVertexArray(vao[0]);

		
		// VBO for position
		GLES32.glGenBuffers(1, vbo_position, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position[0]);


		// prepare triangle vertices array for GL Buffer Data 
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sqaure_position.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(sqaure_position);
		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sqaure_position.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// VBO for Texture
		GLES32.glGenBuffers(1, vbo_texture, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture[0]);


		// prepare triangle vertices array for GL Buffer Data 
		//byteBuffer = ByteBuffer.allocateDirect(4 * 2 * 4);
		//byteBuffer.order(ByteOrder.nativeOrder());
		//FloatBuffer textureBuffer = byteBuffer.asFloatBuffer();
		//textureBuffer.put(rectangle_texcoords);
		//textureBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 2 * 4, null , GLES32.GL_DYNAMIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORDS, 2, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORDS);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);



		GLES32.glBindVertexArray(0);

		Matrix.setIdentityM(perspectiveProjectionMatrix, 0);


		smileyTexture[0] = loadGLTexture(R.raw.smiley);		// kundali, stone
		// about texture image


		// Depth enable setting
		GLES32.glClearDepthf(1.0f);
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		// enable culling for back faces
		//GLES32.glEnable(GLES32.GL_CULL_FACE);

		GLES32.glEnable(GLES32.GL_TEXTURE_2D);

		// clear color
		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		singleTap = 0;

	}

	private int loadGLTexture(int imageResourceID)
	{
		// Create Bitmap factory options objects
		BitmapFactory.Options options = new BitmapFactory.Options();

		// Dont scale the image
		options.inScaled = false;

		// Create the Bitmap Imgae from image resource
		Bitmap bitmap = BitmapFactory.decodeResource(
			context.getResources(),
			imageResourceID,
			options
		);

		// Create texture
		int[] texture = new int[1];

		GLES32.glGenTextures(1, texture, 0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);
		GLES32.glPixelStorei(
			GLES32.GL_UNPACK_ALIGNMENT,
			1
		);
		GLES32.glTexParameteri(
			GLES32.GL_TEXTURE_2D,
			GLES32.GL_TEXTURE_MAG_FILTER,
			GLES32.GL_LINEAR
		);
		GLES32.glTexParameteri(
			GLES32.GL_TEXTURE_2D,
			GLES32.GL_TEXTURE_MIN_FILTER,
			GLES32.GL_LINEAR_MIPMAP_LINEAR
		);

		GLUtils.texImage2D(			// it internall calls glTexImage2D()
			GLES32.GL_TEXTURE_2D,
			0,						// mimap level
			bitmap,					// this parameter itself covers 6 parameters of glTexImage2D() - 4. interal image format 5. width, 6. height, 7. external format, 8. data cha type 9. actual data
			0						// border width
		);

		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);
		return texture[0];
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
		float[] sqaure_texcoord = new float[8];

		// code 
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);

		// Transformations
		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -3.0f);
		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);
		
		//GLES32.glBindVertexArray(vao[0]);
		//GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
		//GLES32.glBindVertexArray(0);


		//For Texture
		// We can as many texures to a geomatry
		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, smileyTexture[0]);
		GLES32.glUniform1i(textureSamplerUniform, 0);

		GLES32.glBindVertexArray(vao[0]);

		if (singleTap == 1)
		{
			sqaure_texcoord[0] = 1.0f;
			sqaure_texcoord[1] = 1.0f;
			sqaure_texcoord[2] = 0.0f;
			sqaure_texcoord[3] = 1.0f;
			sqaure_texcoord[4] = 0.0f;
			sqaure_texcoord[5] = 0.0f;
			sqaure_texcoord[6] = 1.0f;
			sqaure_texcoord[7] = 0.0f;

			GLES32.glUniform1i(keyPressedUniform, 1);
		}
		else if (singleTap == 2)
		{
			sqaure_texcoord[0] = 0.5f;
			sqaure_texcoord[1] = 0.5f;
			sqaure_texcoord[2] = 0.0f;
			sqaure_texcoord[3] = 0.5f;
			sqaure_texcoord[4] = 0.0f;
			sqaure_texcoord[5] = 0.0f;
			sqaure_texcoord[6] = 0.5f;
			sqaure_texcoord[7] = 0.0f;

			GLES32.glUniform1i(keyPressedUniform, 1);

		}
		else if (singleTap == 3)
		{
			sqaure_texcoord[0] = 2.0f;
			sqaure_texcoord[1] = 2.0f;
			sqaure_texcoord[2] = 0.0f;
			sqaure_texcoord[3] = 2.0f;
			sqaure_texcoord[4] = 0.0f;
			sqaure_texcoord[5] = 0.0f;
			sqaure_texcoord[6] = 2.0f;
			sqaure_texcoord[7] = 0.0f;

			GLES32.glUniform1i(keyPressedUniform, 1);

		}
		else if (singleTap == 4)
		{
			sqaure_texcoord[0] = 0.5f;
			sqaure_texcoord[1] = 0.5f;
			sqaure_texcoord[2] = 0.5f;
			sqaure_texcoord[3] = 0.5f;
			sqaure_texcoord[4] = 0.5f;
			sqaure_texcoord[5] = 0.5f;
			sqaure_texcoord[6] = 0.5f;
			sqaure_texcoord[7] = 0.5f;

			GLES32.glUniform1i(keyPressedUniform, 1);

		}
		else
		{
			GLES32.glUniform1i(keyPressedUniform, 0);

		}

	// KeyPressed Uniforms
	//if (keyPressed == 1)
	//{
	//
	//}

	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture[0]);

	ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sqaure_texcoord.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	FloatBuffer textureBuffer = byteBuffer.asFloatBuffer();
	textureBuffer.put(sqaure_texcoord);
	textureBuffer.position(0);

	GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sqaure_texcoord.length * 4, textureBuffer, GLES32.GL_DYNAMIC_DRAW);

	//GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sizeof(sqaureTexcoord), sqaureTexcoord, GL_DYNAMIC_DRAW);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

	GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
	GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);
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
		if (smileyTexture[0] > 0)
		{
			GLES32.glDeleteTextures(1, smileyTexture, 0);
			smileyTexture[0] = 0;
		}
		if (vbo_texture[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_texture, 0);
			vbo_texture[0] = 0;
		}
		if(vbo_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_position, 0);
			vbo_position[0] = 0;
		}
		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}
	}

}