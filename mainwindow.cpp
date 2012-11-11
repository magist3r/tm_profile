///*
//tm_profile - program for calculation of cylinder-bevel transmissions
//Copyright (C) 2011 Sergey Lopatin

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program. If not, see <http://www.gnu.org/licenses/>.
//*/

///*#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include "profile.h"
//#include "math.h"
//#include <QSettings>
//#include <QDebug>

//Profile profile;
//QMap<double, QMap<double, double> > s_tr_map;


//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);

//    connect(&profile, SIGNAL(addToDebugConsole(QString)), this, SLOT(addToDebugConsole(QString)));

//    QSettings settings("tm_profile", "zb-susu");
//    settings.beginGroup("properties");
//    QStringList groups = settings.childGroups();
//    for (int i = 0; i < groups.count(); i++)
//    {
//        new QListWidgetItem(groups[i], ui->settingsList);
//    }
//    ui->radioButton_2->setChecked(true);
//}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}

//void MainWindow::setVars()
//{
//    /*profile.m_m = ui->m->value();
//    profile.m_z1 = ui->z1->value();
//    profile.m_z2 = ui->z2->value();
//    profile.m_x2 = ui->x2->value();
//    profile.m_E = ui->E->value() * M_PI / 180;
//    profile.m_W0 = ui->W0->value();
//    profile.m_alpha = ui->alpha->value() * M_PI / 180;
//    profile.m_ha = ui->ha->value();
//    profile.m_c = ui->c->value();
//    profile.m_z0 = ui->z0->value();
//    profile.m_x0 = ui->x0->value();
//    profile.m_da0 = ui->da0->value();

//    profile.m_d0 = ui->d0->value();
//    profile.m_bw = ui->bw->value();
//    profile.m_ra2 = ui->ra2->value();
//    profile.m_rf2 = ui->rf2->value();
//    profile.m_dx = ui->dx->value();
//    profile.m_dx_0 = ui->dx_0->value();
//    profile.m_dx_bw = ui->dx_bw->value();

//    profile.m_nW = ui->detalization->value();
//    profile.m_nr = ui->detalization->value();

//    profile.m_useSmooth = ui->radioButton->isChecked();
//    profile.m_useOldPaintMode = ui->oldPaintMode->isChecked();*/
//}

//void MainWindow::on_pushButton_clicked()
//{
// /*   setVars();
//    if (profile.getRadius())
//    {
//        ui->ra2->setValue(profile.m_ra2);
//        ui->rf2->setValue(profile.m_rf2);
//    }
//*/
//}

//void MainWindow::on_MainButton_clicked()
//{
//    setVars();

//    MyThread thread(profile);
//    connect(&thread, SIGNAL(finished()), this, SLOT(drawArea()));

//    thread.start();
//    ui->textBrowser->clear();
//}



//void MainWindow::on_pushButton_2_clicked()
//{
//    if (ui->m->value() == 0 || ui->z1->value() == 0 || ui->z2->value() == 0 || ui->bw->value() == 0)
//    {

//    }
//    else
//    {
//        //saveProperties();
//    }
//}

///*void MainWindow::on_pushButton_3_clicked()
//{

//    if (ui->settingsList->currentItem() != 0)
//    {
//         loadProperties(ui->settingsList->currentItem()->text());
//    }
//    else
//    {
//         loadProperties();
//    }
//}*/

//void MainWindow::addToDebugConsole(QString text)
//{
//    ui->textBrowser->append(text);
//}

//void MainWindow::drawArea()
//{
//    /*if (profile.m_diagnosticMode)
//    {
//        profile.m_diagnosticMode = false;
//    }
//    else
//    {
//        ui->label_xt_w->setText("xt(w) = " + QString::number(profile.m_xt_w[2]) + "w^2 " + QString::number(profile.m_xt_w[1]) + "w +" + QString::number(profile.m_xt_w[0]));
//        ui->PaintContactArea->delta = ui->delta->value();
//        ui->PaintContactArea->drawImage(&profile);
//        if (ui->diagnostic->isChecked())
//        {
//            profile.m_diagnosticMode = true;
//            profile.m_nr = 4;
//            profile.m_nW = 5;
//            MyThread thread(profile);
//            connect(&thread, SIGNAL(finished()), this, SLOT(drawArea()));
//            thread.start();
//        }
//    }*/
//}

//void MainWindow::drawArea_d_x2()
//{
//    //ui->label_xt_w->setText("xt(w) = " + QString::number(profile.xt_w[2]) + "w^2 " + QString::number(profile.xt_w[1]) + "w +" + QString::number(profile.xt_w[0]));
//    //ui->PaintContactArea->delta = ui->delta->value();
//   /* double r_av_0 = (s_tr_map.begin().value().begin().key() + s_tr_map.begin().value().end().key()) / 2;
//    double s_0 = s_tr_map.begin().value().upperBound(r_av_0).value() / 2;*/
//    //qDebug() << r_av_0 << s_0;
//   // QMap<double,double> s_tr_w0 = s_tr_map.begin().value();
//  /*  QMapIterator<double, QMap<double,double> > i(s_tr_map);
//    QMapIterator<double, QMap<double,double> > i2(profile.result_s_tr);
//    ui->textBrowser->clear();
//    profile.result.clear();
//    double w_begin = 0;
//    double w_end = 0;
//    double s_prev;
//    bool draw = false;
//    double s_max = 0;
//    while (i.hasNext()) {
//        i.next();
//        double r_av = (i.value().begin().key() + i.value().end().key()) / 2;
//        double s = i.value().upperBound(r_av).value() / 2;
//        if (s > s_max) {
//            s_max = s;
//            w_begin = i.key();
//            w_end = w_begin;
//        }
//        if (i2.hasNext()) {
//            i2.next();
//            double r_av2 = (i2.value().begin().key() + i.value().end().key()) / 2;
//            double s2 = i2.value().upperBound(r_av2).value() / 2;
//            if (w_end == i.key())
//                s_prev = s2;

