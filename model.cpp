#include <cmath>
#include <iostream>
#include <fstream>
#include "model.h"

using namespace std;

inline double sign(double n) { return n > 0 ? 1 : (n < 0 ? -1 : 0);}

model::model(modelinput *extinput)
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

  // Aditions for A-Gs scheme
  mco2       =  46.;                    // molecular weight CO2 [g mol -1]
  mair       =  28.9;                   // molecular weight air [g mol -1]
  nuco2q     =  1.6;                    // ratio molecular viscosity water to carbon dioxide

  // Shallow-cumulus / variance calculations
  dz         =  150.;                   // Transition layer thickness [m]

  //input      =  extinput;
  input = *extinput;

  //input.reactions  =  new Reaction[input.rsize];
  //for(int i=0; i<input.rsize; i++)
  //  input.reactions[i] = extinput.reactions[i]; // CvH check is assignment operator needs to be overloaded
//
//  // mixed-layer
//  input.sw_ml      =  extinput.sw_ml;
//  input.h          =  extinput.h;                // initial ABL height [m]
//  input.Ps         =  extinput.Ps;               // surface pressure [Pa]
//  input.ws         =  extinput.ws;               // large scale vertical velocity [m s-1]
//  input.fc         =  extinput.fc;               // coriolis parameter [s-1]
//
//  input.theta      =  extinput.theta;            // initial mixed-layer potential temperature [K]
//  input.dtheta     =  extinput.dtheta;           // initial temperature jump at h [K]
//  input.gammatheta =  extinput.gammatheta;       // free atmosphere potential temperature lapse rate [K m-1]
//  input.advtheta   =  extinput.advtheta;         // advection of heat [K s-1]
//  input.beta       =  extinput.beta;             // entrainment ratio for virtual heat [-]
//  input.wtheta     =  extinput.wtheta;           // surface kinematic heat flux [K m s-1]
//
//  input.q          =  extinput.q;                // initial mixed-layer specific humidity [kg kg-1]
//  input.dq         =  extinput.dq;               // initial specific humidity jump at h [kg kg-1]
//  input.gammaq     =  extinput.gammaq;           // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
//  input.advq       =  extinput.advq;             // advection of moisture [kg kg-1 s-1]
//  input.wq         =  extinput.wq;               // surface kinematic moisture flux [kg kg-1 m s-1]
//
//  input.sw_wind    =  extinput.sw_wind;          // prognostic wind switch
//  input.u          =  extinput.u;                // initial mixed-layer u-wind speed [m s-1]
//  input.du         =  extinput.du;               // initial u-wind jump at h [m s-1]
//  input.gammau     =  extinput.gammau;           // free atmosphere u-wind speed lapse rate [s-1]
//  input.advu       =  extinput.advu;             // advection of u-wind [m s-2]
//
//  input.v          =  extinput.v;                // initial mixed-layer u-wind speed [m s-1]
//  input.dv         =  extinput.dv;               // initial u-wind jump at h [m s-1]
//  input.gammav     =  extinput.gammav;           // free atmosphere v-wind speed lapse rate [s-1]
//  input.advv       =  extinput.advv;             // advection of v-wind [m s-2]
//
//  // surface-layer
//  input.sw_sl      =  extinput.sw_sl;      // surface layer switch
//  input.ustar      =  extinput.ustar;      // surface friction velocity [m s-1]
//  input.z0m        =  extinput.z0m;        // roughness length for momentum [m]
//  input.z0h        =  extinput.z0h;        // roughness length for scalars [m]
//
//  // radiation
//  input.sw_rad     =  extinput.sw_rad;     // radiation switch
//  input.lat        =  extinput.lat;        // latitude [deg]
//  input.lon        =  extinput.lon;        // longitude [deg]
//  input.doy        =  extinput.doy;        // day of the year [-]
//  input.tstart     =  extinput.tstart;     // time of the day [h UTC]
//  input.cc         =  extinput.cc;         // cloud cover fraction [-]
//  input.Q          =  extinput.Q;          // net radiation [-]
//
//  // land surface
//  input.sw_ls      =  extinput.sw_ls;      // land surface switch
//  input.wg         =  extinput.wg;         // volumetric water content top soil layer [m3 m-3]
//  input.w2         =  extinput.w2;         // volumetric water content deeper soil layer [m3 m-3]
//  input.Tsoil      =  extinput.Tsoil;      // temperature top soil layer [K]
//  input.T2         =  extinput.T2;         // temperature deeper soil layer [K]
//
//  input.a          =  extinput.a;          // Clapp and Hornberger retention curve parameter a
//  input.b          =  extinput.b;          // Clapp and Hornberger retention curve parameter b
//  input.p          =  extinput.p;          // Clapp and Hornberger retention curve parameter p
//  input.CGsat      =  extinput.CGsat;      // saturated soil conductivity for heat
//
//  input.wsat       =  extinput.wsat;       // saturated volumetric water content ECMWF config [-]
//  input.wfc        =  extinput.wfc;        // volumetric water content field capacity [-]
//  input.wwilt      =  extinput.wwilt;      // volumetric water content wilting point [-]
//
//  input.C1sat      =  extinput.C1sat;
//  input.C2ref      =  extinput.C2ref;
//
//  input.LAI        =  extinput.LAI;        // leaf area index [-]
//  input.gD         =  extinput.gD;         // correction factor transpiration for VPD [-]
//  input.rsmin      =  extinput.rsmin;      // minimum resistance transpiration [s m-1]
//  input.rssoilmin  =  extinput.rssoilmin;  // minimum resistance soil evaporation [s m-1]
//  input.alpha      =  extinput.alpha;      // surface albedo [-]
//
//  input.Ts         =  extinput.Ts;         // initial surface temperature [K]
//
//  input.cveg       =  extinput.cveg;       // vegetation fraction [-]
//  input.Wmax       =  extinput.Wmax;       // thickness of water layer on wet vegetation [m]
//  input.Wl         =  extinput.Wl;         // equivalent water layer depth for wet vegetation [m]
//
//  input.Lambda     =  extinput.Lambda;     // thermal diffusivity skin layer [-]

  // set output flag to false;
  hasoutput = false;

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
  sw_wsft    =  input.sw_ftcws;
  sw_shearwe =  input.sw_shearwe;
  h          =  input.h;                // initial ABL height [m]
  Ps         =  input.Ps;               // surface pressure [Pa]
  omegas     =  input.omegas;           // large scale vertical velocity [m s-1]
  ws         =  -1.;
  wf         =  0.;                     // mixed-layer growth due to dFz [m s-1]
  wstar      =  0.;                     // Deardorff vertical velocity scale [m s-1]
  fc         =  input.fc;               // coriolis parameter [s-1]

  theta      =  input.theta;            // initial mixed-layer potential temperature [K]
  dtheta     =  input.dtheta;           // initial temperature jump at h [K]
  gammatheta =  input.gammatheta;       // free atmosphere potential temperature lapse rate [K m-1]
  advtheta   =  input.advtheta;         // advection of heat [K s-1]
  beta       =  input.beta;             // entrainment ratio for virtual heat [-]
  wtheta     =  input.wtheta;           // surface kinematic heat flux [K m s-1]
  wtheta0    =  input.wtheta;           // maximum surface kinematic heat flux [K m s-1]
  wthetaM    =  0.;                     // mass-flux kinematic heat flux [K m s-1]
  sigmatheta2 =  0.;                    // mixed-layer top potential temperature variance [kg2 kg-2]
  sw_wtheta  =  input.sw_wtheta;

  thetasurf  =  input.theta;            // surface potential temperature [K]

  q          =  input.q;                // initial mixed-layer specific humidity [kg kg-1]
  dq         =  input.dq;               // initial specific humidity jump at h [kg kg-1]
  gammaq     =  input.gammaq;           // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq       =  input.advq;             // advection of moisture [kg kg-1 s-1]
  wq         =  input.wq;               // surface kinematic moisture flux [kg kg-1 m s-1]
  wq0        =  input.wq;               // maximum surface kinematic moisture flux [kg kg-1 m s-1]
  wqM        =  0.;                     // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  sigmaq2    =  0.;                     // mixed-layer top specific humidity variance [kg2 kg-2]
  sw_wq      =  input.sw_wq;

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

  // BvS; a scalar, without the need for the chemistry scheme :)
  sca        = input.sca;               // initial mixed-layer scalar [kg kg-1]
  dsca       = input.dsca;              // initial scalar jump at h [kg kg-1]
  gammasca   = input.gammasca;          // free atmosphere scalar lapse rate [kg kg-1 m-1]
  advsca     = input.advsca;            // advection of scalar [kg kg-1 s-1]
  wsca       = input.wsca;              // surface kinematic scalar flux [kg kg-1 m s-1]

  CO2        = input.CO2;               // initial mixed-layer CO2 [ppm]
  dCO2       = input.dCO2;              // initial CO2 jump at h [ppm]
  gammaCO2   = input.gammaCO2;          // free atmosphere CO2 lapse rate [ppm]
  advCO2     = input.advCO2;            // advection of CO2 [ppm]
  wCO2       = input.wCO2;              // surface kinematic CO2 flux [ppm]

  // Other scalars/species (chemistry code..)
  nsc        =  input.nsc;
  sc         =  new double[nsc];
  dsc        =  new double[nsc];
  gammasc    =  new double[nsc];
  advsc      =  new double[nsc];
  wsc        =  new double[nsc];
  wscM       =  new double[nsc];
  sigmasc2   =  new double[nsc];
  sw_wsc     =  new int[nsc];

  sctend     =  new double[nsc];
  dsctend    =  new double[nsc];
  wsc0       =  new double[nsc];
  wsce       =  new double[nsc];

  for(int i=0; i<nsc; i++)
  {
    sc[i]       =  input.sc[i];
    dsc[i]      =  input.dsc[i];
    gammasc[i]  =  input.gammasc[i];
    advsc[i]    =  input.advsc[i];
    wsc[i]      =  input.wsc[i];
    wsc0[i]     =  input.wsc[i];
    wscM[i]     =  0.;
    sigmasc2[i] =  0.;

    sw_wsc[i]   =  input.sw_wsc[i];
    //cout << i << ", " << sc[i] << ", " << dsc[i] << ", " << wsc0[i] << endl;
  }

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
  sw_jarvis  =  input.sw_jarvis;        // Jarvis / A-Gs switch
  C3C4       =  input.C3C4;             // C3 or C4 vegetation
  sw_sea     =  input.sw_sea;           // land / sea switch
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

  // initialize plant physilogical model (A-gs)
  ci         =  -1.;                    // CO2 concentration inside the leaf [mg m-3]
  cfrac      =  -1.;                    // CO2 concentration fraction [-]
  Ds         =  -1.;                    // vapor pressure deficit [kPa]
  D0         =  -1.;                    // vapor pressure deficit stomata closes [kPa]
  gm         =  -1.;                    // mesophyll conducatnce [mm s-1]
  fmin       =  -1.;                    // minimum value cfrac [-]
  fmin0      =  -1.;                    // function to calculate fmin [-]
  Ammax      =  -1.;                    // CO2 maximal primary productivity [mg m-2 s-1]
  Am         =  -1.;                    // CO2 primray productivity [mg m-2 s-1]
  An         =  -1.;                    // net CO2 flow into the plant [mg m-2 s-1]
  Rdark      =  -1.;                    // CO2 dark respiration [mg m-2 s-1]
  PAR        =  -1.;                    // Photosyntetically Active Radiation [W m-2]
  gcCo2      =  -1.;                    // CO2 conductance at canopy level [mm s-1]
  // BvS, calculat rs either with Jarvis or A-Gs...
  // rsAgs      =  -1.;                 // surface resistance moisture [s mm-1]
  rsCO2      =  -1.;                    // surface resistance carbon dioxide [s mm-1]
  betaw      =  -1.;                    // function depending on soil moisture content to calculate stress function [-]
  fstr       =  -1.;                    // stress function included in canopy conductance [-]

  // initialize constants depending C3 or C4 plants
  // C3 plants:
  CO2comp298[0] =  68.5;                   // CO2 compensation concentration [mg m-3]
  Q10CO2[0]     =  1.5;                    // function parameter to calculate CO2 compensation concentration [-]
  gm298[0]      =  7.0;                    // mesophyill conductance at 298 K [mm s-1]
  Ammax298[0]   =  2.2;                    // CO2 maximal primary productivity [mg m-2 s-1]
  Q10gm[0]      =  2.0;                    // function parameter to calculate mesophyll conductance [-]
  T1gm[0]       =  278.;                   // reference temperature to calculate mesophyll conductance gm [K]
  T2gm[0]       =  301.;                   // reference temperature to calculate mesophyll conductance gm [K]
  Q10Am[0]      =  2.0;                    // function parameter to calculate maximal primary profuctivity Ammax
  T1Am[0]       =  281.;                   // reference temperature to calculate maximal primary profuctivity Ammax [K]
  T2Am[0]       =  311.;                   // reference temperature to calculate maximal primary profuctivity Ammax [K]
  f0[0]         =  0.89;                   // maximum value Cfrac [-]
  ad[0]         =  0.07;                   // regression coefficient to calculate Cfrac [kPa-1]
  alpha0[0]     =  0.017;                  // initial low light conditions [mg J-1]
  Kx[0]         =  0.7;                    // extinction coefficient PAR [-]
  gmin[0]       =  0.25e-3;                // cuticular (minimum) conductance [mm s-1]
  // C4 plants:
  CO2comp298[1] =  4.3;                    // CO2 compensation concentration [mg m-3]
  Q10CO2[1]     =  1.5;                    // function parameter to calculate CO2 compensation concentration [-]
  gm298[1]      =  17.5;                   // mesophyill conductance at 298 K [mm s-1]
  Ammax298[1]   =  1.7;                    // CO2 maximal primary productivity [mg m-2 s-1]
  Q10gm[1]      =  2.0;                    // function parameter to calculate mesophyll conductance [-]
  T1gm[1]       =  286.;                   // reference temperature to calculate mesophyll conductance gm [K]
  T2gm[1]       =  309.;                   // reference temperature to calculate mesophyll conductance gm [K]
  Q10Am[1]      =  2.0;                    // function parameter to calculate maximal primary profuctivity Ammax
  T1Am[1]       =  286.;                   // reference temperature to calculate maximal primary profuctivity Ammax [K]
  T2Am[1]       =  311.;                   // reference temperature to calculate maximal primary profuctivity Ammax [K]
  f0[1]         =  0.85;                   // maximum value Cfrac [-]
  ad[1]         =  0.015;                   // regression coefficient to calculate Cfrac [kPa-1]
  alpha0[1]     =  0.014;                  // initial low light conditions [mg J-1]
  Kx[1]         =  0.7;                    // extinction coefficient PAR [-]
  gmin[1]       =  0.25e-3;                // cuticular (minimum) conductance [mm s-1]

  // initialize soil  -1. ration model (coupled to A-gs)
  Cw         =  0.0016;                 // constant water stress correction (eq. 13 Jacobs et al. 2007) [-]
  wmax       =  0.55;                   // upper reference value soil water [-]
  wmin       =  0.005;                  // lower reference value soil water [-]
  R10        =  0.23;                   // respiration at 10 C [mg CO2 m-2 s-1]
  E0         =  53.3e3;                 // activation energy [53.3 kJ kmol-1]
  // END HARD-CODED

  // initialize soil  -1. ration model (coupled to A-gs)
  fw         =  -1.;                    // water stress correction function [-]
  Resp       =  -1.;                    // soil surface carbon dioxide flux [mg m-2 s-1]

  // shallow-cumulus
  sw_cu      = input.sw_cu;             // shallow-cumulus switch [-]
  ac         =  0.;                     // cloud core fraction [-]
  M          =  0.;                     // mass-flux (/rho) [m s-1]

  // stratocumulus
  dFz        = input.dFz;               // cloud-top radiative divergence [W m-2]

  // chemistry
  sw_chem           = input.sw_chem;
  sw_chem_constant  = input.sw_chem_constant;
  sw_photo_constant = input.sw_photo_constant;
  rsize      =  input.rsize;
  csize      =  input.csize;
  sw_chemoutput = new bool[csize];
  reactions  =  input.reactions; // CvH forward address to reaction array
  sw_reactions = new bool[rsize];
  for(int i=0; i<rsize; i++)
    sw_reactions[i] = input.sw_reactions[i];

  P_ref      =  input.P_ref;
  Tcbl_ref   =  input.Tcbl_ref;
  Tfc_ref    =  input.Tfc_ref;
  qcbl_ref   =  input.qcbl_ref;
  qfc_ref    =  input.qfc_ref;
  tod_ref    =  input.tod_ref;
  stocoef    =  input.stocoef;

  //for(int i=0; i<input.rsize; i++)
  //  reactions[i] = input.reactions[i];


  // initialize time variables
  tsteps = int(runtime / dt) + 1;
  t      = 0;

  // CvH initialize chemistry
  if(sw_chem)
    initchemmodel();

  if(sw_rad)
    runradmodel();

  if(sw_sl)
    // spin up surface layer, both Cs and L are unknown, iterate towards consistent solution
    for(int i = 0; i < 10; i++)
      runslmodel();

  if(sw_ls)
    runlsmodel();

  // BvS shallow-cumulus
  if(sw_cu){
   runmlmodel();
   runcumodel();
  }

  if(sw_ml)
    runmlmodel();

  // initialize chemistry using a very small time step value
  if(sw_chem)
    runchemmodel(dt / 1000.);

  if(hasoutput)
  {
    output->reset(nsc);
    output->reload(tsteps, nsc);
  }
  else
  {
    output    = new modeloutput(tsteps, nsc);
    hasoutput = true;
  }

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

    // BvS shallow-cumulus
    if(sw_cu)
      runcumodel();

    if(sw_ml)
      runmlmodel();

    if(sw_ls)
      intlsmodel();

    if(sw_ml)
      intmlmodel();

    if(sw_chem)
      runchemmodel(dt);

    store();
  }

  return;
}

