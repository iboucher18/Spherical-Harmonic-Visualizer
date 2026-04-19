#include <vector>
#include <map>
#include <SphereData.h>
#include <SphericalHarmonicGenerator.h>

double milnes_rule(
   std::vector<double> values,
   double x_min, 
   double x_max) 
{
   int num_pts = values.size();
   double interval = ((x_max - x_min)/double(num_pts - 1));  // called h in notes
   double sum=  0.;  // initialize integration sum to zero		 

   // n=0 is first point, n=num_pts-1 is last point. we want to ignore first+last point and do them at end
   for (int n=1; n<num_pts-1; n++)
   {
      double point = values[n];
      if ((n-1)%2 == 0) {
         sum += (64./45.)*interval*point;
      } 
      else {
         if (n%4 == 0) {
            sum += (28./45.)*interval*point;
         }
         else {
            sum += (24./45.)*interval*point;
         }
      }
   } 
   // add in the endpoint contributions   
   sum +=  (14./45.*interval) * (values[0] + values.back());	

   return sum;
}


inline SphereData CalculateFourierTransformOfSphere(const SphereData& sphere, int l) {
  // Calculate m values
  auto m_values = std::vector<int>();
  for (int m = -1*l; m <= l; m++) {
    m_values.emplace_back(m);
  }

  // for each l/m combination, calculate the fourier transform and add it to the sum
  auto fourierSphereData_real = sphere;
  fourierSphereData_real.values = std::map<std::pair<double, double>, double>();

  auto fourierSphereData_imag = fourierSphereData_real;

  for (int m : m_values) {
    auto sphericalHarmonicTimesF_real = std::map<std::pair<double, double>, double>();
    auto sphericalHarmonicTimesF_imag = std::map<std::pair<double, double>, double>();  

    for (const auto& theta : sphere.theta) {
      for (const auto& phi : sphere.phi) {
        auto thetaPhi = std::make_pair(theta, phi);

        auto sphericalHarmonic = CalculateSphericalHarmonic(l, m, theta, phi);

        double sphericalHarmonic_real = sphericalHarmonic.first;
        double sphericalHarmonic_imag = sphericalHarmonic.second;

        sphericalHarmonicTimesF_real.insert({thetaPhi, sphericalHarmonic_real * sphere.values.at(thetaPhi)});
        sphericalHarmonicTimesF_imag.insert({thetaPhi, sphericalHarmonic_imag * sphere.values.at(thetaPhi)});
      }
    }

    //  Do the numeric integration using milne's method. We must integrate twice, since this is a 2D integral
    auto valuesToIntegrate = std::vector<double>();
    for (const auto& theta : sphere.theta) {
      auto valuesAtThisTheta = std::vector<double>();
      for (const auto& phi : sphere.phi) {
        valuesAtThisTheta.emplace_back(sphere.values.at(std::make_pair(theta, phi)));
      }
      valuesToIntegrate.emplace_back(milnes_rule(valuesAtThisTheta, 0, M_PI));
    }

    double cn = milnes_rule(valuesToIntegrate, 0, 2*M_PI);

    for (const auto& theta : sphere.theta) {
      for (const auto& phi : sphere.phi) {
        auto thetaPhi = std::make_pair(theta, phi);

        if (fourierSphereData.values.find(thetaPhi) != fourierSphereData.values.end()) {
          fourierSphereData.values[thetaPhi] += cn * sphericalHarmonicTimesF_real.at(thetaPhi);
        } else {
          fourierSphereData.values.insert({thetaPhi, cn * sphericalHarmonicTimesF_real.at(thetaPhi)});
        }
      }
    }
  }

  return fourierSphereData;
}