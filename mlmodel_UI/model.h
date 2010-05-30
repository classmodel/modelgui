#include "modelinput.h"
#include "modeloutput.h"

class model
{
public:
  model(modelinput);
  void runmodel();
  modelinput input;
  modeloutput *output;
  void run2file(std::string, std::string);
 
private:
  void initmodel();
  void runmlmodel();
  void intmlmodel();
  void store();

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
  
  // time variables
  double runtime;   // duration of model run [s]
  double dt;        // time step [s]
  int tsteps;       // total time steps [-]
  int t;            // current time step [-]

  // mixed-layer
  bool   sw_ml;     // mixed-layer model switch;
  double h;         // initial ABL height [m]
  double Ps;        // surface pressure [Pa]
  double ws;        // large scale vertical velocity [m s-1]
  double fc;        // coriolis parameter [s-1]
  double we;        // entrainment velocity [m s-1]
  
  double theta;     // initial mixed-layer potential temperature [K]
  double dtheta;    // initial temperature jump at h [K]
  double gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double advtheta;  // advection of heat [K s-1]
  double beta;      // entrainment ratio for virtual heat [-]
  double wtheta;    // surface kinematic heat flux [K m s-1]
  double wthetae;   // entrainment kinematic heat flux [K m s-1]

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
  double wqe;       // entrainment kinematic moisture flux [kg kg-1 m s-1]

  bool   sw_wind;   // prognostic wind switch
  double u;         // initial mixed-layer u-wind speed [m s-1]
  double du;        // initial u-wind jump at h [m s-1]
  double gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double advu;      // advection of u-wind [m s-2]
  
  double v;         // initial mixed-layer u-wind speed [m s-1]
  double dv;        // initial u-wind jump at h [m s-1]
  double gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double advv;      // advection of v-wind [m s-2]
  
  double uw;        // surface momentum flux in u-direction [m2 s-2]
  double vw;        // surface momentum flux in v-direction [m2 s-2]

  // surface layer variables
  bool   sw_sl;     // surface layer switch
  double ustar;     // surface friction velocity [m s-1]

  double z0m;       // roughness length for momentum [m]
  double z0h;       // roughness length for scalars [m]
  double Cm;        // drag coefficient for momentum [-]
  double Cs;        // drag coefficient for scalars [-]
  double L;         // Obukhov length [-]
  double Rib;       // bulk Richardson number [-]

  // radiation parameters
  bool   sw_rad;    // radiation switch
  double lat;       // latitude [deg]
  double lon;       // longitude [deg]
  double doy;       // day of the year [-]
  double tstart;    // time of the day [h UTC]
  double cc;        // cloud cover fraction [-]

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

  double Ts;        // initial surface temperature [K]

  double cveg;      // vegetation fraction [-]
  double Wmax;      // thickness of water layer on wet vegetation [m]
  double Wl;        // equivalent water layer depth for wet vegetation [m]
  double cliq;      // wet fraction [-]

  double Lambda;    // thermal diffusivity skin layer [-]

  // new variables ... place them at right position
  double htend;
  double thetatend, qtend, utend, vtend;
  double dthetatend, dqtend, dutend, dvtend;
};
