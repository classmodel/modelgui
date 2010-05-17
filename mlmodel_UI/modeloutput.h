#include <string>

class outputvar
{
public:
  double *data;
  std::string name;
  std::string unit;
  std::string description;
  std::string id;
};

class modeloutput
{
public:
  outputvar t;         // time [s]

  // mixed-layer variables
  outputvar h;         // CBL height [m]
  outputvar Ps;        // surface pressure [Pa]
  outputvar ws;        // large scale vertical velocity [m s-1]
  
  outputvar theta;     // initial mixed-layer potential temperature [K]
  outputvar thetav;    // initial mixed-layer virtual potential temperature [K]
  outputvar dtheta;    // initial potential temperature jump at h [K]
  outputvar dthetav;   // initial virtual potential temperature jump at h [K]
  outputvar gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  outputvar advtheta;  // advection of heat [K s-1]
  outputvar beta;      // entrainment ratio for virtual heat [-]
  outputvar wtheta;    // surface kinematic heat flux [K m s-1]
  outputvar wthetav;   // surface kinematic virtual heat flux [K m s-1]
  
  outputvar q;         // mixed-layer specific humidity [kg kg-1]
  //double *qsat;      // mixed-layer saturated specific humidity [kg kg-1]
  //double *e;         // mixed-layer vapor pressure [Pa]
  //double *esat;      // mixed-layer saturated vapor pressure [Pa]
  outputvar dq;        // initial specific humidity jump at h [kg kg-1]
  outputvar gammaq;    // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  outputvar advq;      // advection of moisture [kg kg-1 s-1]
  outputvar wq;        // surface kinematic moisture flux [kg kg-1 m s-1]
  
  outputvar u;         // initial mixed-layer u-wind speed [m s-1]
  outputvar du;        // initial u-wind jump at h [m s-1]
  outputvar gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  outputvar advu;      // advection of u-wind [m s-2]
  
  outputvar v;         // initial mixed-layer u-wind speed [m s-1]
  outputvar dv;        // initial u-wind jump at h [m s-1]
  outputvar gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  outputvar advv;      // advection of v-wind [m s-2]
  
  outputvar ustar;     // friction velocity [m s-1]
  outputvar uw;        // u-momentum flux [m2 s-2]
  outputvar vw;        // v-momentum flux [m2 s-2]

  modeloutput(int);
};
