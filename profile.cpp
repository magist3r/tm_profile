#include "profile.h"
#include "math.h"
#include <QDebug>

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
double x0; // Коэффициент  смещения
double da0; // Диаметр вершин зубьев
// Расчетные величины
double i21; // Передаточное отношение
double rb2; // Радиус основной окружности колеса
double delta2;
double psi_b2;

double d0; // Делительный диаметр фрезы

double ra2; // Радиус вершин зубьев колеса
double rf2; // Радиус впадин колеса

double eps = 0.000001; // Требуемая точность расчётов
double dx; // Величина модификации

void radius()
{
    // Расчет радиусов вершин и впадин колеса (ra2 и rf2)

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
}

QMap<QString,double> t_profile(double ra2, double rf2)
{
    QList<double> list;
    QMap<QString,double> out_list;

    double n = 4; // Количество радиусов
    double dW = 0.5; // Шаг торцовых сечений
    double dr = (rf2 - ra2 - c * m) / n; // Шаг радиусов
    double ry1;

    for (double Wi = W0; Wi <= W0 + bw; Wi += dW)
    {
        for (double ry2 = ra2; ry2 <= rf2 - c * m; ry2 += dr)
        {
            ry1 = ry2 / cos(E) - Wi * tan(E);
            list = a_tw(ry1, Wi);
        }
    }
    return out_list;
}

