#include <stdlib.h> // Remove at Uni.
// Change back at Uni.
#include <glut.h>
#include <glaux.h>
#include <math.h>
#include <iostream>
/*
	
	- Q - Quit 
	- E - Lights on/off (Also changes switch animation, right bottom wall)
	- P - Lights on/off Upstairs
	- W - Move Camera Forward
	- S - Move Camera Backward
	- A - Change State Right
	- S - Change State Left
	- F - Enter Full Screen
	- 1 - Turn Anaglyph On
	- 2 - Turn Anaglyph Off
	
*/



// Pre-Define methods.
void renderOutput();	// Render Output Function 
void keyboardFunction(unsigned char key, int x, int y);	// Keyboard function accepting parameteres key, x, and y
void processSpecialKeys(int key, int x, int y);	// Special Keys function, used for moving the camera with the directional arrows
void CreateTexture(LPSTR strFileName, int textureID); // Create Texture Function
void createScene();	// Create Scene (Own Function) This creates the floors and the walls
void createObjects();	// Create Objects (Own function) This creates objects that are placed around the room 
void createLegs(float x, float y, float z);	// Create Legs (own function) this accepts parameters x, y, and z for translation
void init_gl_enviroment(void);	// Init_gl_enviroment (Anaglyph Code) function
void processCamera(bool look);	// Process Camera (Anaglyph Code) function
void mouse(int button, int state, int x, int y);
void offOnLights();	// OffOnLights (Own Function) 
void offOnLightsUpstairs();	// offOnLights (own function)
void createBooks(float x, float y, float z, int texture); // Create Books (Own function) accepting parameters x, y and z

// Create Variables used through the program
float moveX = 0;
float moveY = 0;
float moveZ = 0;
float rotate = 0;
UINT g_Texture[10];
bool textured= true;
int windowHeight = 600;	 // Define window height
int windowWidth = 1000;	// Define Window Width
float seperation;
float aspect;
float camX, camY, camZ, radius, camVerticleAngle, camHorizontalAngle;

// Booleans used for light switches
bool lightOn = false;
bool lightOff = true;

bool lightTwoOn = true;
bool lightTwoOff = false;

// Boolean for Anaglyph
bool anaglyphOn = false;

// Manipulate vertixs (Used for transforming the light switch)
float theVertexOn = 0.02;
float theVertexOff = 0.1;

// Main Function (Entry Point) 
void main (int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow("3D Room Application");

	init_gl_enviroment(); // Render the enviroment

	// Load Textures
	CreateTexture("Carpet.bmp", 1);
	CreateTexture("WallpaperBottomFloor.bmp", 2);
	CreateTexture("WoodDeskTexture.bmp", 3);
	CreateTexture("UniLogo.bmp", 4);
	CreateTexture("DarkWood.bmp", 5);
	CreateTexture("BookShelfTexture.bmp", 6);
	CreateTexture("BookTextureOne.bmp", 7);
	CreateTexture("ComputerBaseTexture.bmp", 8);
	CreateTexture("ComputerBaseTextureFront.bmp", 9);
	CreateTexture("Painting.bmp", 10);

	glutDisplayFunc(renderOutput);
	glutKeyboardFunc(keyboardFunction);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(mouse);
	glutMainLoop(); // Means keep going

}

// Process Camera Function
void processCamera(bool look)
{
	if(look)
	{
		camX = fabs(radius) * (sin(camHorizontalAngle+seperation) * cos(camVerticleAngle));
		camZ = fabs(radius) * (cos(camHorizontalAngle+seperation) * cos(camVerticleAngle));
		camY = fabs(radius) * sin(camVerticleAngle);
	}
	else // Else we are calculating the right eye.
	{
		camX = fabs(radius) * (sin(camHorizontalAngle-seperation)*cos(camVerticleAngle));
		camZ = fabs(radius) * (cos(camHorizontalAngle-seperation)*cos(camVerticleAngle) );
		camY = fabs(radius) * sin(camVerticleAngle);
	}

	// Config the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, 1.3, 1, 50);

	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay(); // Constantly update the scene
}

