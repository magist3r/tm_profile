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
#include <QStandardPaths>
#include <QDebug>

Profile::Profile(QObject *parent)
{
    m_m = 2;
    m_z0 = 19;
    m_x0 = 0.105;
    m_da0 = 43.42;
    setAlpha(20);
    m_ha = 1;
    m_c = 0.25;
    m_nr = 200;
    m_nW = 250;
    setMod0(0.0);
    setModCenter(0.0);
    setModBw(0.0);
    setUseManualXtList(false);
    setListOfParameters();


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

    double inv_alpha_w02 = ((m_x2 - m_x0) / (m_z2 - m_z0)) * 2 * tan(m_alpha_rad) + tan(m_alpha_rad) - m_alpha_rad;

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
    setRf2((m_m * (m_z2 - m_z0) * (cos(m_alpha_rad)/cos(alpha_w02)) + m_da0) / 2);
    setRa2(m_rf2 - m_m * (2 * m_ha + m_c));
    return true;
}

void Profile::calculate()
{
    double imageHeight = (m_rf2 - m_ra2 - m_c * m_m) / cos(m_E_rad);
    double imageWidth = m_bw;
    m_result.clear();

    double i21 = z1() / z2();
    m_rb2 = 0.5 * m_m * m_z2 * cos(m_alpha_rad);
    m_delta2 = atan(sin(m_E_rad) / (cos(m_E_rad) - i21));
    m_psi_b2 = M_PI / (2 * m_z2) + 2 * m_x2 * tan(m_alpha_rad) / m_z2 + tan(m_alpha_rad) - m_alpha_rad;

    double dW = m_bw / m_nW; // Шаг торцовых сечений
    double rav2 = (m_rf2 + m_ra2 - m_c * m_m) / 2;
    double dr = (m_rf2 - m_ra2 - m_c * m_m) / m_nr; // Шаг радиусов

    double x_tr = 0.0, y_tr = 0.0;

    QList<double> list;

    QMap<double, double> XtList;

    double Wi = m_W0;
    // Расчет коэффициентов смещения
    if (m_useManualXtList) {
        m_trajectory = squareMethod(manualXtList());
    } else {
        for (int i=0; i <= m_nW; i++) {
            double delta_oi = m_E_rad; // Угол аксоидного конуса шестерни
            double alpha1 = atan(tan(m_alpha_rad) * cos(delta_oi));
            double rb1 = 0.5 * m_m * m_z1 * cos(alpha1);

            double ry1 = rav2 / cos(m_E_rad) - (Wi + 0.1 * m_m) * tan(m_E_rad);
            a_tw(ry1, Wi + 0.1 * m_m, x_tr, y_tr);
            double psi_yi = x_tr / ry1;
            double alpha_y1 = acos(rb1 / ry1);
            double xt1 = (m_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

            ry1 = rav2 / cos(m_E_rad) - (Wi) * tan(m_E_rad);
            a_tw(ry1, Wi, x_tr, y_tr);
            psi_yi = x_tr / ry1;
            alpha_y1 = acos(rb1 / ry1);
            double xt2 = (m_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

            delta_oi = atan(10 * (xt2 - xt1));
            alpha1 = atan(tan(m_alpha_rad) * cos(delta_oi));
            rb1 = 0.5 * m_m * m_z1 * cos(alpha1);
            alpha_y1 = acos(rb1 / ry1);
            double xt = (m_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1)); // Коэф. смещения

            XtList[Wi - m_W0] = xt;

            Wi += dW;
        }

        m_trajectory = squareMethod(XtList);

    }

  //  saveTrajectory();

    // Задание модификации
    if( !useManualXtList() && (mod0() != 0 || modCenter() != 0 || modBw() != 0) ) {
        XtList.clear();

        XtList[0] = m_trajectory[2] * 0 + m_trajectory[1] * 0 + m_trajectory[0] + mod0();
        XtList[bw()/2] = m_trajectory[2] * pow(bw() / 2, 2) + m_trajectory[1] * bw() / 2 + m_trajectory[0] + modCenter();
        XtList[bw()] = m_trajectory[2] * pow(bw(), 2) + m_trajectory[1] * bw() + m_trajectory[0] + modBw();

        m_trajectory = squareMethod(XtList);
    }

    double w0 = -m_trajectory[1] / (2 * m_trajectory[2]);

    Wi = m_W0;

    m_delta_s_max=0;
    // Расчет толщин зубьев
    for (int i=0; i <= m_nW; i++) {
        double wi = Wi - m_W0;
        double delta_oi = -atan(m_m * (2 * m_trajectory[2] * wi + m_trajectory[1]));
        double xt = m_trajectory[2] * pow(wi, 2) + m_trajectory[1] * wi + m_trajectory[0];
        double aw0 = m_m * (0.5 * m_z1 + xt) + 0.5 * m_d0 * cos(delta_oi);
        double ha_2 = m_ha / cos(delta_oi);
        double c_2 = m_c / cos(delta_oi);
        double d = m_m * m_z1;QString getModName();
        double alpha_t = atan(tan(m_alpha_rad) * cos(delta_oi));
        double d_b = d * cos(alpha_t);
        double ry1_min = m_ra2 / cos(m_E_rad) - (m_W0 + m_bw) * tan(m_E_rad);
        double ry2 = m_ra2;

        for (int j=0; j <= m_nr; j++) {
            double ry1 = ry2 / cos(m_E_rad) - Wi * tan(m_E_rad);

            // Теоретический профиль
            a_tw(ry1, Wi, x_tr, y_tr);
            double s_tr = 2 * ry1 * atan(x_tr / y_tr); // Толщина зуба теоретическая

            // Практический профиль
            double alpha_ty = acos(0.5 * d_b / ry1);
            double st = m_m * (M_PI / 2 + 2 * xt * tan(m_alpha_rad) * cos(delta_oi));
            double s_pr = ry1 * (2 * st / d + 2 * (tan(alpha_t) - alpha_t) - 2 * (tan(alpha_ty) - alpha_ty)); // Толщина зуба практическая
            double delta_s = (s_pr - s_tr) / 2;
            if (delta_s > m_delta_s_max)
                m_delta_s_max = delta_s;

            m_result[wi][ry1 - ry1_min] = delta_s;

            ry2 += dr;
        }
        Wi += dW;
    }
    QString baseName = getBaseName();
    if (m_useManualXtList)
        baseName += "_manual";
    else
        baseName += "_" + getModName();

    emit calculateFinished(m_result, 0.006 * sqrt(m_m), m_delta_s_max, imageWidth, imageHeight, baseName);
}

QList<double> Profile::squareMethod(const QMap<double, double> &S)
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
    for (i = S.begin(); i != S.end(); ++i) {
        sum_x += i.key();
        sum_x2 += pow(i.key(), 2);
        sum_x3 += pow(i.key(), 3);
        sum_x4 += pow(i.key(), 4);
        sum_y += i.value();
        sum_xy += i.key() * i.value();
        sum_x2y += pow(i.key(), 2) * i.value();
        if (floor(fmod(i.key(), 0.5)*1000) == 0)
            qDebug() << i.key() << i.value();
        n++;
    }

    double A[3][3] = // Основная матрица
    {
        {n * 1.0, sum_x, sum_x2},
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

    for (int i = 0; i < 3; i++) {
        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++) {
                A_bkp[a][b] = A[a][b];
            }
        }
        for (int j = 0; j < 3; j++) {
            A_bkp[j][i] = B[j];
        }
        X.append(det(A_bkp) / detA);
    }

    double sum = 0;
    double otk = 0;
    double max_otk;
    for (i = S.begin(); i != S.end(); ++i) {
        otk = pow(i.value() - (X[2] * pow(i.key(),2) + X[1] * i.key() + X[0]), 2);
        max_otk = qMax(otk, max_otk);
        sum += max_otk;
    }

    return X;
}

