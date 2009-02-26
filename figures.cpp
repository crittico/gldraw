#include <iostream>
#include <GL/glut.h>
#include "figures.h"

using namespace std;

// Point methods
Point::Point(int a = 0, int b = 0) : x(a), y(b) {}

int* Point::getPoints() {
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

void Line::setFirst(Point *p) {
  delete pt1;
  pt1 = p;
}

void Line::setSecond(Point *p) {
  delete pt2;
  pt2 = p;
}

void Line::draw() {
  GLfloat *c = getColor();
  glColor3f(c[0], c[1], c[2]);
  int *pts1 = pt1->getPoints();
  int *pts2 = pt2->getPoints();
  //  cout << c[0] << c[1] << c[2] << endl;
  glBegin(GL_LINES);
  glVertex2f(pts1[0], pts1[1]);
  glVertex2f(pts2[0], pts2[1]);
  glEnd();
}


// Quad methods
Quad::Quad(Point *p1, Point *p3, GLfloat r, GLfloat g, GLfloat b) {
  pt1 = p1;
  pt3 = p3;
  int *pts1 = pt1->getPoints();
  int *pts3 = pt3->getPoints();
  Point *p2 = new Point(pts3[0], pts1[1]);
  Point *p4 = new Point(pts1[0], pts3[1]);
  pt2 = p2;
  pt4 = p4;
  setColor(r, g, b);
}

void Quad::setFirst(Point *p) {
  delete pt1;
  pt1 = p;
}

void Quad::setSecond(Point *p) {
  delete pt2;
  pt2 = p;
}

void Quad::setThird(Point *p) {
  delete pt3;
  pt3 = p;
  int *pts1 = pt1->getPoints();
  int *pts3 = pt3->getPoints();
  Point *p2 = new Point(pts3[0], pts1[1]);
  Point *p4 = new Point(pts1[0], pts3[1]);
  setSecond(p2);
  setFourth(p4);
}

void Quad::setFourth(Point *p) {
  delete pt4;
  pt4 = p;
}

void Quad::draw() {
  GLfloat *c = getColor();
  glColor3f(c[0], c[1], c[2]);
  int *pts1 = pt1->getPoints();
  int *pts2 = pt2->getPoints();
  int *pts3 = pt3->getPoints();
  int *pts4 = pt4->getPoints();
  //  cout << c[0] << c[1] << c[2] << endl;
  glBegin(GL_QUADS);
  glVertex2f(pts1[0], pts1[1]);
  glVertex2f(pts2[0], pts2[1]);
  glVertex2f(pts3[0], pts3[1]);
  glVertex2f(pts4[0], pts4[1]);
  glEnd();
}
