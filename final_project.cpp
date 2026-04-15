// #include "FourierTransformCalculator.h"
// #include "SphericalHarmonicGenerator.h"

#define _USE_MATH_DEFINES

// TODO: alphabetize (maybe)
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <utility>
#include <random>
#include <cmath>
#include <filesystem>
#include <fstream>

const inline double radius = 3;

// TODO: move all shape data to new file
enum class ShapeType {
  Circle,
  Square,
  Rectangle
};

static inline const std::map<ShapeType, std::string> ShapeTypeToString({
	{ShapeType::Circle, "circle"},
	{ShapeType::Square, "square"},
	{ShapeType::Rectangle, "rectangle"},
});

class ShapeData {
public:
  virtual double CalculateDistanceFromCenterToFarthestPoint(double r) = 0;
};

class CircleData : public ShapeData {
public:
  CircleData(double _radius) 
    : ShapeData()
    , radius(_radius)
  {}

  double CalculateDistanceFromCenterToFarthestPoint(double r) override {throw std::runtime_error("NOT IMPLEMENTED");}
  
  const double radius;
};

class SquareData : public ShapeData {
public:
  SquareData(double _side_length) 
    : ShapeData()
    , side_length(_side_length)
  {}
  
  double CalculateDistanceFromCenterToFarthestPoint(
    std::vector<double>x_vec,
    std::vector<double>x_vec,
    double r
  ) override {
    for (auto& x : std::vector<double>(x_vec))
    // return sqrt(pow(side_length, 2) / 2); 
  }
  
  const double side_length;
};

class RectangleData : public ShapeData {
public:
  RectangleData(double _length, double _width) 
    : ShapeData()
    , length(_length)
    , width(_width)
  {}
  
  double CalculateDistanceFromCenterToFarthestPoint(double r) override {throw std::runtime_error("NOT IMPLEMENTED");}

  const double length;
  const double width;
};

struct Location {
  double x;
  double y;
  double z;
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
  const Location location; // first element is the theta location, the second location is the phi location
  const std::shared_ptr<ShapeData> data;
};


// END SHAPEDATA

struct RuntimeArguments {
  bool runFourier;
  std::string outputPath;
  std::vector<Shape> shapes;
};

bool IsStringAShape(std::string str) {
  for (auto& [shape, shapeString] : ShapeTypeToString) {
    if (shapeString == str) {return true;}
  }
  return false;
}

struct SphereData {
  std::vector<double> theta;
  std::vector<double> phi;
  std::map<std::pair<double, double>, double> values;
};

Location DetermineShapeLocation(int argc, char *argv[], int& counter, int& seed) {
    double x, y, z;
    if (counter == argc || IsStringAShape(argv[counter])) {
      // if the next line is a shape, that means a location wasn't specified for this shape. Randomly generate a location 
      std::mt19937 gen(seed);
      std::uniform_real_distribution<double> distribution(-2*radius, 2*radius); // TODO: must be greater than radius of sphere, but figure out waht the right thing to do here is


      x = distribution(gen);
      y = distribution(gen);
      z = distribution(gen);
      seed++;

      auto distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z,2));

      if (distance < radius) {
        while (x < radius && y < radius && z < radius) {
          x = distribution(gen);
          y = distribution(gen);
          z = distribution(gen);
          seed++;
        }
      }
    } else {
      // if the next command line argument isn't a shape, 
      //  and there is a next command line argument to read,
      //  assume the next 3 command line arguments are the shape's location in x/y/z coordinates
      x = std::stod(argv[counter]);
      y = std::stod(argv[counter+1]);
      z = std::stod(argv[counter+2]);
      // Shift the counter up 2, so that the next time we attempt to read a shape 
      //  we account for the 2 extra variables that were passed in that represented the location
      counter += 3;

      auto distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z,2));
      if (distance < radius) {
        throw std::runtime_error("Given location of shape is inside the sphere. The center of the shape must be greater than the radius of the sphere\n");
        std::cerr << "Given x: " << x << std::endl;
        std::cerr << "Given y: " << y << std::endl;
        std::cerr << "Given z: " << z << std::endl;
        std::cerr << "Radius of Circle: " << radius << std::endl;
      }

    }

    return Location{x, y, z};
}

