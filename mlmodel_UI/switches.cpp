#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

void MainWindow::loadfieldslots()
{
  connect(ui->input_advq,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_adv_theta,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_adv_u,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_adv_v,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_beta,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_dq0,          SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_d_theta0,     SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ug,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_vg,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_fc,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_q,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_theta,  SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_u,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_v,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_h0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_name,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ps,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_q0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_theta0,       SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_time,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_timestep,     SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_u0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ustar,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_v0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wq,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ws,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wtheta,       SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->switch_wind,        SIGNAL(clicked()),         this, SLOT(updateInputdata()));
  connect(ui->input_name,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_name,         SIGNAL(editingFinished()), this, SLOT(updateRunName()));

}


void plotwindow::getdata(outputvar *data, modelrun n, QString plotvar)
{
  if(!n.hasrun)
    return;

  // mixed-layer
  if (plotvar == "t")
    *data = n.run->output->t;
  else if (plotvar == "h")
    *data = n.run->output->h;
  else if (plotvar == "Ps")
    *data = n.run->output->Ps;
  else if (plotvar == "beta")
    *data = n.run->output->beta;
  else if (plotvar == "ws")
    *data = n.run->output->ws;

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
    *data = n.run->output->gammaq;
  else if (plotvar == "advu")
    *data = n.run->output->advq;
  else if (plotvar == "v")
    *data = n.run->output->v;
  else if (plotvar == "dv")
    *data = n.run->output->dv;
  else if (plotvar == "gammav")
    *data = n.run->output->gammaq;
  else if (plotvar == "advv")
    *data = n.run->output->advq;

  // surface-layer
  else if (plotvar == "ustar")
    *data = n.run->output->ustar;
  else if (plotvar == "uw")
    *data = n.run->output->uw;
  else if (plotvar == "vw")
    *data = n.run->output->vw;

}


void plotwindow::setplotvar(const QString label, QString *plotvar)
{
  // mixed-layer
  if (label == "t")
    *plotvar = label;
  else if (label == "h")
    *plotvar = label;
  else if (label == "Ps")
    *plotvar = label;
  else if (label == "beta")
    *plotvar = label;
  else if (label == "ws")
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
  else if (label == "v")
    *plotvar = label;
  else if (label == "dv")
    *plotvar = label;
  else if (label == "gammav")
    *plotvar = label;
  else if (label == "advv")
    *plotvar = label;

  // surface-layer
  else if (label == "ustar")
    *plotvar = label;
  else if (label == "uw")
    *plotvar = label;
  else if (label == "vw")
    *plotvar = label;
}
