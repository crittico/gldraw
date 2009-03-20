/*
 * Name: glDraw
 * Author: Geremia Mirco
 * License: BSD
 */

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "figures.h"

using namespace std;

void setColor(GLfloat, GLfloat, GLfloat);
void drawQuad(int, int, int, int);
void drawBorder(int, int, int, int);
void drawSel();
void selection(int, int, int);
void loadRAWs();
void reshapeWin1(int, int);
void displayWin1();
void selWin1(GLuint*);
void mouseWin1(int, int, int, int);
void reshapeWin2(int, int);
void displayWin2();
void mouseWin2(int, int, int, int);
void mouseMotion(int, int);

int window1 = 0, window2 = 0;
int W1 = 100, H1 = 500; // width and heigth of the Strumenti window
int W2 = 500, H2 = 500; // width and heigth of the Foglio window

// define figure names (for textures too)
#define POINTER 0
#define LINE 1
#define TRIANGLE 2
#define QUAD 3
#define TEX_COUNT 4  // the number of textures
int figType = POINTER; // the figure type selected: 1 line, 2 quad, etc
GLuint textures[TEX_COUNT];
const char *texFiles[TEX_COUNT] = 
{"images/pointer.raw", "images/line.raw", "images/triangle.raw", "images/quad.raw"};

// define color names
#define WHITE 6
#define GREY 7
#define DGREY 8
#define BLACK 9
#define RED 10
#define DRED 11
#define PURPLE 12
#define DPURPLE 13
#define BLUE 14
#define DBLUE 15
#define CYAN 16
#define DCYAN 17
#define GREEN 18
#define DGREEN 19
#define YELLOW 20
#define DYELLOW 21
#define ORANGE 22
#define DORANGE 23

GLfloat *clr = new GLfloat[3]; // the current color
vector<Figure*> figureSet; // the vector that contains all the figures created
bool sel = false;
int selected = 0; // current selected figure
/* the numeration of the control points starts from the first number 
available to put a new 'int name' on the selection buffer stack */
int cpsel = false;
int cp = figureSet.size() + 1; // current selected control point

// set the selected color 
void setColor(GLfloat r, GLfloat g, GLfloat b) {
  clr[0] = r;
  clr[1] = g;
  clr[2] = b;
}

// draw a quad with the top-left and the bottom-right points
void drawQuad(int x, int y, int w, int z) {
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex2f(x, y);
  glTexCoord2f(1, 0);
  glVertex2f(w, y);
  glTexCoord2f(1, 1);
  glVertex2f(w, z);
  glTexCoord2f(0, 1);
  glVertex2f(x, z);
  glEnd();
}

// draw the borders of the selected button with the top-left and the bottom-right points
void drawBorder(int x, int y, int w, int z) {
  glColor3f(1, 0, 0.2);
  glBegin(GL_LINES);
  glVertex2f(x, y);
  glVertex2f(w, y);
  glVertex2f(w, y);
  glVertex2f(w, z);
  glVertex2f(w, z);
  glVertex2f(x, z);
  glVertex2f(x, z);
  glVertex2f(x, y);
  glEnd();
}

// draw controls point around a figure
void drawSel() {
  glColor3f(0, 0, 0);
  int size = figureSet.size();
  Figure *f = figureSet[selected];

  /* getPoint() is a virtual method: every figure has at least 
	  two points (is at least a line) */
  Point *p1 = f->getPoint(1);
  Point *p2 = f->getPoint(2);
  int *pt1 = p1->getCoords();
  int *pt2 = p2->getCoords();
  glLoadName(size+1);
  drawQuad(pt1[0]-3, pt1[1]-3, pt1[0]+3, pt1[1]+3);
  glLoadName(size+2);
  drawQuad(pt2[0]-3, pt2[1]-3, pt2[0]+3, pt2[1]+3);

  if (Triangle *t = dynamic_cast<Triangle*>(f)) { // triangle: one more point
	 Point *p3 = t->getPoint(3);
	 int *pt3 = p3->getCoords();
	 glLoadName(size+3);
	 drawQuad(pt3[0]-3, pt3[1]-3, pt3[0]+3, pt3[1]+3);
  }
  else if (Quad *q = dynamic_cast<Quad*>(f)) { // quad: two more point
	 Point *p3 = q->getPoint(3);
	 Point *p4 = q->getPoint(4);
	 int *pt3 = p3->getCoords();
	 int *pt4 = p4->getCoords();
	 glLoadName(size+3);
	 drawQuad(pt3[0]-3, pt3[1]-3, pt3[0]+3, pt3[1]+3);
	 glLoadName(size+4);
	 drawQuad(pt4[0]-3, pt4[1]-3, pt4[0]+3, pt4[1]+3);
  }
}


