#include <iostream>
#include <GL/glut.h>
#include "figures.h"

using namespace std;

int window1 = 0, window2 = 0;
int X = 0, Y = 0;
int W1 = 200, H1 = 500;
int W2 = 500, H2 = 500;
GLfloat *clr = new GLfloat[3]; // the current color

// set the current color
void setColor(GLfloat r, GLfloat g, GLfloat b) {
  clr[0] = r;
  clr[1] = g;
  clr[2] = b;
}

//Strumenti window
void reshapeWin1(int neww, int newh) {
  W1 = neww;
  H1 = newh;
  glViewport(0, 0, W1, H1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., (GLdouble)W1, 0., (GLdouble)H1);
  glMatrixMode(GL_MODELVIEW);
}

void displayWin1() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
  glVertex2f(0, H1/2);
  glVertex2f(W1, H1/2);
  glEnd();

  glutSwapBuffers();
}

void mouseWin1(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){
	 if ((H1 - y) < H1/2) 
		setColor(1, 0, 0);
	 else 
		setColor(0, 1, 0);
  }
}


//Foglio window
void reshapeWin2(int neww, int newh) {
  W2 = neww;
  H2 = newh;
  glViewport(0, 0, W2, H2);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., (GLdouble)W2, 0., (GLdouble)H2);
  glMatrixMode(GL_MODELVIEW);
}

void displayWin2() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Point *p1 = new Point(X, Y);
  Point *p2 = new Point(X+20, Y);
  Figure *f = new Line(p1, p2, clr);
  f->draw();

  glutSwapBuffers();
}

void mouseWin2(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){
	 X = x;
	 Y = H2 - y;
	 glutPostRedisplay();
  }
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
		 
  glutInitWindowPosition(200, 0);
  glutInitWindowSize(W2, H2);
  window2 = glutCreateWindow("Foglio");
  glutReshapeFunc(reshapeWin2);
  glutDisplayFunc(displayWin2);
  glutMouseFunc(mouseWin2);

  glutMainLoop();  
}
