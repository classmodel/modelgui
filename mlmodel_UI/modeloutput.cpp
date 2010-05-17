#include "modeloutput.h"

modeloutput::modeloutput(int tsteps)
{
  t.data                    = new double[tsteps];   // time [s]
  t.name                    = "time";
  t.unit                    = "h";
  t.description             = "Elapsed time since start of model";
  t.id                      = "t";

  // mixed-layer variables
  h.data                    = new double[tsteps];   // CBL height [m]
  h.name                    = "h";
  h.unit                    = "m";
  h.description             = "CBL height";
  h.id                      = "h";

  Ps.data                   = new double[tsteps];   // surface pressure [Pa]
  Ps.name                   = "Ps";
  Ps.unit                   = "Pa";
  Ps.description            = "Surface pressure";
  Ps.id                     = "Ps";

  ws.data                   = new double[tsteps];   // large scale vertical velocity [m s-1]
  ws.name                   = "ws";
  ws.unit                   = "m s\u207B\u00B9";
  ws.description            = "Large scale vertical velocity";
  ws.id                     = "ws";

  theta.data                = new double[tsteps];   // initial mixed-layer potential temperature [K]
  theta.name                = "\u03B8";
  theta.unit                = "K";
  theta.description         = "Mixed-layer potential temperature";
  theta.id                  = "theta";

  thetav.data               = new double[tsteps];   // initial mixed-layer virtual potential temperature [K]
  thetav.name               = "\u03B8\u1D65";
  thetav.unit               = "K";
  thetav.description        = "Mixed-layer virtual potential temperature";
  thetav.id                 = "thetav";

  dtheta.data               = new double[tsteps];   // initial potential temperature jump at h [K]
  dtheta.name               = "\u0394\u03B8";
  dtheta.unit               = "K";
  dtheta.description        = "Potential temperature jump at boundary layer top";
  dtheta.id                 = "dtheta";

  dthetav.data              = new double[tsteps];   // initial virtual potential temperature jump at h [K]
  gammatheta.data           = new double[tsteps];   // free atmosphere potential temperature lapse rate [K m-1]
  advtheta.data             = new double[tsteps];   // advection of heat [K s-1]
  beta.data                 = new double[tsteps];   // entrainment ratio for virtual heat [-]

  wtheta.data               = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wtheta.name               = "w'\u03B8'";
  wtheta.unit               = "K m s\u207B\u00B9";
  wtheta.description        = "Surface kinematic heat flux";

  wthetav.data              = new double[tsteps];   // surface kinematic virtual heat flux [K m s-1]

  q.data                    = new double[tsteps];   // mixed-layer specific humidity [kg kg-1]
  q.name                    = "q";
  q.unit                    = "g kg\u207B\u00B9";
  q.description             = "Mixed-layer specific humidity";
  q.id                      = "q";

  //qsat                    = new double[tsteps];   // mixed-layer saturated specific humidity [kg kg-1]
  //e                       = new double[tsteps];   // mixed-layer vapor pressure [Pa]
  //esat                    = new double[tsteps];   // mixed-layer saturated vapor pressure [Pa]

  dq.data                   = new double[tsteps];   // specific humidity jump at h [kg kg-1]
  dq.name                   = "\u0394q";
  dq.unit                   = "g kg\u207B\u00B9";
  dq.description            = "Specific humidity jump at boundary layer top";
  dq.id                     = "dq";

  gammaq.data               = new double[tsteps];   // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq.data                 = new double[tsteps];   // advection of moisture [kg kg-1 s-1]

  wq.data                   = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wq.name                   = "w'q'";
  wq.unit                   = "g kg\u207B\u00B9 m s\u207B\u00B9";
  wq.description            = "Surface kinematic moisture flux";
  wq.id                     = "wq";

  u.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  du.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  gammau.data               = new double[tsteps];   // free atmosphere u-wind speed lapse rate [s-1]
  advu.data                 = new double[tsteps];   // advection of u-wind [m s-2]

  v.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  dv.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  gammav.data               = new double[tsteps];   // free atmosphere v-wind speed lapse rate [s-1]
  advv.data                 = new double[tsteps];   // advection of v-wind [m s-2]

  ustar.data                = new double[tsteps];   // friction velocity [m s-1]
  uw.data                   = new double[tsteps];   // u-momentum flux [m2 s-2]
  vw.data                   = new double[tsteps];   // v-momentum flux [m2 s-2]

  return;
}
