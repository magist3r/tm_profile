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

//extern double m, z1, z2, x2, W0, E, alpha, c, ha, z0, x0, da0, i21, rb2, delta2, psi_b2, d0, bw, ra2, rf2, dx, dx_0, dx_bw;

class Profile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double m READ m WRITE setM NOTIFY onMChanged)  // Module
    Q_PROPERTY(double z1 READ z1 WRITE setZ1 NOTIFY onZ1Changed) // Number of teeth of pinion
    Q_PROPERTY(double z2 READ z2 WRITE setZ2 NOTIFY onZ2Changed) // Number of teeth of gear
    Q_PROPERTY(double bw READ bw WRITE setBw NOTIFY onBwChanged)
    Q_PROPERTY(double W0 READ W0 WRITE setW0 NOTIFY onW0Changed)
    Q_PROPERTY(double E READ E WRITE setE NOTIFY onEChanged)
    Q_PROPERTY(double x2 READ x2 WRITE setX2 NOTIFY onX2Changed)
    Q_PROPERTY(double d0 READ d0 WRITE setD0 NOTIFY onD0Changed)

    Q_PROPERTY(double alpha READ alpha WRITE setAlpha NOTIFY onAlphaChanged)
    Q_PROPERTY(double c READ c WRITE setC NOTIFY onCChanged)
    Q_PROPERTY(double ha READ ha WRITE setHa NOTIFY onHaChanged)

    Q_PROPERTY(double z0 READ z0 WRITE setZ0 NOTIFY onZ0Changed)
    Q_PROPERTY(double x0 READ x0 WRITE setX0 NOTIFY onX0Changed)
    Q_PROPERTY(double da0 READ da0 WRITE setDa0 NOTIFY onDa0Changed)

public:
    Profile(QObject *parent = 0);
    Profile(const Profile&);
    Profile& operator=(const Profile&);

    ~Profile();



    QMap<double, QMap<double,double> > result;
    QMap<double, QMap<double,double> > result_s_tr;
    QMap<double, QMap<double,double> > result_s_pr;

    bool getRadius(); // Расчет радиусов вершин и впадин колеса (ra2 и rf2)

    void calculate(); // Расчет толщин зубьев практического и теоретического профилей
    double m() const
    {
        return m_m;
    }

    double z1() const
    {
        return m_z1;
    }

    double z2() const
    {
        return m_z2;
    }

    double x2() const
    {
        return m_x2;
    }

    double W0() const
    {
        return m_W0;
    }

    double E() const
    {
        return m_E;
    }

    double alpha() const
    {
        return m_alpha;
    }

    double c() const
    {
        return m_c;
    }

    double ha() const
    {
        return m_ha;
    }

    double z0() const
    {
        return m_z0;
    }

    double x0() const
    {
        return m_x0;
    }

    double da0() const
    {
        return m_da0;
    }

    double d0() const
    {
        return m_d0;
    }

    double bw() const
    {
        return m_bw;
    }

public slots:
    void setBw(double arg)
    {
        if (m_bw != arg) {
            m_bw = arg;
            emit onBwChanged(arg);
        }
    }

    void setM(double arg)
    {
        if (m_m != arg) {
            m_m = arg;
            emit onMChanged(arg);
        }
    }

    void setZ1(double arg)
    {
        if (m_z1 != arg) {
            m_z1 = arg;
            emit onZ1Changed(arg);
        }
    }

    void setZ2(double arg)
    {
        if (m_z2 != arg) {
            m_z2 = arg;
            emit onZ2Changed(arg);
        }
    }

    void setW0(double arg)
    {
        if (m_W0 != arg) {
            m_W0 = arg;
            emit onW0Changed(arg);
        }
    }

    void setE(double arg)
    {
        if (m_E != arg) {
            m_E = arg;
            emit onEChanged(arg);
        }
    }

    void setX2(double arg)
    {
        if (m_x2 != arg) {
            m_x2 = arg;
            emit onX2Changed(arg);
        }
    }

    void setD0(double arg)
    {
        if (m_d0 != arg) {
            m_d0 = arg;
            emit onD0Changed(arg);
        }
    }

    void setAlpha(double arg)
    {
        if (m_alpha != arg) {
            m_alpha = arg;
            emit onAlphaChanged(arg);
        }
    }

    void setC(double arg)
    {
        if (m_c != arg) {
            m_c = arg;
            emit onCChanged(arg);
        }
    }

    void setHa(double arg)
    {
        if (m_ha != arg) {
            m_ha = arg;
            emit onHaChanged(arg);
        }
    }

    void setZ0(double arg)
    {
        if (m_z0 != arg) {
            m_z0 = arg;
            emit onZ0Changed(arg);
        }
    }

    void setX0(double arg)
    {
        if (m_x0 != arg) {
            m_x0 = arg;
            emit onX0Changed(arg);
        }
    }

    void setDa0(double arg)
    {
        if (m_da0 != arg) {
            m_da0 = arg;
            emit onDa0Changed(arg);
        }
    }

signals:
    void addToDebugConsole(QString text);

    void onMChanged(double arg);

    void onZ1Changed(double arg);

    void onZ2Changed(double arg);

    void onBwChanged(double arg);

    void onW0Changed(double arg);

    void onEChanged(double arg);

    void onX2Changed(double arg);

    void onD0Changed(double arg);

    void onAlphaChanged(double arg);

    void onCChanged(double arg);

    void onHaChanged(double arg);

    void onZ0Changed(double arg);

    void onX0Changed(double arg);

    void onDa0Changed(double arg);

private:

    // Расчетные величины
    double m_i21; // Передаточное отношение
    double m_rb2; // Радиус основной окружности колеса
    double m_delta2;
    double m_psi_b2;

    QList<double> square_method(const QMap<double, double> &S);

    void a_tw(double ry1, double Wi, double &x_tr, double &y_tr); // Подбор угла профиля в торцовом сечении

    double det(const double A[3][3]); // Расчет определителя матрицы

    double m_m;

    double m_z1; // Число зубьев шестерни
    double m_z2; // Число зубьев колеса
    double m_x2; // Коэффициент смещения колеса
    double m_W0; // Аппликата большего торцового сечения венца шестерни
    double m_E; // Межосевой угол
    double m_bw; // Ширина зубчатого венца

    // Параметры исходного контура
    double m_alpha; // Угол профиля
    double m_c; // Коэффициент радиального зазора
    double m_ha; // Коэффициент высоты зуба

    // Параметры зуборезного долбяка
    double m_z0; // Число зубьев инструмента
    double m_x0; // Коэффициент смещения
    double m_da0; // Диаметр вершин зубьев

    double m_d0; // Делительный диаметр фрезы

    double m_ra2; // Радиус вершин зубьев колеса
    double m_rf2; // Радиус впадин колеса

    static const double m_eps = 0.000001; // Требуемая точность расчётов
    double m_nW;
    double m_nr;
    double m_dx; // Величина модификации
    double m_dx_0;
    double m_dx_bw;

    double m_delta_s_max;

    bool m_useSmooth;
    bool m_useOldPaintMode;
    bool m_diagnosticMode;

    QList<double> m_xt_w;
};

#endif // PROFILE_H
