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


	private int[] vao_sphere = new int[1];
	private int[] vbo_sphere_position = new int[1];
	private int[] vbo_sphere_normal = new int[1];
	private int[] vbo_sphere_element = new int[1];

	private int numVertices = 0;
	private int numElements = 0;

	private int modelMatrixUniform;
	private int viewMatrixUniform;
	private int projectionMatrixUniform;
	float[] perspectiveProjectionMatrix = new float[16];

	
	private int lightDiffuseUniform = 0;
	private int lightAmbientUniform = 0;
	private int lightSpecularUniform = 0;
	private int lightPositionUniform = 0;
	
	private int materialDiffuseUniform = 0;
	private int materialAmbientUniform = 0;
	private int materialSpecularUniform = 0;
	private int materialShinynessUniform = 0;
	private int doubleTapUniform = 0;

	private float angle_sphere = 0;
	
	private boolean bLightingEnable = false;
	private boolean bAnimationEnable = false;

	private float[] lightAmbient = { 0.1f, 0.1f, 0.1f, 1.0f };
	private float[] lightDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	private float[] lightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	private float[] lightPosition = { 100.0f, 100.0f, 100.0f, 1.0f };
	
	
	private float[] materialAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };
	private float[] materialDiffuse = { 0.5f, 0.2f, 0.7f, 1.0f };
	private float[] materialSpecular = { 0.7f, 0.7f, 0.7f, 1.0f };
	private float materialShinyness = 50.0f;
	
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
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec3 uLightAmbient;"+
			"uniform vec3 uLightDiffuse;"+
			"uniform vec3 uLightSpecular;"+
			"uniform vec4 uLightPosition;"+
			"uniform vec3 uMaterialAmbient;"+
			"uniform vec3 uMaterialDiffuse;"+
			"uniform vec3 uMaterialSpecular;"+
			"uniform float uMaterialShinyness;"+
			"uniform mediump int uDoubleTap;"+
			"out vec3 oPhongADSLight;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap==1)"+
			"{"+
			"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
			"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);"+
			"vec3 lightDirection = normalize(vec3(uLightPosition - eyeCoordinates));"+
			"vec3 reflectionVector=reflect(-lightDirection,transformedNormals);"+
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);"+
			"vec3 ambientLight=uLightAmbient*uMaterialAmbient;"+
			"vec3 diffuseLight=uLightDiffuse*uMaterialDiffuse*max(dot(lightDirection,transformedNormals),0.0);"+
			"vec3 specularLight=uLightSpecular*uMaterialSpecular*pow(max(dot(reflectionVector,viewerVector),0.0),uMaterialShinyness);"+
			"oPhongADSLight = ambientLight + diffuseLight + specularLight;"+
			"}"+
			"else"+
			"{"+
			"oPhongADSLight = vec3(0.0f,0.0f,0.0f);"+
			"}"+
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
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
			"in vec3 oPhongADSLight;"+
			"uniform mediump int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap==1)"+
			"{"+
			"FragColor=vec4(oPhongADSLight,1.0f);"+
			"}"+
			"else"+
			"{"+
			"FragColor=vec4(1.0f,1.0f,1.0f,1.0f);"+
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
		modelMatrixUniform			= GLES32.glGetUniformLocation(shaderProgramObject, "uModelMatrix");
		viewMatrixUniform			= GLES32.glGetUniformLocation(shaderProgramObject, "uViewMatrix");
		projectionMatrixUniform		= GLES32.glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
		lightAmbientUniform			= GLES32.glGetUniformLocation(shaderProgramObject, "uLightAmbient");
		lightDiffuseUniform			= GLES32.glGetUniformLocation(shaderProgramObject, "uLightDiffuse");
		lightSpecularUniform		= GLES32.glGetUniformLocation(shaderProgramObject, "uLightSpecular");
		lightPositionUniform		= GLES32.glGetUniformLocation(shaderProgramObject, "uLightPosition");
									
		materialAmbientUniform		= GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");
		materialDiffuseUniform		= GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");
		materialSpecularUniform		= GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");
		materialShinynessUniform	= GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialShinyness");

		doubleTapUniform			= GLES32.glGetUniformLocation(shaderProgramObject, "uDoubleTap");


		Sphere sphere = new Sphere();
		float[] sphere_vertices = new float[1146];
		float[] sphere_normals = new float[1146];
		float[] sphere_textures = new float[764];
		short[] sphere_elements = new short[2280];

		sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
		numVertices = sphere.getNumberOfSphereVertices();
		numElements = sphere.getNumberOfSphereElements();

		// Geomatry / shape

		//final float[] trianglePositions = {0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f};

		// vao
		GLES32.glGenVertexArrays(1, vao_sphere, 0);

		// Bind
		GLES32.glBindVertexArray(vao_sphere[0]);

		
		// VBO for position
		GLES32.glGenBuffers(1, vbo_sphere_position, 0);
		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_position[0]);

		// prepare triangle vertices array for GL Buffer Data 
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(sphere_vertices);
		verticesBuffer.position(0);

		GLES32.glBufferData(
			GLES32.GL_ARRAY_BUFFER, 
			sphere_vertices.length * 4, 
			verticesBuffer, 
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
		GLES32.glGenBuffers(1, vbo_sphere_normal, 0);
		// bind
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_normal[0]);

		// prepare triangle vertices array for GL Buffer Data 
		byteBuffer = ByteBuffer.allocateDirect(sphere_normals.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer normalsBuffer = byteBuffer.asFloatBuffer();
		normalsBuffer.put(sphere_normals);
		normalsBuffer.position(0);

		GLES32.glBufferData(
			GLES32.GL_ARRAY_BUFFER, 
			sphere_normals.length * 4, 
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


		// vbo for elements
		GLES32.glGenBuffers(1, vbo_sphere_element, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_element[0]);

		byteBuffer = byteBuffer.allocateDirect(sphere_elements.length * 2);
		byteBuffer.order(ByteOrder.nativeOrder());
		ShortBuffer elementsBuffer = byteBuffer.asShortBuffer();
		elementsBuffer.put(sphere_elements);
		elementsBuffer.position(0);

		GLES32.glBufferData(
			GLES32.GL_ARRAY_BUFFER,
			sphere_elements.length * 2,
			elementsBuffer,
			GLES32.GL_STATIC_DRAW
		);
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
		float[] modelMatrix = new float[16];
		float[] viewMatrix = new float[16];
		//float[] projectionMatrix = new float[16];

		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.setIdentityM(viewMatrix, 0);
		//Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		float[] translationMatrix = new float[16];
		float[] rotationMatrix = new float[16];

		Matrix.setIdentityM(translationMatrix, 0);
		Matrix.setIdentityM(rotationMatrix, 0);


		Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -3.0f);
		//Matrix.setRotateM(rotationMatrix, 0, angle_sphere, 0.0f, 1.0f, 0.0f);
		
		//Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);


		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);

		if (bLightingEnable == true)
		{
			GLES32.glUniform1i(doubleTapUniform, 1);
			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbient, 0);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuse, 0);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecular, 0); 
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0); 
			
			GLES32.glUniform3fv(materialAmbientUniform, 1, materialAmbient, 0);
			GLES32.glUniform3fv(materialDiffuseUniform, 1, materialDiffuse, 0);
			GLES32.glUniform3fv(materialSpecularUniform, 1, materialSpecular, 0);
			GLES32.glUniform1f(materialShinynessUniform, materialShinyness);
			

		}
		else
		{
			GLES32.glUniform1i(doubleTapUniform, 0);
		}

		
		 // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
        
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
		angle_sphere = angle_sphere + 0.4f;
		if(angle_sphere >= 360.0f)
		{
			angle_sphere = 0.0f;
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

		if(vbo_sphere_normal[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
			vbo_sphere_normal[0] = 0;
		}
		if(vbo_sphere_element[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
			vbo_sphere_element[0] = 0;
		}
		if(vbo_sphere_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
			vbo_sphere_position[0] = 0;
		}
		if(vao_sphere[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
			vao_sphere[0] = 0;
		}
	}

}