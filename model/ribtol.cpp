// fast conversion of bulk Richardson number to Obukhov length

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <cmath>
using namespace std;

inline double psim(double zeta)
{
  double psim;
  double x;
  if(zeta <= 0.)
  {
    //x     = (1. - 16. * zeta) ** (0.25)
    //psim  = 3.14159265 / 2. - 2. * arctan(x) + log( (1.+x) ** 2. * (1. + x ** 2.) / 8.)
    x    = pow(1. + pow(3.6 * abs(zeta),2./3.), -0.5);
    psim = 3. * log( (1. + 1. / x) / 2.);
  }
  else
  {
    psim  = -2./3. * (zeta - 5./0.35) * exp(-0.35 * zeta) - zeta - (10./3.) / 0.35;
  }
  return psim;
}
    
inline double psih(double zeta)
{
  double psih;
  double x;
  if(zeta <= 0.)
  {
    // x     = (1. - 16. * zeta) ** (0.25)
    // psih  = 2. * log( (1. + x ** 2.) / 2. )
    x     = pow(1. + pow(7.9 * abs(zeta), (2./3.)), -0.5);
    psih  = 3. * log( (1. + 1. / x) / 2.);
  }
  else
  {
    psih  = -2./3. * (zeta - 5./0.35) * exp(-0.35 * zeta) - pow(1. + (2./3.) * zeta, 1.5) - (10./3.) / 0.35 + 1.;
  }
  return psih;
}


double ribtol(double Rib, double zsl, double z0m, double z0h)
{
  double L, L0;
  double Lstart, Lend;
  double fx, fxdif;

  if(Rib > 0.)
  {
    L    = 1.;
    L0   = 2.;
  }
  else
  {
    L  = -1.;
    L0 = -2.;
  }
    
  while (abs(L - L0) > 0.001)
  {
    L0      = L;
    fx      = Rib - zsl / L * (log(zsl / z0h) - psih(zsl / L) + psih(z0h / L)) / pow(log(zsl / z0m) - psim(zsl / L) + psim(z0m / L), 2.);
    Lstart  = L - 0.001 * L;
    Lend    = L + 0.001 * L;
    fxdif   = ( (- zsl / Lstart * (log(zsl / z0h) - psih(zsl / Lstart) + psih(z0h / Lstart)) / pow(log(zsl / z0m) - psim(zsl / Lstart) + psim(z0m / Lstart), 2.)) - (-zsl / Lend * (log(zsl / z0h) - psih(zsl / Lend) + psih(z0h / Lend)) / pow(log(zsl / z0m) - psim(zsl / Lend) + psim(z0m / Lend), 2.)) ) / (Lstart - Lend);
    L       = L - fx / fxdif;
  }
  
  return L;

}

BOOST_PYTHON_MODULE(ribtol)
{
    using namespace boost::python;
    def("ribtol", ribtol);
}