/* Used to obtain the figure selected in the first
or second window, depending on 'win' parameter */
#define BUFFER_LENGTH 64
void selection(int x, int y, int win) {
	 // Space for selection buffer
	 static GLuint selectBuff[BUFFER_LENGTH];
	 GLint hits, viewport[4];
	 
	 // Setup selection buffer and get the view port
	 glSelectBuffer(BUFFER_LENGTH, selectBuff);
	 glGetIntegerv(GL_VIEWPORT, viewport);
	 
	 // Switch to projection and save the matrix
	 glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 
	 // Change render mode
	 glRenderMode(GL_SELECT);
	 glLoadIdentity();
	 // clipping volume
	 gluPickMatrix(x, viewport[3] - y + viewport[1], 2,2, viewport);
	 

	 if (win == 1)
		glOrtho(0, W1, 0, H1, 1, -1);
	 else if (win == 2)
		glOrtho(0, W2, 0, H2, 1, -1);
	 glMatrixMode(GL_MODELVIEW);
	 
	 // Draw the scene
	 if (win == 1)
		displayWin1();
	 else if (win == 2)
		displayWin2();
	 
	 // Collect the hits
	 hits = glRenderMode(GL_RENDER);
	 
	 // Restore the projection matrix
	 glMatrixMode(GL_PROJECTION);
	 glPopMatrix();
	 
	 // Go back to modelview for normal rendering
	 glMatrixMode(GL_MODELVIEW);
	 
	 if(hits > 0){ //something's been selected
		unsigned int choiche;

		// fi there's more than one figure (or control point) selected, take the top one
		if (hits > 1)
		  choiche = selectBuff[(hits*4)-1]; 
		else
		  choiche = selectBuff[3];

		cpsel = false;
		if (win == 1) {
		  selWin1(selectBuff);
		}
		else if (win == 2){
			 sel = true;
			 if (choiche < figureSet.size()) // figure selected
				selected = choiche;
			 else{ // it's a control point
				cpsel = true;
				cp = choiche;
			 }
		}
	 }
	 else // a click with no hits -> deselection
		sel = false;
	 
	 glutPostRedisplay();
}

GLubyte *texture[256 * 256 * 3];
void loadRAWs(){
  // generate all the textures
  glGenTextures(TEX_COUNT, textures);
  for (int i = 0; i < TEX_COUNT; i++) {
	 FILE *fHan = fopen(texFiles[i], "rb");
	 if(fHan == NULL) exit(1);
	 fread(texture, 256 * 256, 3, fHan);
	 fclose(fHan);
	 
	 glBindTexture(GL_TEXTURE_2D, textures[i]);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	 
	 gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, texture);
  }
}

