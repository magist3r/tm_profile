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

#include "profile.h"
#include "math.h"
#include <QSettings>
#include <QDebug>

Profile::Profile(QObject *parent)
{

}

Profile::Profile(const Profile&)
{

}

Profile& Profile::operator=(const Profile&)
{

}

Profile::~Profile()
{

}

bool Profile::getRadius()
{
    double alpha_w02;

    double inv_alpha_w02 = ((m_x2 - m_x0) / (m_z2 - m_z0)) * 2 * tan(m_alpha) + tan(m_alpha) - m_alpha;

    double alpha_n = M_PI / 180;
    double alpha_k = 89 * M_PI / 180;
    while (alpha_k - alpha_n > m_eps)
    {
         alpha_w02 = (alpha_n + alpha_k) / 2;
         if ((inv_alpha_w02 - (tan(alpha_w02) - alpha_w02)) > 0)
         {
            alpha_n = alpha_w02;
         }
         else
         {
            alpha_k = alpha_w02;
         }
    }
    m_rf2 = (m_m * (m_z2 - m_z0) * (cos(m_alpha)/cos(alpha_w02)) + m_da0) / 2;
    m_ra2 = m_rf2 - m_m * (2 * m_ha + m_c);
    return true;
}

void Profile::calculate()
{
    m_i21 = m_z1 / m_z2;
    m_rb2 = 0.5 * m_m * m_z2 * cos(m_alpha);
    m_delta2 = atan(sin(m_E) / (cos(m_E) - m_i21));
    m_psi_b2 = M_PI / (2 * m_z2) + 2 * m_x2 * tan(m_alpha) / m_z2 + tan(m_alpha) - m_alpha;
    result.clear();
    result_s_tr.clear();
    result_s_pr.clear();

    double dW = m_bw / m_nW; // Шаг торцовых сечений
    double rav2 = (m_rf2 + m_ra2 - m_c * m_m) / 2;
    double dr = (m_rf2 - m_ra2 - m_c * m_m) / m_nr; // Шаг радиусов

    double x_tr = 0.0, y_tr = 0.0;

    QList<double> list;

    QMap<double, double> S;

    double Wi = m_W0;
    // Расчет коэффициентов смещения
    for (int i=0; i <= m_nW; i++)
    {
        double delta_oi = m_E; // Угол аксоидного конуса шестерни
        double alpha1 = atan(tan(m_alpha) * cos(delta_oi));
        double rb1 = 0.5 * m_m * m_z1 * cos(alpha1);

        double ry1 = rav2 / cos(m_E) - (Wi + 0.1 * m_m) * tan(m_E);
        a_tw(ry1, Wi + 0.1 * m_m, x_tr, y_tr);
        double psi_yi = x_tr / ry1;
        double alpha_y1 = acos(rb1 / ry1);
        double xt1 = (m_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

        ry1 = rav2 / cos(m_E) - (Wi) * tan(m_E);
        a_tw(ry1, Wi, x_tr, y_tr);
        psi_yi = x_tr / ry1;
        alpha_y1 = acos(rb1 / ry1);
        double xt2 = (m_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

        delta_oi = atan(10 * (xt2 - xt1));
        alpha1 = atan(tan(m_alpha) * cos(delta_oi));
        rb1 = 0.5 * m_m * m_z1 * cos(alpha1);
        alpha_y1 = acos(rb1 / ry1);
        double xt = (m_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1)); // Коэф. смещения

        S[Wi - m_W0] = xt;

        Wi += dW;
    }

    m_xt_w = square_method(S);

    // Задание модификации
    if(m_dx != 0 || m_dx_0 != 0 || m_dx_bw != 0)
    {
        S.clear();

        S[0] = m_xt_w[2] * 0 + m_xt_w[1] * 0 + m_xt_w[0] - m_dx_0;
        S[m_bw/2] = m_xt_w[2] * pow(m_bw/2,2) + m_xt_w[1] * m_bw/2 + m_xt_w[0] + m_dx;
        S[m_bw] = m_xt_w[2] * pow(m_bw,2) + m_xt_w[1] * m_bw + m_xt_w[0] - m_dx_bw;

        m_xt_w = square_method(S);
    }

    double w0 = -m_xt_w[1] / (2 * m_xt_w[2]);

    Wi = m_W0;

    m_delta_s_max=0;
    // Расчет толщин зубьев
    for (int i=0; i <= m_nW; i++)
    {
        double wi = Wi - m_W0;
        double delta_oi = -atan(m_m * (2 * m_xt_w[2] * wi + m_xt_w[1]));
        double xt = m_xt_w[2] * pow(wi, 2) + m_xt_w[1] * wi + m_xt_w[0];
        double aw0 = m_m * (0.5 * m_z1 + xt) + 0.5 * m_d0 * cos(delta_oi);
        double ha_2 = m_ha / cos(delta_oi);
        double c_2 = m_c / cos(delta_oi);
        double d = m_m * m_z1;
        double alpha_t = atan(tan(m_alpha) * cos(delta_oi));
        double d_b = d * cos(alpha_t);
        double ry1_min = m_ra2 / cos(m_E) - (m_W0 + m_bw) * tan(m_E);
        double ry2 = m_ra2;

        for (int j=0; j <= m_nr; j++)
        {

            double ry1 = ry2 / cos(m_E) - Wi * tan(m_E);

            // Теоретический профиль
            a_tw(ry1, Wi, x_tr, y_tr);
            double s_tr = 2 * ry1 * atan(x_tr / y_tr); // Толщина зуба теоретическая

            // Практический профиль
            double alpha_ty = acos(0.5 * d_b / ry1);
            double st = m_m * (M_PI / 2 + 2 * xt * tan(m_alpha) * cos(delta_oi));
            double s_pr = ry1 * (2 * st / d + 2 * (tan(alpha_t) - alpha_t) - 2 * (tan(alpha_ty) - alpha_ty)); // Толщина зуба практическая
            double delta_s = (s_pr - s_tr) / 2;
            if (delta_s > m_delta_s_max)
                m_delta_s_max = delta_s;

            result[wi][ry1 - ry1_min] = delta_s;
            result_s_tr[wi][ry1 - ry1_min] = s_tr;
            result_s_pr[wi][ry1 - ry1_min] = s_pr;

            if (m_diagnosticMode)
            {
                emit addToDebugConsole("Wi= " + QString::number(wi) + " | ry= " + QString::number(ry1) + " | s_tr = " + QString::number(s_tr) + " | delta_s = " + QString::number(delta_s));
            }

            ry2 += dr;
        }
        Wi += dW;
    }
}

QList<double> Profile::square_method(const QMap<double, double> &S)
{
    double sum_x = 0;
    double sum_x2 = 0;
    double sum_x3 = 0;
    double sum_x4 = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_x2y = 0;
    QList<double> X; // Матрица результатов (коэффициенты уравнения)

    int n = 0;
    QMap<double, double>::const_iterator i;
    for (i = S.begin(); i != S.end(); ++i)
    {
        sum_x += i.key();
        sum_x2 += pow(i.key(), 2);
        sum_x3 += pow(i.key(), 3);
        sum_x4 += pow(i.key(), 4);
        sum_y += i.value();
        sum_xy += i.key() * i.value();
        sum_x2y += pow(i.key(), 2) * i.value();
        n++;
    }
    double A[3][3] = // Основная матрица
    {
        {n, sum_x, sum_x2},
        {sum_x, sum_x2, sum_x3},
        {sum_x2, sum_x3, sum_x4}
    };

    double B[3] = // Матрица коэффициентов
    {
        sum_y,
        sum_xy,
        sum_x2y
    };

    double detA = det(A);
    double A_bkp[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int a = 0; a < 3; a++)
        {
            for (int b = 0; b < 3; b++)
            {
                A_bkp[a][b] = A[a][b];

            }
        }
        for (int j = 0; j < 3; j++)
        {
            A_bkp[j][i] = B[j];
        }
        X.append(det(A_bkp) / detA);
    }

   /* double sum = 0;
    for (i = S.begin(); i != S.end(); ++i)
    {
        sum += pow(i.value() - (X[2] * pow(i.key(),2) + X[1] * i.key() + X[0]), 2);
    }
    qDebug() << "square" << sum;*/

    return X;
}

