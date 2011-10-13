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
#include <QDebug>

Profile::Profile(QObject *parent) : QThread(parent)
{

}

Profile::~Profile()
{
    wait();
}

bool Profile::getRadius()
{
    double alpha_w02;

    double inv_alpha_w02 = ((x2 - x0) / (z2 - z0)) * 2 * tan(alpha) + tan(alpha) - alpha;

    double alpha_n = M_PI / 180;
    double alpha_k = 89 * M_PI / 180;
    while (alpha_k - alpha_n > eps)
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
    rf2 = (m * (z2 - z0) * (cos(alpha)/cos(alpha_w02)) + da0) / 2;
    ra2 = rf2 - m * (2 * ha + c);
    return true;
}

void Profile::run()
{
    i21 = z1 / z2;
    rb2 = 0.5 * m * z2 * cos(alpha);
    delta2 = atan(sin(E) / (cos(E) - i21));
    psi_b2 = M_PI / (2 * z2) + 2 * x2 * tan(alpha) / z2 + tan(alpha) - alpha;
    result.clear();

    double dW = bw / n_W; // Шаг торцовых сечений
    double rav2 = (rf2 + ra2 - c * m) / 2;
    double dr = (rf2 - ra2 - c * m) / n_r; // Шаг радиусов

    QList<double> list;

    QMap<double, double> S;

    double Wi = W0;
    // Расчет коэффициентов смещения
    for (int i=0; i <= n_W; i++)
    {
        double delta_oi = E; // Угол аксоидного конуса шестерни
        double alpha1 = atan(tan(alpha) * cos(delta_oi));
        double rb1 = 0.5 * m * z1 * cos(alpha1);

        double ry1 = rav2 / cos(E) - (Wi + 0.1 * m) * tan(E);
        double psi_yi = a_tw(ry1, Wi + 0.1 * m).value(0) / ry1;
        double alpha_y1 = acos(rb1 / ry1);
        double xt1 = (z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

        ry1 = rav2 / cos(E) - (Wi) * tan(E);
        psi_yi = a_tw(ry1, Wi).value(0) / ry1;
        alpha_y1 = acos(rb1 / ry1);
        double xt2 = (z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

        delta_oi = atan(10 * (xt2 - xt1));
        alpha1 = atan(tan(alpha) * cos(delta_oi));
        rb1 = 0.5 * m * z1 * cos(alpha1);
        alpha_y1 = acos(rb1 / ry1);
        double xt = (z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1)); // Коэф. смещения

        S[Wi - W0] = xt;

        Wi += dW;
    }

    xt_w = square_method(S);

    // Задание модификации
    if(dx != 0 || dx_0 != 0 || dx_bw != 0)
    {
        S.clear();

        S[0] = xt_w[2] * 0 + xt_w[1] * 0 + xt_w[0] - dx_0;
        S[bw/2] = xt_w[2] * pow(bw/2,2) + xt_w[1] * bw/2 + xt_w[0] + dx;
        S[bw] = xt_w[2] * pow(bw,2) + xt_w[1] * bw + xt_w[0] - dx_bw;

        xt_w = square_method(S);
    }

    double w0 = -xt_w[1] / (2 * xt_w[2]);

    Wi = W0;

    // Расчет толщин зубьев
    for (int i=0; i <= n_W; i++)
    {
        double wi = Wi - W0;
        double delta_oi = -atan(m * (2 * xt_w[2] * wi + xt_w[1]));
        double xt = xt_w[2] * pow(wi, 2) + xt_w[1] * wi + xt_w[0];
        double aw0 = m * (0.5 * z1 + xt) + 0.5 * d0 * cos(delta_oi);
        double ha_2 = ha / cos(delta_oi);
        double c_2 = c / cos(delta_oi);
        double d = m * z1;
        double alpha_t = atan(tan(alpha) * cos(delta_oi));
        double d_b = d * cos(alpha_t);
        double ry1_min = ra2 / cos(E) - (W0 + bw) * tan(E);
        double ry2 = ra2;

        for (int j=0; j <= n_r; j++)
        {

            double ry1 = ry2 / cos(E) - Wi * tan(E);

            // Теоретический профиль
            list = a_tw(ry1, Wi);
            double x_tr = list[0];
            double y_tr = list[1];
            double s_tr = 2 * ry1 * atan(x_tr / y_tr); // Толщина зуба теоретическая

            // Практический профиль
            double alpha_ty = acos(0.5 * d_b / ry1);
            double st = m * (M_PI / 2 + 2 * xt * tan(alpha) * cos(delta_oi));
            double s_pr = ry1 * (2 * st / d + 2 * (tan(alpha_t) - alpha_t) - 2 * (tan(alpha_ty) - alpha_ty)); // Толщина зуба практическая
            result[wi][ry1 - ry1_min] = (s_pr - s_tr) / 2;
            double delta_s = (s_pr - s_tr) / 2;

            if (diagnosticMode)
            {
                emit addToDebugConsole("Wi= " + QString::number(wi) + " | ry= " + QString::number(ry1) + " | delta_s = " + QString::number((s_pr - s_tr) / 2));
            }

            ry2 += dr;
        }
        Wi += dW;
    }
}

QList<double> Profile::square_method(QMap<double, double> &S)
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
    QMap<double, double>::iterator i;
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

QList<double> Profile::a_tw (double ry1, double Wi)
{

    QList<double> out_list;

    double vy2, p, q, alpha_tw, phi1;
    double alpha_tw_n = M_PI / 180;
    double alpha_tw_k = acos((Wi/rb2 - sqrt(pow(Wi/rb2,2) - 2 * sin(2*E) / tan(delta2))) / (2 * sin(E)));
    while (alpha_tw_k - alpha_tw_n > eps)
    {
        alpha_tw = (alpha_tw_n + alpha_tw_k) / 2;
        vy2 = (Wi / (rb2 * sin(E)) - (1 / (tan(E) * tan(delta2) * cos(alpha_tw))) - cos(alpha_tw)) / sin(alpha_tw);
        p = cos(E) * (cos(alpha_tw) + vy2 * sin(alpha_tw)) - sin(E) / (cos(alpha_tw) * tan(delta2));
        q = sin(alpha_tw) - vy2 * cos(alpha_tw);
        if ((rb2 * sqrt(pow(q,2) + pow(p,2)) - ry1) > 0)
        {
             alpha_tw_n = alpha_tw;
        }
        else
        {
             alpha_tw_k = alpha_tw;
        }
    }
    phi1 = (alpha_tw + psi_b2 - vy2) / i21;
    out_list.append(rb2 * (p * sin(phi1) - q * cos(phi1)));
    out_list.append(rb2 * (p * cos(phi1) + q * sin(phi1)));
    return out_list;
}

double Profile::det(double A[3][3])
{
   return A[0][0] * A[1][1] * A[2][2] +
          A[0][1] * A[1][2] * A[2][0] +
          A[1][0] * A[2][1] * A[0][2] -
          A[2][0] * A[1][1] * A[0][2] -
          A[1][0] * A[0][1] * A[2][2] -
          A[2][1] * A[1][2] * A[0][0];
}
