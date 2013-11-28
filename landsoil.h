/*
 * CLASS
 * Copyright (c) 2010-2013 Meteorology and Air Quality section, Wageningen University and Research centre
 * Copyright (c) 2011-2013 Jordi Vila-Guerau de Arellano
 * Copyright (c) 2011-2013 Chiel van Heerwaarden
 * Copyright (c) 2011-2013 Bart van Stratum
 * Copyright (c) 2011-2013 Kees van den Dries
 *
 * This file is part of CLASS
 *
 * CLASS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CLASS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CLASS.  If not, see <http://www.gnu.org/licenses/>.
 */

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