// IMPORTANT NOTE: in addition to gathering data from command line arguments, 
//  this function also randomly generates a location for each given shape, if a location is not provided
//  TODO (maybe): use std::optional instead, and draw when iterating over shapes in the sphere generation function
RuntimeArguments ParseRuntimeArguments(int argc, char *argv[]) {
  if (argc == 1) {
    throw std::runtime_error("Additional runtime argument needed: Should we perform a fourier transform on the spherical data? (Input True or False)");
  }

  bool do_fourier;
  if (std::string(argv[1]) == "True") {
    do_fourier = true;
  } else if (std::string(argv[1]) == "False") {
    do_fourier = false;
  } else {
    throw std::runtime_error("Did not receive True or False in the first argument! (whether we should perform a fourier transform on the spherical data)");
  }

  if (argc == 2) {
    throw std::runtime_error("Additional runtime argument needed: folder where outputs will be stored!");
  }
  std::string output_filepath = std::string(argv[2]);

  if (argc == 3) {
    throw std::runtime_error("Additional runtime argument needed: Give a seed!");
  }
  int seed = std::stoi(std::string(argv[3]));

  std::vector<Shape> shapes = std::vector<Shape>();
  if (argc <= 4) {
    throw std::runtime_error("No shapes were given. There is nothing to calculate or plot!");
  } else {
    int counter = 4;
    while (counter < argc) {
      std::string new_shape = argv[counter];
      if (new_shape == "circle") {
        auto circleData = std::make_shared<CircleData>(std::stod(argv[counter+1]));
        counter += 2;
        auto shapeLocation = DetermineShapeLocation(argc, argv, counter, seed);
        shapes.emplace_back(Shape(ShapeType::Circle, shapeLocation, std::dynamic_pointer_cast<ShapeData>(circleData)));
      }
      else if (new_shape == "square") {
        auto squareData = std::make_shared<SquareData>(std::stod(argv[counter+1]));
        counter += 2;
        auto shapeLocation = DetermineShapeLocation(argc, argv, counter, seed);
        shapes.emplace_back(Shape(ShapeType::Square, shapeLocation, std::dynamic_pointer_cast<ShapeData>(squareData)));
      }
      else if (new_shape == "rectangle") {
        auto rectangleData = std::make_shared<RectangleData>(std::stod(argv[counter+1]), std::stod(argv[counter+2]));
        counter += 3;
        auto shapeLocation = DetermineShapeLocation(argc, argv, counter, seed);
        shapes.emplace_back(Shape(ShapeType::Rectangle, shapeLocation, std::dynamic_pointer_cast<ShapeData>(rectangleData)));
      } 
      else {
        throw std::runtime_error("Unrecognized Shape: " + new_shape);
      }
    }
  }
  return RuntimeArguments{do_fourier, output_filepath, shapes};
}