QList<double> Profile::squareMethod(const QList<qreal> &S)
{
    QMap<double, double> map;
    QListIterator<qreal> i(S);
    int n = 0;
    while (i.hasNext()) {
        map[n * 1.0 / 2] = i.next();
        n++;
    }
    return squareMethod(map);
}

void Profile::a_tw(double ry1, double Wi, double &x_tr, double &y_tr)
{
    double vy2, p, q, alpha_tw, phi1;
    double i21 = z1() / z2();
    double alpha_tw_n = M_PI / 180;
    double alpha_tw_k = acos((Wi/m_rb2 - sqrt(pow(Wi/m_rb2,2) - 2 * sin(2*m_E_rad) / tan(m_delta2))) / (2 * sin(m_E_rad)));
    while (alpha_tw_k - alpha_tw_n > m_eps) {
        alpha_tw = (alpha_tw_n + alpha_tw_k) / 2;
        vy2 = (Wi / (m_rb2 * sin(m_E_rad)) - (1 / (tan(m_E_rad) * tan(m_delta2) * cos(alpha_tw))) - cos(alpha_tw)) / sin(alpha_tw);
        p = cos(m_E_rad) * (cos(alpha_tw) + vy2 * sin(alpha_tw)) - sin(m_E_rad) / (cos(alpha_tw) * tan(m_delta2));
        q = sin(alpha_tw) - vy2 * cos(alpha_tw);
        if ((m_rb2 * sqrt(pow(q,2) + pow(p,2)) - ry1) > 0) {
             alpha_tw_n = alpha_tw;
        } else {
             alpha_tw_k = alpha_tw;
        }
    }
    phi1 = (alpha_tw + m_psi_b2 - vy2) / i21;
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

QString Profile::getBaseName()
{
    QString name = "m" + QString::number(m()) +
                   "_" + QString::number(z1()) +
                   "_" + QString::number(z2()) +
                   "_" + QString::number(bw());
    return name;

}

QString Profile::getModName()
{
    QString name = QString::number(mod0()) +
                   "_" + QString::number(modCenter()) +
                   "_" + QString::number(modBw());
    return name;
}



void Profile::saveTrajectory()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    settings.beginGroup(getBaseName());
    settings.setValue("a", m_trajectory[2]);
    settings.setValue("b", m_trajectory[1]);
    settings.setValue("c", m_trajectory[0]);

}

