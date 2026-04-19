#include <iostream>
// TODO: include spherical harmonic calculator, when it exists

void ExecuteTests() {
  // ---------------------------------------------------------------
  // LEGENDRE POLYNOMAL TESTS
  // TEST ONE
  auto polyone = CalculateAssociatedLegendrePolynomial(0, 0, 0);
  auto polytwo = CalculateAssociatedLegendrePolynomial(0, 0, 1);
  auto polythree = CalculateAssociatedLegendrePolynomial(0, 0, -1);
  if (polyone != 1 || polytwo != 1 || polythree != 1) {
    std::cout << "FAILED LEGENDRE POLYNOMIAL TEST 1: l=m=0 did not return a value of one" << std::endl;
  }
  else {
    std::cout << "PASSED LEGENDRE POLYNOMIAL TEST 1" << std::endl;
  }

  // TEST TWO
  double x = 0.73;
  polyone = CalculateAssociatedLegendrePolynomial(1, 0, x);
  if (polyone != x) {
    std::cout << "FAILED LEGENDRE POLYNOMIAL TEST 2: l=1, m=0, x=0.73 did not return 0.73" << std::endl;
  }
  else {
    std::cout << "PASSED LEGENDRE POLYNOMIAL TEST 2" << std::endl;
  }
  // ---------------------------------------------------------------

  // SPHERICAL HARMONIC TESTS
  // TEST ONE
  auto harmone = CalculateSphericalHarmonic(0, 0, 0, 0);
  auto harmtwo = CalculateSphericalHarmonic(0, 0, 1.5, 0.5);
  auto harmthree = CalculateSphericalHarmonic(0, 0, -1, 1);
  double expected_real_answer = 1/sqrt(4*M_PI);
  double expected_imag_answer = 0;
  if (harmone.first != expected_real_answer || harmtwo.first != expected_real_answer || harmthree.first != expected_real_answer
    || harmone.second != expected_imag_answer || harmtwo.second != expected_imag_answer || harmthree.second != expected_imag_answer
  ) {
    std::cout << "FAILED SPHERICAL HARMONIC TEST 1: l=m=0 did not return the expected value" << std::endl;
  }
  else {
    std::cout << "PASSED SPHERICAL HARMONIC TEST 1" << std::endl;
  }

  // TEST TWO
  double theta = 4.2;
  double phi = 0.58;
  expected_real_answer = sqrt(3.0/8.0/M_PI)*cos(phi)*sin(theta);
  expected_imag_answer = -1.0*sqrt(3.0/8.0/M_PI)*sin(phi)*sin(theta);
  harmone = CalculateSphericalHarmonic(1, -1, theta, phi);
  if (fabs(harmone.first - expected_real_answer) > 1e-10
   || fabs(harmone.second - expected_imag_answer) > 1e-10
  ) {
    std::cout << "FAILED SPHERICAL HARMONIC TEST 2: spherical harmonic calculation for l=1, m=-1 did not return the expected value" << std::endl;
  }
  else {
    std::cout << "PASSED SPHERICAL HARMONIC TEST 2" << std::endl;
  }

  
  // TEST THREE
  theta = 1.21;
  phi = 0.52;
  expected_real_answer = -1.0*sqrt(3.0/8.0/M_PI)*cos(phi)*sin(theta);
  expected_imag_answer = -1.0*sqrt(3.0/8.0/M_PI)*sin(phi)*sin(theta);
  harmone = CalculateSphericalHarmonic(1, 1, theta, phi);
  if (fabs(harmone.first - expected_real_answer) > 1e-10
   || fabs(harmone.second - expected_imag_answer) > 1e-10
  ) {
    std::cout << "FAILED SPHERICAL HARMONIC TEST 3: spherical harmonic calculation for l=1, m=1 did not return the expected value" << std::endl;
  }
  else {
    std::cout << "PASSED SPHERICAL HARMONIC TEST 3" << std::endl;
  }




}
