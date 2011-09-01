#ifndef PROFILE_H
#define PROFILE_H
#include <QMap>

extern double m, z1, z2, x2, W0, E, alpha, c, ha, z0, x0, da0, i21, rb2, delta2, psi_b2, d0, bw, ra2, rf2, dx;

void radius();

QMap<QString,double> t_profile(double ra2, double rf2);

QMap<double, QMap<double,double> > pr_profile(double ra2, double rf2);

QList<double> a_tw(double ry1, double Wi);

double det(double A[3][3]);

#endif // PROFILE_H
