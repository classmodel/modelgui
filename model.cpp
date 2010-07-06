#include <cmath>
#include <iostream>
#include <fstream>
#include "model.h"
using namespace std;

inline double sign(double n) { return n > 0 ? 1 : (n < 0 ? -1 : 0);}

model::model(modelinput extinput)
{
  // model constants
  Lv         =  2.5e6;                  // heat of vaporization [J kg-1]
  cp         =  1005.;                  // specific heat of dry air [J kg-1 K-1]
  rho        =  1.2;                    // density of air [kg m-3]
  k          =  0.4;                    // Von Karman constant [-]
  g          =  9.81;                   // gravity acceleration [m s-2]
  Rd         =  287.;                   // gas constant for dry air [J kg-1 K-1]
  Rv         =  461.5;                  // gas constant for moist air [J kg-1 K-1]
  bolz       =  5.67e-8;                // Bolzman constant [-]
  rhow       =  1000.;                  // density of water [kg m-3]
  S0         =  1368.;                  // solar constant [W m-2]
  pi         =  3.14159265359;          // Pi

  // read initial and boundary conditions from input file
  input.runtime    =  extinput.runtime;          // duration of model run [s]
  input.dt         =  extinput.dt;               // time step [s]

  // mixed-layer
  input.sw_ml      =  extinput.sw_ml;
  input.h          =  extinput.h;                // initial ABL height [m]
  input.Ps         =  extinput.Ps;               // surface pressure [Pa]
  input.ws         =  extinput.ws;               // large scale vertical velocity [m s-1]
  input.fc         =  extinput.fc;               // coriolis parameter [s-1]
  
  input.theta      =  extinput.theta;            // initial mixed-layer potential temperature [K]
  input.dtheta     =  extinput.dtheta;           // initial temperature jump at h [K]
  input.gammatheta =  extinput.gammatheta;       // free atmosphere potential temperature lapse rate [K m-1]
  input.advtheta   =  extinput.advtheta;         // advection of heat [K s-1]
  input.beta       =  extinput.beta;             // entrainment ratio for virtual heat [-]
  input.wtheta     =  extinput.wtheta;           // surface kinematic heat flux [K m s-1]
  
  input.q          =  extinput.q;                // initial mixed-layer specific humidity [kg kg-1]
  input.dq         =  extinput.dq;               // initial specific humidity jump at h [kg kg-1]
  input.gammaq     =  extinput.gammaq;           // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  input.advq       =  extinput.advq;             // advection of moisture [kg kg-1 s-1]
  input.wq         =  extinput.wq;               // surface kinematic moisture flux [kg kg-1 m s-1]

  input.sw_wind    =  extinput.sw_wind;          // prognostic wind switch
  input.u          =  extinput.u;                // initial mixed-layer u-wind speed [m s-1]
  input.du         =  extinput.du;               // initial u-wind jump at h [m s-1]
  input.gammau     =  extinput.gammau;           // free atmosphere u-wind speed lapse rate [s-1]
  input.advu       =  extinput.advu;             // advection of u-wind [m s-2]
  
  input.v          =  extinput.v;                // initial mixed-layer u-wind speed [m s-1]
  input.dv         =  extinput.dv;               // initial u-wind jump at h [m s-1]
  input.gammav     =  extinput.gammav;           // free atmosphere v-wind speed lapse rate [s-1]
  input.advv       =  extinput.advv;             // advection of v-wind [m s-2]
  
  // surface-layer
  input.sw_sl      =  extinput.sw_sl;      // surface layer switch
  input.ustar      =  extinput.ustar;      // surface friction velocity [m s-1]
  input.z0m        =  extinput.z0m;        // roughness length for momentum [m]
  input.z0h        =  extinput.z0h;        // roughness length for scalars [m]
  
  // radiation
  input.sw_rad     =  extinput.sw_rad;     // radiation switch
  input.lat        =  extinput.lat;        // latitude [deg]
  input.lon        =  extinput.lon;        // longitude [deg]
  input.doy        =  extinput.doy;        // day of the year [-]
  input.tstart     =  extinput.tstart;     // time of the day [h UTC]
  input.cc         =  extinput.cc;         // cloud cover fraction [-]
  input.Q          =  extinput.Q;          // net radiation [-]
  
  // land surface
  input.sw_ls      =  extinput.sw_ls;      // land surface switch
  input.wg         =  extinput.wg;         // volumetric water content top soil layer [m3 m-3]
  input.w2         =  extinput.w2;         // volumetric water content deeper soil layer [m3 m-3]
  input.Tsoil      =  extinput.Tsoil;      // temperature top soil layer [K]
  input.T2         =  extinput.T2;         // temperature deeper soil layer [K]
  
  input.a          =  extinput.a;          // Clapp and Hornberger retention curve parameter a
  input.b          =  extinput.b;          // Clapp and Hornberger retention curve parameter b
  input.p          =  extinput.p;          // Clapp and Hornberger retention curve parameter p
  input.CGsat      =  extinput.CGsat;      // saturated soil conductivity for heat
  
  input.wsat       =  extinput.wsat;       // saturated volumetric water content ECMWF config [-]
  input.wfc        =  extinput.wfc;        // volumetric water content field capacity [-]
  input.wwilt      =  extinput.wwilt;      // volumetric water content wilting point [-]
  
  input.C1sat      =  extinput.C1sat;
  input.C2ref      =  extinput.C2ref;
  
  input.LAI        =  extinput.LAI;        // leaf area index [-]
  input.gD         =  extinput.gD;         // correction factor transpiration for VPD [-]
  input.rsmin      =  extinput.rsmin;      // minimum resistance transpiration [s m-1]
  input.rssoilmin  =  extinput.rssoilmin;  // minimum resistance soil evaporation [s m-1]
  input.alpha      =  extinput.alpha;      // surface albedo [-]
  
  input.Ts         =  extinput.Ts;         // initial surface temperature [K]
  
  input.cveg       =  extinput.cveg;       // vegetation fraction [-]
  input.Wmax       =  extinput.Wmax;       // thickness of water layer on wet vegetation [m]
  input.Wl         =  extinput.Wl;         // equivalent water layer depth for wet vegetation [m]
  
  input.Lambda     =  extinput.Lambda;     // thermal diffusivity skin layer [-]

  return;
}

