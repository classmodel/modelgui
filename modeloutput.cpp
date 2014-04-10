/*
 * CLASS
 * Copyright (c) 2010-2013 Meteorology and Air Quality section, Wageningen University and Research centre
 * Copyright (c) 2011-2013 Jordi Vila-Guerau de Arellano
 * Copyright (c) 2011-2013 Chiel van Heerwaarden
 * Copyright (c) 2011-2013 Bart van Stratum
 * Copyright (c) 2011-2013 Kees van den Dries
 *
 * This file is part of CLASS
 *
 * CLASS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CLASS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CLASS.  If not, see <http://www.gnu.org/licenses/>.
 */

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
  ws.unit                   = "m s-\u00B9";
  ws.description            = "Large scale vertical velocity";
  ws.id                     = "ws";

  lcl.data                  = new double[tsteps];   // Lifting Condensation level [m]
  lcl.name                  = "LCL";
  lcl.unit                  = "m";
  lcl.description           = "Lifting Condensation Level";
  lcl.id                    = "LCL";

  we.data                   = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  we.name                   = "we";
  we.unit                   = "m s-\u00B9";
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
  thetav.name               = "\u03B8v";
  thetav.unit               = "K";
  thetav.description        = "Mixed-layer virtual potential temperature";
  thetav.id                 = "thetav";

  dtheta.data               = new double[tsteps];   //  potential temperature jump at h [K]
  dtheta.name               = "\u0394\u03B8";
  dtheta.unit               = "K";
  dtheta.description        = "Potential temperature jump at boundary layer top";
  dtheta.id                 = "dtheta";

  dthetav.data              = new double[tsteps];   // virtual potential temperature jump at h [K]
  dthetav.name              = "\u0394\u03B8v";
  dthetav.unit              = "K";
  dthetav.description       = "Virtual potential temperature jump at boundary layer top";
  dthetav.id                = "dthetav";

  gammatheta.data           = new double[tsteps];   // free atmosphere potential temperature lapse rate [K m-1]
  gammatheta.name           = "gammatheta";
  gammatheta.unit           = "K m-\u00B9";
  gammatheta.description    = "Potential temperature lapse rate";
  gammatheta.id             = "gammatheta";

  sigmatheta.data           = new double[tsteps];   // mixed-layer top potential temperature std dev [K]
  sigmatheta.name           = "\u03C3\u03B8";
  sigmatheta.unit           = "K";
  sigmatheta.description    = "stddev(\u03B8) at mixed-layer top";
  sigmatheta.id             = "sigmatheta";

  advtheta.data             = new double[tsteps];   // advection of heat [K s-1]
  advtheta.name             = "adv-\u03B8";
  advtheta.unit             = "K s-\u00B9";
  advtheta.description      = "Large-scale temperature advection";
  advtheta.id               = "advtheta";

  beta.data                 = new double[tsteps];   // entrainment ratio for virtual heat [-]
  beta.name                 = "beta";
  beta.unit                 = "-";
  beta.description          = "Entrainment ratio";
  beta.id                   = "beta";

  wtheta.data               = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wtheta.name               = "w'\u03B8'(s)";
  wtheta.unit               = "K m s-\u00B9";
  wtheta.description        = "Surface kinematic heat flux";
  wtheta.id                 = "wtheta";

  wthetae.data              = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wthetae.name              = "w'\u03B8'(e)";
  wthetae.unit              = "K m s-\u00B9";
  wthetae.description       = "Entrainment kinematic heat flux";
  wthetae.id                = "wthetae";

  wthetav.data              = new double[tsteps];   // surface kinematic virtual heat flux [K m s-1]
  wthetav.name              = "w'\u03B8v'(s)";
  wthetav.unit              = "K m s-\u00B9";
  wthetav.description       = "Surface virtual temperature flux";
  wthetav.id                = "wthetav";

  wthetaM.data              = new double[tsteps];   // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  wthetaM.name              = "w'\u03B8'(M)";
  wthetaM.unit              = "K m s-\u00B9";
  wthetaM.description       = "Mass-flux kinematic heat flux";
  wthetaM.id                = "wthetaM";

  wstar.data                = new double[tsteps];   // Deardorff vertical velocity scale [m s-1]
  wstar.name                = "w*";
  wstar.unit                = "m s-\u00B9";
  wstar.description         = "Convective velocity scale";
  wstar.id                  = "wstar";

  q.data                    = new double[tsteps];   // mixed-layer specific humidity [kg kg-1]
  q.name                    = "q";
  q.unit                    = "g kg-\u00B9";
  q.description             = "Mixed-layer specific humidity";
  q.id                      = "q";

  //qsat                    = new double[tsteps];   // mixed-layer saturated specific humidity [kg kg-1]
  //e                       = new double[tsteps];   // mixed-layer vapor pressure [Pa]
  //esat                    = new double[tsteps];   // mixed-layer saturated vapor pressure [Pa]

  dq.data                   = new double[tsteps];   // specific humidity jump at h [kg kg-1]
  dq.name                   = "\u0394q";
  dq.unit                   = "g kg-\u00B9";
  dq.description            = "Specific humidity jump at boundary layer top";
  dq.id                     = "dq";

  gammaq.data               = new double[tsteps];   // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  gammaq.name               = "gammaq";
  gammaq.unit               = "g kg-\u00B9 m-\u00B9";
  gammaq.description        = "Specific humidity lapse rate";
  gammaq.id                 = "gammaq";

  advq.data                 = new double[tsteps];   // advection of moisture [kg kg-1 s-1]
  advq.name                 = "adv-q";
  advq.unit                 = "g kg-\u00B9 m-\u00B9";
  advq.description          = "Large-scale moisture advection";
  advq.id                   = "advq";

  sigmaq.data               = new double[tsteps];   // mixed-layer top specific humidity std dev [kg kg-1]
  sigmaq.name               = "\u03C3q";
  sigmaq.unit               = "g kg-\u00B9";
  sigmaq.description        = "stddev(q) at mixed-layer top";
  sigmaq.id                 = "sigmaq";

  wq.data                   = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wq.name                   = "w'q'(s)";
  wq.unit                   = "g kg-\u00B9 m s-\u00B9";
  wq.description            = "Surface kinematic moisture flux";
  wq.id                     = "wq";

  wqe.data                  = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wqe.name                  = "w'q'(e)";
  wqe.unit                  = "g kg-\u00B9 m s-\u00B9";
  wqe.description           = "Entrainment kinematic moisture flux";
  wqe.id                    = "wqe";

  wqM.data                  = new double[tsteps];   // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  wqM.name                  = "w'q'(M)";
  wqM.unit                  = "g kg-\u00B9 m s-\u00B9";
  wqM.description           = "Mass-flux kinematic moisture flux";
  wqM.id                    = "wqM";

  u.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  u.name                    = "u";
  u.unit                    = "m s-\u00B9";
  u.description             = "u-wind";
  u.id                      = "u";

  du.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  du.name                   = "\u0394u";
  du.unit                   = "m s-\u00B9";
  du.description            = "u-wind jump at boundary layer top";
  du.id                     = "du";

  gammau.data               = new double[tsteps];   // free atmosphere u-wind speed lapse rate [s-1]
  gammau.name               = "gammau";
  gammau.unit               = "m s-\u00B9 m-\u00B9";
  gammau.description        = "u-wind lapse rate";
  gammau.id                 = "gammau";


  advu.data                 = new double[tsteps];   // advection of u-wind [m s-2]
  advu.name                 = "adv-u";
  advu.unit                 = "m s-\u00B9 s-\u00B9";
  advu.description          = "Large-scale u-wind advection";
  advu.id                   = "advu";

  v.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  v.name                    = "v";
  v.unit                    = "m s-\u00B9";
  v.description             = "v-wind";
  v.id                      = "v";

  dv.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  dv.name                   = "\u0394v";
  dv.unit                   = "m s-\u00B9";
  dv.description            = "v-wind jump at boundary layer top";
  dv.id                     = "dv";

  gammav.data               = new double[tsteps];   // free atmosphere v-wind speed lapse rate [s-1]
  gammav.name               = "gammav";
  gammav.unit               = "m s-\u00B9 m-\u00B9";
  gammav.description        = "v-wind lapse rate";
  gammav.id                 = "gammav";

  advv.data                 = new double[tsteps];   // advection of v-wind [m s-2]
  advv.name                 = "adv-v";
  advv.unit                 = "m s-\u00B9 s-\u00B9";
  advv.description          = "Large-scale v-wind advection";
  advv.id                   = "advv";


  uw.data                   = new double[tsteps];   // u-momentum flux [m2 s-2]
  uw.name                   = "u'w'(s)";
  uw.unit                   = "m\u00B2 s-\u00B2";
  uw.description            = "Surface u-wind momentum flux";
  uw.id                     = "uw";

  vw.data                   = new double[tsteps];   // v-momentum flux [m2 s-2]
  vw.name                   = "v'w'(s)";
  vw.unit                   = "m\u00B2 s-\u00B2";
  vw.description            = "Surface v-wind momentum flux";
  vw.id                     = "vw";

  uwe.data                   = new double[tsteps];   // u-momentum flux [m2 s-2]
  uwe.name                   = "u'w'(e)";
  uwe.unit                   = "m\u00B2 s-\u00B2";
  uwe.description            = "Entrainment u-wind momentum flux";
  uwe.id                     = "uwe";

  vwe.data                   = new double[tsteps];   // v-momentum flux [m2 s-2]
  vwe.name                   = "v'w'(e)";
  vwe.unit                   = "m\u00B2 s-\u00B2";
  vwe.description            = "Entrainment v-wind momentum flux";
  vwe.id                     = "vwe";

  // Always present scalars (non-chemistry)
  sca.data                  = new double[tsteps];       // mixed-layer scalar [kg kg-1]
  sca.name                  = "scalar";
  sca.unit                  = "ppm";
  sca.description           = "Scalar";
  sca.id                    = "sca";

  dsca.data                 = new double[tsteps];      // initial scalar jump at h [kg kg-1]
  dsca.name                 = "\u0394sca";
  dsca.unit                 = "ppm";
  dsca.description          = "Scalar jump at boundary layer top";
  dsca.id                   = "dsca";

  gammasca.data             = new double[tsteps];  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  gammasca.name             = "gammasca";
  gammasca.unit             = "ppm m-\u00B9";
  gammasca.description      = "Scalar lapse rate";
  gammasca.id               = "gammasca";

  advsca.data               = new double[tsteps];  // advection of scalar [kg kg-1 s-1]
  advsca.name               = "advsca";
  advsca.unit               = "ppm s-\u00B9";
  advsca.description        = "Large-scale scalar advection";
  advsca.id                 = "advsca";

  wsca.data                 = new double[tsteps];  // surface kinematic scalar flux [kg kg-1 m s-1]
  wsca.name                 = "wsca";
  wsca.unit                 = "ppm m s-\u00B9";
  wsca.description          = "Surface kinematic scalar flux";
  wsca.id                   = "wsca";

  wscae.data                = new double[tsteps];  // entrainment kinematic scalar flux [kg kg-1 m s-1]
  wscae.name                = "wscae";
  wscae.unit                = "ppm m s-\u00B9";
  wscae.description         = "Entrainment kinematic scalar flux";
  wscae.id                  = "wscae";

  wscaM.data                = new double[tsteps];  // mass-flux kinematic scalar flux [kg kg-1 m s-1]
  wscaM.name                = "wscaM";
  wscaM.unit                = "ppm m s-\u00B9";
  wscaM.description         = "Mass-flux kinematic scalar flux";
  wscaM.id                  = "wscaM";

  sigmasca.data             = new double[tsteps];   // mixed-layer top scalar std dev [kg kg-1]
  sigmasca.name             = "\u03C3sca";
  sigmasca.unit             = "ppm";
  sigmasca.description      = "stddev(sca) at mixed-layer top";
  sigmasca.id               = "sigmasca";

  CO2.data                  = new double[tsteps];       // mixed-layer CO2 [kg kg-1]
  CO2.name                  = "CO2";
  CO2.unit                  = "ppm";
  CO2.description           = "CO2 concentration";
  CO2.id                    = "CO2";

  dCO2.data                 = new double[tsteps];      // initial CO2 jump at h [kg kg-1]
  dCO2.name                 = "\u0394CO2";
  dCO2.unit                 = "ppm";
  dCO2.description          = "CO2 jump at boundary layer top";
  dCO2.id                   = "dCO2";

  gammaCO2.data             = new double[tsteps];  // free atmosphere CO2 lapse rate [kg kg-1 m-1]
  gammaCO2.name             = "gammaCO2";
  gammaCO2.unit             = "ppm m-\u00B9";
  gammaCO2.description      = "CO2 lapse rate";
  gammaCO2.id               = "gammaCO2";

  advCO2.data               = new double[tsteps];  // advection of CO2 [kg kg-1 s-1]
  advCO2.name               = "advCO2";
  advCO2.unit               = "ppm s-\u00B9";
  advCO2.description        = "Large-scale CO2 advection";
  advCO2.id                 = "advCO2";

  wCO2.data                 = new double[tsteps];  // surface kinematic CO2 flux [mgC/m2s]
  wCO2.name                 = "wCO2";
  wCO2.unit                 = "mgC m-2 s-1";
  wCO2.description          = "Surface kinematic CO2 flux";
  wCO2.id                   = "wCO2";

  wCO2A.data                 = new double[tsteps];  // Assimilation CO2 flux [kg kg-1 m s-1]
  wCO2A.name                 = "wCO2A";
  wCO2A.unit                 = "mgC m-2 s-1";
  wCO2A.description          = "Assimilation kinematic CO2 flux";
  wCO2A.id                   = "wCO2A";

  wCO2R.data                 = new double[tsteps];  // Respiration CO2 flux [kg kg-1 m s-1]
  wCO2R.name                 = "wCO2R";
  wCO2R.unit                 = "mgC m-2 s-1";
  wCO2R.description          = "Respiration kinematic CO2 flux";
  wCO2R.id                   = "wCO2R";

  wCO2e.data                = new double[tsteps];  // entrainment kinematic CO2 flux [kg kg-1 m s-1]
  wCO2e.name                = "wCO2e";
  wCO2e.unit                = "mgC m-2 s-1";
  wCO2e.description         = "Entrainment kinematic CO2 flux";
  wCO2e.id                  = "wCO2e";

  wCO2M.data                = new double[tsteps];  // mass-flux kinematic CO2 flux [kg kg-1 m s-1]
  wCO2M.name                = "wCO2M";
  wCO2M.unit                = "mgC m-2 s-1";
  wCO2M.description         = "Mass-flux kinematic CO2 flux";
  wCO2M.id                  = "wCO2M";

  sigmaCO2.data             = new double[tsteps];   // mixed-layer top CO2 std dev [ppm]
  sigmaCO2.name             = "\u03C3CO2";
  sigmaCO2.unit             = "ppm";
  sigmaCO2.description      = "stddev(CO2) at mixed-layer top";
  sigmaCO2.id               = "sigmaCO2";

  // surface layer
  ustar.data                = new double[tsteps];   // friction velocity [m s-1]
  ustar.name                = "u*";
  ustar.unit                = "m s-\u00B9";
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
  ra.unit                   = "s m-\u00B9";
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
  Swin.unit                 = "W m-\u00B2";
  Swin.description          = "Incoming short wave radiation";
  Swin.id                   = "Swin";

  Swout.data                = new double[tsteps];   // Outgoing short wave radiation [W m-2]
  Swout.name                = "Swout";
  Swout.unit                = "W m-\u00B2";
  Swout.description         = "Outgoing short wave radiation";
  Swout.id                  = "Swout";

  Lwin.data                 = new double[tsteps];   // Incoming long wave radiation [W m-2]
  Lwin.name                 = "Lwin";
  Lwin.unit                 = "W m-\u00B2";
  Lwin.description          = "Incoming long wave radiation";
  Lwin.id                   = "Lwin";

  Lwout.data                = new double[tsteps];   // Outgoing long wave radiation [W m-2]
  Lwout.name                = "Lwout";
  Lwout.unit                = "W m-\u00B2";
  Lwout.description         = "Outgoing long wave radiation";
  Lwout.id                  = "Lwout";

  Q.data                    = new double[tsteps];   // Net radiation [W m-2]
  Q.name                    = "Q";
  Q.unit                    = "W m-\u00B2";
  Q.description             = "Net radiation";
  Q.id                      = "Q";

  // land and soil
  wg.data                   = new double[tsteps];   // Soil moisture top layer [m3 m-3]
  wg.name                   = "Wsoil1";
  wg.unit                   = "m3 m-3";
  wg.description            = "Volumetric water content top soil layer";
  wg.id                     = "wg";

  Tsoil.data                = new double[tsteps];   //  Soil temperature [K]
  Tsoil.name                = "Tsoil1";
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
  rs.unit                   = "s m-\u00B9";
  rs.description            = "Surface resistance";
  rs.id                     = "rs";

  H.data                    = new double[tsteps];   // Net radiation [W m-2]
  H.name                    = "H";
  H.unit                    = "W m-\u00B2";
  H.description             = "Sensible heat flux";
  H.id                      = "H";

  LE.data                   = new double[tsteps];   // Net radiation [W m-2]
  LE.name                   = "LE";
  LE.unit                   = "W m-\u00B2";
  LE.description            = "Latent heat flux";
  LE.id                     = "LE";

  G.data                    = new double[tsteps];   // Net radiation [W m-2]
  G.name                    = "G";
  G.unit                    = "W m-\u00B2";
  G.description             = "Ground heat flux";
  G.id                      = "G";

  // shallow-cumulus
  ac.data                   = new double[tsteps];   // cloud core fraction [-]
  ac.name                   = "ac";
  ac.unit                   = "-";
  ac.description            = "Cloud core fraction";
  ac.id                     = "ac";

  M.data                    = new double[tsteps];   // mass-flux (/rho) [m s-1]
  M.name                    = "M";
  M.unit                    = "m s-\u00B9";
  M.description             = "Mass-flux";
  M.id                      = "M";

  cc.data                   = new double[tsteps];   // total cloud fraction [-]
  cc.name                   = "cc";
  cc.unit                   = "-";
  cc.description            = "Total cloud fraction";
  cc.id                     = "cc";

  // vertical profiles
  thetaprof.data            = new double[tsteps*4];
  thetaprof.name            = "\u03B8";
  thetaprof.unit            = "K";
  thetaprof.description     = "Mixed-layer potential temperature";
  thetaprof.id              = "thetaprof";

  qprof.data                = new double[tsteps*4];   // mixed-layer specific humidity [kg kg-1]
  qprof.name                = "q";
  qprof.unit                = "g kg-\u00B9";
  qprof.description         = "Mixed-layer specific humidity";
  qprof.id                  = "qprof";

  scaprof.data              = new double[tsteps*4];   // mixed-layer scalar [kg kg-1]
  scaprof.name              = "scalar";
  scaprof.unit              = "ppm";
  scaprof.description       = "Scalar";
  scaprof.id                = "scaprof";

  wthetaprof.data           = new double[tsteps*4];   // Kinematic heat flux [K m s-1]
  wthetaprof.name           = "w'\u03B8'";
  wthetaprof.unit           = "K m s-\u00B9";
  wthetaprof.description    = "Kinematic heat flux";
  wthetaprof.id             = "wthetaprof";

  wthetavprof.data          = new double[tsteps*4];   // Virtual kinematic heat flux [K m s-1]
  wthetavprof.name          = "w'\u03B8v'";
  wthetavprof.unit          = "K m s-\u00B9";
  wthetavprof.description   = "Virtual kinematic heat flux";
  wthetavprof.id            = "wthetavprof";

  wqprof.data               = new double[tsteps*4];   // Kinematic moisture flux [kg kg-1 m s-1]
  wqprof.name               = "w'q'";
  wqprof.unit               = "g kg-\u00B9 m s-\u00B9";
  wqprof.description        = "Kinematic moisture flux";
  wqprof.id                 = "wqprof";

  wscaprof.data             = new double[tsteps*4];   // Kinematic scalar flux [ppm m s-1]
  wscaprof.name             = "w'sca'";
  wscaprof.unit             = "ppm m s-\u00B9";
  wscaprof.description      = "Kinematic scalar flux";
  wscaprof.id               = "wscaprof";

  zprof.data                = new double[tsteps*4];
  zprof.name                = "z";
  zprof.unit                = "m";
  zprof.description         = "Height";
  zprof.id                  = "zprof";

  //chemistry
  phi.data                  = new double[tsteps];   // Net radiation [W m-2]
  phi.name                  = "phi";
  phi.unit                  = "-";
  phi.description           = "Photostationary state";
  phi.id                    = "phi";

  k_r05.data                = new double[tsteps];   // Net radiation [W m-2]
  k_r05.name                = "k_r05";
  k_r05.unit                = "-";
  k_r05.description         = "NO2 photolysis rate (r05)";
  k_r05.id                  = "k_r05";

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

