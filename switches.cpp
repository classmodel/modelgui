#include <sstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

void MainWindow::loadfieldslots()
{
  // MIXED-LAYER
  connect(ui->input_ml_h,               SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ml_ps,              SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ml_ws,              SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ml_beta,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // HEAT
  connect(ui->input_heat_theta,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_heat_dtheta,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_heat_gammatheta,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_heat_wtheta,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_heat_advtheta,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // MOISTURE
  connect(ui->input_moisture_q,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_moisture_dq,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_moisture_gammaq,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_moisture_wq,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_moisture_advq,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // WIND
  //connect(ui->switch_wind,        SIGNAL(clicked()),         this, SLOT(updateInputdata()));
  connect(ui->input_wind_u,             SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_ug,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_gammau,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_advu,          SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_wind_v,             SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_vg,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_gammav,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_advv,          SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_wind_fc,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wind_ustar,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_surfacelayer_z0h,   SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surfacelayer_z0m,   SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // SOIL
  connect(ui->input_soil_T2,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_Tsoil,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_W2,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_Wg,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_soil_wsat,          SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_wfc,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_wwilt,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_c1sat,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_c2ref,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_soil_a,             SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_b,             SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_p,             SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_soil_CGsat,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // SURFACE
  connect(ui->input_surface_Ts,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_Wl,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_surface_LAI,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_gD,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_rsmin,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_alpha,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_cveg,       SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_surface_Lambda,     SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_z0m,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_surface_z0h,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // RADIATION
  connect(ui->input_rad_DOY,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_rad_lat,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_rad_lon,            SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_rad_time,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_rad_Qnet,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_rad_clouds,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // OTHER
  connect(ui->input_name,               SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_time,               SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_timestep,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_name,               SIGNAL(editingFinished()), this, SLOT(updateRunName()));

  // CHEMISTRY -> SPECIES
  connect(ui->input_species_photolysis_tref, SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_species_ref_Tcbl,   SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_ref_Tft,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_ref_qcbl,   SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_ref_qft,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_ref_pref,   SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->input_species_scalar,     SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_dscalar,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_gammascalar,SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_wscalar,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_species_advscalar,  SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
}


void plotwindow::getdata(outputvar *data, modelrun n, QString plotvar)
{
  if(!n.hasrun)
    return;

  // mixed-layer
  if (plotvar == "t")
    *data = n.run->output->t;
  else if (plotvar == "tutc")
    *data = n.run->output->tutc;
  else if (plotvar == "h")
    *data = n.run->output->h;
  else if (plotvar == "Ps")
    *data = n.run->output->Ps;
  else if (plotvar == "beta")
    *data = n.run->output->beta;
  else if (plotvar == "ws")
    *data = n.run->output->ws;
  else if (plotvar == "LCL")
    *data = n.run->output->lcl;
  else if (plotvar == "we")
    *data = n.run->output->we;

  // temperature
  else if (plotvar == "theta")
    *data = n.run->output->theta;
  else if (plotvar == "thetav")
    *data = n.run->output->thetav;
  else if (plotvar == "dtheta")
    *data = n.run->output->dtheta;
  else if (plotvar == "dthetav")
    *data = n.run->output->dthetav;
  else if (plotvar == "gammatheta")
    *data = n.run->output->gammatheta;
  else if (plotvar == "advtheta")
    *data = n.run->output->advtheta;
  else if (plotvar == "wtheta")
    *data = n.run->output->wtheta;
  else if (plotvar == "wthetae")
    *data = n.run->output->wthetae;
  else if (plotvar == "wthetav")
    *data = n.run->output->wthetav;

  // moisture
  else if (plotvar == "q")
    *data = n.run->output->q;
  else if (plotvar == "dq")
    *data = n.run->output->dq;
  else if (plotvar == "gammaq")
    *data = n.run->output->gammaq;
  else if (plotvar == "advq")
    *data = n.run->output->advq;
  else if (plotvar == "wq")
    *data = n.run->output->wq;
  else if (plotvar == "wqe")
    *data = n.run->output->wqe;

  // wind
  else if (plotvar == "u")
    *data = n.run->output->u;
  else if (plotvar == "du")
    *data = n.run->output->du;
  else if (plotvar == "gammau")
    *data = n.run->output->gammau;
  else if (plotvar == "advu")
    *data = n.run->output->advu;
  else if (plotvar == "uw")
    *data = n.run->output->uw;
  else if (plotvar == "uwe")
    *data = n.run->output->uwe;
  else if (plotvar == "v")
    *data = n.run->output->v;
  else if (plotvar == "dv")
    *data = n.run->output->dv;
  else if (plotvar == "gammav")
    *data = n.run->output->gammav;
  else if (plotvar == "advv")
    *data = n.run->output->advv;
  else if (plotvar == "vw")
    *data = n.run->output->vw;
  else if (plotvar == "vwe")
    *data = n.run->output->vwe;

  // surface-layer
  else if (plotvar == "ustar")
    *data = n.run->output->ustar;
  else if (plotvar == "L")
    *data = n.run->output->L;
  else if (plotvar == "Rib")
    *data = n.run->output->Rib;
  else if (plotvar == "ra")
    *data = n.run->output->ra;
  else if (plotvar == "Cm")
    *data = n.run->output->Cm;
  else if (plotvar == "Cs")
    *data = n.run->output->Cs;

  // radiation
  else if (plotvar == "Swin")
    *data = n.run->output->Swin;
  else if (plotvar == "Swout")
    *data = n.run->output->Swout;
  else if (plotvar == "Lwin")
    *data = n.run->output->Lwin;
  else if (plotvar == "Lwout")
    *data = n.run->output->Lwout;
  else if (plotvar == "Q")
    *data = n.run->output->Q;

  // land and soil
  else if (plotvar == "wg")
    *data = n.run->output->wg;
  else if (plotvar == "Tsoil")
    *data = n.run->output->Tsoil;
  else if (plotvar == "Ts")
    *data = n.run->output->Ts;
  else if (plotvar == "Wl")
    *data = n.run->output->Wl;
  else if (plotvar == "rs")
    *data = n.run->output->rs;
  else if (plotvar == "H")
    *data = n.run->output->H;
  else if (plotvar == "LE")
    *data = n.run->output->LE;
  else if (plotvar == "G")
    *data = n.run->output->G;

  // vertical profiles
  else if (plotvar == "zprof")
    *data = n.run->output->zprof;
  else if (plotvar == "thetaprof")
    *data = n.run->output->thetaprof;
  else if (plotvar == "qprof")
    *data = n.run->output->qprof;

  // chemistry
  for(int i=0; i<22; i++) // nsc hardcoded
  {
    QString scalar = "sc" + QString::number(i);
    if (plotvar == scalar)
       *data = n.run->output->sc[i];
  }
}


void plotwindow::setplotvar(const QString label, QString *plotvar)
{
  // mixed-layer
  if (label == "t")
    *plotvar = label;
  else if (label == "tutc")
    *plotvar = label;
  else if (label == "h")
    *plotvar = label;
  else if (label == "Ps")
    *plotvar = label;
  else if (label == "beta")
    *plotvar = label;
  else if (label == "ws")
    *plotvar = label;
  else if (label == "LCL")
    *plotvar = label;
  else if (label == "we")
    *plotvar = label;

  // temperature
  else if (label == "theta")
    *plotvar = label;
  else if (label == "thetav")
    *plotvar = label;
  else if (label == "dtheta")
    *plotvar = label;
  else if (label == "dthetav")
    *plotvar = label;
  else if (label == "gammatheta")
    *plotvar = label;
  else if (label == "advtheta")
    *plotvar = label;
  else if (label == "wtheta")
    *plotvar = label;
  else if (label == "wthetae")
    *plotvar = label;
  else if (label == "wthetav")
    *plotvar = label;

  // moisture
  else if (label == "q")
    *plotvar = label;
  else if (label == "dq")
    *plotvar = label;
  else if (label == "gammaq")
    *plotvar = label;
  else if (label == "advq")
    *plotvar = label;
  else if (label == "wq")
    *plotvar = label;
  else if (label == "wqe")
    *plotvar = label;

  // wind
  else if (label == "u")
    *plotvar = label;
  else if (label == "du")
    *plotvar = label;
  else if (label == "gammau")
    *plotvar = label;
  else if (label == "advu")
    *plotvar = label;
  else if (label == "uw")
    *plotvar = label;
  else if (label == "uwe")
    *plotvar = label;
  else if (label == "v")
    *plotvar = label;
  else if (label == "dv")
    *plotvar = label;
  else if (label == "gammav")
    *plotvar = label;
  else if (label == "advv")
    *plotvar = label;
  else if (label == "vw")
    *plotvar = label;
  else if (label == "vwe")
    *plotvar = label;

  // surface-layer
  else if (label == "ustar")
    *plotvar = label;
  else if (label == "L")
    *plotvar = label;
  else if (label == "Rib")
    *plotvar = label;
  else if (label == "ra")
    *plotvar = label;
  else if (label == "Cm")
    *plotvar = label;
  else if (label == "Cs")
    *plotvar = label;


  // radiation
  else if (label == "Swin")
    *plotvar = label;
  else if (label == "Swout")
    *plotvar = label;
  else if (label == "Lwin")
    *plotvar = label;
  else if (label == "Lwout")
    *plotvar = label;
  else if (label == "Q")
    *plotvar = label;

  //land and soil
  else if (label == "wg")
    *plotvar = label;
  else if (label == "Tsoil")
    *plotvar = label;
  else if (label == "Ts")
    *plotvar = label;
  else if (label == "Wl")
    *plotvar = label;
  else if (label == "rs")
    *plotvar = label;
  else if (label == "H")
    *plotvar = label;
  else if (label == "LE")
    *plotvar = label;
  else if (label == "G")
    *plotvar = label;

  // vertical profiles
  else if (label == "zprof")
    *plotvar = label;
  else if (label == "qprof")
    *plotvar = label;
  else if (label == "thetaprof")
    *plotvar = label;

  for(int i=0; i<22; i++) // nsc hardcoded!
  {
    QString scalar = "sc" + QString::number(i);
    if (label == scalar)
      *plotvar = label;
  }
}
