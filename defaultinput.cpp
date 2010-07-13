#include "mainwindow.h"

void MainWindow::readdefaultinput()
{
  // temporary function to fill run form with default values

  // model test
  defaultinput.dt         = 60.;      // time step [s]
  defaultinput.runtime    = 36000.;   // total run time [s]
  defaultinput.sinperiod  = 43200.;   // period for sinusoidal heat fluxes [s]

  // mixed-layer input
  defaultinput.sw_ml      = true;     // mixed-layer model switch
  defaultinput.h          = 200.;     // initial ABL height [m]
  defaultinput.Ps         = 101300.;  // surface pressure [Pa]
  defaultinput.ws         = 0.;       // large scale vertical velocity [m s-1]
  defaultinput.fc         = 1.e-4;    // Coriolis parameter [m s-1]

  defaultinput.theta      = 288.;     // initial mixed-layer potential temperature [K]
  defaultinput.dtheta     = 5.;       // initial temperature jump at h [K]
  defaultinput.gammatheta = 0.006;    // free atmosphere potential temperature lapse rate [K m-1]
  defaultinput.advtheta   = 0.;       // advection of heat [K s-1]
  defaultinput.beta       = 0.2;      // entrainment ratio for virtual heat [-]
  defaultinput.wtheta     = 0.1;      // surface kinematic heat flux [K m s-1]
  defaultinput.sw_wtheta  = false;    // switch for sinusoidal wtheta

  defaultinput.q          = 0.007;    // initial mixed-layer specific humidity [kg kg-1]
  defaultinput.dq         = -0.001;   // initial specific humidity jump at h [kg kg-1]
  defaultinput.gammaq     = 0.;       // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  defaultinput.advq       = 0.;       // advection of moisture [kg kg-1 s-1]
  defaultinput.wq         = 0.0001;   // surface kinematic moisture flux [kg kg-1 m s-1]
  defaultinput.sw_wq      = false;    // switch for sinusoidal wq

  defaultinput.sw_wind    = true;     // prognostic wind switch
  defaultinput.u          = 6.;       // initial mixed-layer u-wind speed [m s-1]
  defaultinput.du         = 4.;       // initial u-wind jump at h [m s-1]
  defaultinput.gammau     = 0.;       // free atmosphere u-wind speed lapse rate [s-1]
  defaultinput.advu       = 0.;       // advection of u-wind [m s-2]

  defaultinput.v          = -4.0;     // initial mixed-layer u-wind speed [m s-1]
  defaultinput.dv         = 4.0;      // initial u-wind jump at h [m s-1]
  defaultinput.gammav     = 0.;       // free atmosphere v-wind speed lapse rate [s-1]
  defaultinput.advv       = 0.;       // advection of v-wind [m s-2]

  // surface layer input
  defaultinput.sw_sl      = false;    // surface layer switch
  defaultinput.ustar      = 0.3;      // surface friction velocity [m s-1]
  defaultinput.z0m        = 0.05;     // roughness length for momentum [m]
  defaultinput.z0h        = 0.01;     // roughness length for scalars [m]
  
  // radiation parameters
  defaultinput.sw_rad     = false;    // radiation switch
  defaultinput.lat        = 51.97;    // latitude [deg]
  defaultinput.lon        = -4.93;    // longitude [deg]
  defaultinput.doy        = 268.;     // day of the year [-]
  defaultinput.tstart     = 6.8;      // time of the day [h UTC]
  defaultinput.cc         = 0.0;      // cloud cover fraction [-]
  defaultinput.Q          = 400.;      // net radiation [W m-2]
  
  // land surface parameters
  defaultinput.sw_ls      = false;    // land surface switch
  defaultinput.sw_sea     = false;    // land / sea switch
  defaultinput.wg         = 0.21;     // volumetric water content top soil layer [m3 m-3]
  defaultinput.w2         = 0.21;     // volumetric water content deeper soil layer [m3 m-3]
  defaultinput.cveg       = 0.9;      // vegetation fraction [-]
  defaultinput.Tsoil      = 285.;     // temperature top soil layer [K]
  defaultinput.T2         = 286.;     // temperature deeper soil layer [K]
  defaultinput.a          = 0.219;    // Clapp and Hornberger retention curve parameter a
  defaultinput.b          = 4.90;     // Clapp and Hornberger retention curve parameter b
  defaultinput.p          = 4.;       // Clapp and Hornberger retention curve parameter c
  defaultinput.CGsat      = 3.56e-6;  // saturated soil conductivity for heat
  
  defaultinput.wsat       = 0.472;    // saturated volumetric water content ECMWF config [-]
  defaultinput.wfc        = 0.323;    // volumetric water content field capacity [-]
  defaultinput.wwilt      = 0.171;    // volumetric water content wilting point [-]
  
  defaultinput.C1sat      = 0.132;
  defaultinput.C2ref      = 1.8;
  
  defaultinput.LAI        = 2.;       // leaf area index [-]
  defaultinput.gD         = 0.0;      // correction factor transpiration for VPD [-]
  defaultinput.rsmin      = 40.;      // minimum resistance transpiration [s m-1]
  defaultinput.rssoilmin  = 50.;      // minimun resistance soil evaporation [s m-1]
  defaultinput.alpha      = 0.25;     // surface albedo [-]
  
  defaultinput.Ts         = 290.;     // initial surface temperature [K]
  
  defaultinput.Wmax       = 0.0002;   // thickness of water layer on wet vegetation [m]
  defaultinput.Wl         = 0.0000;   // equivalent water layer depth for wet vegetation [m]
  
  defaultinput.Lambda     = 5.9;      // thermal diffusivity skin layer [-]
}
