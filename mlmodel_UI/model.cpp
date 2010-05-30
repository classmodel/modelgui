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
  input.Cm         =  extinput.Cm;         // drag coefficient for momentum [-]
  input.Cs         =  extinput.Cs;         // drag coefficient for scalars [-]
  input.L          =  extinput.L;          // Obukhov length [-]
  input.Rib        =  extinput.Rib;        // bulk Richardson number [-]
  
  // radiation
  input.sw_rad     =  extinput.sw_rad;     // radiation switch
  input.lat        =  extinput.lat;        // latitude [deg]
  input.lon        =  extinput.lon;        // longitude [deg]
  input.doy        =  extinput.doy;        // day of the year [-]
  input.tstart     =  extinput.tstart;     // time of the day [h UTC]
  input.cc         =  extinput.cc;         // cloud cover fraction [-]
  
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
  input.cliq       =  extinput.cliq;       // wet fraction [-]
  
  input.Lambda     =  extinput.Lambda;     // thermal diffusivity skin layer [-]

  return;
}

void model::initmodel()
{
  // read initial and boundary conditions from input file
  runtime    =  input.runtime;          // duration of model run [s]
  dt         =  input.dt;               // time step [s]

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
  
  thetasurf  =  input.theta;            // surface potential temperature [K]
  
  q          =  input.q;                // initial mixed-layer specific humidity [kg kg-1]
  dq         =  input.dq;               // initial specific humidity jump at h [kg kg-1]
  gammaq     =  input.gammaq;           // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq       =  input.advq;             // advection of moisture [kg kg-1 s-1]
  wq         =  input.wq;               // surface kinematic moisture flux [kg kg-1 m s-1]
  
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
  Cm         =  input.Cm;               // drag coefficient for momentum [-]
  Cs         =  input.Cs;               // drag coefficient for scalars [-]
  L          =  input.L;                // Obukhov length [-]
  Rib        =  input.Rib;              // bulk Richardson number [-]
  
  // radiation
  sw_rad     =  input.sw_rad;           // radiation switch
  lat        =  input.lat;              // latitude [deg]
  lon        =  input.lon;              // longitude [deg]
  doy        =  input.doy;              // day of the year [-]
  tstart     =  input.tstart;           // time of the day [h UTC]
  cc         =  input.cc;               // cloud cover fraction [-]
  
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
  cliq       =  input.cliq;             // wet fraction [-]
  
  Lambda     =  input.Lambda;           // thermal diffusivity skin layer [-]


  // initialize time variables
  tsteps = int(runtime / dt) + 1;
  t      = 0;

  if(sw_ml)
    runmlmodel();

  if(sw_sl)
    // spin up surface layer, both Cs and L are unknown, iterate towards consistent solution
    for(int i = 0; i < 10; i++)
      runslmodel();

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
    if(sw_sl)
      runslmodel();

    if(sw_ml)
      runmlmodel();

    if(sw_ml)
      intmlmodel();

    store();
  }

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

  thetatend   = (wtheta + we * dtheta) / h + advtheta;
  qtend       = (wq     + we * dq)     / h + advq;

  dthetatend  = gammatheta * we - thetatend;
  dqtend      = gammaq     * we - qtend;

  // assume u + du = ug, so ug - u = du
  if(sw_wind)
  {
    utend       = -fc * dv + (uw + we * du)  / h + advu;
    vtend       =  fc * du + (vw + we * dv)  / h + advv;

    dutend      = gammau * we - utend;
    dvtend      = gammav * we - vtend;
  }
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
  double    U;     // total wind speed [m s-1]
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


void model::store()
{
  cout << "(t,h,theta,q,u,v) " << t * dt << ", " << h << ", " << theta << ", " << q*1000. << ", " << u << ", " << v << endl;
  output->t.data[t]          = t * dt / 3600.; // + tstart;

  output->h.data[t]          = h;
  output->Ps.data[t]         = Ps;
  output->ws.data[t]         = ws;
  output->beta.data[t]       = beta;
  
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
  
  output->v.data[t]          = v;
  output->dv.data[t]         = dv;
  output->gammav.data[t]     = gammav;
  output->advv.data[t]       = advv;
  
  output->ustar.data[t]      = ustar;
  output->uw.data[t]         = uw;
  output->vw.data[t]         = vw;

  return;
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