void Profile::saveMainSettings()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    settings.beginGroup(getBaseName());
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

    settings.setValue("useManualTr", m_useManualXtList);
    if (m_useManualXtList) {
        QVariantList variantList;
        QListIterator<qreal> i(m_manualXtList);
        while (i.hasNext())
            variantList << i.next();
        settings.setValue("XtList", variantList);
    }

    settings.beginGroup(getModName());
    settings.setValue("mod0", mod0());
    settings.setValue("modCenter", modCenter());
    settings.setValue("modBw", modBw());
  /*  settings.setValue("a", trajectory().at(2));
    settings.setValue("b", trajectory().at(1));
    settings.setValue("c", trajectory().at(0));*/
    settings.endGroup();

    setModificationList(settings.childGroups());

    settings.endGroup();

    setListOfParameters();

}

bool Profile::areEmpty()
{
    if ( m() == 0 ||
         z1() == 0 ||
         z2() == 0 ||
         bw() == 0 ||
         W0() == 0 ||
         E() == 0 ||
         x2() == 0 ||
         d0() == 0 ||
         ra2() == 0 ||
         rf2() == 0 ||
         alpha() == 0 ||
         ha() == 0 ||
         c() == 0 ||
         x0() == 0 ||
         z0() == 0 ||
         da0() == 0 )
        return true;

    if (manualXtList().empty() && useManualXtList())
        return true;

    QListIterator<qreal> i(manualXtList());
    while (i.hasNext()) {
        if (i.next() == 0)
            return true;
    }

    return false;
}

/*void Profile::saveOtherSettings()
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
    settings.endGroup();
}*/

void Profile::loadSettings(QString value)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    settings.beginGroup(value);

    setM(settings.value("m").toDouble());
    setZ1(settings.value("z1").toDouble());
    setZ2(settings.value("z2").toDouble());
    setBw(settings.value("bw").toDouble());
    setW0(settings.value("W0").toDouble());
    setE(settings.value("E").toDouble());
    setX2(settings.value("x2").toDouble());
    setD0(settings.value("d0").toDouble());
    setRa2(settings.value("ra2").toDouble());
    setRf2(settings.value("rf2").toDouble());
    setUseManualXtList(settings.value("useManualTr").toBool());
    QList<QVariant> variantList = settings.value("XtList").toList();
    if ( !variantList.empty() ) {
        QList<qreal> qList;
        QListIterator<QVariant> i(variantList);
        while (i.hasNext())
            qList << i.next().toDouble();

        setManualXtList(qList);
    }

    //if ( !settings.childGroups().isEmpty() ) {
        setModificationList(settings.childGroups());
   // } else {
  //      QStringList list;
     //   list << QString("0_0_0");
   //     setModificationList(list);
    //}

    settings.endGroup();
}

void Profile::loadModSettings(QString value, QString modValue)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    qDebug() << value << modValue;
    settings.beginGroup(value);
    settings.beginGroup(modValue);
    setMod0(settings.value("mod0").toDouble());
    setModCenter(settings.value("modCenter").toDouble());
    setModBw(settings.value("modBw").toDouble());
    settings.endGroup();
    settings.endGroup();
}

void Profile::setListOfParameters()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    QRegExp rx("m*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QStringList list = settings.childGroups().filter(rx);
    if (m_listOfParameters != list) {
        m_listOfParameters = list;
        emit listOfParametersChanged(list);
    }
}

void Profile::setModificationList(QStringList arg)
{
    if (arg.isEmpty()) {
        setMod0(0.0);
        setModCenter(0.0);
        setModBw(0.0);
    }

    if (m_modificationList != arg) {
        m_modificationList = arg;
        emit modificationListChanged(arg);
    }
}
