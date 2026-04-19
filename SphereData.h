#pragma once

#include <vector>
#include <map>
#include <utility>

struct SphereData {
  std::vector<double> theta;
  std::vector<double> phi;
  std::map<std::pair<double, double>, double> values;
};