void Profile::a_tw (double ry1, double Wi, double &x_tr, double &y_tr)
{
    double vy2, p, q, alpha_tw, phi1;
    double alpha_tw_n = M_PI / 180;
    double alpha_tw_k = acos((Wi/m_rb2 - sqrt(pow(Wi/m_rb2,2) - 2 * sin(2*m_E) / tan(m_delta2))) / (2 * sin(m_E)));
    while (alpha_tw_k - alpha_tw_n > m_eps)
    {
        alpha_tw = (alpha_tw_n + alpha_tw_k) / 2;
        vy2 = (Wi / (m_rb2 * sin(m_E)) - (1 / (tan(m_E) * tan(m_delta2) * cos(alpha_tw))) - cos(alpha_tw)) / sin(alpha_tw);
        p = cos(m_E) * (cos(alpha_tw) + vy2 * sin(alpha_tw)) - sin(m_E) / (cos(alpha_tw) * tan(m_delta2));
        q = sin(alpha_tw) - vy2 * cos(alpha_tw);
        if ((m_rb2 * sqrt(pow(q,2) + pow(p,2)) - ry1) > 0)
        {
             alpha_tw_n = alpha_tw;
        }
        else
        {
             alpha_tw_k = alpha_tw;
        }
    }
    phi1 = (alpha_tw + m_psi_b2 - vy2) / m_i21;
    x_tr = m_rb2 * (p * sin(phi1) - q * cos(phi1));
    y_tr = m_rb2 * (p * cos(phi1) + q * sin(phi1));
    return;
}