//            if ((fabs(s2 - s_max) < 0.001)
//                && (s_prev != s2)
//                && (fabs(s_prev - s_max) > fabs(s2 - s_max)) ) {
//                w_end = i2.key();
//            }


//      /*      ui->textBrowser->append("W = " + QString::number(i.key()));
//            ui->textBrowser->append("PR1: r_av = " + QString::number(r_av) + "; s = " + QString::number(s));
//            ui->textBrowser->append("PR2: r_av = " + QString::number(r_av2) + "; s = " + QString::number(s2));*/
//  //      }
//   // }

// /*   double delta_w = w_end - w_begin;
//    qDebug() << delta_w;
//    if (delta_w == 0) {
//        ui->textBrowser->append("Delta_w NOT FOUND (delta_w = 0)");
//        return;
//    }
//*/
// /*   double delta_s_max = 0;
//    QMapIterator<double, QMap<double,double> > ii(profile.result_s_tr);
//    ii.toBack();
//    while (ii.hasPrevious()) {
//        ii.previous();
//        //qDebug() << "AAAAAAAAAAAA" << s_tr_map.lowerBound(.key();
//        if ((ii.key() - delta_w) > 0) {
//            QMapIterator<double,double> j(ii.value());
//            QMapIterator<double,double> k(s_tr_map.value(ii.key() - delta_w));
//            while (j.hasNext()) {
//                j.next();

//                double first_s = j.value();
//                //qDebug() << first_s;


//                if (k.hasNext()) {
//                    k.next();
//                    double second_s = k.value();
//                    double delta_s = (first_s - second_s) / 2;
//                    if (delta_s > delta_s_max)
//                        delta_s_max = delta_s;
//                    if (ui->diagnostic->isChecked()) {
//                        ui->textBrowser->append("W = " + QString::number(ii.key()));
//                        ui->textBrowser->append("PR1: s = " + QString::number(first_s) + "; PR2: s = " + QString::number(second_s));
//                        ui->textBrowser->append("delta_s = " + QString::number(delta_s));
//                    }
//                    //qDebug() << first_s << second_s;
//                    profile.result[ii.key() - delta_w][j.key()] = delta_s;
//                }

//       //   qDebug() << s_pr << j.value();
//            }
//        } else
//            break;
//    }
//    ui->textBrowser->append("delta_W = " + QString::number(delta_w));
//    profile.m_delta_s_max = delta_s_max;
///*

//    double s_tr_0 = s_tr_map.begin().value().upperBound(2).value();
//    double s_pr_0 = profile.result_s_pr.begin().value().upperBound(2).value();
//    qDebug() << s_tr_0 << s_pr_0;
//    QMap<double, QMap<double,double> > first_map;
//    QMap<double, QMap<double,double> > second_map;
//    double s;
//    if (s_pr_0 > s_tr_0) {
//        first_map = profile.result_s_pr;
//        second_map = s_tr_map;
//        s = s_tr_0;
//    } else {
//        first_map = s_tr_map;
//        second_map = profile.result_s_pr;
//        s = s_pr_0;
//    }
//    profile.result.clear();
//    bool draw = false;
//    double w_draw = 0.0;
//    double first_s;
//    double second_s;

//    QMapIterator<double, QMap<double,double> > i(first_map);
//    while (i.hasNext()) {
//        i.next();
//        first_s = i.value().upperBound(2).value();
//       // qDebug() << first_s << s;
//        if (!draw && ((first_s - s) / 2 < 0.01) ) {
//            draw = true;
//            w_draw = i.key();
//            qDebug() << w_draw;
//        }

// //       if(draw) {
//            QMapIterator<double,double> j(i.value());
//        //    j.toBack();
//            QMapIterator<double,double> k(second_map.value(i.key() - w_draw));
//          //  k.toBack();
//            while (j.hasNext()) {
//                j.next();

//                first_s = j.value();
//                //qDebug() << first_s;


//                if (k.hasNext()) {
//                    k.next();
//                    second_s = k.value();
//                    qDebug() << first_s << second_s;
//                    profile.result[i.key() - w_draw][j.key()] = (first_s - second_s) / 2;
//                }

//       //   qDebug() << s_pr << j.value();
//            }
//        }
//    }*/
//    ui->PaintContactArea->drawImage(&profile);
//}


//void MainWindow::on_pushButton_4_clicked()
//{
//    setVars();
//    MyThread thread(profile);
//    thread.start();

//    connect(&thread, SIGNAL(finished()), this, SLOT(startSecondThread()));
//}

//void MainWindow::startSecondThread()
//{
//  /*  s_tr_map = profile.result_s_tr;
//    profile.result_s_tr.clear();
//    profile.m_x2 = profile.m_x2 + ui->delta_x2->value() * 1.0 / 100;
//  //  qDebug() << profile.x2;
//    MyThread thread(profile);
//    connect(&thread, SIGNAL(finished()), this, SLOT(drawArea_d_x2()));
//    thread.start();*/
//}
