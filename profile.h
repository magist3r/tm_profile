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

#ifndef PROFILE_H
#define PROFILE_H
#include <QMap>
#include <QThread>
#include <QImage>
#include <QPainter>
#include <QDebug>

#define _USE_MATH_DEFINES
#include <math.h>

class Profile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double m READ m WRITE setM NOTIFY mChanged)  // Module
    Q_PROPERTY(double z1 READ z1 WRITE setZ1 NOTIFY z1Changed) // Number of teeth of pinion
    Q_PROPERTY(double z2 READ z2 WRITE setZ2 NOTIFY z2Changed) // Number of teeth of gear
    Q_PROPERTY(double bw READ bw WRITE setBw NOTIFY bwChanged)
    Q_PROPERTY(double W0 READ W0 WRITE setW0 NOTIFY W0Changed)
    Q_PROPERTY(double E READ E WRITE setE NOTIFY EChanged)
    Q_PROPERTY(double x2 READ x2 WRITE setX2 NOTIFY x2Changed)
    Q_PROPERTY(double d0 READ d0 WRITE setD0 NOTIFY d0Changed)

    Q_PROPERTY(double alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
    Q_PROPERTY(double c READ c WRITE setC NOTIFY cChanged)
    Q_PROPERTY(double ha READ ha WRITE setHa NOTIFY haChanged)

    Q_PROPERTY(double z0 READ z0 WRITE setZ0 NOTIFY z0Changed)
    Q_PROPERTY(double x0 READ x0 WRITE setX0 NOTIFY x0Changed)
    Q_PROPERTY(double da0 READ da0 WRITE setDa0 NOTIFY da0Changed)

    Q_PROPERTY(double ra2 READ ra2 WRITE setRa2 NOTIFY ra2Changed)
    Q_PROPERTY(double rf2 READ rf2 WRITE setRf2 NOTIFY rf2Changed)

    Q_PROPERTY(double mod0 READ mod0 WRITE setMod0 NOTIFY mod0Changed)
    Q_PROPERTY(double modCenter READ modCenter WRITE setModCenter NOTIFY modCenterChanged)
    Q_PROPERTY(double modBw READ modBw WRITE setModBw NOTIFY modBwChanged)

    Q_PROPERTY(QList<qreal> trajectory READ trajectory NOTIFY trajectoryChanged)

    Q_PROPERTY(QStringList listOfParameters READ listOfParameters NOTIFY listOfParametersChanged)
    Q_PROPERTY(QStringList modificationList READ modificationList NOTIFY modificationListChanged)

    Q_PROPERTY(QList<qreal> manualXtList READ manualXtList WRITE setManualXtList NOTIFY manualXtListChanged)
    Q_PROPERTY(bool useManualXtList READ useManualXtList WRITE setUseManualXtList NOTIFY useManualXtListChanged)
    Q_PROPERTY(QString dataLocation READ dataLocation WRITE setdataLocation NOTIFY dataLocationChanged)



public:
    Profile(QObject *parent = 0);

    Q_INVOKABLE bool getRadius(); // Расчет радиусов вершин и впадин колеса (ra2 и rf2)

    Q_INVOKABLE void calculate();
    Q_INVOKABLE QMap<double, double> calculateTheoretical(); // Расчет толщин зубьев теоретического профиля
    Q_INVOKABLE void calculatePractical(); // Расчет толщин зубьев практического профиля



    /* getters */

    double m() const { return m_m; }
    double z1() const { return m_z1; }
    double z2() const { return m_z2; }
    double x2() const { return m_x2; }
    double W0() const { return m_W0; }
    double E() const { return m_E; }
    double alpha() const { return m_alpha; }
    double c() const { return m_c; }
    double ha() const { return m_ha; }
    double z0() const { return m_z0; }
    double x0() const { return m_x0; }
    double da0() const { return m_da0; }
    double d0() const { return m_d0; }
    double bw() const { return m_bw; }
    double ra2() const { return m_ra2; }
    double rf2() const { return m_rf2; }

    double mod0() const { return m_mod0; }
    double modCenter() const { return m_modCenter; }
    double modBw() const { return m_modBw; }

    QList<qreal> trajectory() const {  return m_trajectory; }
    bool useManualXtList() const { return m_useManualXtList; }
    QList<qreal> manualXtList() const { return m_manualXtList; }
    QString dataLocation() const { return m_dataLocation; }

    QStringList listOfParameters() const { return m_listOfParameters; }

    QStringList modificationList() const
    {
        return m_modificationList;
    }

    /* end getters */

    Q_INVOKABLE void saveMainSettings();

    Q_INVOKABLE bool areEmpty();

    Q_INVOKABLE bool imageExists(QString basename);


    Q_INVOKABLE QString getBaseName();

    Q_INVOKABLE QString getModName();

public slots:

    /* setters */
    void setBw(double arg)
    {
        if (m_bw != arg) {
            m_bw = arg;
            emit bwChanged(arg);
        }
    }

    void setM(double arg)
    {
        if (m_m != arg) {
            m_m = arg;
            emit mChanged(arg);
        }
    }

    void setZ1(double arg)
    {
        if (m_z1 != arg) {
            m_z1 = arg;
            emit z1Changed(arg);
        }
    }

    void setZ2(double arg)
    {
        if (m_z2 != arg) {
            m_z2 = arg;
            emit z2Changed(arg);
        }
    }

    void setW0(double arg)
    {
        if (m_W0 != arg) {
            m_W0 = arg;
            emit W0Changed(arg);
        }
    }

    void setE(double arg)
    {
        if (m_E != arg) {
            m_E = arg;
            m_E_rad = M_PI * arg / 180;
            emit EChanged(arg);
        }
    }

    void setX2(double arg)
    {
        if (m_x2 != arg) {
            m_x2 = arg;
            emit x2Changed(arg);
        }
    }

    void setD0(double arg)
    {
        if (m_d0 != arg) {
            m_d0 = arg;
            emit d0Changed(arg);
        }
    }

    void setAlpha(double arg)
    {
        if (m_alpha != arg) {
            m_alpha = arg;
            m_alpha_rad = M_PI * arg / 180;
            emit alphaChanged(arg);
        }
    }

    void setC(double arg)
    {
        if (m_c != arg) {
            m_c = arg;
            emit cChanged(arg);
        }
    }

    void setHa(double arg)
    {
        if (m_ha != arg) {
            m_ha = arg;
            emit haChanged(arg);
        }
    }

    void setZ0(double arg)
    {
        if (m_z0 != arg) {
            m_z0 = arg;
            emit z0Changed(arg);
        }
    }

    void setX0(double arg)
    {
        if (m_x0 != arg) {
            m_x0 = arg;
            emit x0Changed(arg);
        }
    }

    void setDa0(double arg)
    {
        if (m_da0 != arg) {
            m_da0 = arg;
            emit da0Changed(arg);
        }
    }

    void setRa2(double arg)
    {
        if (m_ra2 != arg) {
            m_ra2 = arg;
            emit ra2Changed(arg);
        }
    }

    void setRf2(double arg)
    {
        if (m_rf2 != arg) {
            m_rf2 = arg;
            emit rf2Changed(arg);
        }
    }

    void setUseManualXtList(bool arg)
    {
        if (m_useManualXtList != arg) {
            m_useManualXtList = arg;
            emit useManualXtListChanged(arg);
        }
    }

    void setManualXtList(QList<qreal> arg)
    {
        if (m_manualXtList != arg) {
            m_manualXtList = arg;
            emit manualXtListChanged(arg);
        }
    }

    void setdataLocation(QString arg)
    {
        if (m_dataLocation != arg) {
            m_dataLocation = arg;
            emit dataLocationChanged(arg);
        }
    }

    void setModificationList(QStringList arg);

    void setMod0(double arg)
    {
        if (m_mod0 != arg) {
            m_mod0 = arg;
            emit mod0Changed(arg);
        }
    }

    void setModCenter(double arg)
    {
        if (m_modCenter != arg) {
            m_modCenter = arg;
            emit modCenterChanged(arg);
        }
    }

    void setModBw(double arg)
    {
        if (m_modBw != arg) {
            m_modBw = arg;
            emit modBwChanged(arg);
        }
    }

    /* end setters */

    void loadSettings(QString value = "");

    void loadModSettings(QString value, QString modValue);

    void setListOfParameters();

signals:

    /* NOTIFY */

    void mChanged(double arg);
    void z1Changed(double arg);
    void z2Changed(double arg);
    void bwChanged(double arg);
    void W0Changed(double arg);
    void EChanged(double arg);
    void x2Changed(double arg);
    void d0Changed(double arg);
    void alphaChanged(double arg);
    void cChanged(double arg);
    void haChanged(double arg);
    void z0Changed(double arg);
    void x0Changed(double arg);
    void da0Changed(double arg);
    void listOfParametersChanged(QStringList arg);
    void ra2Changed(double arg);
    void rf2Changed(double arg);
    void trajectoryChanged(QList<qreal> arg);
    void useManualXtListChanged(bool arg);
    void manualXtListChanged(QList<qreal> arg);
    void dataLocationChanged(QString arg);
    void modificationListChanged(QStringList arg);
    void mod0Changed(double arg);
    void modCenterChanged(double arg);
    void modBwChanged(double arg);

    /* end NOTIFY */

    void calculateFinished(QMap<double, QMap<double,double> > &_result, double delta, double _delta_s_max, double image_width, double image_height, QString image_basename);

private:

    // Расчетные величины
    double m_i21; // Передаточное отношение
    double m_rb2; // Радиус основной окружности колеса
    double m_delta2;
    double m_psi_b2;

    QList<double> squareMethod(const QMap<double, double> &S);

    QList<double> squareMethod(const QList<qreal> &S);

    void a_tw(double ry1, double Wi, double &x_tr, double &y_tr); // Подбор угла профиля в торцовом сечении

    double det(const double A[3][3]); // Расчет определителя матрицы

    QColor getPointColor(double delta_s);

    void saveTrajectory();

    QMap<double, QMap<double,double> > m_theoreticalProfile;
    QMap<double, QMap<double,double> > m_result;


    double m_m;

    double m_z1; // Число зубьев шестерни
    double m_z2; // Число зубьев колеса
    double m_x2; // Коэффициент смещения колеса
    double m_W0; // Аппликата большего торцового сечения венца шестерни
    double m_E; // Межосевой угол
    double m_E_rad;
    double m_bw; // Ширина зубчатого венца

    // Параметры исходного контура
    double m_alpha; // Угол профиля
    double m_alpha_rad;
    double m_c; // Коэффициент радиального зазора
    double m_ha; // Коэффициент высоты зуба

    // Параметры зуборезного долбяка
    double m_z0; // Число зубьев инструмента
    double m_x0; // Коэффициент смещения
    double m_da0; // Диаметр вершин зубьев

    double m_d0; // Делительный диаметр фрезы

    double m_ra2; // Радиус вершин зубьев колеса
    double m_rf2; // Радиус впадин колеса

    static constexpr double m_eps = 0.000001; // Требуемая точность расчётов
    double m_nW;
    double m_nr;
/*    double m_dx; // Величина модификации
    double m_dx_0;
    double m_dx_bw;*/

    double m_delta_s_max;
    double m_delta_s_min;

    bool m_useSmooth;
    bool m_useOldPaintMode;
    bool m_diagnosticMode;

    QList<qreal> m_trajectory;
    QStringList m_listOfParameters;
    bool m_useManualXtList;
    QList<qreal> m_manualXtList;
    QString m_dataLocation;
    QStringList m_modificationList;
    double m_mod0;
    double m_modCenter;
    double m_modBw;
};

#endif // PROFILE_H
