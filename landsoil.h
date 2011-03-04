#ifndef LANDSOIL_H
#define LANDSOIL_H

#include <QString>

class soiltype
{
public:
  QString name;

  double wsat;      // saturated volumetric water content ECMWF config [-]
  double wfc;       // volumetric water content field capacity [-]
  double wwilt;     // volumetric water content wilting point [-]

  double C1sat;
  double C2ref;

  double a;         // Clapp and Hornberger retention curve parameter a
  double b;         // Clapp and Hornberger retention curve parameter b
  double p;         // Clapp and Hornberger retention curve parameter p
  double CGsat;     // saturated soil conductivity for heat
};

class surfacetype
{
public:
  QString name;

  double LAI;       // leaf area index [-]
  double gD;        // correction factor transpiration for VPD [-]
  double rsmin;     // minimum resistance transpiration [s m-1]
  double alpha;     // surface albedo [-]
  double cveg;      // vegetation fraction [-]
  double Lambda;    // thermal diffusivity skin layer [-]
  double z0m;       // roughness length for momentum [m]
  double z0h;       // roughness length for scalars [m]
  int C3C4;         // 0 = no C3C4, 1=C3, 2=C4
};

#endif // LANDSOIL_H
