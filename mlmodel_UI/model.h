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
  
  bool   sw_wind;   // prognostic wind switch
  double u;         // initial mixed-layer u-wind speed [m s-1]
  double du;        // initial u-wind jump at h [m s-1]
  double gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double advu;      // advection of u-wind [m s-2]
  
  double v;         // initial mixed-layer u-wind speed [m s-1]
  double dv;        // initial u-wind jump at h [m s-1]
  double gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double advv;      // advection of v-wind [m s-2]
  
  double ustar;     // surface friction velocity [m s-1]
  double uw;        // surface momentum flux in u-direction [m2 s-2]
  double vw;        // surface momentum flux in v-direction [m2 s-2]
};