QMap<double, QMap<double,double> > pr_profile(double ra2, double rf2)
{
    int n = 10; // Количество разбиений
    int n2 = 50; // Количество радиусов
    double dW = bw / n; // Шаг торцовых сечений
    double rav2 = (rf2 + ra2 - c * m) / 2;
    double dr = (rf2 - ra2 - c * m) / n2; // Шаг радиусов

    QList<double> list;

    double S[n+1];

    double sum_Wi = 0;
    double sum_Wi_2 = 0;
    double sum_Wi_3 = 0;
    double sum_Wi_4 = 0;
    double sum_xt = 0;
    double sum_xt_2 = 0;
    double sum_xt_3 = 0;

    int i = 0;

    for (double Wi = W0; Wi <= W0 + bw; Wi += dW)
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

     //   if (i == 0) {double xt_max = xt;}
        S[i] = xt;

        sum_Wi += Wi - W0;
        sum_Wi_2 += pow(Wi - W0, 2);
        sum_Wi_3 += pow(Wi - W0, 3);
        sum_Wi_4 += pow(Wi - W0, 4);
        sum_xt += xt;
        sum_xt_2 += xt * (Wi - W0);
        sum_xt_3 += xt * pow(Wi - W0, 2);
        i++;
    }

    // Решение СЛАУ методом Крамера
    double A[3][3] = // Основная матрица
    {
        {n + 1, sum_Wi, sum_Wi_2},
        {sum_Wi, sum_Wi_2, sum_Wi_3},
        {sum_Wi_2, sum_Wi_3, sum_Wi_4}
    };

    double B[3] = // Матрица коэффициентов
    {
        sum_xt,
        sum_xt_2,
        sum_xt_3
    };

    double X[3]; // Матрица результатов
    double T[3][3];

    double detA = det(A);

    for (int m = 0; m < 3; m++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                T[i][j] = A[i][j];
            }
        }
        for (int j = 0; j < 3; j++)
            {
                T[j][m] = B[j];
            }
        X[m] = det(T) / detA;
    }


    double w0 = -X[1] / (2 * X[2]);
    double s_kw = 0;
    i = 0;
    for (double Wi = W0; Wi <= W0 + bw; Wi += dW)
    {
        double s = X[2] * pow((Wi - W0),2) + X[1] * (Wi - W0) + X[0];
        s_kw += pow(s - S[i],2);
  //      qDebug() << s << S[i];

        i++;
    }

    double wnk = -m;
    double wnn = 0;
    double wn;
    while (fabs(wnk-wnn) > eps)
    {
        wn = (wnn + wnk) / 2;
        double f = wn + m * sin(-atan(m * (2 * X[2] * wn + X[1])));
        if (f > 0)
        {
            wnn = wn;
        }
        else
        {
            wnk = wn;
        }

    }
    double won = wn + 0.5 * d0 * sin(-atan(m * (2 * X[2] * wn + X[1])));

    double wkn = bw;
    double wkk = bw + m;
    double wk;
    while (wkk-wkn > eps)
    {
        wk = (wkn + wkk) / 2;
        double f = wk - bw - m * (ha + c) * sin(-atan(m * (2 * X[2] * wk + X[1])));
        if (f > 0)
        {
            wkk = wk;
        }
        else
        {
            wkn = wk;
        }

    }
    double wok = wk + 0.5 * d0 * sin(-atan(m * (2 * X[2] * wk + X[1])));
    int w00 = int(won);
    int w0k = int(wok) + 1;
 //   qDebug() << w00 << w0k;
    dW = 0.05;
    i = 3;
    QMap<double, QMap<double, double> > profile;
    for (double Woi = w00; Woi <= w0k; Woi += dW)
    {
        double wn = w0;
        double wk = bw + m;
        double wi;
        while (wk-wn > eps)
        {
            wi = (wk + wn) / 2;
            double f = Woi - (wi + 0.5 * d0 * sin(-atan(m * (2 * X[2] * wi + X[1]))));
            if (f > 0)
            {
                wn = wi;
            }
            else
            {
                wk = wi;
            }

        }
        if (wi < 0 || wi > bw)
        {
            continue;
        }
        //X[i] = wi;
        //double Wi = Woi - 0.5 * d0
        double delta_oi = -atan(m * (2 * X[2] * wi + X[1]));
        double xt = X[2] * pow(wi, 2) + X[1] * wi + X[0];
        double aw0 = m * (0.5 * z1 + xt) + 0.5 * d0 * cos(delta_oi);
        double ha_2 = ha / cos(delta_oi);
        double c_2 = c / cos(delta_oi);
        double alpha_2 = atan(tan(alpha) * cos(delta_oi));
        double d = m * z1;
        double d_b = d * cos(alpha_2);
        double ry1_min = ra2 / cos(E) - (W0 + bw) * tan(E);
        for (double ry2 = ra2; ry2 <= rf2 - c * m; ry2 += dr)
        {
            double Wi = wi + W0;
           // ry2 = rav2;
            double ry1 = ry2 / cos(E) - Wi * tan(E);

            // Теоретический профиль
            list = a_tw(ry1, Wi);
            double x_tr = list[0];
            double y_tr = list[1];
            double s_tr = 2 * ry1 * atan(x_tr / y_tr);

            // Практический профиль
            double alpha_ty = acos(0.5 * d_b / ry1);
            double st = m * (M_PI / 2 + 2 * xt * tan(alpha_2) * cos(delta_oi));
            double s_pr = ry1 * (2 * st / d + 2 * (tan(alpha) - alpha) - 2 * (tan(alpha_ty) - alpha_ty));
            profile[wi][ry1 - ry1_min] = s_tr - s_pr;
         //   qDebug() << wi << ry1 << s_tr << s_pr << s_tr - s_pr;
        }

        //qDebug() << Woi << aw0 << wi << xt << delta_oi * 180 / M_PI;
    }



    return profile;

}



QList<double> a_tw (double ry1, double Wi)
{
    // Подбор угла профиля в торцовом сечении
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

// Расчет определителя матрицы

double det(double A[3][3])
{
   return A[0][0] * A[1][1] * A[2][2] +
          A[0][1] * A[1][2] * A[2][0] +
          A[1][0] * A[2][1] * A[0][2] -
          A[2][0] * A[1][1] * A[0][2] -
          A[1][0] * A[0][1] * A[2][2] -
          A[2][1] * A[1][2] * A[0][0];
}