std::map<std::pair<double, double>, double> ProjectShapeOntoSphereAndCalculateDistanceFromCenterOfShape(
  double r,
  const std::vector<double>& theta_sphere,
  const std::vector<double>& phi_sphere,
  const std::vector<double>& x_shape,
  const std::vector<double>& y_shape,
  const std::vector<double>& z_shape, // TODO: would like to not iterate over all 3 vectors - only 2 is necessary due to planar assumption that the code makes prior
  const Location& shape_center,
  const std::shared_ptr<ShapeData> shape_data,
  double theta_step,
  double phi_step
) {
  // TODO: handle case where we cross theta/phi = 0!

  // TODO: normalize should be 'normalizing' to r value!!!! - do for center and reg shape!!

  // double min_x, min_y, min_z = std::numeric_limits<double>::max();
  // double max_x, max_y, max_z = -1*std::numeric_limits<double>::max();

  double mag_center = sqrt(pow(shape_center.x, 2) + pow(shape_center.y, 2) + pow(shape_center.z, 2));
  double center_x_normalized = shape_center.x/mag_center * r;
  double center_y_normalized = shape_center.y/mag_center * r;
  double center_z_normalized = shape_center.z/mag_center * r;

  double farthestLocationFromCenterPoint = shape_data->CalculateDistanceFromCenterToFarthestPoint(r)

  // TODO: inefficient as all fuck, but it works for all shape types. revisit if time
  std::map<std::pair<double, double>, double> values_beforepruning; // TODO: better name
  for (double x : x_shape) {
    for (double y : y_shape) {
      for (double z : z_shape) {

        // TODO: say it's smoothing
        double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

        double x_norm = x/magnitude * r;
        double y_norm = y/magnitude * r;
        double z_norm = z/magnitude * r;

        // TODO: have to fix issue where a shape that's farther away has higher values across the board. 
        //  Due to farthestDistanceFromCenterPoint. 
        //  I guess we just have to find x_norm first, then loop over all shapes again to calculate value for real

        double distanceFromPointToCenter = sqrt(pow(x_norm - center_x_normalized, 2) + pow(y_norm - center_y_normalized, 2) + pow(z_norm - center_z_normalized, 2));
        double farthestDistanceNormalized = farthestDistanceFromCenterToPoint;
        double value = 1-distanceFromPointToCenter/farthestDistanceNormalized;  // TODO: better name

        double theta = atan2(y_norm, x_norm);
        // We choose to use 0 <= theta <= 2pi, while atan2 uses -pi < theta < pi. 
        //  If the returned theta is negative, shift it up 2pi to be within the range of thetas that we want
        if (theta < 0) {
          theta += 2*M_PI;
        }
        double phi = acos(z_norm/r);

        values_beforepruning.insert({std::make_pair(theta, phi), value});
      }
    }
  }

  auto values_afterpruning = std::map<std::pair<double, double>, double>();

  for (int theta_index = 0; theta_index < theta_sphere.size(); theta_index++) {
    double theta = theta_sphere[theta_index];

    for (int phi_index = 0; phi_index < phi_sphere.size(); phi_index++) {
      double phi = phi_sphere[phi_index];
      double closest_distance = 100;
      double value_to_use = 0;

      for (const auto& [key, value] : values_beforepruning) {
        double this_theta = key.first;
        double this_phi = key.second;
        if (fabs(this_theta-theta) + fabs(this_phi-phi) < closest_distance
          && fabs(this_theta-theta) < theta_step
          && fabs(this_phi - phi) < phi_step
        ) {
          closest_distance = fabs(this_theta-theta) + fabs(this_phi-phi);
          value_to_use = value;
        }
      }

      values_afterpruning.insert({std::make_pair(theta, phi), value_to_use});
    }
  }

  return values_afterpruning;
}