/*
 * Strumenti window and relative functions
 */
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
  int hb = H1/10; // height of the 'buttons'
  glClearColor(1, 1, 1, 1); // white background
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  loadRAWs(); // load all the textures

  // initialize the stack for the selection
  glInitNames();
  glPushName(0);

  glColor3f(1, 1, 1);
   
  // apply a texture to explain each figures button
  // pointer
  glLoadName(POINTER);
  glBindTexture(GL_TEXTURE_2D, textures[POINTER]);
  drawQuad(0, H1, W1/2.0, H1-hb);

  // line
  glLoadName(LINE);
  glBindTexture(GL_TEXTURE_2D, textures[LINE]);
  drawQuad(W1/2.0, H1, W1, H1-hb);

  glPushMatrix();
  glTranslatef(0, -hb, 0); // translated to maintain H1-hb

  // triangle
  glLoadName(TRIANGLE);
  glBindTexture(GL_TEXTURE_2D, textures[TRIANGLE]);
  drawQuad(0, H1, W1/2.0, H1-hb);

  // quad
  glLoadName(QUAD);
  glBindTexture(GL_TEXTURE_2D, textures[QUAD]);
  drawQuad(W1/2, H1, W1, H1-hb);
  
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -3*(hb), 0);
  
  glDeleteTextures(TEX_COUNT, textures);

  // selected color
  glColor3f(clr[0], clr[1], clr[2]);
  drawQuad(0, H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -4*(hb), 0);
  
  // colors button
  // first line of colors
  glColor3f(1, 1, 1);
  glLoadName(WHITE);
  drawQuad(0, H1, W1/3.0, H1-hb);
  glColor3f(0.7, 0.7, 0.7);
  glLoadName(GREY);
  drawQuad(W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  glColor3f(0.3, 0.3, 0.3);
  glLoadName(DGREY);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -5*(hb), 0);

  // second line of colors
  glColor3f(0, 0, 0);
  glLoadName(BLACK);
  drawQuad(0, H1, W1/3.0, H1-hb);
  glColor3f(1, 0, 0);
  glLoadName(RED);
  drawQuad(W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  glColor3f(0.5, 0, 0);
  glLoadName(DRED);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -6*(hb), 0);

  // third line of colors
  glColor3f(1, 0, 1);
  glLoadName(PURPLE);
  drawQuad(0, H1, W1/3.0, H1-hb);
  glColor3f(0.5, 0, 0.5);
  glLoadName(DPURPLE);
  drawQuad(W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  glColor3f(0, 0, 1);
  glLoadName(BLUE);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -7*(hb), 0);

  // fourth line of colors
  glColor3f(0, 0, 0.5);
  glLoadName(DBLUE);
  drawQuad(0, H1, W1/3.0, H1-hb);
  glColor3f(0, 1, 1);
  glLoadName(CYAN);
  drawQuad(W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  glColor3f(0, 0.5, 0.5);
  glLoadName(DCYAN);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -8*(hb), 0);

  // fifth line of colors
  glColor3f(0, 1, 0);
  glLoadName(GREEN);
  drawQuad(0, H1, W1/3.0, H1-hb);
  glColor3f(0, 0.5, 0);
  glLoadName(DGREEN);
  drawQuad(W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  glColor3f(1, 1, 0);
  glLoadName(YELLOW);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);

  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -9*(hb), 0);

  // sixth line of colors
  glColor3f(0.5, 0.5, 0);
  glLoadName(DYELLOW);
  drawQuad(0, H1, W1/3.0, H1-hb);
  glColor3f(1, 0.5, 0);
  glLoadName(ORANGE);
  drawQuad(W1/3.0, H1, W1*(2.0/3.0), H1-hb);
  glColor3f(0.5, 0.25, 0);
  glLoadName(DORANGE);
  drawQuad(W1*(2.0/3.0), H1, W1, H1-hb);

  glPopMatrix();
  
  // borders of the buttons
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
  glVertex2f(W1/2, H1);
  glVertex2f(W1/2, H1-(3*hb));
  glVertex2f(W1/3, H1-(4*hb));
  glVertex2f(W1/3, 0);
  glVertex2f(W1*(2.0/3.0), H1-(4*hb));
  glVertex2f(W1*(2.0/3.0), 0);
  for (int i = hb; i < H1; i += hb){
	 glVertex2f(0, H1-i);
	 glVertex2f(W1, H1-i);
  }
  glEnd();

  // borders of the selected figure
  switch(figType){
  case POINTER:
	 drawBorder(0, H1, W1/2.0, H1-hb);
	 break;
  case LINE:
	 drawBorder(W1/2.0, H1, W1, H1-hb);
	 break;
  case TRIANGLE:
	 drawBorder(0, H1-hb, W1/2.0, H1-2*hb);
	 break;
  case QUAD:
	 drawBorder(W1/2, H1-hb, W1, H1-2*hb);
	 break;
  }
  
  glutSwapBuffers();
}