void modeloutput::reset(int nsc)
{
  delete[] t.data;
  delete[] tutc.data;
  delete[] h.data;
  delete[] Ps.data;
  delete[] ws.data;
  delete[] lcl.data;
  delete[] we.data;
  delete[] RH.data;
  delete[] RHtop.data;
  delete[] theta.data;
  delete[] thetav.data;
  delete[] dtheta.data;
  delete[] dthetav.data;
  delete[] gammatheta.data;
  delete[] sigmatheta.data;
  delete[] advtheta.data;
  delete[] beta.data;
  delete[] wtheta.data;
  delete[] wthetae.data;
  delete[] wthetav.data;
  delete[] wthetaM.data;
  delete[] wstar.data;
  delete[] q.data;
  delete[] dq.data;
  delete[] gammaq.data;
  delete[] advq.data;
  delete[] sigmaq.data;
  delete[] wq.data;
  delete[] wqe.data;
  delete[] wqM.data;
  delete[] u.data;
  delete[] du.data;
  delete[] gammau.data;
  delete[] advu.data;
  delete[] v.data;
  delete[] dv.data;
  delete[] gammav.data;
  delete[] advv.data;
  delete[] uw.data;
  delete[] vw.data;
  delete[] uwe.data;
  delete[] vwe.data;
  delete[] sca.data;
  delete[] dsca.data;
  delete[] gammasca.data;
  delete[] advsca.data;
  delete[] wsca.data;
  delete[] wscae.data;
  delete[] wscaM.data;
  delete[] sigmasca.data;
  delete[] CO2.data;
  delete[] dCO2.data;
  delete[] gammaCO2.data;
  delete[] advCO2.data;
  delete[] wCO2.data;
  delete[] wCO2A.data;
  delete[] wCO2R.data;
  delete[] wCO2e.data;
  delete[] wCO2M.data;
  delete[] sigmaCO2.data;
  delete[] ustar.data;
  delete[] L.data;
  delete[] Rib.data;
  delete[] ra.data;
  delete[] Cm.data;
  delete[] Cs.data;
  delete[] Swin.data;
  delete[] Swout.data;
  delete[] Lwin.data;
  delete[] Lwout.data;
  delete[] Q.data;
  delete[] wg.data;
  delete[] Tsoil.data;
  delete[] Ts.data;
  delete[] Wl.data;
  delete[] rs.data;
  delete[] H.data;
  delete[] LE.data;
  delete[] G.data;
  delete[] ac.data;
  delete[] M.data;
  delete[] cc.data;
  delete[] thetaprof.data;
  delete[] wthetaprof.data;
  delete[] wthetavprof.data;
  delete[] wqprof.data;
  delete[] wscaprof.data;
  delete[] scaprof.data;
  delete[] qprof.data;
  delete[] zprof.data;
  delete[] phi.data;
  delete[] k_r05.data;

  for(int n=0; n<nsc; n++)
    delete[] sc[n].data;

  delete[] sc;

  return;
}

