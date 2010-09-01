#include <cstring>
#include "modelinput.h"
#include "modeloutput.h"
#include "modelchem.h"

class model
{
public:
  model(modelinput*);
  void runmodel();
  modelinput input;
  modeloutput *output;
  void run2file(std::string, std::string);
 
private:
  void initmodel();

  void runmlmodel();
  void intmlmodel();

  void runslmodel();

  void runradmodel();

  void runlsmodel();
  void intlsmodel();

  void initchemmodel();
  void runchemmodel(double);

  void store();

  double ribtol(double, double, double, double);
  inline double psim(double);
  inline double psih(double);

  // model constants
  double Lv;        // heat of vaporization [J kg-1]
  double cp;        // specific heat of dry air [J kg-1 K-1]
  double rho;       // density of air [kg m-3]
  double k;         // Von Karman constant [-]
  double g;         // gravity acceleration [m s-2]
  double Rd;        // gas constant for dry air [J kg-1 K-1]
  double Rv;        // gas constant for moist air [J kg-1 K-1]
  double bolz;      // Bolzman constant [-]
  double rhow;      // density of water [kg m-3]
  double S0;        // solar constant [W m-2]
  double pi;
  
  // time variables
  double runtime;   // duration of model run [s]
  double dt;        // time step [s]
  double sinperiod; // period for sinusoidal fluxes [s]
  int tsteps;       // total time steps [-]
  int t;            // current time step [-]

  // mixed-layer
  bool   sw_ml;     // mixed-layer model switch;
  double h;         // initial ABL height [m]
  double Ps;        // surface pressure [Pa]
  double omegas;    // large scale divergence [s-1]
  double ws;        // large scale vertical velocity [m s-1]
  double fc;        // coriolis parameter [s-1]
  double we;        // entrainment velocity [m s-1]
  double lcl;       // lifted condensation level [m]
  double RH;        // Relative humidity at T=theta [-]
  double RHtop;     // Relative humidity at mixed-layer top [-]
  
  double theta;     // initial mixed-layer potential temperature [K]
  double dtheta;    // initial temperature jump at h [K]
  double gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double advtheta;  // advection of heat [K s-1]
  double beta;      // entrainment ratio for virtual heat [-]
  double wtheta;    // surface kinematic heat flux [K m s-1]
  double wtheta0;   // maximum surface kinematic heat flux [K m s-1]
  double wthetae;   // entrainment kinematic heat flux [K m s-1]
  bool   sw_wtheta; // switch for sinusoidal temperature flux

  double T2m;       // 2m temperature [K]
  double q2m;       // 2m specific humidity [kg kg-1]
  double e2m;       // 2m vapor pressure [Pa]
  double esat2m;    // 2m saturated vapor pressure [Pa]
  double u2m;       // 2m u-wind [m s-1]
  double v2m;       // 2m v-wind [m s-1]

  double thetasurf; // surface potential temperature [K]
  double thetav;    // initial mixed-layer potential temperature [K]
  double dthetav;   // initial virtual temperature jump at h [K]
  double thetavsurf;// surface virtual potential temperature [K]
  double qsurf;     // surface specific humidity [g kg-1]
  double wthetav;   // surface kinematic virtual heat flux [K m s-1]
  
  double q;         // initial mixed-layer specific humidity [kg kg-1]
  double dq;        // initial specific humidity jump at h [kg kg-1]
  double gammaq;    // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  double advq;      // advection of moisture [kg kg-1 s-1]
  double wq;        // surface kinematic moisture flux [kg kg-1 m s-1]
  double wq0;       // maximum surface kinematic moisture flux [kg kg-1 m s-1]
  double wqe;       // entrainment kinematic moisture flux [kg kg-1 m s-1]
  bool   sw_wq;     // switch for sinusoidal moisture flux

  double qsat;      // mixed-layer saturated specific humidity [kg kg-1]
  double esat;      // mixed-layer saturated vapor pressure [Pa]
  double e;         // mixed-layer vapor pressure [Pa]
  double qsatsurf;  // surface saturated specific humidity [g kg-1]
  double dqsatdT;   // slope saturated specific humidity curve [g kg-1 K-1]