void model::initmodel()
{
  // read initial and boundary conditions from input file
  runtime    =  input.runtime;          // duration of model run [s]
  dt         =  input.dt;               // time step [s]
  sinperiod  =  input.sinperiod;        // period for sinusoidal heat fluxes [s]

  // mixed-layer
  sw_ml      =  input.sw_ml;
  h          =  input.h;                // initial ABL height [m]
  Ps         =  input.Ps;               // surface pressure [Pa]
  ws         =  input.ws;               // large scale vertical velocity [m s-1]
  fc         =  input.fc;               // coriolis parameter [s-1]
  
  theta      =  input.theta;            // initial mixed-layer potential temperature [K]
  dtheta     =  input.dtheta;           // initial temperature jump at h [K]
  gammatheta =  input.gammatheta;       // free atmosphere potential temperature lapse rate [K m-1]
  advtheta   =  input.advtheta;         // advection of heat [K s-1]
  beta       =  input.beta;             // entrainment ratio for virtual heat [-]
  wtheta     =  input.wtheta;           // surface kinematic heat flux [K m s-1]
  wtheta0    =  input.wtheta;           // maximum surface kinematic heat flux [K m s-1]
  
  thetasurf  =  input.theta;            // surface potential temperature [K]
  
  q          =  input.q;                // initial mixed-layer specific humidity [kg kg-1]
  dq         =  input.dq;               // initial specific humidity jump at h [kg kg-1]
  gammaq     =  input.gammaq;           // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq       =  input.advq;             // advection of moisture [kg kg-1 s-1]
  wq         =  input.wq;               // surface kinematic moisture flux [kg kg-1 m s-1]
  wq0        =  input.wq;               // maximum surface kinematic moisture flux [kg kg-1 m s-1]
  
  qsat       =  -1.;                    // mixed-layer saturated specific humidity [kg kg-1]
  esat       =  -1.;                    // mixed-layer saturated vapor pressure [Pa]
  e          =  -1.;                    // mixed-layer vapor pressure [Pa]
  qsatsurf   =  -1.;                    // surface saturated specific humidity [g kg-1]
  dqsatdT    =  -1.;                    // slope saturated specific humidity curve [g kg-1 K-1]
  
  sw_wind    =  input.sw_wind;          // prognostic wind switch
  u          =  input.u;                // initial mixed-layer u-wind speed [m s-1]
  du         =  input.du;               // initial u-wind jump at h [m s-1]
  gammau     =  input.gammau;           // free atmosphere u-wind speed lapse rate [s-1]
  advu       =  input.advu;             // advection of u-wind [m s-2]
  
  v          =  input.v;                // initial mixed-layer u-wind speed [m s-1]
  dv         =  input.dv;               // initial u-wind jump at h [m s-1]
  gammav     =  input.gammav;           // free atmosphere v-wind speed lapse rate [s-1]
  advv       =  input.advv;             // advection of v-wind [m s-2]
  
  // surface-layer
  sw_sl      =  input.sw_sl;            // surface layer switch
  ustar      =  input.ustar;            // surface friction velocity [m s-1]
  z0m        =  input.z0m;              // roughness length for momentum [m]
  z0h        =  input.z0h;              // roughness length for scalars [m]
  Cm         =  -1;                     // drag coefficient for momentum [-]
  Cs         =  -1;                     // drag coefficient for scalars [-]
  L          =  -1;                     // Obukhov length [-]
  Rib        =  -1;                     // bulk Richardson number [-]
  ra         =  -1;                     // aerodynamic resistance [s m-1]
  
  // radiation
  sw_rad     =  input.sw_rad;           // radiation switch
  lat        =  input.lat;              // latitude [deg]
  lon        =  input.lon;              // longitude [deg]
  doy        =  input.doy;              // day of the year [-]
  tstart     =  input.tstart;           // time of the day [h UTC]
  cc         =  input.cc;               // cloud cover fraction [-]
  Swin       =  -1;
  Swout      =  -1;
  Lwin       =  -1;
  Lwout      =  -1;
  Q          =  input.Q;                // net radiation [W m-2]
  
  // land surface
  sw_ls      =  input.sw_ls;            // land surface switch
  wg         =  input.wg;               // volumetric water content top soil layer [m3 m-3]
  w2         =  input.w2;               // volumetric water content deeper soil layer [m3 m-3]
  Tsoil      =  input.Tsoil;            // temperature top soil layer [K]
  T2         =  input.T2;               // temperature deeper soil layer [K]
  
  a          =  input.a;                // Clapp and Hornberger retention curve parameter a
  b          =  input.b;                // Clapp and Hornberger retention curve parameter b
  p          =  input.p;                // Clapp and Hornberger retention curve parameter p
  CGsat      =  input.CGsat;            // saturated soil conductivity for heat
  
  wsat       =  input.wsat;             // saturated volumetric water content ECMWF config [-]
  wfc        =  input.wfc;              // volumetric water content field capacity [-]
  wwilt      =  input.wwilt;            // volumetric water content wilting point [-]
  
  C1sat      =  input.C1sat;
  C2ref      =  input.C2ref;
  
  LAI        =  input.LAI;              // leaf area index [-]
  gD         =  input.gD;               // correction factor transpiration for VPD [-]
  rsmin      =  input.rsmin;            // minimum resistance transpiration [s m-1]
  rssoilmin  =  input.rssoilmin;        // minimum resistance soil evaporation [s m-1]
  alpha      =  input.alpha;            // surface albedo [-]

  rs         =  1e6;
  rssoil     =  1e6;
  
  Ts         =  input.Ts;               // initial surface temperature [K]
  
  cveg       =  input.cveg;             // vegetation fraction [-]
  Wmax       =  input.Wmax;             // thickness of water layer on wet vegetation [m]
  Wl         =  input.Wl;               // equivalent water layer depth for wet vegetation [m]
  cliq       =  -1;                     // wet fraction [-]
  
  Lambda     =  input.Lambda;           // thermal diffusivity skin layer [-]

  Tsoiltend  =  -1.;                    // soil temperature tendency [K s-1]
  wgtend     =  -1.;                    // soil moisture tendency [m3 m-3 s-1]
  Wltend     =  -1.;                    // equivalent liquid water tendency [m s-1]

  H          =  -1.;                    // sensible heat flux [W m-2]
  LE         =  -1.;                    // evapotranspiration [W m-2]
  LEliq      =  -1.;                    // open water evaporation [W m-2]
  LEveg      =  -1.;                    // transpiration [W m-2]
  LEsoil     =  -1.;                    // soil evaporation [W m-2]
  LEpot      =  -1.;                    // potential evaporation [W m-2]
  LEref      =  -1.;                    // reference evaporation using rs = rsmin / LAI [W m-2]
  G          =  -1.;                    // ground heat flux [W m-2]

  // initialize time variables
  tsteps = int(runtime / dt) + 1;
  t      = 0;

  if(sw_rad)
    runradmodel();

  if(sw_sl)
    // spin up surface layer, both Cs and L are unknown, iterate towards consistent solution
    for(int i = 0; i < 10; i++)
      runslmodel();

  if(sw_ls)
    runlsmodel();

  if(sw_ml)
    runmlmodel();

  // set output array to given value
  output = new modeloutput(tsteps);

  store();
  return;
} 