// Init GL Enviroment Function
void init_gl_enviroment(void)
{
	glClearColor(0,0,0,0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);								// Depth testing in the Z buffer.
	glEnable(GL_NORMALIZE);									// Enables face normalisation in theory, needs to research more.
	glEnable(GL_COLOR_MATERIAL);							// Enables standard OpenGL colors to behave as materials, not very flexible but quicker in practice.
	
	// Lighting for Light One
	GLfloat ambientLight[] = { 0.5, 0.5, 0.5, 1.0f };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.5f, 1.5f, 1.0f };
	GLfloat position[] = { -8.0f, 5.0f, 0.0f, 1.0f };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	// Create Properties for second light
	GLfloat ambientLightTwo[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
	GLfloat diffuseLightTwo[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLightTwo[] = { 1.0f, 1.5f, 1.5f, 1.0f }; 
	GLfloat positionTwo[] = { 0.0f, -1.0f, 0.0f, 0.0f };
	
	// Add and apply details to second light
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLightTwo);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLightTwo);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLightTwo);
	glLightfv(GL_LIGHT1, GL_POSITION, positionTwo); // Position not working (Could not get light to go higher)


	glEnable(GL_LIGHTING); // Activates Lighting.
	
	// Make the lights turn on on start up
	offOnLights();
	offOnLightsUpstairs();


	// INITIALISE orbit camera variables
	radius = 20;					// sets the distance of the camera from the world center
	camVerticleAngle = 0;			// Sets the angle the camera will use to generate its position
	camHorizontalAngle = 0;			//
	seperation = 0.03;				// Sets the number of degrees the two cameras are seperated by.
	aspect = windowWidth/windowHeight; // not as useeful as I hoped.
}


// Render Output Function 
void renderOutput()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the buffers as normal

	processCamera(false);									// Runs the camera method to set up the left eye.
	if(anaglyphOn == true)
	{
		glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);		// Sets the colour mask to red
	}
	createScene();		
	createObjects();										// Runs the build scen method, were we made our polygons and primitives.
	glClear(GL_DEPTH_BUFFER_BIT);							// Clears only the depth buffer, not the colour buffer.

	processCamera(true);									// Sets up the camera to the right eye position.
	
	if(anaglyphOn == true)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);	// Sets the mask to blue
	}		
	createScene();											// Re-build the geometry	
	createObjects();
	glColorMask(true, true, true, true);					// Set the mask back to normal.

	glutSwapBuffers();
}
// Mouse Function
void mouse(int button, int state, int x, int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
	if(state == GLUT_DOWN)
			moveX = 0;
			moveY = 0;
	break;

	case GLUT_RIGHT_BUTTON:
	if(state == GLUT_DOWN)
	break;

	default:
	break;
	}
	glutPostRedisplay();
}

// Create Scene Function
void createScene()
{
	glPushMatrix();

	glRotatef(rotate,0,1,0);
	glTranslatef(moveX,moveY,moveZ);

	// FLOOR, TEXTURED WITH IMAGE 1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1); // 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 2.0f);
		glVertex3f(-10, 0.0, -10);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(10, 0.0,-10);

		glTexCoord2f(2.0f, 0.0f);
		glVertex3f(10, 0.0,10);

		glTexCoord2f(2.0f, 2.0f);
		glVertex3f(-10, 0.0,10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// FLOOR 2, TEXTURE IMAGE 1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 2.0f);
		glVertex3f(-10, 15, -10); 

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10, 15, 5); 

		glTexCoord2f(2.0f, 2.0f);
		glVertex3f(10, 15, 5);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10, 15, -10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// WALL 1, TEXTURED WITH IMAGE 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2); 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10, 0.0, -10);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-10, 0.0,10);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-10, 15.0,10);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10, 15.0, -10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// WALL 2, TEXTURED WITH IMAGE 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2); 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10, 0.0, -10);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10, 0.0,-10);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10, 15.0,-10);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10, 15.0, -10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// WALL 3, TEXTURED WITH IMAGE 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2); 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10, 15.0, -10);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-10, 15.0,10);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-10, 30.0,10);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10, 30.0, -10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// WALL 4, TEXTURED WITH IMAGE 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2); 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10, 15.0, -10);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10, 15.0,-10);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10, 30.0,-10);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10, 30.0, -10);
	glEnd();

	glPopMatrix();

	glutPostRedisplay(); // Redisplay the scene
}

