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

	private float angle_cube_X = 0.0f;
	private float angle_cube_Y = 0.0f;
	private float angle_cube_Z = 0.0f;

	private int mvpMatrixUniform;
	float[] perspectiveProjectionMatrix = new float[16];

	private int[] kundaliTexture = new int[1];
	private int textureSamplerUniform;

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
			"void main(void)"+
			"{"+
			"FragColor=texture(uTextureSampler, oTexCoord);"+  
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


		// Geomatry / shape

		final float[] cube_position =
		{
			// top
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
		
			// bottom
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
		
			 // front
			 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			 1.0f, -1.0f, 1.0f,
		
			 // back
			 1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
		
			 // right
			 1.0f, 1.0f, -1.0f,
			 1.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,
		
			 // left
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
		
		};

		final float[] cube_texcoords =
		{
			// front
			1.0f, 1.0f, // top-right of front
			0.0f, 1.0f, // top-left of front
			0.0f, 0.0f, // bottom-left of front
			1.0f, 0.0f, // bottom-right of front

			// right
			1.0f, 1.0f, // top-right of right
			0.0f, 1.0f, // top-left of right
			0.0f, 0.0f, // bottom-left of right
			1.0f, 0.0f, // bottom-right of right

			// back
			1.0f, 1.0f, // top-right of back
			0.0f, 1.0f, // top-left of back
			0.0f, 0.0f, // bottom-left of back
			1.0f, 0.0f, // bottom-right of back

			// left
			1.0f, 1.0f, // top-right of left
			0.0f, 1.0f, // top-left of left
			0.0f, 0.0f, // bottom-left of left
			1.0f, 0.0f, // bottom-right of left

			// top
			1.0f, 1.0f, // top-right of top
			0.0f, 1.0f, // top-left of top
			0.0f, 0.0f, // bottom-left of top
			1.0f, 0.0f, // bottom-right of top

			// bottom
			1.0f, 1.0f, // top-right of bottom
			0.0f, 1.0f, // top-left of bottom
			0.0f, 0.0f, // bottom-left of bottom
			1.0f, 0.0f, // bottom-right of bottom
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
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cube_position.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
		positionBuffer.put(cube_position);
		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cube_position.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		// VBO for Texture
		GLES32.glGenBuffers(1, vbo_texture, 0);

		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texture[0]);


		// prepare triangle vertices array for GL Buffer Data 
		byteBuffer = ByteBuffer.allocateDirect(cube_texcoords.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer textureBuffer = byteBuffer.asFloatBuffer();
		textureBuffer.put(cube_texcoords);
		textureBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cube_texcoords.length * 4, textureBuffer, GLES32.GL_STATIC_DRAW);
		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORDS, 2, GLES32.GL_FLOAT, false, 0, 0);
		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORDS);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);



		GLES32.glBindVertexArray(0);

		Matrix.setIdentityM(perspectiveProjectionMatrix, 0);


		kundaliTexture[0] = loadGLTexture(R.raw.kundali);		// kundali, stone
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
		// code 
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		GLES32.glUseProgram(shaderProgramObject);

		// Transformations
		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		float[] translationMatrix = new float[16];
		float[] rotationMatrix = new float[16];
		float[] rotationMatrix_X = new float[16];
		float[] rotationMatrix_Y = new float[16];
		float[] rotationMatrix_XY = new float[16];
		float[] rotationMatrix_Z = new float[16];

		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.setIdentityM(rotationMatrix, 0);
		Matrix.setIdentityM(rotationMatrix_X, 0);
		Matrix.setIdentityM(rotationMatrix_Y, 0);
		Matrix.setIdentityM(rotationMatrix_Z, 0);
		Matrix.setIdentityM(rotationMatrix_XY, 0);


		Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -6.0f);
		Matrix.setRotateM(rotationMatrix_X, 0, angle_cube_X, 1.0f, 0.0f, 0.0f);
		Matrix.setRotateM(rotationMatrix_Y, 0, angle_cube_Y, 0.0f, 1.0f, 0.0f);
		Matrix.setRotateM(rotationMatrix_Z, 0, angle_cube_Z, 0.0f, 0.0f, 1.0f);

		Matrix.multiplyMM(rotationMatrix_XY, 0, rotationMatrix_X, 0, rotationMatrix_Y, 0);

		Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix_XY, 0, rotationMatrix_Z, 0);

		Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);

		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, kundaliTexture[0]);
		GLES32.glUniform1i(textureSamplerUniform, 0);
	
		GLES32.glBindVertexArray(vao[0]);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);
		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);		

		
		update();
		// Render
		requestRender();
	}

	private void update()
	{
		angle_cube_X = angle_cube_X - 0.01f;
		angle_cube_Y = angle_cube_Y - 0.08f;
		angle_cube_Z = angle_cube_Z - 0.1f;

		if (angle_cube_X <= 0.0f)
		{
			angle_cube_X = angle_cube_X + 360.0f;
		}
		if (angle_cube_Y <= 0.0f)
		{
			angle_cube_Y = angle_cube_Y + 360.0f;
		}
		if (angle_cube_Z <= 0.0f)
		{
			angle_cube_Z = angle_cube_Z + 360.0f;
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
		if (kundaliTexture[0] > 0)
		{
			GLES32.glDeleteTextures(1, kundaliTexture, 0);
			kundaliTexture[0] = 0;
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