void model::runmodel()
{
  initmodel();

  for(t = 1; t < tsteps; t++)
  {
    if(sw_rad)
      runradmodel();

    if(sw_sl)
      runslmodel();

    if(sw_ls)
      runlsmodel();

    if(sw_ml)
      runmlmodel();

    if(sw_ls)
      intlsmodel();

    if(sw_ml)
      intmlmodel();

    store();
  }

  storeprof();

  return;
}

void model::runmlmodel()
{
  if(!sw_sl)
  {
    //uw       = - pow((pow(ustar, 4.) / (pow(v, 2.) / pow(u, 2.) + 1.)), 0.5);
    uw       = - sign(u) * pow((pow(ustar, 4.) / (pow(v, 2.) / pow(u, 2.) + 1.)), 0.5);
    //vw       = - pow((pow(ustar, 4.) / (pow(u, 2.) / pow(v, 2.) + 1.)), 0.5);
    vw       = - sign(v) * pow((pow(ustar, 4.) / (pow(u, 2.) / pow(v, 2.) + 1.)), 0.5);
  }

  if(sw_wtheta && (!sw_ls))
    wtheta = wtheta0 * std::sin(pi / sinperiod * t * dt);

  if(sw_wq && (!sw_ls))
    wq     = wq0 * std::sin(pi / sinperiod * t * dt);


  // compute mixed-layer tendencies
  // first compute necessary virtual temperature units
  thetav   = theta  + 0.61 * theta * q;
  wthetav  = wtheta + 0.61 * theta * wq;
  dthetav  = (theta + dtheta) * (1. + 0.61 * (q + dq)) - theta * (1. + 0.61 * q);

  // compute tendencies
  if(beta == 0 && dthetav == 0)
    we    = 1 / gammatheta * wthetav / h;
  else
    we    = (beta * wthetav) / dthetav;

  // compute entrainment fluxes
  wthetae = we * dtheta;
  wqe     = we * dq;

  // we     = (beta * wthetav + 5. * pow(ustar, 3.) * thetav / (g * h)) / dthetav;
  htend       = we + ws;

  thetatend   = (wtheta + wthetae) / h + advtheta;
  qtend       = (wq     + wqe)     / h + advq;

  dthetatend  = gammatheta * we - thetatend;
  dqtend      = gammaq     * we - qtend;

  // assume u + du = ug, so ug - u = du
  if(sw_wind)
  {
    uwe         = we * du;
    vwe         = we * dv;
    utend       = -fc * dv + (uw + uwe)  / h + advu;
    vtend       =  fc * du + (vw + vwe)  / h + advv;

    dutend      = gammau * we - utend;
    dvtend      = gammav * we - vtend;
  }

  // calculate LCL (Bolton (2008), The Computation of Equivalent Potential Temperature)
  double e       = q * Ps / 0.622;
  double Td      = 1. / ((1./273.15) - (Rv/Lv)*log(e/611.));
  double Tlcl    = 1. / ( (1./(Td - 56.0)) + (log(theta/Td)/800.)) + 56.;
  lcl            = 0. - (cp * (Tlcl - theta) / g);
}