// Create Objects function
void createObjects()
{

	// Begin Making Stairs in for Loop
	for (int i = 0; i < 7; i++)
	{
		glPushMatrix();
		glTranslatef(-2, 0, 5);
		glRotatef(-90, 0, 1, 0);
			glPushMatrix();			
			glTranslatef(0, i*2, i*1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 5);
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0, 3, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(4.0, 3, 0.0);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(4.0, 0.0, 0.0);
			glEnd();

			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0, 3, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, 3, 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.0, 0.0, 2);
			glEnd();

			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(4.0, 0.0, 0.0);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(4.0, 3, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(4.0, 3, 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(4.0, 0.0, 2);
			glEnd();
			
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.0, 3, 0.0);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0, 3, 2);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(4.0, 3, 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(4.0, 3, 0.0);
			glEnd();
			glPopMatrix();
		glPopMatrix();
	}
	
	// End Foor loop for stairs

	// Enable GL_TEXTURE_2D for 2d images
	glEnable(GL_TEXTURE_2D);

	// Use this to determine the location of X, Y, Z
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glBegin(GL_LINES);
		//glColor3ub(255,0,0);
		glVertex3f(10,0,0);
		glVertex3f(0,0,0);

		//glColor3ub(0,255,0);
		glVertex3f(0,10,0);
		glVertex3f(0,0,0);

		//glColor3ub(0,0,255);
		glVertex3f(0,0,10);
		glVertex3f(0,0,0);
	glEnd();
	glPopMatrix();
	
	// ** CREATE BED ** //
	glPushMatrix();
	glTranslatef(0, 16, -7);
	glRotatef(0, 0, 1, 0);
	
	// Back Panel
	glBegin(GL_POLYGON);
		glVertex3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(4,0,0);
		glVertex3f(4,1,0);
	glEnd();
		
		// Front Panel
	glBegin(GL_POLYGON);
		glVertex3f(0, 1, 7);
		glVertex3f(0, 0, 7);
		glVertex3f(4, 0, 7);
		glVertex3f(4, 1, 7);
	glEnd();
		
		// Left Panel
	glBegin(GL_POLYGON);
		glVertex3f(0, 1, 7);
		glVertex3f(0, 0, 7);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
	glEnd();
		
		// Right Panel
	glBegin(GL_POLYGON);
		glVertex3f(4, 1, 7);
		glVertex3f(4, 0, 7);
		glVertex3f(4, 0, 0);
		glVertex3f(4, 1, 0);
	glEnd();

		// Top Panel
	glBegin(GL_POLYGON);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 1, 7);
		glVertex3f(4, 1, 7);
		glVertex3f(4, 1, 0);
	glEnd();

		// Bottom Panel
	glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 7);
		glVertex3f(4, 0, 7);
		glVertex3f(4, 0, 0);
	glEnd();
	glPopMatrix();

	// Legs for Bed
	createLegs(0.4, 15, -5.5);
	createLegs(3.5, 15, -5.5);
	createLegs(3.5, 15, -1.5);
	createLegs(0.4, 15, -1.5);

	// End Creating Bed

	// ** CREATE DESK ** // 
	glPushMatrix();
	glTranslatef(-9.8,0,-9.8);
	glBindTexture(GL_TEXTURE_2D, 3); // Enable Desk Texture
	
	// Back Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0); //Left Bottom Vertex

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 4, 0); // Left Top Vertex

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(6.5, 4, 0); // Right Top Vertex

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(6.5, 0, 0); // Right Bottom Vertexd
	glEnd();

	// Back Left Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 12);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 4, 12);

		glTexCoord2f(1.0, 1.0f);
		glVertex3f(0, 4, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 0);
	glEnd();

	// Front Left Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 12);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 4, 12);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 4, 12);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 12); 
	glEnd();

	// Center Right Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(3.25, 0, 3.25);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.25, 4, 3.25);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(6.5, 4, 3.25);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(6.5, 0, 3.25);
	glEnd();

	// Right Panel Back
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(6.5, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(6.5, 4, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(6.5, 4, 3.25);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(6.5, 0, 3.25);
	glEnd();

	// Bottom Left Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 8.75);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 0, 12);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 0, 12);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 8.75);
	glEnd();

	// Bottom Right Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 4.75);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 0, 0);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 0, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 4.75);
	glEnd();
 
	// Bottom Right Panel 2
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(3.25, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.25, 0, 3.25);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(6.5, 0, 3.25);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(6.5, 0, 0);
	glEnd();

	// Top Left Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 4, 0);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 4, 12);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 4, 12);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 4, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(3.25, 0, 12);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.25, 4, 12);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 4, 8.75);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 8.75);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 8.75);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 4, 8.75);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 4, 8.75);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 8.75);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(3.25, 0, 0);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.25, 4, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 4, 4.75);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 4.75);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 4.75);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 4, 4.75);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.25, 4, 4.75);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3.25, 0, 4.75);
	glEnd();

	// Top Right Panel
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(3.25, 4, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.25, 4, 3.25);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(6.5, 4, 3.25);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(6.5, 4, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// End creation of Desk
	
	// Uni Logo
	glPushMatrix();
	glTranslatef(-9.9, 6, -5);
	glRotatef(-90, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 4);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 3.5, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, 3.5, 6);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 6);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	// End Uni Logo

	// Begin Base Light Switch
	glPushMatrix();
	glTranslatef(6, 8, -9.8);
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0.5, 0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(0.5, 0, 0);
	glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0.5, 0);
		glVertex3f(0, 0.5, -0.1);
		glVertex3f(0, 0, -0.1);
	glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.5, 0, 0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(0.5, 0.5, -0.1);
		glVertex3f(0.5, 0, -0.1);
	glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0, 0.5, 0);
		glVertex3f(0, 0.5, -0.1);
		glVertex3f(0.5, 0.5, -0.1);
		glVertex3f(0.5, 0.5, 0);
	glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -0.1);
		glVertex3f(0.5, 0, -0.1);
		glVertex3f(0.5, 0, 0);
	glEnd();

	//BEGIN SWITCH
	glBegin(GL_POLYGON); // Left Side
		glVertex3f(0.1, 0.2, 0);
		glVertex3f(0.1, 0.4, 0);
		glVertex3f(0.1, 0.4, theVertexOff); // Transform Vertex's
		glVertex3f(0.1, 0.2, theVertexOn);	// Transform Vertex's
	glEnd(); 
	//FRONT
	glBegin(GL_POLYGON);
		glVertex3f(0.1, 0.2, theVertexOn);
		glVertex3f(0.1, 0.4, theVertexOff);
		glVertex3f(0.4, 0.4, theVertexOff);
		glVertex3f(0.4, 0.2, theVertexOn);
	glEnd();
	//right
	glBegin(GL_POLYGON);
		glVertex3f(0.4, 0.2, theVertexOn);
		glVertex3f(0.4, 0.4, theVertexOff);
		glVertex3f(0.4, 0.4, 0);
		glVertex3f(0.4, 0.2, 0);
	glEnd();
	// back
	glBegin(GL_POLYGON);
		glVertex3f(0.1, 0.2, 0);
		glVertex3f(0.1, 0.4, 0);
		glVertex3f(0.4, 0.4, 0);
		glVertex3f(0.4, 0.2, 0);
	glEnd();

	// Top
	glBegin(GL_POLYGON);
		glVertex3f(0.1, 0.4, 0); 
		glVertex3f(0.1, 0.4, theVertexOff);
		glVertex3f(0.4, 0.4, theVertexOff);
		glVertex3f(0.4, 0.4, 0);
	glEnd();

	// Bottom
	glBegin(GL_POLYGON);
		glVertex3f(0.1, 0.2, 0);
		glVertex3f(0.1, 0.2, theVertexOn);
		glVertex3f(0.4, 0.2, theVertexOn);
		glVertex3f(0.4, 0.2, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// End creation of switch
	
	// Teapot
	glPushMatrix();
		glTranslatef(-8, 4.3, 0);
		glutSolidTeapot(0.5);
	glPopMatrix();
	// End Teapot

	// Book Shelf
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 6);
	glTranslatef(-6.5, 4, -9.7);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 3, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, 3, 2);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 2);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1.0, 0.0f);
		glVertex3f(0, 3, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3, 3, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3, 0, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(3, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3, 3, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3, 3, 2);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3, 0, 2);
	glEnd();

	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 3, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 3, 2);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3, 3, 2);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3, 3, 0);
	glEnd();

	// Shelf
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 1.5, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 1.5, 2);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3, 1.5, 2);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(3, 1.5, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// End Book Shelf

	// Call Create Books Functions
	createBooks(-5, 5.5, -9, 7);
	createBooks(-5, 5.7, -9, 7);
	createBooks(-5, 6, -9, 7);

	// Create Computer
	glPushMatrix();
	glTranslatef(-9, 0, -4);
	glRotatef(90, 0, 1, 0);
	// Left
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 8);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 3, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, 3, 2.5);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, 2.5);
	glEnd();			
	glDisable(GL_TEXTURE_2D);

	//Front
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 9);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 2.5);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 3, 2.5);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 3, 2.5);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 0, 2.5);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//Right	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 8);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1, 0, 2.5);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1, 3, 2.5);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 3, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 0, 0);
	glEnd();

	//Back 
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 3, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 3, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 0, 0);
	glEnd();
		
	//Top
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 3, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, 3, 2.5);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1, 3, 2.5);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1, 3, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	// End Computer

	// Begin Museum Item
	glPushMatrix();
	glTranslatef(0, 18, -9.7);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 10);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 4, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(4, 4, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(4, 0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// End Museum Item

	glutPostRedisplay();
}

// Create Legs Function
void createLegs(float x, float y, float z)
{
	// Variables used for simple editing
	float zAxis = 0.2;
	float xAxis = 0.2;
	float yAxis = 1.5;

	glPushMatrix();
	glTranslatef(x, y, z);
		// Leg Top Panel
		glBegin(GL_POLYGON);
			glVertex3f(0, yAxis, 0);
			glVertex3f(0, yAxis, zAxis);
			glVertex3f(xAxis, yAxis, zAxis);
			glVertex3f(xAxis, yAxis, 0);
		glEnd();
		
		// Leg Left Panel
		glBegin(GL_POLYGON);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, zAxis);
			glVertex3f(0, yAxis, zAxis);
			glVertex3f(0, yAxis, 0);
		glEnd();

		// Leg Right Panel
		glBegin(GL_POLYGON);
			glVertex3f(xAxis, yAxis, 0);
			glVertex3f(xAxis, yAxis, zAxis);
			glVertex3f(xAxis, 0, zAxis);
			glVertex3f(xAxis, 0, 0);
		glEnd();
		
		// Front Panel
		glBegin(GL_POLYGON);
			glVertex3f(0, 0, zAxis);
			glVertex3f(0, yAxis, zAxis);
			glVertex3f(xAxis, yAxis, zAxis);
			glVertex3f(xAxis, 0, zAxis);
		glEnd();

		// Back Panel
		glBegin(GL_POLYGON);
			glVertex3f(0, 0, 0);
			glVertex3f(0, yAxis, 0);
			glVertex3f(xAxis, yAxis, 0);
			glVertex3f(xAxis, 0, 0);
		glEnd();
	glPopMatrix();
}
// Create Books Function accepting also a texture INT, to change the texture when function is called (WORKS, But not used)
void createBooks(float x, float y, float z, int texture) 
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0, 0, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0, 0.2, 0);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0, 0.2, 1);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0, 0, 1);
		glEnd();

		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0, 0, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0, 0.2, 0);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1, 0.2, 0);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1, 0, 0);
		glEnd();

		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1, 0, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1, 0.2, 0);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1, 0.2, 1);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1, 0, 1);
		glEnd();
	
		
		glBegin(GL_POLYGON);
		glColor3ub(255, 255, 255);
			glVertex3f(0, 0, 1);
			glVertex3f(0, 0.2, 1);
			glVertex3f(0, 0.2, 1);
			glVertex3f(0, 0, 1);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(0, 0, 1);
			glVertex3f(0, 0.2, 1);
			glVertex3f(1, 0.2, 1);
			glVertex3f(1, 0, 1);
		glEnd();

		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0, 0.2, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0, 0.2, 1);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1, 0.2, 1);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1, 0.2, 0);
		glEnd();

		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0, 0, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0, 0, 1);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1, 0, 1);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1, 0, 0);
		glEnd();
		glPopMatrix();
}
// Keyboard Function
void keyboardFunction(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case 'a':	camHorizontalAngle -= 0.05;
		break;
		case 'd':	camHorizontalAngle += 0.05;
		break;
		case 'w':	camVerticleAngle += 0.05;
		break;
		case 's':	camVerticleAngle -= 0.05;
		break;
		case 'e':	
					offOnLights(); // Call offOnLights
		break;
		case 'p':	
					offOnLightsUpstairs();	// Call offOnLightsUpstairs
		break;
		case 'f':	glutFullScreen();	// Go Fullscreen
		break;
		case '1':	anaglyphOn = true;
		break;
		case '2':	anaglyphOn = false;
		break;
		case 'q':
		exit(0);	// Exit Program
		break;
		default:
		break;
	}

	glutPostRedisplay();
}
// Function Process Special Keys
void processSpecialKeys(int key, int x, int y)
{
	switch (key) 
	{
		case GLUT_KEY_LEFT:		seperation -= 0.01;	// Roate the camera left
		break;
		case GLUT_KEY_RIGHT:	seperation += 0.01;	// Rotate the camera Right
		break;
		case GLUT_KEY_UP:		radius -= 0.5;	// Move the camera in 
		break;
		case GLUT_KEY_DOWN:		radius += 0.5;	// Move the camera out
		break;
		default:
		break;
	}

	glutPostRedisplay();
}
// Function offOnLights
void offOnLights()
{
	// If light is on, and button pressed, do the following...
		if(lightOn == true)
		{
			glDisable(GL_LIGHT0);
			lightOn = false;
			lightOff = true;

			theVertexOn = 0.1;
			theVertexOff = 0.02;
		}
		// Else if light is off and button is pressed, do the following...
		else if(lightOff == true)
		{
			glEnable(GL_LIGHT0);
			lightOn = true;
			lightOff = false;

			theVertexOn = 0.02;
			theVertexOff = 0.1;
		}
}
// Function offOnLightsUpstairs, currently the position is wrong but light is working
void offOnLightsUpstairs()
{
	// If statements work the same as the above function
	if(lightTwoOn == true)
	{
		glDisable(GL_LIGHT1);
		lightTwoOn = false;
		lightTwoOff = true;
	}

	else if(lightTwoOff == true)
	{
		glEnable(GL_LIGHT1);
		lightTwoOn = true;
		lightTwoOff = false;
	}
}