double Profile::det(const double A[3][3])
{
   return A[0][0] * A[1][1] * A[2][2] +
          A[0][1] * A[1][2] * A[2][0] +
          A[1][0] * A[2][1] * A[0][2] -
          A[2][0] * A[1][1] * A[0][2] -
          A[1][0] * A[0][1] * A[2][2] -
          A[2][1] * A[1][2] * A[0][0];
}

void Profile::saveMainSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "zb-susu", "tm_profile");
    const QString name = "m_" + QString::number(m_m) +
                         "|z1_" + QString::number(m_z1) +
                         "|z2_" + QString::number(m_z2) +
                         "|bw_" + QString::number(m_bw);
    settings.beginGroup(name);
    settings.setValue("m", m_m);
    settings.setValue("z1", m_z1);
    settings.setValue("z2", m_z2);
    settings.setValue("bw", m_bw);
    settings.setValue("W0", m_W0);
    settings.setValue("E", m_E);
    settings.setValue("x2", m_x2);
    settings.setValue("d0", m_d0);
    settings.setValue("ra2", m_ra2);
    settings.setValue("rf2", m_rf2);
    settings.endGroup();
}

void Profile::saveOtherSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "zb-susu", "tm_profile");
    settings.beginGroup("OtherSettings");
    settings.setValue("alpha", m_alpha);
    settings.setValue("c", m_c);
    settings.setValue("z0", m_z0);
    settings.setValue("x0", m_x0);
    settings.setValue("da0", m_da0);
    settings.setValue("ha", m_ha);
    settings.endGroup();
}

void Profile::saveLastSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "zb-susu", "tm_profile");
    settings.beginGroup("LastSettings");
    settings.setValue("dx", m_dx);
    settings.setValue("dx_0", m_dx_0);
    settings.setValue("dx_bw", m_dx_bw);
    //settings.setValue("detalization", m_detalization);
    settings.endGroup();
}

QStringList Profile::listOfParameters()
{
    if (m_listOfParameters.empty()) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "zb-susu", "tm_profile");
        QRegExp rx("m_*");
        rx.setPatternSyntax(QRegExp::Wildcard);
        m_listOfParameters = settings.childGroups().filter(rx);

    }
    return m_listOfParameters;
}

void Profile::convertSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "zb-susu", "tm_profile");
    QSettings settings2("tm_profile", "zb-susu");

    settings2.beginGroup("properties");
    QStringList groups = settings2.childGroups();
    for (int i = 0; i < groups.count(); i++)
    {
        settings2.beginGroup(groups[i]);
        settings.beginGroup(groups[i]);
        settings.setValue("m", settings2.value("m"));
        settings.setValue("z1", settings2.value("z1"));
        settings.setValue("z2", settings2.value("z2"));
        settings.setValue("bw", settings2.value("bw"));
        settings.setValue("W0", settings2.value("W0"));
        settings.setValue("E", settings2.value("E"));
        settings.setValue("x2", settings2.value("x2"));
        settings.setValue("d0", settings2.value("d0"));
        settings.setValue("ra2", settings2.value("ra2"));
        settings.setValue("rf2", settings2.value("rf2"));
        settings.endGroup();
        settings2.endGroup();
    }
    settings2.endGroup();
}

void Profile::loadSettings(QString value)
{
   QSettings settings(QSettings::IniFormat, QSettings::UserScope, "zb-susu", "tm_profile");
   qDebug() << value;

   if (value == "")
   {
       settings.beginGroup("LastSettings");
   }
   else
   {
       settings.beginGroup(value);
   }
   if (value == "OtherSettings") {

   }
   else {
       setM(settings.value("m").toDouble());
       qDebug() << settings.value("m").toDouble();
       setZ1(settings.value("z1").toDouble());
       setZ2(settings.value("z2").toDouble());
       setBw(settings.value("bw").toDouble());
       setW0(settings.value("W0").toDouble());
       setE(settings.value("E").toDouble());
       setX2(settings.value("x2").toDouble());
       setD0(settings.value("d0").toDouble());
       //setrsettings.setValue("ra2", m_ra2);
       //settings.setValue("rf2", m_rf2);
   }

   settings.endGroup();
}


   /*  ui->m->setValue(settings.value("m").toDouble());
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

}*/
