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
 private:
  GLfloat color[3];
};


class Line : public Figure {
 public:
  Line(Point*, Point*, GLfloat, GLfloat, GLfloat);
  virtual ~Line() {};
  void setFirst(Point*);
  void setSecond(Point*);
  virtual void draw(int);
  Point* getPoint1();
  Point* getPoint2();
 private:
  Point *pt1, *pt2;
};

class Triangle : public Figure {
 public:
  Triangle(Point*, Point*, GLfloat, GLfloat, GLfloat);
  virtual ~Triangle() {};
  void setFirst(Point*);
  void setSecond(Point*);
  void setThird(Point*);
  void draw(int);
  Point* getPoint1();
  Point* getPoint2();
  Point* getPoint3();
 private:
  Point *pt1, *pt2, *pt3;
};

class Quad : public Figure {
 public:
  Quad(Point*, Point*, GLfloat, GLfloat, GLfloat);
  virtual ~Quad() {};
  void setFirst(Point*);
  void setSecond(Point*);
  void setThird(Point*);
  void setFourth(Point*);
  virtual void draw(int);
  Point* getPoint1();
  Point* getPoint2();
  Point* getPoint3();
  Point* getPoint4();
 private:
  Point *pt1, *pt2, *pt3, *pt4;
};

#endif
