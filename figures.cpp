#include <iostream>
#include <GL/glut.h>
#include "figures.h"

#define LINE 1
#define TRIANGLE 2
#define QUAD 3

using namespace std;

// Point methods
Point::Point(int a = 0, int b = 0) : x(a), y(b) {}

int* Point::getCoords() {
  int *pts = new int[2];
  pts[0] = x;
  pts[1] = y;
  return pts;
}


// Figure methods
void Figure::setColor(GLfloat r, GLfloat g, GLfloat b) {
  color[0] = r;
  color[1] = g;
  color[2] = b;
}

GLfloat* Figure::getColor() {
  return color;
}


// Line methods
Line::Line(Point *p1, Point *p2, GLfloat r, GLfloat g, GLfloat b) {
  pt1 = p1;
  pt2 = p2;
  setColor(r, g, b);
}

void Line::setPoint(int n, Point *p) {
  switch (n) {
  case 1:
	 delete pt1;
	 pt1 = p;
	 break;
  case 2:
	 delete pt2;
	 pt2 = p;
	 break;
  }
}

void Line::draw(int n) {
  GLfloat *c = getColor();
  glColor3f(c[0], c[1], c[2]);
  int *pts1 = pt1->getCoords();
  int *pts2 = pt2->getCoords();
  //  cout << c[0] << c[1] << c[2] << endl;
  glLoadName(n);
  glBegin(GL_LINES);
  glVertex2f(pts1[0], pts1[1]);
  glVertex2f(pts2[0], pts2[1]);
  glEnd();
}

Point* Line::getPoint(int n) {
  if (n == 1)
	 return pt1;
  return pt2;
}

// Triangle methods
Triangle::Triangle(Point *p1, Point *p2, GLfloat r, GLfloat g, GLfloat b) {
  pt1 = p1;
  pt2 = p2;
  int *pts1 = pt1->getCoords();
  int *pts2 = pt2->getCoords();
  Point *p3 = new Point((pts1[0]+pts2[0])/2, pts1[1]+(pts2[0]-pts1[0]));
  pt3 = p3;
  setColor(r, g, b);
}

void Triangle::setPoint(int n, Point *p) {
  switch (n) {
  case 1:
	 delete pt1;
	 pt1 = p;
	 break;
  case 2:
	 delete pt2;
	 pt2 = p;
	 break;
  case 3:
	 delete pt3;
	 pt3 = p;
	 break;
  }
}
	 
void Triangle::setTriangle(Point *p) {
  delete pt2;
  pt2 = p;
  int *pts1 = pt1->getCoords();
  int *pts2 = pt2->getCoords();
  Point *p3 = new Point(pts1[0], pts2[1] + (pts2[1]-pts1[1]));
  //  Point *p3 = new Point((pts1[0]+pts2[0])/2, pts1[1]+(pts2[0]-pts1[0]));
  setPoint(3, p3);
}

void Triangle::draw(int n) {
  GLfloat *c = getColor();
  glColor3f(c[0], c[1], c[2]);
  int *pts1 = pt1->getCoords();
  int *pts2 = pt2->getCoords();
  int *pts3 = pt3->getCoords();
  glLoadName(n);
  glBegin(GL_TRIANGLES);
  glVertex2f(pts1[0], pts1[1]);
  glVertex2f(pts2[0], pts2[1]);
  glVertex2f(pts3[0], pts3[1]);
  glEnd();
}

Point* Triangle::getPoint(int n) {
  if (n == 1)
	 return pt1;
  if (n == 2)
	 return pt2;
  return pt3;
}

// Quad methods
Quad::Quad(Point *p1, Point *p3, GLfloat r, GLfloat g, GLfloat b) {
  pt1 = p1;
  pt3 = p3;
  int *pts1 = pt1->getCoords();
  int *pts3 = pt3->getCoords();
  Point *p2 = new Point(pts3[0], pts1[1]);
  Point *p4 = new Point(pts1[0], pts3[1]);
  pt2 = p2;
  pt4 = p4;
  setColor(r, g, b);
}

void Quad::setPoint(int n, Point *p) {
  switch (n) {
  case 1:
	 delete pt1;
	 pt1 = p;
	 break;
  case 2:
	 delete pt2;
	 pt2 = p;
	 break;
  case 3:
	 delete pt3;
	 pt3 = p;
	 break;
  case 4:
	 delete pt4;
	 pt4 = p;
	 break;
  }
}

void Quad::setQuad(Point *p) {
  delete pt3;
  pt3 = p;
  int *pts1 = pt1->getCoords();
  int *pts3 = pt3->getCoords();
  Point *p2 = new Point(pts3[0], pts1[1]);
  Point *p4 = new Point(pts1[0], pts3[1]);
  setPoint(2, p2);
  setPoint(4, p4);
}


void Quad::draw(int n) {
  GLfloat *c = getColor();
  glColor3f(c[0], c[1], c[2]);
  int *pts1 = pt1->getCoords();
  int *pts2 = pt2->getCoords();
  int *pts3 = pt3->getCoords();
  int *pts4 = pt4->getCoords();
  glLoadName(n);
  glBegin(GL_QUADS);
  glVertex2f(pts1[0], pts1[1]);
  glVertex2f(pts2[0], pts2[1]);
  glVertex2f(pts3[0], pts3[1]);
  glVertex2f(pts4[0], pts4[1]);
  glEnd();
}

Point* Quad::getPoint(int n) {
  if (n == 1)
	 return pt1;
  if (n == 2)
	 return pt2;
  if (n == 3)
	 return pt3;
  return pt4;
}