void model::intmlmodel()
{
  double h0;
  double theta0, dtheta0, q0, dq0;
  double u0, du0, v0, dv0;

  // set values previous time step
  h0      = h;

  theta0  = theta;
  dtheta0 = dtheta;
  q0      = q;
  dq0     = dq;

  u0      = u;
  du0     = du;
  v0      = v;
  dv0     = dv;

  // integrate mixed-layer equations
  h        = h0      + dt * htend;

  theta    = theta0  + dt * thetatend;
  dtheta   = dtheta0 + dt * dthetatend;
  q        = q0      + dt * qtend;
  dq       = dq0     + dt * dqtend;

  if(sw_wind)
  {
    u        = u0      + dt * utend;
    du       = du0     + dt * dutend;
    v        = v0      + dt * vtend;
    dv       = dv0     + dt * dvtend;
  }
}

void model::runslmodel()
{
  double    zsl;      // height of surface layer [m]
  double    U;        // total wind speed [m s-1]
  double    esatsurf; // saturated vapor pressure inside vegetation [Pa]
  double    qsatsurf; // saturated specific humidity inside vegetation [kg kg-1]
  double    cq;       // fraction of surface that is wet

  U         = sqrt(pow(u,2.) + pow(v,2.));
  thetasurf = theta + wtheta / (Cs * U);
  esatsurf  = 0.611e3 * exp(17.2694 * (thetasurf - 273.16) / (thetasurf - 35.86));
  qsatsurf  = 0.622 * esatsurf / Ps;
  cq        = 1. / (1. + Cs * U * rs);
  qsurf     = (1. - cq) * q + cq * qsatsurf;

  thetavsurf = thetasurf * (1. + 0.61 * qsurf);
  thetav     = theta * (1. + 0.61 * q);
  wthetav    = wtheta + 0.61 * theta * wq;

  zsl  = 0.1 * h;

  Rib  = g / thetav * zsl * (thetav - thetavsurf) / (pow(u,2.) + pow(v,2.));
  Rib  = min(Rib, 0.2);

  L    = ribtol(Rib, zsl, z0m, z0h);

  Cm   = pow(k,2.) / pow((log(zsl / z0m) - psim(zsl / L) + psim(z0m / L)),2.);
  Cs   = pow(k,2.) / (log(zsl / z0m) - psim(zsl / L) + psim(z0m / L)) / (log(zsl / z0h) - psih(zsl / L) + psih(z0h / L));


  //if(wthetav > 0.):
  //  wstar     = (g / thetav * h * wthetav) ** (1./3.)
  //else:
  //  wstar     = 0.0001
  //ueff      = numpy.sqrt(u ** 2. + wstar ** 2.)

  ustar = sqrt(Cm) * U;
  uw    = - Cm * U * u;
  vw    = - Cm * U * v;

  // diagnostic meteorological variables
  T2m    = thetasurf - wtheta / ustar / k * (log(2. / z0h) - psih(2. / L) + psih(z0h / L));
  q2m    = qsurf     - wq     / ustar / k * (log(2. / z0h) - psih(2. / L) + psih(z0h / L));
  u2m    =           - uw     / ustar / k * (log(2. / z0m) - psim(2. / L) + psim(z0m / L));
  v2m    =           - vw     / ustar / k * (log(2. / z0m) - psim(2. / L) + psim(z0m / L));
  esat2m = 0.611e3 * exp(17.2694 * (T2m - 273.16) / (T2m - 35.86));
  e2m    = q2m * Ps / 0.622;
}