void model::runcumodel()
{
  // Virtual temperature units
  thetav           = theta  + 0.61 * theta * q;
  wthetav          = wtheta + 0.61 * theta * wq;

  // Mixed-layer top properties
  double Ptop    = Ps / exp((g * h)/(Rd * theta));
  double Ttop    = theta / pow(Ps / Ptop,Rd / cp);
  double esattop = 0.611e3 * exp((Lv / Rv) * ((1. / 273.15)-(1. / Ttop)));
  double qsattop  = 0.622 * esattop / Ptop;

  ac               = 0.5 + (0.36 * atan(1.55 * ((q - qsattop) / pow(sigmaq2,0.5))));
  if (ac < 0.)
    ac = 0.;

  M                = ac * wstar;
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

  if(wthetav > 0.)
    wstar  = pow((g * h * wthetav) / thetav,(1./3.));
  else
    wstar  = 1e-6;

  int inputdthetav  = (input.theta + input.dtheta) * (1. + 0.61 * (input.q + input.dq)) - input.theta * (1. + 0.61 * input.q);

  // compute large scale vertical velocity
  ws = -omegas * h;
  // mixed-layer growth due to cloud top radiative divergence
  wf = dFz / (rho * cp * dtheta);

  // Compensate free tropospheric warming due to subsidence
  double C_thetaft, C_qft, C_scaft, C_CO2ft;
  if(sw_wsft)
  {
    C_thetaft = gammatheta    * ws;
    C_qft     = gammaq        * ws;
    C_scaft   = gammasca      * ws;
    C_CO2ft   = gammaCO2      * ws;
  }
  else
  {
    C_thetaft = 0.;
    C_qft     = 0.;
    C_scaft   = 0.;
    C_CO2ft   = 0.;
  }

  // compute tendencies
  if(beta == 0 && inputdthetav == 0)
    we    = 1 / gammatheta * wthetav / h;
  else
  {
    if(sw_shearwe)
      we     = (beta * wthetav + 5. * pow(ustar, 3.) * thetav / (g * h)) / dthetav;
    else
      we     = (beta * wthetav) / dthetav;
  }

  // compute entrainment fluxes
  wthetae = we * dtheta;
  wqe     = we * dq;
  wscae   = we * dsca;
  wCO2e   = we * dCO2;

  // compute mixed-layer top variances and mass-fluxes
  sigmaq2     = wqe     * dq     * h / (dz * wstar);
  sigmatheta2 = wthetae * dtheta * h / (dz * wstar);
  sigmasca2   = wscae   * dsca   * h / (dz * wstar);
  sigmaCO22   = wCO2e   * dCO2   * h / (dz * wstar);

  // Check if variances < 0.
  if (sigmaq2 < 0.)
    sigmaq2 = 1e-5;
  if (sigmatheta2 < 0.)
    sigmatheta2 = 1e-5;
  if (sigmasca2 < 0.)
    sigmasca2 = 1e-5;
  if (sigmaCO22 < 0.)
    sigmaCO22 = 1e-5;

  // Mass-flux kinematic fluxes
  wqM         = M * pow(sigmaq2,0.5);
  wthetaM     = M * pow(sigmatheta2,0.5);
  wscaM       = M * pow(sigmasca2,0.5);
  wCO2M       = M * pow(sigmaCO22,0.5);

  // we     = (beta * wthetav + 5. * pow(ustar, 3.) * thetav / (g * h)) / dthetav;
  htend       = we + ws + wf - M;

  thetatend   = (wtheta + wthetae - wthetaM)  / h + advtheta;
  qtend       = (wq     + wqe     - wqM)      / h + advq;
  scatend     = (wsca   + wscae   - wscaM)    / h + advsca;
  CO2tend     = (wCO2   + wCO2e   - wCO2M)    / h + advCO2;

  // Set tendency dtheta & dq to zero when shallow-cumulus is present
  if(sw_cu && ac > 0.){
    dthetatend  = 0.;
    dqtend      = 0.;
    dscatend    = 0.;
    dCO2tend    = 0.;
  }
  else {
    dthetatend  = gammatheta * (we + wf) - thetatend  + C_thetaft;
    dqtend      = gammaq     * (we + wf) - qtend      + C_qft;
    dscatend    = gammasca   * (we + wf) - scatend    + C_scaft;
    dCO2tend    = gammaCO2   * (we + wf) - CO2tend    + C_CO2ft;
  }

  for(int i=0; i<nsc; i++)
  {
    if(sw_wsc[i] == 1)
      wsc[i] = wsc0[i] * std::sin(pi / sinperiod * t * dt);
    else if(sw_wsc[i] == 2)
    {
      double sda       = 0.409 * cos(2. * pi * (doy - 173.) / 365.);
      double sinleamax = sin(2. * pi * lat / 360.) * sin(sda) + cos(2. * pi * lat / 360.) * cos(sda);
      double sinlea    = sin(2. * pi * lat / 360.) * sin(sda) - cos(2. * pi * lat / 360.) * cos(sda) * cos(2. * pi * (t * dt + tstart * 3600.) / 86400. - 2. * pi * lon / 360.);
      sinlea  = max(sinlea, 0.);
      wsc[i]  = wsc0[i] * sinlea / sinleamax;
    }

    wsce[i]        = we * dsc[i];

    sigmasc2[i]    = wsce[i] * dsc[i] * h / (dz * wstar);
    if (sigmasc2[i] < 0.)
      sigmasc2[i] = 1e-5;

    wscM[i]        = M * pow(sigmasc2[i],0.5);

    sctend[i]      = (wsc[i] + wsce[i] - wscM[i]) / h + advsc[i];
    dsctend[i]     = gammasc[i] * we - sctend[i];
  }

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

  // LCL (Bolton (2008), The Computation of Equivalent Potential Temperature)
  double e       = q * Ps / 0.622;
  double Td      = 1. / ((1./273.15) - (Rv/Lv)*log(e/611.));
  double Tlcl    = 1. / ( (1./(Td - 56.0)) + (log(theta/Td)/800.)) + 56.;
  lcl            = 0. - (cp * (Tlcl - theta) / g);

  // RH evaluated at T = theta
  double esat    = 0.611e3 * exp(17.2694 * (theta - 273.16) / (theta - 35.86));
  RH             = e / esat;

  // RH at mixed-layer top
  double Ptop    = Ps / exp((g * h)/(Rd * theta));
  double Ttop    = theta / pow(Ps / Ptop,Rd / cp);
  double esattop = 0.611e3 * exp((Lv / Rv) * ((1. / 273.15)-(1. / Ttop)));
  double etop    = q * Ptop / 0.622;
  RHtop          = etop / esattop;
}

