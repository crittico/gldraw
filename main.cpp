#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "figures.h"

using namespace std;

void setColor(GLfloat, GLfloat, GLfloat);
void reshapeWin1(int, int);
void displayWin1();
void mouseWin1(int, int, int, int);
void reshapeWin2(int, int);
void displayWin2();
void mouseWin2(int, int, int, int);
void mouseMotion(int, int);
void drawQuad(int, int, int, int);

int window1 = 0, window2 = 0;
int X = 0, Y = 0;
int W1 = 100, H1 = 500; // width and length of the Strumenti window
int W2 = 500, H2 = 500; // width and length of the Foglio window

// define figure names
#define LINE 1
#define QUAD 2
#define TRIANGLE 3
int figType = LINE; // the figure type selected: 0 line; 1 quad

// define color names
#define RED 4
#define GREEN 5
GLfloat *clr = new GLfloat[3]; // the current color

vector<Figure*> figureSet; // the vector that contains all the figures created


// set the current color
void setColor(GLfloat r, GLfloat g, GLfloat b) {
  clr[0] = r;
  clr[1] = g;
  clr[2] = b;
}

// draw a quad
void drawQuad(int x, int y, int w, int z) {
  glBegin(GL_QUADS);
  glVertex3f(x, y, 0);
  glVertex3f(w, y, 0);
  glVertex3f(w, z, 0);
  glVertex3f(x, z, 0);
  glEnd();
}


//Strumenti window
void reshapeWin1(int neww, int newh) {
  W1 = neww;
  H1 = newh;
  glViewport(0, 0, W1, H1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, (GLdouble)W1, 0, (GLdouble)H1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void displayWin1() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  // initialize the stack for the selection
  glInitNames();
  glPushName(0);
  
  //line
  glColor3f(1, 1, 1);
  glLoadName(LINE);
  drawQuad(0, H1, W1/2, H1-20);
  // quad
  glColor3f(0, 0, 0);
  glLoadName(QUAD);
  drawQuad(W1/2, H1, W1, H1-20);
  // red
  glColor3f(1, 0, 0);
  glLoadName(RED);
  drawQuad(0, H1-20, W1, H1/2);
  // green
  glColor3f(0, 1, 0);
  glLoadName(GREEN);
  drawQuad(0, H1/2, W1, 0);

  glPopMatrix();
  glutSwapBuffers();
}

// process selection
void processSelection(GLuint *pselectBuff){
  //  int count = pselectBuff[0];
  int id = pselectBuff[3];
  
  switch(id){
  case LINE:
	 cout << "Line\n";
	 figType = 0;
	 break;
  case QUAD:
	 cout << "Quad\n";
	 figType = 1;
	 break;
  case RED:
	 cout << "Red\n";
	 setColor(1, 0, 0);
	 break;
  case GREEN:
	 cout << "Green\n";
	 setColor(0, 1, 0);
	 break;
  }
}

void mouseWin1(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){

	 #define BUFFER_LENGTH 64
	 // Space for selection buffer
	 static GLuint selectBuff[BUFFER_LENGTH];
	 	// Hit counter and viewport storage
	 GLint hits, viewport[4];

	// Setup selection buffer
	glSelectBuffer(BUFFER_LENGTH, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending two pixels
	// in the vertical and horizontal direction
	glLoadIdentity();
	gluPickMatrix(x, viewport[3] - y + viewport[1], 2,2, viewport);
	//gluPickMatrix(x, y, 2, 2, viewport);

	// Apply perspective matrix 
	//gluPerspective(60.0f, W1/H1, 1.0, 425.0);
	gluOrtho2D(0, (GLdouble)W1, 0, (GLdouble)H1);
	glMatrixMode(GL_MODELVIEW);

	// Draw the scene
	displayWin1();

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

	// If a single hit occurred, display the info.
	if(hits == 1){
	  cout << hits << "foo";
	  cout << selectBuff[3] << endl;
	  processSelection(selectBuff);
	}

	glutPostRedisplay();

	 /*	 if (y < 20){
		if (x < W1/2)
		  figType = 0;
		else
		  figType = 1;
	 }
	 else if ((y < H1/2) && (y > 20)) 
		setColor(1, 0, 0);
	 else if (y > H1/2)
		setColor(0, 1, 0);
		}*/
  }
}


//Foglio window
void reshapeWin2(int neww, int newh) {
  W2 = neww;
  H2 = newh;
  glViewport(0, 0, W2, H2);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, (GLdouble)W2, (GLdouble)H2, 0);
  glMatrixMode(GL_MODELVIEW);
}

void displayWin2() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < (int)figureSet.size(); i++){
	 figureSet[i]->draw();
  }

  glutSwapBuffers();
}

void mouseWin2(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){
	 Point *p1 = new Point(x, y);
	 Point *p2 = new Point(x, y);
	 Figure *f;
	 if (figType == 0)
		f = new Line(p1, p2, clr[0], clr[1], clr[2]);
	 else 
		f = new Quad(p1, p2, clr[0], clr[1], clr[2]);
	 figureSet.push_back(f);
  }

  glutPostRedisplay();
}

void mouseMotion(int x, int y) {
  Point *p = new Point(x, y);
  if (figType == 0){
	 Line *l = (Line*) figureSet.back();
	 l->setSecond(p);
  }
  else {
	 Quad *q = (Quad*) figureSet.back();
	 q->setThird(p);
  }
  glutPostRedisplay();
}

int main(int argc, char* argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(W1, H1);
  window1 = glutCreateWindow("Strumenti");
  glutReshapeFunc(reshapeWin1);
  glutDisplayFunc(displayWin1);
  glutMouseFunc(mouseWin1);
		 
  glutInitWindowPosition(W1, 0);
  glutInitWindowSize(W2, H2);
  window2 = glutCreateWindow("Foglio");
  glutReshapeFunc(reshapeWin2);
  glutDisplayFunc(displayWin2);
  glutMouseFunc(mouseWin2);
  glutMotionFunc(mouseMotion);

  glutMainLoop();  
}