inline double model::psim(double zeta)
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

inline double model::psih(double zeta)
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


double model::ribtol(double Rib, double zsl, double z0m, double z0h)
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

void model::runradmodel()
{
  double  sda;      // solar declination angle [rad]
  double  sinlea;   // sinus of local declination angle [-]
  double  Ta;       // absolute temperature at top of surface layer [K]
  double  Tr;       // atmospheric transmissivity [-]

  sda    = 0.409 * cos(2. * pi * (doy - 173.) / 365.);
  sinlea = sin(2. * pi * lat / 360.) * sin(sda) - cos(2. * pi * lat / 360.) * cos(sda) * cos(2. * pi * (t * dt + tstart * 3600.) / 86400. - 2. * pi * lon / 360.);
  sinlea = max(sinlea, 0.0001);

  Ta  = theta * pow(((Ps - 0.1 * h * rho * g) / Ps ), Rd / cp);
  Tr  = (0.6 + 0.2 * sinlea) * (1. - 0.4 * cc);

  Swin  = S0 * Tr * sinlea;
  Swout = alpha * S0 * Tr * sinlea;
  Lwin  = 0.8 * bolz * pow(Ta, 4.);
  Lwout = bolz * pow(Ts, 4.);

  Q     = Swin - Swout + Lwin - Lwout;
}