// Create Texture Function
void CreateTexture(LPSTR strFileName, int textureID)
{	
	glEnable(GL_TEXTURE_2D);// Enable Texture Mapping
	AUX_RGBImageRec *pBitmap = NULL;
		
	if(!strFileName)// Return from the function if no file name was passed in
	return;
		
	// We need to load the texture data, so we use a cool API that the glaux.lib offers.
	//LPCWSTR castFileName = (LPCWSTR)strFileName;
	pBitmap = auxDIBImageLoad(strFileName);// Load the bitmap and store the data
		
	if(pBitmap == NULL)// If we can't load the file, quit!
	exit(0);

	// Now that we have the texture data, we need to register our texture with OpenGL
	// To do this we need to call glGenTextures().  The 1 for the first parameter is
	// how many texture we want to register this time (we could do a bunch in a row).
	// The second parameter is the array index that will hold the reference to this texture.

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &g_Texture[textureID]);

	// Now that we have a reference for the texture, we need to bind the texture
	// to tell OpenGL this is the reference that we are assigning the bitmap data too.
	// The first parameter tells OpenGL we want are using a 2D texture, while the
	// second parameter passes in the reference we are going to assign the texture too.
	// We will use this function later to tell OpenGL we want to use this texture to texture map.

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, g_Texture[textureID]);

	// Now comes the important part, we actually pass in all the data from the bitmap to
	// create the texture. Here is what the parameters mean in gluBuild2DMipmaps():
	// (We want a 2D texture, 3 channels (RGB), bitmap width, bitmap height, It's an RGB format,
	//  the data is stored as unsigned bytes, and the actuall pixel data);

	// What is a Mip map?  Mip maps are a bunch of scaled pictures from the original.  This makes
	// it look better when we are near and farther away from the texture map.  It chooses the
	// best looking scaled size depending on where the camera is according to the texture map.
	// Otherwise, if we didn't use mip maps, it would scale the original UP and down which would
	// look not so good when we got far away or up close, it would look pixelated.

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  Read more about 
	// the MIN and MAG filters at the bottom of main.cpp
			
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)	// If we loaded the bitmap
	{
		if (pBitmap->data)	// If there is texture data
		{
			free(pBitmap->data);	// Free the texture data, we don't need it anymore
		}
		free(pBitmap);	// Free the bitmap structure
	}
}