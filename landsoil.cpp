#include "mainwindow.h"

void MainWindow::initLandSoil()
{
  // Define soil types
  soiltypes[0].name  = "Loamy";
  soiltypes[0].wsat  = 0.472;
  soiltypes[0].wfc   = 0.323;
  soiltypes[0].wwilt = 0.171;
  soiltypes[0].a     = 0.219;
  soiltypes[0].b     = 4.9;
  soiltypes[0].p     = 4.;
  soiltypes[0].CGsat = 3.56e-6;
  soiltypes[0].C1sat = 0.132;
  soiltypes[0].C2ref = 1.8;

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
  surfacetypes[0].C3C4   = 1;                   // 1 = C3

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
  surfacetypes[1].C3C4   = 2;                   // 2 = C4

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
  surfacetypes[2].C3C4   = 0;                   // 0 = no CO2

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
  surfacetypes[3].C3C4   = 0;                   // 0 = no CO2


}
