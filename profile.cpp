#include "profile.h"
#include "math.h"
#include <QDebug>
#include <QGenericMatrix>

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
double dx_0;
double dx_bw;

/*
void radius()
// Расчет радиусов вершин и впадин колеса (ra2 и rf2)
*/
void radius()
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
}

/* QMap<double, QMap<double,double> > pr_profile(double ra2, double rf2)
// Расчет толщин зубьев практического и теоретического профилей
*/
QMap<double, QMap<double,double> > pr_profile(double ra2, double rf2)
{
    int n = 100; // Количество разбиений (должно быть четным)
    int n2 = 50; // Количество радиусов
    double dW = bw / n; // Шаг торцовых сечений
    double rav2 = (rf2 + ra2 - c * m) / 2;
    double dr = (rf2 - ra2 - c * m) / n2; // Шаг радиусов

    QList<double> list;



    QMap<double, double> S;
    QList<double> X;
    double Wi = W0;
    // Расчет коэффициентов смещения
    //for (double Wi = W0; Wi <= W0 + bw; Wi += dW)
    for (int i=0; i <= n; i++)
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
        /*if (i == 0 || i == n + 1) {xt += dx;} //Модификация

        sum_Wi += Wi - W0;
        sum_Wi_2 += pow(Wi - W0, 2);
        sum_Wi_3 += pow(Wi - W0, 3);
        sum_Wi_4 += pow(Wi - W0, 4);
        sum_xt += xt;
        sum_xt_2 += xt * (Wi - W0);
        sum_xt_3 += xt * pow(Wi - W0, 2);
        qDebug() << Wi << i;*/
        Wi += dW;
    }

    X = square_metod(S);
    qDebug() << "0>>" << X[2] * 0 + X[1] * 0 + X[0];
    qDebug() << "2.5>>" << X[2] * pow(bw/2,2) + X[1] * bw/2 + X[0];
    qDebug() << "5>>" << X[2] * pow(bw,2) + X[1] * bw + X[0];
    if(dx != 0 || dx_0 != 0 || dx_bw != 0)
    {
        S.clear();
        double Wi = W0;
      /*  for (int i=0; i <= n; i++)
        {

            qDebug() << mod;
            S[Wi-W0] = X[2] * pow(Wi-W0,2) + X[1] * (Wi-W0) + X[0] - mod;
            Wi += dW;
        }*/
        double center = 0.1 * n;
        double mod1 = dx * fabs(0 - center) / (0.5 * n);
        double mod2 = dx * fabs(n - center) / (0.5 * n);
        S[0] = X[2] * 0 + X[1] * 0 + X[0] - dx_0;
        S[bw/2] = X[2] * pow(bw/2,2) + X[1] * bw/2 + X[0] + dx;
        S[bw] = X[2] * pow(bw,2) + X[1] * bw + X[0] - dx_bw;
        X = square_metod(S);
    }
    qDebug() << "0>>" << X[2] * 0 + X[1] * 0 + X[0];
    qDebug() << "2.5>>" << X[2] * pow(bw/2,2) + X[1] * bw/2 + X[0];
    qDebug() << "5>>" << X[2] * pow(bw,2) + X[1] * bw + X[0];
    double w0 = -X[1] / (2 * X[2]);
    qDebug() << ">>>>>>>>>>>>>>>>>.>" << w0;
 /*   double wnk = -m;
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
//    qDebug() << -atan(m * (2 * X[2] * 1 + X[1])) * 180 / M_PI;
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
    qDebug() << w00 << w0k;
    dW = 0.05;
    int i = 3;*/

    QMap<double, QMap<double, double> > profile;

    Wi = W0;
    // Расчет коэффициентов смещения
    //for (double Wi = W0; Wi <= W0 + bw; Wi += dW)
    for (int i=0; i <= n; i++)
    {
        double wi = Wi - W0;
   /*     double wn = w0;
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
   //     qDebug() << wi;
        if (wi < 0 || wi > bw) // Выбор реальных торцовых сечений
        {
            continue;
        }*/

        double delta_oi = -atan(m * (2 * X[2] * wi + X[1]));
        double xt = X[2] * pow(wi, 2) + X[1] * wi + X[0];
        double aw0 = m * (0.5 * z1 + xt) + 0.5 * d0 * cos(delta_oi);
        double ha_2 = ha / cos(delta_oi);
        double c_2 = c / cos(delta_oi);
        double alpha_2 = atan(tan(alpha) * cos(delta_oi));
        double d = m * z1;
        double d_b = d * cos(alpha_2);
        double ry1_min = ra2 / cos(E) - (W0 + bw) * tan(E);

        double ry2 = ra2;
        for (int j=0; j <= n2; j++)
     //   for (double ry2 = ra2; ry2 <= rf2 - c * m; ry2 += dr)
        {

            double ry1 = ry2 / cos(E) - Wi * tan(E);

            // Теоретический профиль
            list = a_tw(ry1, Wi);
            double x_tr = list[0];
            double y_tr = list[1];
            double s_tr = 2 * ry1 * atan(x_tr / y_tr); // Толщина зуба теоретическая

            // Практический профиль
            double alpha_ty = acos(0.5 * d_b / ry1);
            double st = m * (M_PI / 2 + 2 * xt * tan(alpha_2) * cos(delta_oi));
            double s_pr = ry1 * (2 * st / d + 2 * (tan(alpha_2) - alpha_2) - 2 * (tan(alpha_ty) - alpha_ty)); // Толщина зуба практическая
            profile[wi][ry1 - ry1_min] = (s_pr - s_tr) / 2;

            ry2 += dr;
        }
        Wi += dW;
    }
    return profile;
}

QList<double> square_metod(QMap<double, double> S)
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
        qDebug() << "X(i)" << X[i] << det(A_bkp);
    }
    return X;
}

/*
QList<double> a_tw (double ry1, double Wi)
// Подбор угла профиля в торцовом сечении
*/
QList<double> a_tw (double ry1, double Wi)
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

/* double det(double A[3][3])
// Расчет определителя матрицы
*/
double det(double A[3][3])
{
   return A[0][0] * A[1][1] * A[2][2] +
          A[0][1] * A[1][2] * A[2][0] +
          A[1][0] * A[2][1] * A[0][2] -
          A[2][0] * A[1][1] * A[0][2] -
          A[1][0] * A[0][1] * A[2][2] -
          A[2][1] * A[1][2] * A[0][0];
}
