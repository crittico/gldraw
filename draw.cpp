#include <iostream>
#include <GL/glut.h>
#include "figures.h"

using namespace std;

int window1 = 0, window2 = 0;
int X = 0, Y = 0;
int W2 = 500, H2 = 500;

//Strumenti window
void reshapeWin1(int w, int h) {
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(1, -1, 1, -1, 1, -1);
  glMatrixMode(GL_MODELVIEW);
}

void displayWin1() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glutSwapBuffers();
}

void mouseWin1(int button, int state, int x, int y) {}


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

  glColor3f(1, 0, 1);
  int d = 20;
  glBegin(GL_LINES);
  glVertex2i(X, Y);
  glVertex2i(X+d, Y);
  glEnd();

  glutSwapBuffers();
}

void mouseWin2(int button, int state, int x, int y) {
  if (state == GLUT_DOWN){
	 X = x;
	 Y = H2 - y;
	 glutPostRedisplay();
	 cout<< x << "," << y << endl;
  }
}


int main(int argc, char* argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(200, 500);
  window1 = glutCreateWindow("Strumenti");
  glutReshapeFunc(reshapeWin1);
  glutDisplayFunc(displayWin1);
  glutMouseFunc(mouseWin1);
		 
  glutInitWindowPosition(200, 0);
  glutInitWindowSize(500, 500);
  window2 = glutCreateWindow("Foglio");
  glutReshapeFunc(reshapeWin2);
  glutDisplayFunc(displayWin2);
  glutMouseFunc(mouseWin2);

  glutMainLoop();  
}