void model::runlsmodel()
{
  double U;                 // total wind speed [m s-1]
  double desatdT;           // slope vapor pressure curve [Pa K-1]
  double esatsurf;          // surface vapor pressure [Pa]
  double f1, f2, f3, f4;    // Jarvis-Stewart correction functions [-]
  double Wlmx;              // Maximum layer of water stored on vegetation [m]
  double wgeq;              // equilibrium soil moisture content [m3 m-3]

  double CG,d1,C1,C2;       // force-restore parameters

  // compute ra
  U       = sqrt(pow(u,2.) + pow(v,2.));

  if(sw_sl)
    ra    = 1. / (Cm * U);
  else
    ra    = U / pow(ustar,2.);

  // first calculate essential thermodynamic variables
  esat    = 0.611e3 * exp(17.2694 * (theta - 273.16) / (theta - 35.86));
  qsat    = 0.622 * esat / Ps;
  desatdT = esat * (17.2694 / (theta - 35.86) - 17.2694 * (theta - 273.16) / pow(theta - 35.86,2.));
  dqsatdT = 0.622 * desatdT / Ps;
  e       = q * Ps / 0.622;

  // calculate surface resistances using Jarvis-Stewart model
  if(sw_rad)
    f1   = 1. / ((0.004 * Swin + 0.05) / (0.81 * (0.004 * Swin + 1.)));
  else
    f1   = 1.;

  if(w2 > wwilt)
    f2   = (wfc - wwilt) / (w2 - wwilt);
  else
    f2   = 1.e8;

  f3     = 1. / exp(- gD * (esat2m - e2m) / 100.);
  f4     = 1./ (1. - 0.0016 * pow(298.0 - T2m, 2.));

  rs     = rsmin / LAI * f1 * f2 * f3;

  // recompute f2 using wg instead of w2
  if(wg > wwilt)
    f2   = (wfc - wwilt) / (wg - wwilt);
  else
    f2   = 1.e8;

  rssoil = rssoilmin * f2;

  Wlmx   = LAI * Wmax;
  cliq   = min(1., Wl / Wlmx);

  // calculate skin temperature implictly
  Ts   = (Q  + rho * cp / ra * theta \
      + cveg * (1. - cliq) * rho * Lv / (ra + rs) * (dqsatdT * theta - qsat + q)       // transpiration
      + (1. - cveg) * rho * Lv / (ra + rssoil) * (dqsatdT * theta - qsat + q)          // bare soil evaporation
      + cveg * cliq * rho * Lv / ra * (dqsatdT * theta - qsat + q) + Lambda * Tsoil)   // liquid water evaporation
    / (rho * cp / ra + cveg * (1. - cliq) * rho * Lv / (ra + rs) * dqsatdT + (1. - cveg) * rho * Lv / (ra + rssoil) * dqsatdT + cveg * cliq * rho * Lv / ra * dqsatdT + Lambda);

  esatsurf  = 0.611e3 * exp(17.2694 * (Ts - 273.16) / (Ts - 35.86));
  qsatsurf  = 0.622 * esatsurf / Ps;

  LEveg  = (1. - cliq) * cveg * rho * Lv / (ra + rs) * (dqsatdT * (Ts - theta) + qsat - q);
  LEliq  = cliq * cveg * rho * Lv / ra * (dqsatdT * (Ts - theta) + qsat - q);
  LEsoil = (1. - cveg) * rho * Lv / (ra + rssoil) * (dqsatdT * (Ts - theta) + qsat - q);

  Wltend = - LEliq / (rhow * Lv);

  LE     = LEsoil + LEveg + LEliq;
  H      = rho * cp / ra * (Ts - theta);
  G      = Lambda * (Ts - Tsoil);
  LEpot  = (dqsatdT * (Q - G) + rho * cp / ra * (qsat - q)) / (dqsatdT + cp / Lv);
  LEref  = (dqsatdT * (Q - G) + rho * cp / ra * (qsat - q)) / (dqsatdT + cp / Lv * (1. + rsmin / LAI / ra));

  CG         = pow(CGsat * (wsat / w2), b / (2. * log(10.)));

  Tsoiltend  = CG * G - 2. * pi / 86400. * (Tsoil - T2);

  d1         = 0.1;
  C1         = pow(C1sat * (wsat / wg), b / 2. + 1.);
  C2         = C2ref * (w2 / (wsat - w2));
  wgeq       = w2 - wsat * a * ( pow(w2 / wsat, p) * (1. - pow(w2 / wsat,8.*p)) );
  wgtend     = - C1 / (rhow * d1) * LEsoil / Lv - C2 / 86400. * (wg - wgeq);

  // calculate kinematic heat fluxes
  wtheta     = H  / (rho * cp);
  wq         = LE / (rho * Lv);
}

