#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_legendre.h>

int Factorial(int x) {
  return std::tgamma(x+1); // std::tgamma(x + 1) is equivalent to x!
}

inline double CalculateAssociatedLegendrePolynomial(double l, double m, double x) {
  double legendre_value;
  if (m < 0) {
    throw std::runtime_error("m cannot be less than 0!");
  } else {
    legendre_value = gsl_sf_legendre_Plm(l, m, x);
  }

  return legendre_value;
}


inline std::pair<double, double> CalculateSphericalHarmonic(
  int l, 
  int m, 
  double theta,
  double phi
) {
  if (m < 0) {
    m*=-1.0;
    double termTwo = sqrt( (2.0*l+1.0) * Factorial(l-m) / (4.0*M_PI*Factorial(l+m)) );
    double termThree_real = cos(m*phi);
    double termThree_imag = sin(m*phi);

    double legendrePolynomial = CalculateAssociatedLegendrePolynomial(l, m, cos(theta));

    double sphericalHarmonic_real = termTwo * termThree_real * legendrePolynomial;
    double sphericalHarmonic_imag = termTwo * termThree_imag * legendrePolynomial;

    if (theta > M_PI) {
      return std::make_pair(sphericalHarmonic_real, -1.0*sphericalHarmonic_imag);
    } else {
      return std::make_pair(-1.0*sphericalHarmonic_real, sphericalHarmonic_imag);
    }
  } else {
    double termTwo = sqrt( (2.0*l+1.0) * Factorial(l-m) / (4.0*M_PI*Factorial(l+m)) );
    double termThree_real = cos(m*phi);
    double termThree_imag = sin(m*phi);

    double legendrePolynomial = CalculateAssociatedLegendrePolynomial(l, m, cos(theta));

    double sphericalHarmonic_real = termTwo * termThree_real * legendrePolynomial;
    double sphericalHarmonic_imag = termTwo * termThree_imag * legendrePolynomial;

    if (theta > M_PI) {
      return std::make_pair(-1.0*sphericalHarmonic_real, -1.0*sphericalHarmonic_imag);
    } else {
      return std::make_pair(sphericalHarmonic_real, sphericalHarmonic_imag);
    }
  }
}
