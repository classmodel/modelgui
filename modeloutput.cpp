#include <sstream>
#include "modeloutput.h"
modeloutput::modeloutput(int tsteps, int nsc)
{
  t.data                    = new double[tsteps];   // time [h]
  t.name                    = "time";
  t.unit                    = "h";
  t.description             = "Elapsed time since start of model";
  t.id                      = "t";

  tutc.data                 = new double[tsteps];   // time UTC [h]
  tutc.name                 = "time";
  tutc.unit                 = "h UTC";
  tutc.description          = "Time in hours UTC";
  tutc.id                   = "tutc";

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

  lcl.data                  = new double[tsteps];   // Lifting Condensation level [m]
  lcl.name                  = "LCL";
  lcl.unit                  = "m";
  lcl.description           = "Lifting Condensation Level";
  lcl.id                    = "LCL";

  we.data                   = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  we.name                   = "we";
  we.unit                   = "m s\u207B\u00B9";
  we.description            = "Entrainment velocity";
  we.id                     = "we";

  RH.data                   = new double[tsteps];   // Relative humidity at ..? [-]
  RH.name                   = "RH(surf)";
  RH.unit                   = "-";
  RH.description            = "Relative humidity at surface";
  RH.id                     = "RH";

  RHtop.data                = new double[tsteps];   // Relative humidity at mixed-layer top [-]
  RHtop.name                = "RH(top)";
  RHtop.unit                = "-";
  RHtop.description         = "Relative humidity at mixed-layer top";
  RHtop.id                  = "RHtop";

  theta.data                = new double[tsteps];   //  mixed-layer potential temperature [K]
  theta.name                = "\u03B8";
  theta.unit                = "K";
  theta.description         = "Mixed-layer potential temperature";
  theta.id                  = "theta";

  thetav.data               = new double[tsteps];   //  mixed-layer virtual potential temperature [K]
  thetav.name               = "\u03B8\u1D65";
  thetav.unit               = "K";
  thetav.description        = "Mixed-layer virtual potential temperature";
  thetav.id                 = "thetav";

  dtheta.data               = new double[tsteps];   //  potential temperature jump at h [K]
  dtheta.name               = "\u0394\u03B8";
  dtheta.unit               = "K";
  dtheta.description        = "Potential temperature jump at boundary layer top";
  dtheta.id                 = "dtheta";

  dthetav.data              = new double[tsteps];   // virtual potential temperature jump at h [K]
  dthetav.name              = "\u0394\u03B8\u1D65";
  dthetav.unit              = "K";
  dthetav.description       = "Virtual potential temperature jump at boundary layer top";
  dthetav.id                = "dthetav";

  gammatheta.data           = new double[tsteps];   // free atmosphere potential temperature lapse rate [K m-1]
  gammatheta.name           = "gammatheta";
  gammatheta.unit           = "K m\u207B\u00B9";
  gammatheta.description    = "Potential temperature lapse rate";
  gammatheta.id             = "gammatheta";

  advtheta.data             = new double[tsteps];   // advection of heat [K s-1]
  advtheta.name             = "adv-\u03B8";
  advtheta.unit             = "K s\u207B\u00B9";
  advtheta.description      = "Large-scale temperature advection";
  advtheta.id               = "advtheta";

  beta.data                 = new double[tsteps];   // entrainment ratio for virtual heat [-]
  beta.name                 = "beta";
  beta.unit                 = "-";
  beta.description          = "Entrainment ratio";
  beta.id                   = "beta";

  wtheta.data               = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wtheta.name               = "w'\u03B8'(s)";
  wtheta.unit               = "K m s\u02C9\u00B9";
  wtheta.description        = "Surface kinematic heat flux";
  wtheta.id                 = "wtheta";

  wthetae.data              = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wthetae.name              = "w'\u03B8'(e)";
  wthetae.unit              = "K m s\u207B\u00B9";
  wthetae.description       = "Entrainment kinematic heat flux";
  wthetae.id                = "wthetae";

  wthetav.data              = new double[tsteps];   // surface kinematic virtual heat flux [K m s-1]
  wthetav.name              = "w'\u03B8\u1D65'(s)";
  wthetav.unit              = "K m s\u207B\u00B9";
  wthetav.description       = "Surface virtual temperature flux";
  wthetav.id                = "wthetav";

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
  gammaq.name               = "gammaq";
  gammaq.unit               = "g kg\u207B\u00B9 m\u207B\u00B9";
  gammaq.description        = "Specific humidity lapse rate";
  gammaq.id                 = "gammaq";

  advq.data                 = new double[tsteps];   // advection of moisture [kg kg-1 s-1]
  advq.name                 = "adv-q";
  advq.unit                 = "g kg\u207B\u00B9 m\u207B\u00B9";
  advq.description          = "Large-scale moisture advection";
  advq.id                   = "advq";

  wq.data                   = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wq.name                   = "w'q'(s)";
  wq.unit                   = "g kg\u207B\u00B9 m s\u207B\u00B9";
  wq.description            = "Surface kinematic moisture flux";
  wq.id                     = "wq";

  wqe.data                  = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wqe.name                  = "w'q'(e)";
  wqe.unit                  = "g kg\u207B\u00B9 m s\u207B\u00B9";
  wqe.description           = "Entrainment kinematic moisture flux";
  wqe.id                    = "wqe";

  u.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  u.name                    = "u";
  u.unit                    = "m s\u207B\u00B9";
  u.description             = "u-wind";
  u.id                      = "u";

  du.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  du.name                   = "\u0394u";
  du.unit                   = "m s\u207B\u00B9";
  du.description            = "u-wind jump at boundary layer top";
  du.id                     = "du";

  gammau.data               = new double[tsteps];   // free atmosphere u-wind speed lapse rate [s-1]
  gammau.name               = "gammau";
  gammau.unit               = "m s\u207B\u00B9 m\u207B\u00B9";
  gammau.description        = "u-wind lapse rate";
  gammau.id                 = "gammau";


  advu.data                 = new double[tsteps];   // advection of u-wind [m s-2]
  advu.name                 = "adv-u";
  advu.unit                 = "m s\u207B\u00B9 s\u207B\u00B9";
  advu.description          = "Large-scale u-wind advection";
  advu.id                   = "advu";

  v.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  v.name                    = "v";
  v.unit                    = "m s\u207B\u00B9";
  v.description             = "v-wind";
  v.id                      = "v";

  dv.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  dv.name                   = "\u0394v";
  dv.unit                   = "m s\u207B\u00B9";
  dv.description            = "v-wind jump at boundary layer top";
  dv.id                     = "dv";

  gammav.data               = new double[tsteps];   // free atmosphere v-wind speed lapse rate [s-1]
  gammav.name               = "gammav";
  gammav.unit               = "m s\u207B\u00B9 m\u207B\u00B9";
  gammav.description        = "v-wind lapse rate";
  gammav.id                 = "gammav";

  advv.data                 = new double[tsteps];   // advection of v-wind [m s-2]
  advv.name                 = "adv-v";
  advv.unit                 = "m s\u207B\u00B9 s\u207B\u00B9";
  advv.description          = "Large-scale v-wind advection";
  advv.id                   = "advv";


  uw.data                   = new double[tsteps];   // u-momentum flux [m2 s-2]
  uw.name                   = "u'w'(s)";
  uw.unit                   = "m\u00B2 s\u207B\u00B2";
  uw.description            = "Surface u-wind momentum flux";
  uw.id                     = "uw";

  vw.data                   = new double[tsteps];   // v-momentum flux [m2 s-2]
  vw.name                   = "v'w'(s)";
  vw.unit                   = "m\u00B2 s\u207B\u00B2";
  vw.description            = "Surface v-wind momentum flux";
  vw.id                     = "vw";

  uwe.data                   = new double[tsteps];   // u-momentum flux [m2 s-2]
  uwe.name                   = "u'w'(e)";
  uwe.unit                   = "m\u00B2 s\u207B\u00B2";
  uwe.description            = "Entrainment u-wind momentum flux";
  uwe.id                     = "uwe";

  vwe.data                   = new double[tsteps];   // v-momentum flux [m2 s-2]
  vwe.name                   = "v'w'(e)";
  vwe.unit                   = "m\u00B2 s\u207B\u00B2";
  vwe.description            = "Entrainment v-wind momentum flux";
  vwe.id                     = "vwe";

  // surface layer
  ustar.data                = new double[tsteps];   // friction velocity [m s-1]
  ustar.name                = "u*";
  ustar.unit                = "m s\u207B\u00B9";
  ustar.description         = "Friction velocity";
  ustar.id                  = "ustar";

  L.data                    = new double[tsteps];   // Obukhov length [m]
  L.name                    = "L";
  L.unit                    = "m";
  L.description             = "Obukhov length";
  L.id                      = "L";

  Rib.data                  = new double[tsteps];   // Bulk Richardson number [-]
  Rib.name                  = "Rib";
  Rib.unit                  = "-";
  Rib.description           = "Bulk Richardson number";
  Rib.id                    = "Rib";

  ra.data                   = new double[tsteps];   // aerodynamic resistance [s m-1]
  ra.name                   = "ra";
  ra.unit                   = "s m\u207B\u00B9";
  ra.description            = "Aerodynamic resistance";
  ra.id                     = "ra";

  Cm.data                   = new double[tsteps];   // drag coefficient for momentum [-]
  Cm.name                   = "Cm";
  Cm.unit                   = "-";
  Cm.description            = "Drag coefficient for momentum";
  Cm.id                     = "Cm";

  Cs.data                   = new double[tsteps];   // drag coefficient for scalars [-]
  Cs.name                   = "Cs";
  Cs.unit                   = "-";
  Cs.description            = "Drag coefficient for scalars";
  Cs.id                     = "Cs";

  // radiation
  Swin.data                 = new double[tsteps];   // Incoming short wave radiation [W m-2]
  Swin.name                 = "Swin";
  Swin.unit                 = "W m\u207B\u00B2";
  Swin.description          = "Incoming short wave radiation";
  Swin.id                   = "Swin";

  Swout.data                = new double[tsteps];   // Outgoing short wave radiation [W m-2]
  Swout.name                = "Swout";
  Swout.unit                = "W m\u207B\u00B2";
  Swout.description         = "Outgoing short wave radiation";
  Swout.id                  = "Swout";

  Lwin.data                 = new double[tsteps];   // Incoming long wave radiation [W m-2]
  Lwin.name                 = "Lwin";
  Lwin.unit                 = "W m\u207B\u00B2";
  Lwin.description          = "Incoming long wave radiation";
  Lwin.id                   = "Lwin";

  Lwout.data                = new double[tsteps];   // Outgoing long wave radiation [W m-2]
  Lwout.name                = "Lwout";
  Lwout.unit                = "W m\u207B\u00B2";
  Lwout.description         = "Outgoing long wave radiation";
  Lwout.id                  = "Lwout";

  Q.data                    = new double[tsteps];   // Net radiation [W m-2]
  Q.name                    = "Q";
  Q.unit                    = "W m\u207B\u00B2";
  Q.description             = "Net radiation";
  Q.id                      = "Q";

  // land and soil
  wg.data                   = new double[tsteps];   // Soil moisture top layer [m3 m-3]
  wg.name                   = "wg";
  wg.unit                   = "m3 m-3";
  wg.description            = "Volumetric water content top soil layer";
  wg.id                     = "wg";

  Tsoil.data                = new double[tsteps];   //  Soil temperature [K]
  Tsoil.name                = "Tsoil";
  Tsoil.unit                = "K";
  Tsoil.description         = "Temperature top soil layer";
  Tsoil.id                  = "Tsoil";

  Ts.data                   = new double[tsteps];   //  Skin temperature [K]
  Ts.name                   = "Ts";
  Ts.unit                   = "K";
  Ts.description            = "Skin temperature";
  Ts.id                     = "Ts";

  Wl.data                   = new double[tsteps];   // Liquid water on vegetation [m]
  Wl.name                   = "Wl";
  Wl.unit                   = "m3 m-3";
  Wl.description            = "Liquid water on vegetation";
  Wl.id                     = "Wl";

  rs.data                   = new double[tsteps];   // surface resistance [s m-1]
  rs.name                   = "rs";
  rs.unit                   = "s m\u207B\u00B9";
  rs.description            = "Surface resistance";
  rs.id                     = "rs";

  H.data                    = new double[tsteps];   // Net radiation [W m-2]
  H.name                    = "H";
  H.unit                    = "W m\u207B\u00B2";
  H.description             = "Sensible heat flux";
  H.id                      = "H";

  LE.data                   = new double[tsteps];   // Net radiation [W m-2]
  LE.name                   = "LE";
  LE.unit                   = "W m\u207B\u00B2";
  LE.description            = "Latent heat flux";
  LE.id                     = "LE";

  G.data                    = new double[tsteps];   // Net radiation [W m-2]
  G.name                    = "G";
  G.unit                    = "W m\u207B\u00B2";
  G.description             = "Ground heat flux";
  G.id                      = "G";

  // shallow-cumulus
  ac.data                   = new double[tsteps];   // cloud core fraction [-]
  ac.name                   = "ac";
  ac.unit                   = "-";
  ac.description            = "Cloud core fraction";
  ac.id                     = "ac";

  sigmaq.data               = new double[tsteps];   // mixed-layer top specific humidity std dev [kg kg-1]
  sigmaq.name               = "\u03C3q";
  sigmaq.unit               = "g kg\u207B\u00B9";
  sigmaq.description        = "stddev(q) at mixed-layer top";
  sigmaq.id                 = "sigmaq";

  M.data                    = new double[tsteps];   // mass-flux (/rho) [m s-1]
  M.name                    = "M";
  M.unit                    = "m s\u207B\u00B9";
  M.description             = "Mass-flux";
  M.id                      = "M";

  wqM.data                  = new double[tsteps];   // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  wqM.name                  = "w'q'(M)";
  wqM.unit                  = "g kg\u207B\u00B9 m s\u207B\u00B9";
  wqM.description           = "Mass-flux kinematic moisture flux";
  wqM.id                    = "wqM";

  // vertical profiles
  thetaprof.data            = new double[tsteps*4];
  thetaprof.name            = "\u03B8";
  thetaprof.unit            = "K";
  thetaprof.description     = "Mixed-layer potential temperature";
  thetaprof.id              = "thetaprof";

  qprof.data                = new double[tsteps*4];   // mixed-layer specific humidity [kg kg-1]
  qprof.name                = "q";
  qprof.unit                = "g kg\u207B\u00B9";
  qprof.description         = "Mixed-layer specific humidity";
  qprof.id                  = "qprof";

  zprof.data                = new double[tsteps*4];
  zprof.name                = "z";
  zprof.unit                = "m";
  zprof.description         = "Height";
  zprof.id                  = "zprof";

  //chemistry
  phi.data                    = new double[tsteps];   // Net radiation [W m-2]
  phi.name                    = "phi";
  phi.unit                    = "-";
  phi.description             = "Photostationary state";
  phi.id                      = "phi";

  k_r05.data                    = new double[tsteps];   // Net radiation [W m-2]
  k_r05.name                    = "k_r05";
  k_r05.unit                    = "-";
  k_r05.description             = "NO2 photolysis rate (r05)";
  k_r05.id                      = "k_r05";

  sc                        = new outputvar[nsc];
  for(int n=0; n<nsc; n++)
  {
    sc[n].data = new double[tsteps];
    std::stringstream an;
    an << n;
    sc[n].name = "scalar" + an.str();
    sc[n].unit = "ppb";
    sc[n].description = "Chemistry scalar " + an.str();
    sc[n].id   = "sc" + an.str();
  }

  // CvH: for now, set the name of the chemical species...
  sc[0].name  = "Inert";
  sc[1].name  = "O3";
  sc[2].name  = "O1D";
  sc[3].name  = "NO";
  sc[4].name  = "NO2";
  sc[5].name  = "CH4";
  sc[6].name  = "CH2O";
  sc[7].name  = "CH3O2";
  sc[8].name  = "MVK";
  sc[9].name  = "ISO";
  sc[10].name = "RO2";
  sc[11].name = "OH";
  sc[12].name = "HO2";
  sc[13].name = "CO";
  sc[14].name = "H2O";
  sc[15].name = "Product";
  sc[16].name = "O2";
  sc[17].name = "N2";
  sc[18].name = "HNO3";
  sc[19].name = "H2O2";
  sc[20].name = "NO3";
  sc[21].name = "N2O5";

  sc[0].description  = "Inert";
  sc[1].description  = "O3";
  sc[2].description  = "O1D";
  sc[3].description  = "NO";
  sc[4].description  = "NO2";
  sc[5].description  = "CH4";
  sc[6].description  = "CH2O";
  sc[7].description  = "CH3O2";
  sc[8].description  = "MVK";
  sc[9].description  = "ISO";
  sc[10].description = "RO2";
  sc[11].description = "OH";
  sc[12].description = "HO2";
  sc[13].description = "CO";
  sc[14].description = "H2O";
  sc[15].description = "Product";
  sc[16].description = "O2";
  sc[17].description = "N2";
  sc[18].description = "HNO3";
  sc[19].description = "H2O2";
  sc[20].description = "NO3";
  sc[21].description = "N2O5";
  // CvH remove later...

  return;
}