SphereData CreateSphereFromShapes(const std::vector<Shape>& shapes) {
  // TODO: what to do about this r value? input or no? same for num steps in sphere?
  // TODO: define theta_lower and theta_upper  and same for phi
  double r = radius;
  int num_theta_steps = 40; // TODO: There will be 1+ this many steps, to have the last point equal the first point 0, 2pi). this is for plotting purposes, matplotlib needs it
  int num_phi_steps = 40;
  double theta_step = 2*M_PI/double(num_theta_steps);
  double phi_step = M_PI/double(num_phi_steps);

  // These are the theta and phi values that the sphere will have. 
  auto theta_sphere = std::vector<double>();
  auto phi_sphere = std::vector<double>();

  // Map that links theta/phi locations to a value TODO: make
  auto values_map = std::map<std::pair<double, double>, double>();

  for (double theta_inc = 0 ; theta_inc < 2*M_PI; theta_inc += theta_step) {
    theta_sphere.emplace_back(theta_inc);
  }
  theta_sphere.emplace_back(2*M_PI);
  for (double phi_inc = 0; phi_inc < M_PI; phi_inc += phi_step) {
    phi_sphere.emplace_back(phi_inc);
  }
  phi_sphere.emplace_back(M_PI);

  // Place shapes onto sphere
  for (const auto& shape : shapes) {
    // TODO: circle and rectangle!
    if (shape.type == ShapeType::Square) {
      // get angle of square, so that we can determine which points the square covers
      auto squareData = std::dynamic_pointer_cast<SquareData>(shape.data);
      auto sideLength = squareData->side_length;

      // determine orientation of shape relative to sphere by picking the farthest direction and make the square face the sphere from that direction 
      //  (there are 3 orientations, the plane going in the x-y directions, x-z, and y-z)
      auto centerOfSquare = shape.location;
      auto x = std::vector<double>();
      auto y = std::vector<double>();
      auto z = std::vector<double>();
      double square_step = 0.1; // arbitrary nu-mber to initialize x/y/z values for the square
      if (centerOfSquare.x == std::max(centerOfSquare.x, std::max(centerOfSquare.y, centerOfSquare.z))){
        for (double y_inc = centerOfSquare.y - sideLength/2; y_inc < centerOfSquare.y + sideLength/2; y_inc +=square_step) {
          y.emplace_back(y_inc);
        }
        for (double z_inc = centerOfSquare.z - sideLength/2; z_inc < centerOfSquare.z + sideLength/2; z_inc +=square_step) {
          z.emplace_back(z_inc);
        }
        x = std::vector<double>(y.size(), centerOfSquare.x);

      } else if (centerOfSquare.y == std::max(centerOfSquare.x, std::max(centerOfSquare.y, centerOfSquare.z))) {
        for (double x_inc = centerOfSquare.x - sideLength/2; x_inc < centerOfSquare.x + sideLength/2; x_inc +=square_step) {
          x.emplace_back(x_inc);
        }
        for (double z_inc = centerOfSquare.z - sideLength/2; z_inc < centerOfSquare.z + sideLength/2; z_inc +=square_step) {
          z.emplace_back(z_inc);
        }
        y = std::vector<double>(x.size(), centerOfSquare.y);

      } else if (centerOfSquare.z == std::max(centerOfSquare.x, std::max(centerOfSquare.y, centerOfSquare.z))) {
        for (double x_inc = centerOfSquare.x - sideLength/2; x_inc < centerOfSquare.x + sideLength/2; x_inc +=square_step) {
          x.emplace_back(x_inc);
        }
        for (double y_inc = centerOfSquare.y - sideLength/2; y_inc < centerOfSquare.y + sideLength/2; y_inc +=square_step) {
          y.emplace_back(y_inc);
        }
        z = std::vector<double>(x.size(), centerOfSquare.z);
      }

      // TODO: that's a long name
      auto mapOfNewValues = ProjectShapeOntoSphereAndCalculateDistanceFromCenterOfShape(
        r, 
        theta_sphere, 
        phi_sphere, 
        x, 
        y, 
        z, 
        shape.location, 
        shape.data->CalculateDistanceFromCenterToFarthestPoint(), 
        theta_step, 
        phi_step
      );

      for (auto& [key, value] : mapOfNewValues) {
        if (value < 0 || value > 1) {
          std::cerr << value << std::endl;
          throw std::runtime_error("OH NO VAL > 1 or VAL < 0 WEEWOOWEEWOO");
        }

        if (values_map.find(key) != values_map.end()) {
          values_map[key] += value;
        } else {
          values_map.insert({key, value});
        }
      }
    }
  }

  // values can be greater than one if multiple shapes overlap. Any time this occurs, reduce the value to 1
  for (auto& [key, value] : values_map) {
    if (value > 1) {
      value = 1;
    }
  }

  return SphereData{theta_sphere, phi_sphere, values_map};
}

void OutputSphereToFile(const SphereData& sphereData, const std::string& outputFolder) {
  std::string outputFilepath = outputFolder + "/SphereData.csv";
  std:: cout << outputFilepath;
  std::ofstream sphereFile(outputFilepath);

  sphereFile << "Theta,Phi,Value" << std::endl;

  for (int theta_counter = 0; theta_counter < int(sphereData.theta.size()); theta_counter++) {
    for (int phi_counter = 0; phi_counter < int(sphereData.theta.size()); phi_counter++) {
      double theta = sphereData.theta[theta_counter];
      double phi = sphereData.phi[phi_counter];

      auto key = std::make_pair(theta, phi);

      double value;
      if (sphereData.values.find(key) != sphereData.values.end()) {
        value = sphereData.values.at(key);
      } else {
        value = 0;
      }

      sphereFile << theta << "," << phi << "," << value << std::endl;
    }
  }
}


int main(int argc, char *argv[]) {
  // TODO: check at read time to see if given theta/phi locations are valid

  // read command line arguments, which contain the information needed to initialize the desired code
  auto run_specifications = ParseRuntimeArguments(argc, argv);

  // TODO: using shape data from runtime arguments, create a lattice that represents theta and phi of the sphere. 
  //  The map from this function uses a key as a theta/phi location, and the value is the value from the 
  auto plotted_sphere = CreateSphereFromShapes(run_specifications.shapes);

  // Take points from sphere and output to a csv file
  OutputSphereToFile(plotted_sphere, run_specifications.outputPath);

  if (run_specifications.runFourier == true) {
    // TODO: do fourier transform on the created sphere

    // TODO: output the fourier transform data to a file
  }


  return (0);
}