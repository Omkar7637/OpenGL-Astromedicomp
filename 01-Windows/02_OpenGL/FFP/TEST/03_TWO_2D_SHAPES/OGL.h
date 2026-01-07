#pragma once

#define MYICON 101

// 	// code
// 	// clear OpenGL buffers
// 	glClear(GL_COLOR_BUFFER_BIT); //Clears the color buffer to prepare for new drawing.
// 	glLoadIdentity();
    
// // 
// 	float glX1, glY1, glZ1;
// 	float glX2, glY2, glZ2;
// 	float glX3, glY3, glZ3;
// 	float glX4, glY4, glZ4;
// 	float glX5, glY5, glZ5;

// 	// ff7f2aff :- orange
// 	// deaa87ff :- Skin dark
// 	// ffccaaff :- Skin light 

// 	float r, g, b;
// 	hexToRGB("#ff7f2aff", &r, &g, &b); // Example: Orange Color

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(60, 540, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(60, 550, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(450, 550, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(450, 540, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();

// 	hexToRGB("#deaa87ff", &r, &g, &b); // Example: Orange Color

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(65, 525, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(60, 540, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(450, 540, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(448, 525, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();

// 	hexToRGB("#e9c6afff", &r, &g, &b); // Example: Orange Color

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(65, 505, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(65, 525, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(448, 525, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(450, 505, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();


// 	hexToRGB("#d38d5fff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(65, 500, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(65, 505, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(448, 505, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(448, 500, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();

// 	hexToRGB("#e1a171ff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(65, 495, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(65, 500, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(448, 500, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(448, 495, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();



// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(35, 540, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(35, 545, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(60, 550, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(60, 540, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(20, 531, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(20, 531, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(35, 545, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(35, 540, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();	
// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(450, 540, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(450, 550, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(470, 562, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(470, 555, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();


// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(470, 555, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(470, 562, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(687, 562, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(687, 555, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(740, 525, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(740, 530, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(784, 530, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(784, 525, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();


// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(700, 534, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(700, 539, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(757, 539, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(757, 534, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
	
// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(666, 542, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(666, 550, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(713, 550, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(713, 542, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
	
// 	hexToRGB("#ff7f2aff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(116, 555, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(116, 563, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(398, 563, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(398, 555, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
	
// 	hexToRGB("#deaa87ff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(123, 532, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(123, 555, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(390, 555, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(390, 532, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
	
// 	hexToRGB("#ffccaaff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(125, 510, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(123, 532, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(390, 532, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(388, 510, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
	
// 	hexToRGB("#c87137ff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(125, 495, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(121, 510, 0, &glX2, &glY2, &glZ2, 800, 600, 1000);
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(390, 510, 0, &glX3, &glY3, &glZ3, 800, 600, 1000);
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(388, 495, 0, &glX4, &glY4, &glZ4, 800, 600, 1000);
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
// 	hexToRGB("#a05a2cff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(125, 487, 0, &glX1, &glY1, &glZ1, 800, 600, 1000);
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(125, 495, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // 1
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(388, 495, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); // 4
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(388, 487, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	

// 	hexToRGB("#deaa87ff", &r, &g, &b); 

// 	glBegin(GL_QUADS);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(34, 522, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); // 4
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(35, 540, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // 3
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(60, 540, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(65, 525, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
// 	hexToRGB("#deaa87ff", &r, &g, &b); 

// 	glBegin(GL_POLYGON);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(20, 515, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(20, 531, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); 
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(35, 540, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(34, 522, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
// 	hexToRGB("#e6c0a6ff", &r, &g, &b); 

// 	glBegin(GL_POLYGON);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(20, 505, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(20, 515, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // done
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(34, 522, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(34, 505, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	

// 	hexToRGB("#e6c0a6ff", &r, &g, &b); 
// 	glBegin(GL_POLYGON);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(34, 504, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(34, 522, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); 
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(65, 526, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(65, 506, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
// 	hexToRGB("#d38d5fff", &r, &g, &b); 
// 	glBegin(GL_POLYGON);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(19, 501, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(20, 505, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); 
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(65, 506, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); 
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(65, 500, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	
// 	hexToRGB("#ff9955ff", &r, &g, &b); 
// 	glBegin(GL_POLYGON);
// 	glColor3f(r, g, b);
// 	screenToGLVertex(19, 495, 0, &glX1, &glY1, &glZ1, 800, 600, 1000); 
// 	glVertex3f(glX1, glY1, glZ1);
// 	screenToGLVertex(19, 501, 0, &glX2, &glY2, &glZ2, 800, 600, 1000); // done
// 	glVertex3f(glX2, glY2, glZ2);
// 	screenToGLVertex(65, 500, 0, &glX3, &glY3, &glZ3, 800, 600, 1000); // done
// 	glVertex3f(glX3, glY3, glZ3);
// 	screenToGLVertex(65, 495, 0, &glX4, &glY4, &glZ4, 800, 600, 1000); 
// 	glVertex3f(glX4, glY4, glZ4);
// 	glEnd();
	