// Model input class
class modelinput
{
public:
  // general model variables
  double runtime;   // duration of model run [s]
  double dt;        // time step [s]

  // mixed-layer variables  
  double h;         // initial ABL height [m]
  double Ps;        // surface pressure [Pa]
  double ws;        // large scale vertical velocity [m s-1]
  double fc;        // Coriolis parameter [s-1]
  
  double theta;     // initial mixed-layer potential temperature [K]
  double dtheta;    // initial temperature jump at h [K]
  double gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double advtheta;  // advection of heat [K s-1]
  double beta;      // entrainment ratio for virtual heat [-]
  double wtheta;    // surface kinematic heat flux [K m s-1]
  
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
 
  modelinput()
  {
    runtime    = -1.;  // duration of model run [s]
    dt         = -1.;  // time step [s]

    // mixed-layer variables  
    h          = -1.;  // initial ABL height [m]
    Ps         = -1.;  // surface pressure [Pa]
    ws         = -1.;  // large scale vertical velocity [m s-1]
    fc         = -1.;  // Coriolis parameter [s-1]
    
    theta      = -1.;  // initial mixed-layer potential temperature [K]
    dtheta     = -1.;  // initial temperature jump at h [K]
    gammatheta = -1.;  // free atmosphere potential temperature lapse rate [K m-1]
    advtheta   = -1.;  // advection of heat [K s-1]
    beta       = -1.;  // entrainment ratio for virtual heat [-]
    wtheta     = -1.;  // surface kinematic heat flux [K m s-1]
    
    q          = -1.;  // initial mixed-layer specific humidity [kg kg-1]
    dq         = -1.;  // initial specific humidity jump at h [kg kg-1]
    gammaq     = -1.;  // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    advq       = -1.;  // advection of moisture [kg kg-1 s-1]
    wq         = -1.;  // surface kinematic moisture flux [kg kg-1 m s-1]
    
    sw_wind    = false;// prognostic wind switch
    u          = -1.;  // initial mixed-layer u-wind speed [m s-1]
    du         = -1.;  // initial u-wind jump at h [m s-1]
    gammau     = -1.;  // free atmosphere u-wind speed lapse rate [s-1]
    advu       = -1.;  // advection of u-wind [m s-2]
    
    v          = -1.;  // initial mixed-layer u-wind speed [m s-1]
    dv         = -1.;  // initial u-wind jump at h [m s-1]
    gammav     = -1.;  // free atmosphere v-wind speed lapse rate [s-1]
    advv       = -1.;  // advection of v-wind [m s-2]

    ustar      = -1.;  // surface friction velocity [m s-1]
  }
};

