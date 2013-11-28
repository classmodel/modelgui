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

#include "modelchemtypes.h"

// Model input class
class modelinput
{
public:
  modelinput();
  modelinput(const modelinput&);
  modelinput &operator=(const modelinput&);
  bool operator!=(const modelinput&);

  // general model variables
  double runtime;   // duration of model run [s]
  double dt;        // time step [s]
  double sinperiod; // period for sinusoidal flux [s]

  // mixed-layer variables  
  bool   sw_ml;     // mixed-layer model switch;
  bool   sw_ftcws;  // compensate FT warming due to subsidence
  bool   sw_shearwe; // Include shear effect entrainment
  double h;         // initial ABL height [m]
  double Ps;        // surface pressure [Pa]
  double omegas;    // large scale divergence [s-1]
  double fc;        // Coriolis parameter [s-1]
  double wstar;     // Deardorff vertical velocity scale [m s-1]
  
  double theta;     // initial mixed-layer potential temperature [K]
  double dtheta;    // initial temperature jump at h [K]
  double gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double advtheta;  // advection of heat [K s-1]
  double beta;      // entrainment ratio for virtual heat [-]
  double wtheta;    // surface kinematic heat flux [K m s-1]
  bool   sw_wtheta; // switch for sinusoidal wtheta
  
  double q;         // initial mixed-layer specific humidity [kg kg-1]
  double dq;        // initial specific humidity jump at h [kg kg-1]
  double gammaq;    // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  double advq;      // advection of moisture [kg kg-1 s-1]
  double wq;        // surface kinematic moisture flux [kg kg-1 m s-1]
  bool   sw_wq;     // switch for sinusoidal wq

  bool   sw_wind;   // prognostic wind switch
  double u;         // initial mixed-layer u-wind speed [m s-1]
  double du;        // initial u-wind jump at h [m s-1]
  double gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double advu;      // advection of u-wind [m s-2]
  
  double v;         // initial mixed-layer u-wind speed [m s-1]
  double dv;        // initial u-wind jump at h [m s-1]
  double gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double advv;      // advection of v-wind [m s-2]

  // BvS; a scalar, without the need for the chemistry scheme :)
  double sca;       // initial mixed-layer scalar [kg kg-1]
  double dsca;      // initial scalar jump at h [kg kg-1]
  double gammasca;  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  double advsca;    // advection of scalar [kg kg-1 s-1]
  double wsca;      // surface kinematic scalar flux [kg kg-1 m s-1]

  double CO2;       // initial mixed-layer CO2 [ppm]
  double dCO2;      // initial CO2 jump at h [ppm]
  double gammaCO2;  // free atmosphere CO2 lapse rate [ppm]
  double advCO2;    // advection of CO2 [ppm]
  double wCO2;      // surface kinematic CO2 flux [ppm]

  // Other scalars/species (chemistry code..)
  int nsc;
  double *sc;       // initial mixed-layer scalar [kg kg-1]
  double *dsc;      // initial scalar jump at h [kg kg-1]
  double *gammasc;  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  double *advsc;    // advection of moisture [kg kg-1 s-1]
  double *wsc;      // surface kinematic moisture flux [kg kg-1 m s-1]
  int    *sw_wsc;   // switch for sinusoidal wsc

  // surface layer variables
  bool   sw_sl;     // surface layer switch
  double ustar;     // surface friction velocity [m s-1]
  double z0m;       // roughness length for momentum [m]
  double z0h;       // roughness length for scalars [m]

  // radiation parameters
  bool   sw_rad;    // radiation switch
  double lat;       // latitude [deg]
  double lon;       // longitude [deg]
  double doy;       // day of the year [-]
  double tstart;    // time of the day [h UTC]
  double cc;        // cloud cover fraction [-]
  double Q;         // net radiation [W m-2]

  // land surface parameters
  bool   sw_ls;     // land surface switch
  bool   sw_jarvis; // Jarvis / A-Gs switch
  int    C3C4;      // C3 or C4 vegetation
  bool   sw_sea;    // land / sea switch
  double wg;        // volumetric water content top soil layer [m3 m-3]
  double w2;        // volumetric water content deeper soil layer [m3 m-3]
  double Tsoil;     // temperature top soil layer [K]
  double T2;        // temperature deeper soil layer [K]
  
  double a;         // Clapp and Hornberger retention curve parameter a
  double b;         // Clapp and Hornberger retention curve parameter b
  double p;         // Clapp and Hornberger retention curve parameter p 
  double CGsat;     // saturated soil conductivity for heat
  
  double wsat;      // saturated volumetric water content ECMWF config [-]
  double wfc;       // volumetric water content field capacity [-]
  double wwilt;     // volumetric water content wilting point [-]
  
  double C1sat;     // Coefficient force term moisture [-]
  double C2ref;     // Coefficient restore term moisture [-]
    
  double LAI;       // leaf area index [-]
  double gD;        // correction factor transpiration for VPD [-]
  double rsmin;     // minimum resistance transpiration [s m-1]
  double rssoilmin; // minimum resistance soil evaporation [s m-1]
  double alpha;     // surface albedo [-]
    
  double Ts;        // initial surface temperature [K]
    
  double cveg;      // vegetation fraction [-]
  double Wmax;      // thickness of water layer on wet vegetation [m]
  double Wl;        // equivalent water layer depth for wet vegetation [m]
  
  double Lambda;    // thermal diffusivity skin layer [-]

  // shallow-cumulus
  bool sw_cu;       // shallow-cumulus switch
  bool sw_curad;    // link ac -> cc -> radiation

  double dFz;       // Cloud top radiative divergence (stratocumulus)

  // chemistry
  bool   sw_chem;
  bool   sw_chem_constant;
  bool   sw_photo_constant;
  Reaction *reactions;
  bool   *sw_reactions;
  int    rsize;
  int    csize;
  double P_ref;
  double Tcbl_ref;
  double Tfc_ref;
  double qcbl_ref;
  double qfc_ref;
  double tod_ref;
  double stocoef;
};

