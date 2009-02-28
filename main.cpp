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
void selWin1(GLuint*);

int window1 = 0, window2 = 0;
int X = 0, Y = 0;
int W1 = 100, H1 = 500; // width and length of the Strumenti window
int W2 = 500, H2 = 500; // width and length of the Foglio window

// define figure names
#define LINE 1
#define TRIANGLE 2
#define QUAD 3
int figType = LINE; // the figure type selected: 1 line; 2 quad

// define color names
#define RED 6
#define GREEN 7
#define BLUE 8
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
  glVertex2f(x, y);
  glVertex2f(w, y);
  glVertex2f(w, z);
  glVertex2f(x, z);
  glEnd();
}


//Strumenti window
void reshapeWin1(int neww, int newh) {
  W1 = neww;
  H1 = newh;
  glViewport(0, 0, W1, H1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, W1, 0, H1, 1, -1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void displayWin1() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  int hb = H1/10; // height of the buttons

  // initialize the stack for the selection
  glInitNames();
  glPushName(0);
  
  //line
  glColor3f(1, 1, 1);
  glLoadName(LINE);
  drawQuad(0, H1, (GLfloat)W1/2.0, H1-hb);
  // triangle
  glColor3f(0, 0, 0);
  glLoadName(TRIANGLE);
  drawQuad(W1/2, H1, W1, H1-hb);

  glPushMatrix();
  glTranslatef(0, -hb, 0);

  // quad
  glColor3f(0.5, 0.5, 0.5);
  glLoadName(QUAD);
  drawQuad(0, H1, (GLfloat)W1/2.0, H1-hb);
  
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -3*(hb), 0);
  
  // current color
  glColor3f(clr[0], clr[1], clr[2]);
  drawQuad(0, H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -4*(hb), 0);
  
  // red
  glColor3f(1, 0, 0);
  glLoadName(RED);
  drawQuad(0, H1, (GLfloat)W1/3.0, H1-hb);
  // green
  glColor3f(0, 1, 0);
  glLoadName(GREEN);
  drawQuad((GLfloat)W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  // blue
  glColor3f(0, 0, 1);
  glLoadName(BLUE);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);
  
  glPopMatrix();
  
  // borders of the buttons
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
  glVertex2f((GLfloat)W1/2, H1);
  glVertex2f((GLfloat)W1/2, H1-(3*hb));
  glVertex2f((GLfloat)W1/3, H1-(4*hb));
  glVertex2f((GLfloat)W1/3, 0);
  glVertex2f(W1*(2.0/3.0), H1-(4*hb));
  glVertex2f(W1*(2.0/3.0), 0);
  for (int i = hb; i < H1; i += hb){
	 glVertex2f(0, H1-i);
	 glVertex2f(W1, H1-i);
  }
  glEnd();
  
  glutSwapBuffers();
}

// process selection
void selWin1(GLuint *pselectBuff){
  //  int count = pselectBuff[0];
  int id = pselectBuff[3];
  
  switch(id){
  case LINE:
	 cout << "Line\n";
	 figType = LINE;
	 break;
  case QUAD:
	 cout << "Quad\n";
	 figType = QUAD;
	 break;
  case TRIANGLE:
	 cout << "Triangle\n";
	 figType = TRIANGLE;
	 break;
  case RED:
	 cout << "Red\n";
	 setColor(1, 0, 0);
	 break;
  case GREEN:
	 cout << "Green\n";
	 setColor(0, 1, 0);
	 break;
  case BLUE:
	 cout << "Blue\n";
	 setColor(0, 0, 1);
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
	 glOrtho(0, W1, 0, H1, 1, -1);
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
		selWin1(selectBuff);
	 }
	 
	 glutPostRedisplay();
  }
}


//Foglio window
void reshapeWin2(int neww, int newh) {
  W2 = neww;
  H2 = newh;
  glViewport(0, 0, W2, H2);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, (GLdouble)W2, 0, (GLdouble)H2);
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
	 Point *p1 = new Point(x, H2-y);
	 Point *p2 = new Point(x, H2-y);
	 Figure *f;
	 
	 switch(figType) {
	 case LINE:
		f = new Line(p1, p2, clr[0], clr[1], clr[2]);
		break;
	 case QUAD: 
		f = new Quad(p1, p2, clr[0], clr[1], clr[2]);
		break;
	 case TRIANGLE:
		f = new Triangle(p1, p2, clr[0], clr[1], clr[2]);
		break;
	 }
	 figureSet.push_back(f);
  }

  glutPostRedisplay();
}

void mouseMotion(int x, int y) {
  Point *p = new Point(x, H2-y);

  switch(figType) {
  case LINE: {
	 Line *l = (Line*) figureSet.back();
	 l->setSecond(p);
	 break;
  }
  case QUAD: {
	 Quad *q = (Quad*) figureSet.back();
	 q->setThird(p);
	 break;
  }
  case TRIANGLE: {
	 Triangle *t = (Triangle*) figureSet.back();
	 t->setSecond(p);
	 break;
  }
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
