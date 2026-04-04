// #include <gsl/gsl_rng.h>
// #include <gsl/gsl_randist.h>
#include <vector>

class FourierTransformCalculator
{
public:
  FourierTransformCalculator(std::vector<double> x, std::vector<double> y);

  // TODO: determine output type
  static void CalculateFourierTransform(std::vector<double> x, std::vector<double> y);

  // TODO: determine output type
  void CalculateFourierTransform();

private:
  // TODO: create parameters
  std::vector<double> x;
  std::vector<double> y;

};