  bool   sw_wind;   // prognostic wind switch
  double u;         // initial mixed-layer u-wind speed [m s-1]
  double du;        // initial u-wind jump at h [m s-1]
  double gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double advu;      // advection of u-wind [m s-2]
  double uw;        // surface momentum flux in u-direction [m2 s-2]
  double uwe;       // Entrainment momentum flux in u-direction [m2 s-2]
  
  double v;         // initial mixed-layer u-wind speed [m s-1]
  double dv;        // initial u-wind jump at h [m s-1]
  double gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double advv;      // advection of v-wind [m s-2]
  double vw;        // surface momentum flux in v-direction [m2 s-2]
  double vwe;       // Entrainment momentum flux in v-direction [m2 s-2]

  int nsc;
  double *sc;       // initial mixed-layer scalar 
  double *dsc;      // initial scalar jump at h [kg kg-1]
  double *gammasc;  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  double *advsc;    // advection of scalar [kg kg-1 s-1]
  double *wsc;      // surface kinematic scalar flux [kg kg-1 m s-1]
  int    *sw_wsc;   // switch for sinusoidal wsc
  double *wsc0;
  double *wsce;
  double *sctend, *dsctend;

  double htend;
  double thetatend, qtend, utend, vtend;
  double dthetatend, dqtend, dutend, dvtend;

  // surface layer variables
  bool   sw_sl;     // surface layer switch
  bool   sw_sea;    // land / sea switch
  double ustar;     // surface friction velocity [m s-1]

  double z0m;       // roughness length for momentum [m]
  double z0h;       // roughness length for scalars [m]
  double Cm;        // drag coefficient for momentum [-]
  double Cs;        // drag coefficient for scalars [-]
  double L;         // Obukhov length [-]
  double Rib;       // bulk Richardson number [-]
  double ra;        // aerodynamic resistance [s m-1]

  // radiation parameters
  bool   sw_rad;    // radiation switch
  double lat;       // latitude [deg]
  double lon;       // longitude [deg]
  double doy;       // day of the year [-]
  double tstart;    // time of the day [h UTC]
  double cc;        // cloud cover fraction [-]

  double Swin;      // incoming short wave radiation [W m-2]
  double Swout;     // outgoing short wave radiation [W m-2]
  double Lwin;      // incoming long wave radiation [W m-2]
  double Lwout;     // outgoing long wave radiation [W m-2]
  double Q;         // net radiation [W m-2]

  // land surface parameters
  bool   sw_ls;     // land surface switch
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

  double C1sat;
  double C2ref;

  double LAI;       // leaf area index [-]
  double gD;        // correction factor transpiration for VPD [-]
  double rsmin;     // minimum resistance transpiration [s m-1]
  double rssoilmin; // minimum resistance soil evaporation [s m-1]
  double alpha;     // surface albedo [-]

  double rs;        // surface resistance [s m-1]
  double rssoil;    // soil resistance [s m-1]

  double Ts;        // initial surface temperature [K]

  double cveg;      // vegetation fraction [-]
  double Wmax;      // thickness of water layer on wet vegetation [m]
  double Wl;        // equivalent water layer depth for wet vegetation [m]
  double cliq;      // wet fraction [-]

  double Lambda;    // thermal diffusivity skin layer [-]

  double H;         // sensible heat flux [W m-2]
  double LE;        // evapotranspiration [W m-2]
  double LEliq;     // open water evaporation [W m-2]
  double LEveg;     // transpiration [W m-2]
  double LEsoil;    // soil evaporation [W m-2]
  double LEpot;     // potential evaporation [W m-2]
  double LEref;     // reference evaporation using rs = rsmin / LAI [W m-2]
  double G;         // ground heat flux [W m-2]

  double Tsoiltend;
  double wgtend;
  double Wltend;

  // chemistry
  modelchem *cm;
  bool   sw_chem;
  bool   sw_chem_constant;
  bool   sw_photo_constant;
  int    rsize;
  int    csize;
  bool   *sw_chemoutput;
  Reaction *reactions;
  bool     *sw_reactions;
  Name_Number *PL_scheme;
  double P_ref;
  double Tcbl_ref;
  double Tfc_ref;
  double qcbl_ref;
  double qfc_ref;
  double tod_ref;
  double stocoef;
  double phi;
};