void model::intlsmodel()
{
  double Tsoil0, wg0, Wl0;

  // integrate soil equations
  Tsoil0   = Tsoil;
  wg0      = wg;
  Wl0      = Wl;

  Tsoil    = Tsoil0  + dt * Tsoiltend;
  wg       = wg0     + dt * wgtend;
  Wl       = Wl0     + dt * Wltend;
}

void model::store()
{
  cout << "(t,h,LCL,theta,q,u,v) " << t * dt << ", " << h << ", " << lcl << ", " << theta << ", " << q*1000. << ", " << u << ", " << v << endl;
  output->t.data[t]          = t * dt / 3600.; // + tstart;

  output->h.data[t]          = h;
  output->Ps.data[t]         = Ps;
  output->ws.data[t]         = ws;
  output->beta.data[t]       = beta;
  output->lcl.data[t]        = lcl;

  // mixed-layer
  output->theta.data[t]      = theta;
  output->thetav.data[t]     = thetav;
  output->dtheta.data[t]     = dtheta;
  output->dthetav.data[t]    = dthetav;
  output->gammatheta.data[t] = gammatheta;
  output->advtheta.data[t]   = advtheta;
  output->wtheta.data[t]     = wtheta;
  output->wthetae.data[t]    = wthetae;
  output->wthetav.data[t]    = wthetav;
  
  output->q.data[t]          = q * 1000.;
  //output.qsat[t]       = qsat;
  //output.e[t]          = e;
  //output.esat[t]       = esat;
  output->dq.data[t]         = dq * 1000.;
  output->gammaq.data[t]     = gammaq * 1000.;
  output->advq.data[t]       = advq * 1000.;
  output->wq.data[t]         = wq * 1000.;
  output->wqe.data[t]        = wqe * 1000.;

  output->u.data[t]          = u;
  output->du.data[t]         = du;
  output->gammau.data[t]     = gammau;
  output->advu.data[t]       = advu;
  output->uw.data[t]         = uw;
  output->uwe.data[t]        = uwe;
  
  output->v.data[t]          = v;
  output->dv.data[t]         = dv;
  output->gammav.data[t]     = gammav;
  output->advv.data[t]       = advv;
  output->vw.data[t]         = vw;
  output->vwe.data[t]        = vwe;

  // surface layer
  output->ustar.data[t]      = ustar;
  output->L.data[t]          = L;
  output->Rib.data[t]        = Rib;
  output->ra.data[t]         = ra;
  output->Cm.data[t]         = Cm;
  output->Cs.data[t]         = Cs;

  // radiation
  output->Swin.data[t]       = Swin;
  output->Swout.data[t]      = Swout;
  output->Lwin.data[t]       = Lwin;
  output->Lwout.data[t]      = Lwout;
  output->Q.data[t]          = Q;

  // land and soil
  output->wg.data[t]         = wg;
  output->Tsoil.data[t]      = Tsoil;
  output->Ts.data[t]         = Ts;
  output->Wl.data[t]         = Wl;
  output->rs.data[t]         = rs;

  output->H.data[t]          = H;
  output->LE.data[t]         = LE;
  output->G.data[t]          = G;

  return;
} 

