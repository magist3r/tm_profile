/*
tm_profile - program for calculation of cylinder-bevel transmissions
Copyright (C) 2011 Sergey Lopatin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profile.h"
#include "math.h"
#include <QSettings>
#include <QDebug>

Profile profile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&profile, SIGNAL(addToDebugConsole(QString)), this, SLOT(addToDebugConsole(QString)));
    connect(&profile, SIGNAL(finished()), this, SLOT(drawArea()));

    QSettings settings("tm_profile", "zb-susu");
    settings.beginGroup("properties");
    QStringList groups = settings.childGroups();
    for (int i = 0; i < groups.count(); i++)
    {
        new QListWidgetItem(groups[i], ui->settingsList);
    }
    ui->radioButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setVars()
{
    profile.m = ui->m->value();
    profile.z1 = ui->z1->value();
    profile.z2 = ui->z2->value();
    profile.x2 = ui->x2->value();
    profile.E = ui->E->value() * M_PI / 180;
    profile.W0 = ui->W0->value();
    profile.alpha = ui->alpha->value() * M_PI / 180;
    profile.ha = ui->ha->value();
    profile.c = ui->c->value();
    profile.z0 = ui->z0->value();
    profile.x0 = ui->x0->value();
    profile.da0 = ui->da0->value();

    profile.d0 = ui->d0->value();
    profile.bw = ui->bw->value();
    profile.ra2 = ui->ra2->value();
    profile.rf2 = ui->rf2->value();
    profile.dx = ui->dx->value();
    profile.dx_0 = ui->dx_0->value();
    profile.dx_bw = ui->dx_bw->value();

    profile.n_W = ui->detalization->value();
    profile.n_r = ui->detalization->value();

    profile.useSmooth = ui->radioButton->isChecked();
}

void MainWindow::on_pushButton_clicked()
{
    setVars();
    if (profile.getRadius())
    {
        ui->ra2->setValue(profile.ra2);
        ui->rf2->setValue(profile.rf2);
    }

}

void MainWindow::on_MainButton_clicked()
{
    setVars();
    profile.start();
    ui->textBrowser->clear();
}

void MainWindow::saveProperties()
{
    QSettings settings("tm_profile", "zb-susu");
    settings.beginGroup("properties");
    const QString name = "m_" + QString::number(ui->m->value()) +
                         "|z1_" + QString::number(ui->z1->value()) +
                         "|z2_" + QString::number(ui->z2->value()) +
                         "|bw_" + QString::number(ui->bw->value());
    settings.beginGroup(name);
    settings.setValue("MainWindow", saveState());
    settings.setValue("m", ui->m->value());
    settings.setValue("z1", ui->z1->value());
    settings.setValue("z2", ui->z2->value());
    settings.setValue("x0", ui->x0->value());
    settings.setValue("x2", ui->x2->value());
    settings.setValue("alpha", ui->alpha->value());
    settings.setValue("c", ui->c->value());
    settings.setValue("z0", ui->z0->value());
    settings.setValue("x0", ui->x0->value());
    settings.setValue("da0", ui->da0->value());
    settings.setValue("ha", ui->ha->value());
    settings.setValue("E", ui->E->value());
    settings.setValue("W0", ui->W0->value());
    settings.setValue("d0", ui->d0->value());
    settings.setValue("bw", ui->bw->value());
    settings.setValue("ra2", ui->ra2->value());
    settings.setValue("rf2", ui->rf2->value());
    settings.setValue("dx", ui->dx->value());
    settings.setValue("dx_0", ui->dx_0->value());
    settings.setValue("dx_bw", ui->dx_bw->value());
    settings.setValue("detalization", ui->detalization->value());

    settings.endGroup();
    settings.endGroup();
}

void MainWindow::loadProperties(QString value = "")
{
   QSettings settings("tm_profile", "zb-susu");
   settings.beginGroup("properties");

   if (value == "")
   {
       const QString name = "m_" + QString::number(ui->m->value()) +
                         "|z1_" + QString::number(ui->z1->value()) +
                         "|z2_" + QString::number(ui->z2->value()) +
                         "|bw_" + QString::number(ui->bw->value());

       settings.beginGroup(name);
   }
   else
   {
       settings.beginGroup(value);
   }

   restoreState(settings.value("MainWindow").toByteArray());
   ui->m->setValue(settings.value("m").toDouble());
   ui->z1->setValue(settings.value("z1").toDouble());
   ui->z2->setValue(settings.value("z2").toDouble());
    ui->x0->setValue(settings.value("x0").toDouble());
    ui->x2->setValue(settings.value("x2").toDouble());
    ui->alpha->setValue(settings.value("alpha").toDouble());
    ui->c->setValue(settings.value("c").toDouble());
    ui->z0->setValue(settings.value("z0").toDouble());
    ui->da0->setValue(settings.value("da0").toDouble());
    ui->ha->setValue(settings.value("ha").toDouble());
    ui->E->setValue(settings.value("E").toDouble());
    ui->W0->setValue(settings.value("W0").toDouble());
    ui->d0->setValue(settings.value("d0").toDouble());
    ui->bw->setValue(settings.value("bw").toDouble());
    ui->ra2->setValue(settings.value("ra2").toDouble());
    ui->rf2->setValue(settings.value("rf2").toDouble());
    ui->dx->setValue(settings.value("dx").toDouble());
    ui->dx_0->setValue(settings.value("dx_0").toDouble());
    ui->dx_bw->setValue(settings.value("dx_bw").toDouble());
    ui->detalization->setValue(settings.value("detalization").toInt());
    settings.endGroup();
    settings.endGroup();

}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->m->value() == 0 || ui->z1->value() == 0 || ui->z2->value() == 0 || ui->bw->value() == 0)
    {

    }
    else
    {
        saveProperties();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (ui->settingsList->currentItem() != 0)
    {
         loadProperties(ui->settingsList->currentItem()->text());
    }
    else
    {
         loadProperties();
    }
}

void MainWindow::addToDebugConsole(QString text)
{
    ui->textBrowser->append(text);
}

void MainWindow::drawArea()
{
    if (profile.diagnosticMode)
    {
        profile.diagnosticMode = false;
    }
    else
    {
        ui->label_xt_w->setText("xt(w) = " + QString::number(profile.xt_w[2]) + "w^2 " + QString::number(profile.xt_w[1]) + "w +" + QString::number(profile.xt_w[0]));
        ui->PaintContactArea->delta = ui->delta->value();
        ui->PaintContactArea->drawImage(&profile);
        if (ui->diagnostic->isChecked())
        {
            profile.diagnosticMode = true;
            profile.n_r = 5;
            profile.n_W = 5;
            profile.start();
        }
    }
}