void modeloutput::reload(int tsteps, int nsc)
{
  t.data                    = new double[tsteps];   // time [h]
  tutc.data                 = new double[tsteps];   // time UTC [h]
  h.data                    = new double[tsteps];   // CBL height [m]
  Ps.data                   = new double[tsteps];   // surface pressure [Pa]
  ws.data                   = new double[tsteps];   // large scale vertical velocity [m s-1]
  lcl.data                  = new double[tsteps];   // Lifting Condensation level [m]
  we.data                   = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  RH.data                   = new double[tsteps];   // Relative humidity at ..? [-]
  RHtop.data                = new double[tsteps];   // Relative humidity at mixed-layer top [-]
  theta.data                = new double[tsteps];   //  mixed-layer potential temperature [K]
  thetav.data               = new double[tsteps];   //  mixed-layer virtual potential temperature [K]
  dtheta.data               = new double[tsteps];   //  potential temperature jump at h [K]
  dthetav.data              = new double[tsteps];   // virtual potential temperature jump at h [K]
  gammatheta.data           = new double[tsteps];   // free atmosphere potential temperature lapse rate [K m-1]
  sigmatheta.data           = new double[tsteps];   // mixed-layer top potential temperature std dev [K]
  advtheta.data             = new double[tsteps];   // advection of heat [K s-1]
  beta.data                 = new double[tsteps];   // entrainment ratio for virtual heat [-]
  wtheta.data               = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wthetae.data              = new double[tsteps];   // surface kinematic heat flux [K m s-1]
  wthetav.data              = new double[tsteps];   // surface kinematic virtual heat flux [K m s-1]
  wthetaM.data              = new double[tsteps];   // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  wstar.data                = new double[tsteps];   // free convective velocity scale [m s-1]
  q.data                    = new double[tsteps];   // mixed-layer specific humidity [kg kg-1]
  dq.data                   = new double[tsteps];   // specific humidity jump at h [kg kg-1]
  gammaq.data               = new double[tsteps];   // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq.data                 = new double[tsteps];   // advection of moisture [kg kg-1 s-1]
  sigmaq.data               = new double[tsteps];   // mixed-layer top specific humidity std dev [kg kg-1]
  wq.data                   = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wqe.data                  = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
  wqM.data                  = new double[tsteps];   // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  u.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  du.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  gammau.data               = new double[tsteps];   // free atmosphere u-wind speed lapse rate [s-1]
  advu.data                 = new double[tsteps];   // advection of u-wind [m s-2]
  v.data                    = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
  dv.data                   = new double[tsteps];   // initial u-wind jump at h [m s-1]
  gammav.data               = new double[tsteps];   // free atmosphere v-wind speed lapse rate [s-1]
  advv.data                 = new double[tsteps];   // advection of v-wind [m s-2]
  uw.data                   = new double[tsteps];   // u-momentum flux [m2 s-2]
  vw.data                   = new double[tsteps];   // v-momentum flux [m2 s-2]
  uwe.data                  = new double[tsteps];   // u-momentum flux [m2 s-2]
  vwe.data                  = new double[tsteps];   // v-momentum flux [m2 s-2]
  sca.data                  = new double[tsteps];       // mixed-layer scalar [kg kg-1]
  dsca.data                 = new double[tsteps];      // initial scalar jump at h [kg kg-1]
  gammasca.data             = new double[tsteps];  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  advsca.data               = new double[tsteps];  // advection of scalar [kg kg-1 s-1]
  wsca.data                 = new double[tsteps];  // surface kinematic scalar flux [kg kg-1 m s-1]
  wscae.data                = new double[tsteps];  // entrainment kinematic scalar flux [kg kg-1 m s-1]
  wscaM.data                = new double[tsteps];  // mass-flux kinematic scalar flux [kg kg-1 m s-1]
  sigmasca.data             = new double[tsteps];   // mixed-layer top scalar std dev [kg kg-1]
  CO2.data                  = new double[tsteps];       // mixed-layer CO2 [kg kg-1]
  dCO2.data                 = new double[tsteps];      // initial CO2 jump at h [kg kg-1]
  gammaCO2.data             = new double[tsteps];  // free atmosphere CO2 lapse rate [kg kg-1 m-1]
  advCO2.data               = new double[tsteps];  // advection of CO2 [kg kg-1 s-1]
  wCO2.data                 = new double[tsteps];  // surface kinematic CO2 flux [kg kg-1 m s-1]
  wCO2A.data                = new double[tsteps];  // Assimilation CO2 flux [kg kg-1 m s-1]
  wCO2R.data                = new double[tsteps];  // Respiration CO2 flux [kg kg-1 m s-1]
  wCO2e.data                = new double[tsteps];  // entrainment kinematic CO2 flux [kg kg-1 m s-1]
  wCO2M.data                = new double[tsteps];  // mass-flux kinematic CO2 flux [kg kg-1 m s-1]
  sigmaCO2.data             = new double[tsteps];   // mixed-layer top CO2 std dev [ppm]
  ustar.data                = new double[tsteps];   // friction velocity [m s-1]
  L.data                    = new double[tsteps];   // Obukhov length [m]
  Rib.data                  = new double[tsteps];   // Bulk Richardson number [-]
  ra.data                   = new double[tsteps];   // aerodynamic resistance [s m-1]
  Cm.data                   = new double[tsteps];   // drag coefficient for momentum [-]
  Cs.data                   = new double[tsteps];   // drag coefficient for scalars [-]
  Swin.data                 = new double[tsteps];   // Incoming short wave radiation [W m-2]
  Swout.data                = new double[tsteps];   // Outgoing short wave radiation [W m-2]
  Lwin.data                 = new double[tsteps];   // Incoming long wave radiation [W m-2]
  Lwout.data                = new double[tsteps];   // Outgoing long wave radiation [W m-2]
  Q.data                    = new double[tsteps];   // Net radiation [W m-2]
  wg.data                   = new double[tsteps];   // Soil moisture top layer [m3 m-3]
  Tsoil.data                = new double[tsteps];   //  Soil temperature [K]
  Ts.data                   = new double[tsteps];   //  Skin temperature [K]
  Wl.data                   = new double[tsteps];   // Liquid water on vegetation [m]
  rs.data                   = new double[tsteps];   // surface resistance [s m-1]
  H.data                    = new double[tsteps];   // Net radiation [W m-2]
  LE.data                   = new double[tsteps];   // Net radiation [W m-2]
  G.data                    = new double[tsteps];   // Net radiation [W m-2]
  ac.data                   = new double[tsteps];   // cloud core fraction [-]
  M.data                    = new double[tsteps];   // mass-flux (/rho) [m s-1]
  cc.data                   = new double[tsteps];   // total cloud fraction [-]
  thetaprof.data            = new double[tsteps*4];
  wthetaprof.data           = new double[tsteps*4];
  wthetavprof.data          = new double[tsteps*4];
  wqprof.data               = new double[tsteps*4];
  wscaprof.data             = new double[tsteps*4];
  scaprof.data              = new double[tsteps*4];
  qprof.data                = new double[tsteps*4];   // mixed-layer specific humidity [kg kg-1]
  zprof.data                = new double[tsteps*4];
  phi.data                  = new double[tsteps];   // Net radiation [W m-2]
  k_r05.data                = new double[tsteps];   // Net radiation [W m-2]
  sc                        = new outputvar[nsc];

  // Reinitialize the chemistry names, because they have been deleted!
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
