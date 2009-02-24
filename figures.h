using namespace std;

class Figure {
public:
  virtual void draw();
};

class Line : public Figure {
public:
  Line();
  void setFirst(int, int);
  void setSecond(int, int);
  virtual void draw();
private:
  int x1, y1, x2, y2;
}
