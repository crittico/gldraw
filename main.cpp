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

int window1 = 0, window2 = 0;
int X = 0, Y = 0;
int W1 = 200, H1 = 500; // width and length of the Strumenti window
int W2 = 500, H2 = 500; // width and length of the Foglio window
GLfloat *clr = new GLfloat[3]; // the current color
int figType = 0; // the figure type selected: 0 line; 1 quad

// the vector that contains all the figures created
vector<Figure*> figureSet;


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
  gluOrtho2D(0, (GLdouble)W1, (GLdouble)H1, 0);
  glMatrixMode(GL_MODELVIEW);
}

void displayWin1() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glBegin(GL_QUADS);
  //line
  glColor3f(1, 1, 1);
  glVertex2f(0, 0);
  glVertex2f(0, 20);
  glVertex2f(W1/2, 20);
  glVertex2f(W1/2, 0);
  // quad
  glColor3f(0, 0, 0);
  glVertex2f(W1/2, 0);
  glVertex2f(W1/2, 20);
  glVertex2f(W1, 20);
  glVertex2f(W1, 0);
  // red
  glColor3f(1, 0, 0);
  glVertex2f(0, 20);
  glVertex2f(0, H1/2);
  glVertex2f(W1, H1/2);
  glVertex2f(W1, 20);
  // green
  glColor3f(0, 1, 0);
  glVertex2f(0, H1/2);
  glVertex2f(0, H1);
  glVertex2f(W1, H1);
  glVertex2f(W1, H1/2);
  glEnd();

  glutSwapBuffers();
}

void mouseWin1(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){
	 if (y < 20){
		if (x < W1/2)
		  figType = 0;
		else
		  figType = 1;
	 }
	 else if ((y < H1/2) && (y > 20)) 
		setColor(1, 0, 0);
	 else if (y > H1/2)
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
		 
  glutInitWindowPosition(200, 0);
  glutInitWindowSize(W2, H2);
  window2 = glutCreateWindow("Foglio");
  glutReshapeFunc(reshapeWin2);
  glutDisplayFunc(displayWin2);
  glutMouseFunc(mouseWin2);
  glutMotionFunc(mouseMotion);

  glutMainLoop();  
}