void model::storeprof()
{
  int nhours = (int)(runtime / 3600);
  
  for(int i=0; i < nhours; i++)
  {
    int starti = i * 4;
    int fetchi = (int)(3600 / dt * i);

    // even numbers
    if(i % 2 == 0)
    {
      output->thetaprof.data[starti + 0] = output->theta.data[fetchi];
      output->zprof.data[starti + 0] = 0;

      output->thetaprof.data[starti + 1] = output->theta.data[fetchi];
      output->zprof.data[starti + 1] = output->h.data[fetchi];

      output->thetaprof.data[starti + 2] = output->theta.data[fetchi] + output->dtheta.data[fetchi];
      output->zprof.data[starti + 2] = output->h.data[fetchi];

      output->thetaprof.data[starti + 3] = output->theta.data[fetchi] + output->dtheta.data[fetchi] + output->gammatheta.data[fetchi] * 1000.;
      output->zprof.data[starti + 3] = output->h.data[fetchi] + 1000.;
    }
    else
    {
      // odd numbers
      output->thetaprof.data[starti + 3] = output->theta.data[fetchi];
      output->zprof.data[starti + 3] = 0;

      output->thetaprof.data[starti + 2] = output->theta.data[fetchi];
      output->zprof.data[starti + 2] = output->h.data[fetchi];

      output->thetaprof.data[starti + 1] = output->theta.data[fetchi] + output->dtheta.data[fetchi];
      output->zprof.data[starti + 1] = output->h.data[fetchi];

      output->thetaprof.data[starti + 0] = output->theta.data[fetchi] + output->dtheta.data[fetchi] + output->gammatheta.data[fetchi] * 1000.;
      output->zprof.data[starti + 0] = output->h.data[fetchi] + 1000.;
    }
  }

  for(int i = nhours*4; i < tsteps*4; i++)
  {
    output->thetaprof.data[i]  = output->thetaprof.data[i-1];
    output->zprof.data[i]      = output->zprof.data[i-1];
  }
}

void model::run2file(std::string filedir, std::string filename)
{
  std::ofstream runsave;

  std::string fullpath = filedir + "/" + filename + ".csv";

  runsave.open(fullpath.c_str());

  std::cout << "Saving file " << fullpath.c_str() << std::endl;

  // Write header first
  runsave << output->t.name << " [" << output->t.unit << "],";
  runsave << output->h.name << " [" << output->h.unit << "]";
  runsave << std::endl;

  for(int nt=0; nt < tsteps; nt++)
  {
    runsave << output->t.data[nt] << ",";
    runsave << output->h.data[nt];
    runsave << std::endl;
  }

  runsave.close();

  return;
}
