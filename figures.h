#ifndef FIGURES_H
#define FIGURES_H

#include <GL/glut.h>

using namespace std;

class Point { 
 public:
  Point(int, int);
  int* getCoords();
 private:
  int x;
  int y;
};


class Figure {  //abstract class
 public:
  Figure() {};
  virtual ~Figure() {};
  void setColor(GLfloat, GLfloat, GLfloat);
  GLfloat *getColor();
  virtual void draw(int) = 0;
  virtual Point* getPoint(int) = 0;
  virtual void setPoint(int, Point*) = 0;
 private:
  GLfloat color[3];
};


class Line : public Figure {
 public:
  Line(Point*, Point*, GLfloat, GLfloat, GLfloat);
  virtual ~Line() {};
  virtual void setPoint(int, Point*);
  virtual void draw(int);
  virtual Point* getPoint(int);
 private:
  Point *pt1, *pt2;
};

class Triangle : public Figure {
 public:
  Triangle(Point*, Point*, GLfloat, GLfloat, GLfloat);
  virtual ~Triangle() {};
  void setTriangle(Point*);
  virtual void setPoint(int, Point*);
  virtual void draw(int);
  virtual Point* getPoint(int);
 private:
  Point *pt1, *pt2, *pt3;
};

class Quad : public Figure {
 public:
  Quad(Point*, Point*, GLfloat, GLfloat, GLfloat);
  virtual ~Quad() {};
  void setQuad(Point*);
  virtual void setPoint(int, Point*);
  virtual void draw(int);
  virtual Point* getPoint(int);
 private:
  Point *pt1, *pt2, *pt3, *pt4;
};

#endif
