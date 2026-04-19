#include <map>
#include <string>
#include <vector>
#include <memory>

struct Location {
  double x;
  double y;
  double z;
};


enum class ShapeType {
  Circle,
  Square,
  Rectangle
};

bool IsStringAShape(std::string str) {
  for (auto& [shape, shapeString] : ShapeTypeToString) {
    if (shapeString == str) {return true;}
  }
  return false;
}

static inline const std::map<ShapeType, std::string> ShapeTypeToString({
	{ShapeType::Circle, "circle"},
	{ShapeType::Square, "square"},
	{ShapeType::Rectangle, "rectangle"},
});

class ShapeData {
public:
  virtual double test() {return -1;}; // TODO: remove if possible
};

class CircleData : public ShapeData {
public:
  CircleData(double _radius) 
    : ShapeData()
    , radius(_radius)
  {}
  
  const double radius;
};

class SquareData : public ShapeData {
public:
  SquareData(double _side_length) 
    : ShapeData()
    , side_length(_side_length)
  {}
  
  const double side_length;
};

class RectangleData : public ShapeData {
public:
  RectangleData(double _length, double _width) 
    : ShapeData()
    , length(_length)
    , width(_width)
  {}

  const double length;
  const double width;
};

class Shape {
public:
  Shape(
    ShapeType _type, 
    Location _location, 
    const std::shared_ptr<ShapeData>& _data
  ) 
    : type(_type)
    , location(_location)
    , data(_data)
  {}

  const ShapeType type;
  const Location location; 
  const std::shared_ptr<ShapeData> data;
};
