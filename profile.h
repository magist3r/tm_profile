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

class Profile : public QThread
{
    Q_OBJECT

public:
    Profile(QObject *parent = 0);

//    Profile();
    ~Profile();

    double m; // Модуль зацепления
    double z1; // Число зубьев шестерни
    double z2; // Число зубьев колеса
    double x2; // Коэффициент смещения колеса
    double W0; // Аппликата большего торцового сечения венца шестерни
    double E; // Межосевой угол
    double bw; // Ширина зубчатого венца

    // Параметры исходного контура
    double alpha; // Угол профиля
    double c; // Коэффициент радиального зазора
    double ha; // Коэффициент высоты зуба

    // Параметры зуборезного долбяка
    double z0; // Число зубьев инструмента
    double x0; // Коэффициент смещения
    double da0; // Диаметр вершин зубьев

    double d0; // Делительный диаметр фрезы

    double ra2; // Радиус вершин зубьев колеса
    double rf2; // Радиус впадин колеса

    static const double eps = 0.000001; // Требуемая точность расчётов
    double n_W;
    double n_r;
    double dx; // Величина модификации
    double dx_0;
    double dx_bw;

    bool useSmooth;
    bool diagnosticMode;

    QList<double> xt_w;

    QMap<double, QMap<double,double> > result;

    QImage image;

    bool getRadius(); // Расчет радиусов вершин и впадин колеса (ra2 и rf2)

   // void calculate(); // Расчет толщин зубьев практического и теоретического профилей
signals:
    void addToDebugConsole(QString text);

private:

    // Расчетные величины
    double i21; // Передаточное отношение
    double rb2; // Радиус основной окружности колеса
    double delta2;
    double psi_b2;

    QList<double> square_method(QMap<double, double> &S);

    QList<double> gauss_zeidel_method(QMap<double, double> &S);

    QList<double> a_tw(double ry1, double Wi); // Подбор угла профиля в торцовом сечении

    double det(double A[3][3]); // Расчет определителя матрицы
protected:
      void run();
};

/*void radius();

QMap<double, QMap<double,double> > pr_profile(double ra2, double rf2);

QList<double> square_metod(QMap<double, double> S);

QList<double> a_tw(double ry1, double Wi);

double det(double A[3][3]);*/

#endif // PROFILE_H
