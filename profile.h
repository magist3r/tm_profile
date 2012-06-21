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
    Q_PROPERTY(double m READ m WRITE setM)  // Module
    Q_PROPERTY(double z1 READ z1 WRITE setZ1) // Number of teeth of pinion
    Q_PROPERTY(double z2 READ z2 WRITE setZ2) // Number of teeth of gear
    Q_PROPERTY(double x2 READ x2 WRITE setX2)
    Q_PROPERTY(double W0 READ W0 WRITE setW0)
    Q_PROPERTY(double E READ E WRITE setE)

    Q_PROPERTY(double alpha READ alpha WRITE setAlpha)
    Q_PROPERTY(double c READ c WRITE setC)
    Q_PROPERTY(double ha READ ha WRITE setHa)

    Q_PROPERTY(double z0 READ z0 WRITE setZ0)
    Q_PROPERTY(double x0 READ x0 WRITE setX0)
    Q_PROPERTY(double da0 READ da0 WRITE setDa0)

    Q_PROPERTY(double d0 READ d0 WRITE setD0)
    Q_PROPERTY(double z1 READ z1 WRITE setZ1)



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

public slots:
    void setM(double arg)
    {
        m_m = arg;
    }

    void setZ1(double arg)
    {
        m_z1 = arg;
    }

    void setZ2(double arg)
    {
        m_z2 = arg;
    }

    void setX2(double arg)
    {
        m_x2 = arg;
    }

    void setW0(double arg)
    {
        m_W0 = arg;
    }

    void setE(double arg)
    {
        m_E = arg;
    }

    void setAlpha(double arg)
    {
        m_alpha = arg;
    }

    void setC(double arg)
    {
        m_c = arg;
    }

    void setHa(double arg)
    {
        m_ha = arg;
    }

    void setZ0(double arg)
    {
        m_z0 = arg;
    }

    void setX0(double arg)
    {
        m_x0 = arg;
    }

    void setDa0(double arg)
    {
        m_da0 = arg;
    }

    void setD0(double arg)
    {
        m_d0 = arg;
    }

signals:
    void addToDebugConsole(QString text);

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