// process selection in 'Strumenti' window
void selWin1(GLuint *pselectBuff){
  int id = pselectBuff[3]; 

  switch(id){
	 // if a figure instrument is selected, than deselect the figure in 'Foglio' window 
  case POINTER:
	 figType = POINTER;
	 break;
  case LINE:
	 figType = LINE;
	 sel = false;
	 break;
  case QUAD:
	 figType = QUAD;
	 sel = false;
	 break;
  case TRIANGLE:
	 figType = TRIANGLE;
	 sel = false;
	 break;
	 // colors: if there's a figure selected, change its color
  case WHITE:
	 setColor(1, 1, 1);
	 if (sel)
		figureSet[selected]->setColor(1, 1, 1);
	 break;
  case GREY:
	 setColor(0.7, 0.7, 0.7);
	 if (sel)
		figureSet[selected]->setColor(0.7, 0.7, 0.7);
	 break;
  case DGREY:
	 setColor(0.3, 0.3, 0.3);
	 if (sel)
		figureSet[selected]->setColor(0.3, 0.3, 0.3);
	 break;
  case BLACK:
	 setColor(0, 0, 0);
	 if (sel)
		figureSet[selected]->setColor(0, 0, 0);
	 break;
  case RED:
	 setColor(1, 0, 0);
	 if (sel)
		figureSet[selected]->setColor(1, 0, 0);
	 break;
  case DRED:
	 setColor(0.5, 0, 0);
	 if (sel)
		figureSet[selected]->setColor(0.5, 0, 0);
	 break;
  case PURPLE:
	 setColor(1, 0, 1);
	 if (sel)
		figureSet[selected]->setColor(1, 0, 1);
	 break;
  case DPURPLE:
	 setColor(0.5, 0, 0.5);
	 if (sel)
		figureSet[selected]->setColor(0.5, 0, 0.5);
	 break;
  case BLUE:
	 setColor(0, 0, 1);
	 if (sel)
		figureSet[selected]->setColor(0, 0, 1);
	 break;
  case DBLUE:
	 setColor(0, 0, 0.5);
	 if (sel)
		figureSet[selected]->setColor(0, 0, 0.5);
	 break;
  case CYAN:
	 setColor(0, 1, 1);
	 if (sel)
		figureSet[selected]->setColor(0, 1, 1);
	 break;
  case DCYAN:
	 setColor(0, 0.5, 0.5);
	 if (sel)
		figureSet[selected]->setColor(0, 0.5, 0.5);
	 break;
  case GREEN:
	 setColor(0, 1, 0);
	 if (sel)
		figureSet[selected]->setColor(0, 1, 0);
	 break;
  case DGREEN:
	 setColor(0, 0.5, 0);
	 if (sel)
		figureSet[selected]->setColor(0, 0.5, 0);
	 break;
  case YELLOW:
	 setColor(1, 1, 0);
	 if (sel)
		figureSet[selected]->setColor(1, 1, 0);
	 break;
  case DYELLOW:
	 setColor(0.5, 0.5, 0);
	 if (sel)
		figureSet[selected]->setColor(0.5, 0.5, 0);
	 break;
  case ORANGE:
	 setColor(1, 0.5, 0);
	 if (sel)
		figureSet[selected]->setColor(1, 0.5, 0);
	 break;
  case DORANGE:
	 setColor(0.5, 0.25, 0);
	 if (sel)
		figureSet[selected]->setColor(0.5, 0.25, 0);
	 break;
  }
  // redisplay 'Foglio' window to change figures color or deselect figures
  glutPostWindowRedisplay(window2);
}

void mouseWin1(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){
	 selection(x, y, 1);
  }
}


/* 
 * Foglio window and relative functions
 */
void reshapeWin2(int neww, int newh) {
  W2 = neww;
  H2 = newh;
  glViewport(0, 0, W2, H2);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, W2, 0, H2, 1, -1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void displayWin2() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glInitNames();
  glPushName(0);

  // iterates through the figures collection 
  for (int i = 0; i < (int)figureSet.size(); i++){
	 figureSet[i]->draw(i); // call virtual method draw()
  }
  if (sel){ // if a figure is selected draw the control points
	 drawSel();
  }

  glutSwapBuffers();
}


void mouseWin2(int button, int state, int x, int y) {
  if ((button == GLUT_LEFT) && (state == GLUT_DOWN)){
	 if (figType == POINTER) // a figure's been selected 
		selection(x, y, 2);
	 else { // create a figure
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
		// add the figure to the collection 
		figureSet.push_back(f);
		sel = true;
		selected = figureSet.size()-1; // select the figure created
	 }
  }

  if (state == GLUT_UP) {
	 cpsel = false;
  }

  glutPostRedisplay();
}

void mouseMotion(int x, int y) {
  Point *p = new Point(x, H2-y);

  if (cpsel){ // the user is trying to move a control point
	 Figure *f = figureSet[selected];
	 if (Line *l = dynamic_cast<Line*>(f)){
		l->setPoint(cp - figureSet.size(), p);
	 }
	 if (Triangle *t = dynamic_cast<Triangle*>(f)){
		t->setPoint(cp - figureSet.size(), p);
	 }
	 if (Quad *q = dynamic_cast<Quad*>(f)){
		q->setPoint(cp - figureSet.size(), p);
	 }
  }
 
  // show the figure while creating
  switch(figType) {
  case LINE: {
	 Line *l = (Line*) figureSet.back();
	 l->setPoint(2, p);
	 break;
  }
  case TRIANGLE: {
	 Triangle *t = (Triangle*) figureSet.back();
	 t->setTriangle(p);
	 break;
  }
  case QUAD: {
	 Quad *q = (Quad*) figureSet.back();
	 q->setQuad(p);
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

  clr[0] = clr[1] = clr[2] = 0; //initial color to black

  glutMainLoop();  
}