void model::intmlmodel()
{
  double h0;
  double theta0, dtheta0, q0, dq0, sca0, dsca0, CO20, dCO20;
  double u0, du0, v0, dv0;
  double *sc0, *dsc0;

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

  sca0    = sca;
  dsca0   = dsca;
  CO20    = CO2;
  dCO20   = dCO2;

  sc0  = new double[nsc];
  dsc0 = new double[nsc];
  for(int i=0; i<nsc; i++)
  {
    sc0[i]  = sc[i];
    dsc0[i] = dsc[i];
  }

  // integrate mixed-layer equations
  h        = h0      + dt * htend;

  theta    = theta0  + dt * thetatend;
  dtheta   = dtheta0 + dt * dthetatend;
  q        = q0      + dt * qtend;
  dq       = dq0     + dt * dqtend;
  sca      = sca0    + dt * scatend;
  dsca     = dsca0   + dt * dscatend;
  CO2      = CO20    + dt * CO2tend;
  dCO2     = dCO20   + dt * dCO2tend;

  for(int i=0; i<nsc; i++)
  {
    sc[i]    = sc0[i]  + dt * sctend[i];
    dsc[i]   = dsc0[i] + dt * dsctend[i];
  }

  if(sw_wind)
  {
    u        = u0      + dt * utend;
    du       = du0     + dt * dutend;
    v        = v0      + dt * vtend;
    dv       = dv0     + dt * dvtend;
  }

  delete[] sc0;
  delete[] dsc0;
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

  while (abs((L - L0) / L0) > 0.001)
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

double model::factorial(int k)
{
  double factorial = 1;
  for(int n = 2;n<k+1;n++)
    factorial = factorial * n;

  return factorial;
}

double model::E1(double x)
{
  double E1sum = 0;
  for(int k=1;k<100;k++)
    E1sum = E1sum + pow((-1.),(k + 0.0)) * pow(x,(k + 0.0)) / ((k + 0.0) * factorial(k));

  double E1 = -0.57721566490153286060 - log(x) - E1sum;
  return E1;
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

  if(sw_sea)
  {
    rs = 0.;

    esat    = 0.611e3 * exp(17.2694 * (theta - 273.16) / (theta - 35.86));
    qsat    = 0.622 * esat / Ps;
    esatsurf  = 0.611e3 * exp(17.2694 * (Ts - 273.16) / (Ts - 35.86));
    qsatsurf  = 0.622 * esatsurf / Ps;

    LEveg  = 0.;
    LEliq  = rho * Lv / ra * (qsatsurf - q);
    LEsoil = 0.;

    LE     = LEsoil + LEveg + LEliq;
    H      = rho * cp / ra * (Ts - theta);
    LEpot  = LE;
  }
  else
  {
    // first calculate essential thermodynamic variables
    esat    = 0.611e3 * exp(17.2694 * (theta - 273.16) / (theta - 35.86));
    qsat    = 0.622 * esat / Ps;
    desatdT = esat * (17.2694 / (theta - 35.86) - 17.2694 * (theta - 273.16) / pow(theta - 35.86,2.));
    dqsatdT = 0.622 * desatdT / Ps;
    e       = q * Ps / 0.622;

    if(sw_jarvis)       // calculate surface resistances using Jarvis-Stewart model
    {
      if(sw_rad)
        f1   = 1. / ((0.004 * Swin + 0.05) / (0.81 * (0.004 * Swin + 1.)));
      else
        f1   = 1.;

      if(w2 > wwilt)
        f2   = (wfc - wwilt) / (w2 - wwilt);
      else
        f2   = 1.e8;

      //f3     = 1. / exp(- gD * (esat2m - e2m) / 100.);
      //f4     = 1./ (1. - 0.0016 * pow(298.0 - T2m, 2.));
      f3     = 1. / exp(- gD * (esat - e) / 100.);
      f4     = 1./ (1. - 0.0016 * pow(298.0 - theta, 2.));

      rs     = rsmin / LAI * f1 * f2 * f3 * f4;
    }

    else    // calculate surface resistances using plant physiological (A-gs) model
    {
      int c = C3C4;                 // Picks C3 of C4 constants from array
      double CO2comp, co2abs, alphac, Ag, y, a1, Dstar, gcco2;

      // calculate CO2 compensation concentration
      CO2comp = CO2comp298[c] * rho * pow(Q10CO2[c],(0.1 * (thetasurf - 298.)));   // CO2 compensation concentration

      // calculate mesophyll conductance
      gm            = gm298[c] *  pow(Q10gm[c],( 0.1 * (thetasurf - 298.))) / ( (1. + exp(0.3 * (T1gm[c] - thetasurf))) * (1. + exp(0.3 * (thetasurf - T2gm[c])))); // mesophyill conductance
      gm            = gm / 1000.;   //conversion from mm s-1 to m s-1

      // calculate CO2 concentration inside the leaf (ci)
      fmin0         = gmin[c] / nuco2q - 1. / 9. * gm;
      fmin          = -fmin0 + pow((pow(fmin0,2.) + 4 * gmin[c]/nuco2q * gm ),0.5) / (2. * gm);

      esatsurf      = 0.611e3 * exp(17.2694 * (Ts - 273.16) / (Ts - 35.86));
      Ds            = (esatsurf - e)   / 1000.;    // in kPa
      D0            = (f0[c] - fmin) / ad[c];

      cfrac         = f0[c] * (1. - (Ds / D0)) + fmin * (Ds / D0);
      co2abs        = CO2 * (mco2 / mair) * rho;                                                   // conversion mumol mol-1 (ppm) to mgCO2 m3
      ci            = cfrac * (co2abs - CO2comp) + CO2comp;

      // calculate maximal gross primary production in high light conditions (Ag)
      Ammax         = Ammax298[c] *  pow(Q10Am[c],(0.1 * (thetasurf - 298.))) / ( (1. + exp(0.3 * (T1Am[c] - thetasurf))) * (1. + exp(0.3 * (thetasurf - T2Am[c]))));

      // calculate effect of soil moisture stress on gross assimilation rate
      betaw         = max(1e-3, min(1.,(wg - wwilt)/(wfc - wwilt)));

      // calculate stress function
      fstr          = betaw;

      // calculate gross assimilation rate (Am)
      Am           = Ammax * (1. - exp(-(gm * (ci - CO2comp) / Ammax)));

      Rdark        = (1. / 9.) * Am;

      PAR          = 0.5 * max(1e-1,Swin * cveg);

      // calculate  light use efficiency
      alphac       = alpha0[c] * (co2abs - CO2comp) / (co2abs + 2. * CO2comp);

      // calculate gross primary productivity
      Ag           = (Am + Rdark) * (1 - exp(alphac * PAR / (Am + Rdark)));

      // 1.- calculate upscaling from leaf to canopy: net flow CO2 into the plant (An)
      y            =  alphac * Kx[c] * PAR / (Am + Rdark);
      An           = (Am + Rdark) * (1. - 1. / (Kx[c] * LAI) * (E1( y * exp(-Kx[c] * LAI)) - E1(y)));

      // 2.- calculate upscaling from leaf to canopy: CO2 conductance at canopy level
      a1           = 1. / (1. - f0[c]);
      Dstar        = D0 / (a1 - 1.);

      gcco2        = LAI * (gmin[c] / nuco2q + a1 * fstr * An / ((co2abs - CO2comp) * (1. + Ds / Dstar)));

      // calculate surface resistance for moisture and carbon dioxide
      rs           = 1. / (1.6 * gcco2);
      rsCO2        = 1. / gcco2;

      // calculate net flux of CO2 into the plant (An)
      An           = -(co2abs - ci) / (ra + rs);

      // CO2 soil surface flux
      fw           = Cw * wmax / (wg + wmin);
      Resp         = R10 * (1. - fw) * exp(E0 / (283.15 * 8.314) * (1. - 283.15 / (thetasurf)));

      // CO2 flux
      awco2        = (An + Resp);                      // conversion mgCO2 m3 to mumol mol-1 (ppm)
      wCO2         = (An + Resp); // * (mair / mco2) * (1. / rho);   // conversion mgCO2 m3 to mumol mol-1 (ppm)
    }

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
  }

  // calculate kinematic heat fluxes
  wtheta     = H  / (rho * cp);
  wq         = LE / (rho * Lv);
}

void model::intlsmodel()
{
  double Tsoil0, wg0, Wl0;

  if(!sw_sea)
  {
    // integrate soil equations
    Tsoil0   = Tsoil;
    wg0      = wg;
    Wl0      = Wl;

    Tsoil    = Tsoil0  + dt * Tsoiltend;
    wg       = wg0     + dt * wgtend;
    Wl       = Wl0     + dt * Wltend;
  }
}

void model::store()
{
  //cout << "(t,h,LCL,theta,q,u,v,CO2) " << t * dt << ", " << h << ", " << lcl << ", " << theta << ", " << q*1000. << ", " << u << ", " << v << ", " << CO2 << endl;
  //cout << "(t,sc0,sc1,sc3,sc9)   " << t * dt << ", " << sc[0] << ", " << sc[1] << ", " << sc[3] << ", " << sc[9] << endl;
  //cout << "(sigmaq,ac,M,wqM)" << pow(sigmaq2,0.5)*1000. << ac << M << wqM << endl;
  output->t.data[t]          = t * dt / 3600.; // + tstart;
  output->tutc.data[t]       = t * dt / 3600. + tstart;
  output->h.data[t]          = h;
  output->Ps.data[t]         = Ps;
  output->ws.data[t]         = ws;
  output->beta.data[t]       = beta;
  output->lcl.data[t]        = lcl;
  output->RH.data[t]         = RH;
  output->RHtop.data[t]      = RHtop;

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
  output->we.data[t]         = we;
  output->sigmatheta.data[t] = pow(sigmatheta2,0.5);
  output->wthetaM.data[t]    = wthetaM;
  output->RH.data[t]         = RH;
  output->RHtop.data[t]      = RHtop;

  output->q.data[t]          = q * 1000.;
  //output.qsat[t]       = qsat;
  //output.e[t]          = e;
  //output.esat[t]       = esat;
  output->dq.data[t]         = dq * 1000.;
  output->gammaq.data[t]     = gammaq * 1000.;
  output->advq.data[t]       = advq * 1000.;
  output->wq.data[t]         = wq * 1000.;
  output->wqe.data[t]        = wqe * 1000.;
  output->sigmaq.data[t]     = pow(sigmaq2,0.5)*1000.;
  output->wqM.data[t]        = wqM * 1000.;

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

  // BvS; a scalar...
  output->sca.data[t]             = sca;
  output->dsca.data[t]            = dsca;
  output->gammasca.data[t]        = gammasca;
  output->advsca.data[t]          = advsca;
  output->wsca.data[t]            = wsca;
  output->wscae.data[t]           = wscae;
  output->wscaM.data[t]           = wscaM;
  output->sigmasca.data[t]        = pow(sigmasca2,0.5);

  output->CO2.data[t]             = CO2;
  output->dCO2.data[t]            = dCO2;
  output->gammaCO2.data[t]        = gammaCO2;
  output->advCO2.data[t]          = advCO2;
  output->wCO2.data[t]            = awco2; //wCO2;
  output->wCO2A.data[t]           = An; //   * (mair / mco2) * (1. / rho);
  output->wCO2R.data[t]           = Resp; // * (mair / mco2) * (1. / rho);
  output->wCO2e.data[t]           = wCO2e;
  output->wCO2M.data[t]           = wCO2M;
  output->sigmaCO2.data[t]        = pow(sigmaCO22,0.5);


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

  // shallow-cumulus
  output->ac.data[t]         = ac;
  output->M.data[t]          = M;

  // vertical profiles
  int startt = t * 4;
  output->thetaprof.data[startt + 0] = output->theta.data[t];
  output->qprof.data[startt + 0] = output->q.data[t];
  output->zprof.data[startt + 0] = 0;

  output->thetaprof.data[startt + 1] = output->theta.data[t];
  output->qprof.data[startt + 1] = output->q.data[t];
  output->zprof.data[startt + 1] = output->h.data[t];

  output->thetaprof.data[startt + 2] = output->theta.data[t] + output->dtheta.data[t];
  output->qprof.data[startt + 2] = output->q.data[t] + output->dq.data[t];
  output->zprof.data[startt + 2] = output->h.data[t];

  output->thetaprof.data[startt + 3] = output->theta.data[t] + output->dtheta.data[t] + output->gammatheta.data[t] * 1.e6;
  output->qprof.data[startt + 3] = output->q.data[t] + output->dq.data[t] + output->gammaq.data[t] * 1.e6;
  output->zprof.data[startt + 3] = output->h.data[t] + 1.e6;

  //chemistry
  output->phi.data[t]     = phi;
  output->k_r05.data[t]   = k_r05;

  for(int n=0;n<nsc; n++)
    if(sw_chemoutput[n])
      output->sc[n].data[t] = sc[n];
    else
      output->sc[n].data[t] = 0;

  return;
}

void model::run2file(std::string filedir, std::string filename)
{
  // Save time series
  std::ofstream runsave;
  std::string fullpath        = filedir + "/" + filename + ".csv";

  // Start with time series
  runsave.open(fullpath.c_str());
  std::cout << "Saving file " << fullpath.c_str() << std::endl;

  // Write header first
  runsave << output->t.name << " [" << output->t.unit << "],";
  runsave << output->tutc.name << " [" << output->tutc.unit << "],";
  runsave << output->h.name << " [" << output->h.unit << "],";
  runsave << output->we.name << " [" << output->we.unit << "],";
  runsave << output->lcl.name << " [" << output->lcl.unit << "],";
  runsave << output->RH.name << " [" << output->RH.unit << "],";
  runsave << output->RHtop.name << " [" << output->RHtop.unit << "],";

  runsave << output->theta.name << " [" << output->theta.unit << "],";
  runsave << output->thetav.name << " [" << output->thetav.unit << "],";
  runsave << output->dtheta.name << " [" << output->dtheta.unit << "],";
  runsave << output->dthetav.name << " [" << output->dthetav.unit << "],";
  runsave << output->wtheta.name << " [" << output->wtheta.unit << "],";
  runsave << output->wthetae.name << " [" << output->wthetae.unit << "],";
  runsave << output->wthetav.name << " [" << output->wthetav.unit << "],";
  runsave << output->wthetaM.name << " [" << output->wthetaM.unit << "],";

  runsave << output->q.name << " [" << output->q.unit << "],";
  runsave << output->dq.name << " [" << output->dq.unit << "],";
  runsave << output->wq.name << " [" << output->wq.unit << "],";
  runsave << output->wqe.name << " [" << output->wqe.unit << "],";
  runsave << output->wqM.name << " [" << output->wqM.unit << "],";

  runsave << output->u.name << " [" << output->u.unit << "],";
  runsave << output->du.name << " [" << output->du.unit << "],";
  runsave << output->v.name << " [" << output->v.unit << "],";
  runsave << output->dv.name << " [" << output->dv.unit << "],";
  runsave << output->uw.name << " [" << output->uw.unit << "],";
  runsave << output->vw.name << " [" << output->vw.unit << "],";
  runsave << output->uwe.name << " [" << output->uwe.unit << "],";
  runsave << output->vwe.name << " [" << output->vwe.unit << "],";

  runsave << output->sca.name << " [" << output->sca.unit << "],";
  runsave << output->dsca.name << " [" << output->dsca.unit << "],";
  runsave << output->wsca.name << " [" << output->wsca.unit << "],";
  runsave << output->wscae.name << " [" << output->wscae.unit << "],";
  runsave << output->wscaM.name << " [" << output->wscaM.unit << "],";

  runsave << output->CO2.name << " [" << output->CO2.unit << "],";
  runsave << output->dCO2.name << " [" << output->dCO2.unit << "],";
  runsave << output->wCO2.name << " [" << output->wCO2.unit << "],";
  runsave << output->wCO2e.name << " [" << output->wCO2e.unit << "],";
  runsave << output->wCO2M.name << " [" << output->wCO2M.unit << "],";

  runsave << output->ustar.name << " [" << output->ustar.unit << "],";
  runsave << output->L.name << " [" << output->L.unit << "],";
  runsave << output->Rib.name << " [" << output->Rib.unit << "],";
  runsave << output->ra.name << " [" << output->ra.unit << "],";
  runsave << output->Cm.name << " [" << output->Cm.unit << "],";
  runsave << output->Cs.name << " [" << output->Cs.unit << "],";

  runsave << output->Swin.name << " [" << output->Swin.unit << "],";
  runsave << output->Swout.name << " [" << output->Swout.unit << "],";
  runsave << output->Lwin.name << " [" << output->Lwin.unit << "],";
  runsave << output->Lwout.name << " [" << output->Lwout.unit << "],";
  runsave << output->Q.name << " [" << output->Q.unit << "],";

  runsave << output->wg.name << " [" << output->wg.unit << "],";
  runsave << output->Tsoil.name << " [" << output->Tsoil.unit << "],";
  runsave << output->Ts.name << " [" << output->Ts.unit << "],";
  runsave << output->Wl.name << " [" << output->Wl.unit << "],";
  runsave << output->rs.name << " [" << output->rs.unit << "],";
  runsave << output->H.name << " [" << output->H.unit << "],";
  runsave << output->LE.name << " [" << output->LE.unit << "],";
  runsave << output->G.name << " [" << output->G.unit << "],";

  runsave << output->ac.name << " [" << output->ac.unit << "],";
  runsave << output->sigmaq.name << " [" << output->sigmaq.unit << "],";
  runsave << output->wqM.name << " [" << output->wqM.unit << "],";
  runsave << output->M.name << " [" << output->M.unit << "],";

  int n;

  for(n=0; n<nsc; n++)
  {
    runsave << output->sc[n].name << " [" << output->sc[n].unit;
    if(n < (nsc-1))
      runsave << "],";
    else
      runsave << "]";
  }

  runsave << std::endl;

  for(int nt=0; nt < tsteps; nt++)
  {
    runsave << output->t.data[nt] << ",";
    runsave << output->tutc.data[nt] << ",";
    runsave << output->h.data[nt] << ",";
    runsave << output->we.data[nt] << ",";
    runsave << output->lcl.data[nt] << ",";
    runsave << output->RH.data[nt] << ",";
    runsave << output->RHtop.data[nt] << ",";

    runsave << output->theta.data[nt] << ",";
    runsave << output->thetav.data[nt] << ",";
    runsave << output->dtheta.data[nt] << ",";
    runsave << output->dthetav.data[nt] << ",";
    runsave << output->wtheta.data[nt] << ",";
    runsave << output->wthetae.data[nt] << ",";
    runsave << output->wthetav.data[nt] << ",";
    runsave << output->wthetaM.data[nt] << ",";

    runsave << output->q.data[nt] << ",";
    runsave << output->dq.data[nt] << ",";
    runsave << output->wq.data[nt] << ",";
    runsave << output->wqe.data[nt] << ",";
    runsave << output->wqM.data[nt] << ",";

    runsave << output->u.data[nt] << ",";
    runsave << output->du.data[nt] << ",";
    runsave << output->v.data[nt] << ",";
    runsave << output->dv.data[nt] << ",";
    runsave << output->uw.data[nt] << ",";
    runsave << output->vw.data[nt] << ",";
    runsave << output->uwe.data[nt] << ",";
    runsave << output->vwe.data[nt] << ",";

    runsave << output->sca.data[nt] << ",";
    runsave << output->dsca.data[nt] << ",";
    runsave << output->wsca.data[nt] << ",";
    runsave << output->wscae.data[nt] << ",";
    runsave << output->wscaM.data[nt] << ",";

    runsave << output->CO2.data[nt] << ",";
    runsave << output->dCO2.data[nt] << ",";
    runsave << output->wCO2.data[nt] << ",";
    runsave << output->wCO2e.data[nt] << ",";
    runsave << output->wCO2M.data[nt] << ",";

    runsave << output->ustar.data[nt] << ",";
    runsave << output->L.data[nt] << ",";
    runsave << output->Rib.data[nt] << ",";
    runsave << output->ra.data[nt] << ",";
    runsave << output->Cm.data[nt] << ",";
    runsave << output->Cs.data[nt] << ",";

    runsave << output->Swin.data[nt] << ",";
    runsave << output->Swout.data[nt] << ",";
    runsave << output->Lwin.data[nt] << ",";
    runsave << output->Lwout.data[nt] << ",";
    runsave << output->Q.data[nt] << ",";

    runsave << output->wg.data[nt] << ",";
    runsave << output->Tsoil.data[nt] << ",";
    runsave << output->Ts.data[nt] << ",";
    runsave << output->Wl.data[nt] << ",";
    runsave << output->rs.data[nt] << ",";
    runsave << output->H.data[nt] << ",";
    runsave << output->LE.data[nt] << ",";
    runsave << output->G.data[nt] << ",";

    runsave << output->ac.data[nt] << ",";
    runsave << output->sigmaq.data[nt] << ",";
    runsave << output->wqM.data[nt] << ",";
    runsave << output->M.data[nt] << ",";

    for(n=0; n<nsc; n++)
    {
      runsave << output->sc[n].data[nt];
      if(n < (nsc-1))
        runsave << ",";
    }

    runsave << std::endl;
  }

  runsave.close();

  // Save vertical profiles
  std::ofstream runprofsave;
  std::string fullpathprofs   = filedir + "/" + filename + "_profs" + ".csv";

  runprofsave.open(fullpathprofs.c_str());
  std::cout << "Saving file " << fullpathprofs.c_str() << std::endl;

  // Header, start with # for gnuplot
  runprofsave << "# ";
  runprofsave << output->zprof.name << " [" << output->zprof.unit << "]   ";
  runprofsave << output->thetaprof.name << " [" << output->thetaprof.unit << "]   ";
  runprofsave << output->qprof.name << " [" << output->qprof.unit << "]";
  runprofsave << std::endl;

  for(int nt=0; nt < tsteps * 4; nt=nt+4)
  {
    runprofsave << "# timestep: " << output->t.data[nt] << " [hour]" << std::endl;
    for (int k=0; k<4; k++)
      runprofsave << output->zprof.data[nt+k] << "   " << output->thetaprof.data[nt+k] << " " << output->qprof.data[nt+k] << std::endl;
    runprofsave << "  " << std::endl;
    runprofsave << "  " << std::endl;
  }

  runprofsave.close();

  return;
}

void model::initchemmodel()
{

  //cout << "Starting initchemmodel" << endl;

  int i;

  //Reaction Reactions[RSIZE];

  Reaction **RC_ptr;

  RC_ptr = new Reaction*[rsize];

  for(i=0;i<rsize;i++)
    RC_ptr[i]=&reactions[i];

  //Name_Number  PL_scheme[CSIZE];
  Name_Number **PL_ptr;
  PL_ptr    = new Name_Number*[csize];

  // initialize PL_scheme, it has not been done yet
  PL_scheme = new Name_Number[csize];

  for(i=0;i<csize;i++)
    PL_ptr[i]=&PL_scheme[i];

  for(i=0;i<csize;i++)
  {
    PL_scheme[i].active = 0; // WAS FALSE
    PL_scheme[i].chem_number = -99;
    PL_scheme[i].nr_PL = 0;
  }

  // HERE THE FINAL MODULE STARTS
  cm = new modelchem(RC_ptr, PL_ptr, rsize, csize);
  cm->inputchem(sw_reactions, sw_chemoutput, stocoef);
  return;
}

void model::runchemmodel(double chemdt)
{
  double *iterin, *iterout, *fsc;
  iterin  = new double[nsc];
  iterout = new double[nsc];
  fsc     = new double[nsc];

  for(int i=0; i<nsc; i++)
  {
    iterin[i]  = sc[i];
    iterout[i] = sc[i];
    fsc[i]     = sc[i] + dsc[i];
  }

  //cout << "Running chemmodel for timestep: " << t << endl;

  if(sw_chem_constant)
  {
    double  sda;     // solar declination angle [rad]
    double  sinlea;  // sinus of local declination angle [-]

    sda    = 0.409 * cos(2. * pi * (doy - 173.) / 365.);

    sinlea = sin(2. * pi * lat / 360.) * sin(sda) - cos(2. * pi * lat / 360.) * cos(sda) * cos(2. * pi * (t * dt + tstart * 3600.) / 86400. - 2. * pi * lon / 360.);

    if(sw_photo_constant)
      if(sinlea >= 0.)
        sinlea = sin(2. * pi * lat / 360.) * sin(sda) - cos(2. * pi * lat / 360.) * cos(sda) * cos(2. * pi * (tod_ref * 3600.) / 86400. - 2. * pi * lon / 360.);

    cm->calc_k(P_ref,P_ref, \
                Tcbl_ref, Tfc_ref, \
                qcbl_ref, qfc_ref, \
                sinlea );

    cm->iter(1, chemdt, qcbl_ref, iterout, iterin, &phi, &k_r05);

    for(int i=0; i<nsc; i++)
     sc[i] = iterout[i];

    for(int i=0; i<nsc; i++)
    {
      iterin[i]  = fsc[i];
      iterout[i] = fsc[i];
    }

    double dummy;

    cm->iter(0, chemdt, qfc_ref, iterout, iterin, &dummy, &dummy);

    for(int i=0; i<nsc; i++)
     dsc[i] = iterout[i] - sc[i];
  }
  else
  {
    double  Ptop;
    double  Tcbl;
    double  Tfc;
    double  sda;     // solar declination angle [rad]
    double  sinlea;  // sinus of local declination angle [-]
    double  qfc;

    Ptop = Ps - rho * g * h;
    Tcbl = theta;
    Tfc  = theta - g / cp * h;
    qfc  = q + dq;

    sda    = 0.409 * cos(2. * pi * (doy - 173.) / 365.);
    sinlea = sin(2. * pi * lat / 360.) * sin(sda) - cos(2. * pi * lat / 360.) * cos(sda) * cos(2. * pi * (t * dt + tstart * 3600.) / 86400. - 2. * pi * lon / 360.);

    if(sw_photo_constant)
      if(sinlea >= 0.)
        sinlea = sin(2. * pi * lat / 360.) * sin(sda) - cos(2. * pi * lat / 360.) * cos(sda) * cos(2. * pi * (tod_ref * 3600.) / 86400. - 2. * pi * lon / 360.);

    cm->calc_k(Ps,   Ptop, \
               Tcbl, Tfc, \
               q,    qfc, \
               sinlea );

    cm->iter(1, chemdt, q, iterout, iterin, &phi, &k_r05);

    for(int i=0; i<nsc; i++)
     sc[i] = iterout[i];

    for(int i=0; i<nsc; i++)
    {
      iterin[i]  = fsc[i];
      iterout[i] = fsc[i];
    }

    cm->iter(0, chemdt, qfc, iterout, iterin, &phi, &k_r05);

    for(int i=0; i<nsc; i++)
      dsc[i] = iterout[i] - sc[i];
  }

  return;
}
