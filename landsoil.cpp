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

#include "mainwindow.h"

void MainWindow::initLandSoil()
{
  // Define soil types
  soiltypes[0].name  = "Sandy loam";
  soiltypes[0].wsat  = 0.472;
  soiltypes[0].wfc   = 0.323;
  soiltypes[0].wwilt = 0.171;
  soiltypes[0].a     = 0.219;
  soiltypes[0].b     = 4.9;
  soiltypes[0].p     = 4.;
  soiltypes[0].CGsat = 3.56e-6;
  soiltypes[0].C1sat = 0.132;
  soiltypes[0].C2ref = 1.8;

  soiltypes[1].name  = "Sand";
  soiltypes[1].wsat  = 0.403;
  soiltypes[1].wfc   = 0.244;
  soiltypes[1].wwilt = 0.059;
  soiltypes[1].a     = 0.387;
  soiltypes[1].b     = 4.05;
  soiltypes[1].p     = 4.;
  soiltypes[1].CGsat = 3.222e-6;
  soiltypes[1].C1sat = 0.082;
  soiltypes[1].C2ref = 3.9;

  soiltypes[2].name  = "Clay";
  soiltypes[2].wsat  = 0.614;
  soiltypes[2].wfc   = 0.541;
  soiltypes[2].wwilt = 0.335;
  soiltypes[2].a     = 0.083;
  soiltypes[2].b     = 11.4;
  soiltypes[2].p     = 12.;
  soiltypes[2].CGsat = 3.6e-6;
  soiltypes[2].C1sat = 0.342;
  soiltypes[2].C2ref = 0.3;

  // Define surface types
  // SHORT GRASS
  surfacetypes[0].name   = "Short grass";
  surfacetypes[0].LAI    = 2.;
  surfacetypes[0].gD     = 0.;
  surfacetypes[0].rsmin  = 110.;
  surfacetypes[0].alpha  = 0.25;
  surfacetypes[0].cveg   = 0.85;
  surfacetypes[0].Lambda = 5.9;
  surfacetypes[0].z0m    = 0.02;
  surfacetypes[0].z0h    = 0.002;
  surfacetypes[0].C3C4   = 0;                   // 0 = C3
  surfacetypes[0].c_beta = 0;

  // MAIZE
  surfacetypes[1].name   = "Maize";
  surfacetypes[1].LAI    = 3.;
  surfacetypes[1].gD     = 0.;
  surfacetypes[1].rsmin  = 180.;
  surfacetypes[1].alpha  = 0.2;
  surfacetypes[1].cveg   = 0.9;
  surfacetypes[1].Lambda = 10;
  surfacetypes[1].z0m    = 0.15;
  surfacetypes[1].z0h    = 0.015;
  surfacetypes[1].C3C4   = 1;                   // 1 = C4
  surfacetypes[1].c_beta = 0;

  // BROADLEAF TREES
  surfacetypes[2].name   = "Broadleaf trees";
  surfacetypes[2].LAI    = 5.;
  surfacetypes[2].gD     = 0.03;
  surfacetypes[2].rsmin  = 200.;
  surfacetypes[2].alpha  = 0.25;
  surfacetypes[2].cveg   = 0.9;
  surfacetypes[2].Lambda = 20;
  surfacetypes[2].z0m    = 2.;
  surfacetypes[2].z0h    = 2.;
  surfacetypes[2].C3C4   = -1;                   // -1 = no CO2
  surfacetypes[2].c_beta = 0;

  // NEEDLELEAF TREES
  surfacetypes[3].name   = "Needleleaf trees";
  surfacetypes[3].LAI    = 5.;
  surfacetypes[3].gD     = 0.03;
  surfacetypes[3].rsmin  = 500.;
  surfacetypes[3].alpha  = 0.25;
  surfacetypes[3].cveg   = 0.9;
  surfacetypes[3].Lambda = 20;
  surfacetypes[3].z0m    = 2.;
  surfacetypes[3].z0h    = 2.;
  surfacetypes[3].C3C4   = -1;                   // 0 = no CO2
  surfacetypes[3].c_beta = 0;
}
