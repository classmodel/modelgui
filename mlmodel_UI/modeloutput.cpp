#include "modeloutput.h"

modeloutput::modeloutput(int tsteps)
{
  t.data          = new double[tsteps];   // time [s]
  t.name          = "time";
  t.unit          = "s";
  t.description   = "Elapsed time since start of model";

  // mixed-layer variables
  h.data          = new double[tsteps];   // CBL height [m]
  h.name          = "h";
  h.unit          = "m";
  h.description   = "CBL height";

  Ps.data         = new double[tsteps];   // surface pressure [Pa]
  Ps.name         = "Ps";
  Ps.unit         = "Pa";
  Ps.description  = "Surface pressure";

  ws.data         = new double[tsteps];   // large scale vertical velocity [m s-1]
  ws.name         = "ws";
  ws.unit         = "m s-1";
  ws.description  = "Large scale vertical velocity";

  theta.data      = new double[tsteps];   // initial mixed-layer potential temperature [K]
  thetav.data     = new double[tsteps];   // initial mixed-layer virtual potential temperature [K]
  dtheta.data     = new double[tsteps];   // initial potential temperature jump at h [K]
  dthetav.data    = new double[tsteps];   // initial virtual potential temperature jump at h [K]
  gammatheta.data = new double[tsteps];   // free atmosphere potential temperature lapse rate [K m-1]
  advtheta.data   = new double[tsteps];   // advection of heat [K s-1]
  beta.data       = new double[tsteps];   // entrainment ratio for virtual heat [-]
  wtheta.data     = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wthetav.data    = new double[tsteps];   // surface kinematic virtual heat flux [K m s-1]

  q.data          = new double[tsteps];   // mixed-layer specific humidity [kg kg-1]
  //qsat       = new double[tsteps];   // mixed-layer saturated specific humidity [kg kg-1]
  //e          = new double[tsteps];   // mixed-layer vapor pressure [Pa]
  //esat       = new double[tsteps];   // mixed-layer saturated vapor pressure [Pa]
  dq.data         = new double[tsteps];   // initial specific humidity jump at h [kg kg-1]
  gammaq.data     = new double[tsteps];   // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq.data       = new double[tsteps];   // advection of moisture [kg kg-1 s-1]
  wq.data         = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
    
  u.data          = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  du.data         = new double[tsteps];   // initial u-wind jump at h [m s-1]
  gammau.data     = new double[tsteps];   // free atmosphere u-wind speed lapse rate [s-1]
  advu.data       = new double[tsteps];   // advection of u-wind [m s-2]

  v.data          = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  dv.data         = new double[tsteps];   // initial u-wind jump at h [m s-1]
  gammav.data     = new double[tsteps];   // free atmosphere v-wind speed lapse rate [s-1]
  advv.data       = new double[tsteps];   // advection of v-wind [m s-2]

  ustar.data      = new double[tsteps];   // friction velocity [m s-1]
  uw.data         = new double[tsteps];   // u-momentum flux [m2 s-2]
  vw.data         = new double[tsteps];   // v-momentum flux [m2 s-2]

  return;
}
