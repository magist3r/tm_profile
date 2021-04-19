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
#include "ui_profile.h"
#include "math.h"
#include <QSettings>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

Profile::Profile(QWidget * parent) : QMainWindow(parent), ui(new Ui::Profile)
{
	ui->setupUi(this);

	c_m = 2;

	setAlpha(20);
	c_c   = 0.25;
	c_ha  = 1;
	c_z0  = 19;
	c_x0  = 0.105;
	c_da0 = 43.42;

	ui->spinAlpha->setValue(m_alpha);
	ui->spinC->setValue(c_c);
	ui->spinHa->setValue(c_ha);
	ui->spinZ0->setValue(c_z0);
	ui->spinX0->setValue(c_x0);
	ui->spinDa0->setValue(c_da0);

	c_nr        = 200;
	c_nW        = 250;
	c_mod0      = 0.0;
	c_modCenter = 0.0;
	c_modBw     = 0.0;
	//setUseManualXtList(false);
	setListOfParameters();
	m_trajectory.append(0.0);
	m_trajectory.append(0.0);
	m_trajectory.append(0.0);

	connect(ui->cboParameters, &QComboBox::currentTextChanged, this, &Profile::loadSettings);
	connect(ui->cboModification, &QComboBox::currentTextChanged, this, &Profile::loadModSettings);

	QSettings::setDefaultFormat(QSettings::IniFormat);
	QSettings settings;

	ui->cboParameters->setCurrentText(settings.value("currentParameters").toString());
}

Profile::~Profile()
{
	saveMainSettings();
	delete ui;
}

bool Profile::getRadius()
{
    double alpha_w02;

    double inv_alpha_w02 = ((c_x2 - c_x0) / (c_z2 - c_z0)) * 2 * tan(m_alpha_rad) + tan(m_alpha_rad) - m_alpha_rad;

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
    c_rf2 = (c_m * (c_z2 - c_z0) * (cos(m_alpha_rad)/cos(alpha_w02)) + c_da0) / 2;
    c_ra2 = c_rf2 - c_m * (2 * c_ha + c_c);
    return true;
}

void Profile::calculate()
{
    double imageHeight = (c_rf2 - c_ra2 - c_c * c_m) / cos(m_E_rad);
    double imageWidth = c_bw;

    QMap<double, double> XtList = calculateTheoretical();
    qDebug() << XtList;

    // Задание модификации
    /*if( !useManualXtList() && (mod0() != 0 || modCenter() != 0 || modBw() != 0) ) {
        XtList.clear();

        XtList[0] = m_trajectory[2] * 0 + m_trajectory[1] * 0 + m_trajectory[0] + mod0();
        XtList[bw()/2] = m_trajectory[2] * pow(bw() / 2, 2) + m_trajectory[1] * bw() / 2 + m_trajectory[0] + modCenter();
        XtList[bw()] = m_trajectory[2] * pow(bw(), 2) + m_trajectory[1] * bw() + m_trajectory[0] + modBw();

        m_trajectory = squareMethod(XtList);
    }*/

    qDebug() << XtList;

    saveTrajectory();
    emit trajectoryChanged(m_trajectory);

	//double w0 = -m_trajectory[1] / (2 * m_trajectory[2]);

	calculatePractical();

	QString _baseName = baseName() + "_" + modName();

	emit calculateFinished(m_result, 0.006 * sqrt(c_m), c_delta_s_max, imageWidth, imageHeight, _baseName);
}

QMap<double, double> Profile::calculateTheoretical()
{
    m_theoreticalProfile.clear();

    double i21 = c_z1 / c_z2;
    c_rb2 = 0.5 * c_m * c_z2 * cos(m_alpha_rad);
    c_delta2 = atan(sin(m_E_rad) / (cos(m_E_rad) - i21));
    c_psi_b2 = M_PI / (2 * c_z2) + 2 * c_x2 * tan(m_alpha_rad) / c_z2 + tan(m_alpha_rad) - m_alpha_rad;

    double dW = c_bw / c_nW; // Шаг торцовых сечений
    double rav2 = (c_rf2 + c_ra2 - c_c * c_m) / 2;
    double dr = (c_rf2 - c_ra2 - c_c * c_m) / c_nr; // Шаг радиусов

    double x_tr = 0.0, y_tr = 0.0;

    //QList<double> list;

    QMap<double, double> XtList;

    double Wi = c_W0;

    // Расчет коэффициентов смещения
    //if (m_useManualXtList) {
    //    ;//m_trajectory = squareMethod(manualXtList());
    /*} else */{
        for (int i=0; i <= c_nW; i++) {
            double delta_oi = m_E_rad; // Угол аксоидного конуса шестерни
            double alpha1 = atan(tan(m_alpha_rad) * cos(delta_oi));
            double rb1 = 0.5 * c_m * c_z1 * cos(alpha1);

            double ry1 = rav2 / cos(m_E_rad) - (Wi + 0.1 * c_m) * tan(m_E_rad);
            a_tw(ry1, Wi + 0.1 * c_m, x_tr, y_tr);
            double psi_yi = x_tr / ry1;
            double alpha_y1 = acos(rb1 / ry1);
            double xt1 = (c_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

            ry1 = rav2 / cos(m_E_rad) - (Wi) * tan(m_E_rad);
            a_tw(ry1, Wi, x_tr, y_tr);
            psi_yi = x_tr / ry1;
            alpha_y1 = acos(rb1 / ry1);
            double xt2 = (c_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1));

            delta_oi = atan(10 * (xt2 - xt1));
            alpha1 = atan(tan(m_alpha_rad) * cos(delta_oi));
            rb1 = 0.5 * c_m * c_z1 * cos(alpha1);
            alpha_y1 = acos(rb1 / ry1);
            double xt = (c_z1 * (psi_yi - tan(alpha1) + alpha1 + tan(alpha_y1) - alpha_y1) - 0.5 * M_PI) / (2 * tan(alpha1)); // Коэф. смещения

            XtList[Wi - c_W0] = xt;

            double ry1_min = c_ra2 / cos(m_E_rad) - (c_W0 + c_bw) * tan(m_E_rad);
            double ry2 = c_ra2;

            for (int j=0; j <= c_nr; j++) {
                double ry1 = ry2 / cos(m_E_rad) - Wi * tan(m_E_rad);

                // Теоретический профиль
                a_tw(ry1, Wi, x_tr, y_tr);
                m_theoreticalProfile[Wi - c_W0][ry1 - ry1_min] = 2 * ry1 * atan(x_tr / y_tr); // Толщина зуба теоретическая

                ry2 += dr;
            }

            Wi += dW;
        }

        m_trajectory = squareMethod(XtList);
    }

    return XtList;
}



void Profile::calculatePractical()
{
    m_result.clear();

    double Wi = c_W0;

    double dW = c_bw / c_nW; // Шаг торцовых сечений
    double dr = (c_rf2 - c_ra2 - c_c * c_m) / c_nr; // Шаг радиусов

    //double x_tr = 0.0, y_tr = 0.0;

    c_delta_s_max = 0;
    c_delta_s_min = 10000000;
    // Расчет толщин зубьев
    for (int i=0; i <= c_nW; i++) {
        double wi = Wi - c_W0;
        double delta_oi = -atan(c_m * (2 * m_trajectory[2] * wi + m_trajectory[1]));
        double xt = m_trajectory[2] * pow(wi, 2) + m_trajectory[1] * wi + m_trajectory[0];
        //double aw0 = m_m * (0.5 * m_z1 + xt) + 0.5 * m_d0 * cos(delta_oi);
        //double ha_2 = m_ha / cos(delta_oi);
        //double c_2 = m_c / cos(delta_oi);
        double d = c_m * c_z1;
        double alpha_t = atan(tan(m_alpha_rad) * cos(delta_oi));
        double d_b = d * cos(alpha_t);
        double ry1_min = c_ra2 / cos(m_E_rad) - (c_W0 + c_bw) * tan(m_E_rad);
        double ry2 = c_ra2;

        for (int j=0; j <= c_nr; j++) {
            double ry1 = ry2 / cos(m_E_rad) - Wi * tan(m_E_rad);

            // Практический профиль
            double alpha_ty = acos(0.5 * d_b / ry1);
            double st = c_m * (M_PI / 2 + 2 * xt * tan(m_alpha_rad) * cos(delta_oi));
            double s_pr = ry1 * (2 * st / d + 2 * (tan(alpha_t) - alpha_t) - 2 * (tan(alpha_ty) - alpha_ty)); // Толщина зуба практическая
            double delta_s = (s_pr - m_theoreticalProfile[wi][ry1 - ry1_min]) / 2;
            if (delta_s > c_delta_s_max)
                c_delta_s_max = delta_s;

            if (delta_s < c_delta_s_min)
                c_delta_s_min = delta_s;

            m_result[wi][ry1 - ry1_min] = delta_s;

            ry2 += dr;
        }
        Wi += dW;
    }
}

QList<qreal> Profile::squareMethod(const QMap<double, double> &S)
{
    double sum_x = 0;
    double sum_x2 = 0;
    double sum_x3 = 0;
    double sum_x4 = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_x2y = 0;
    QList<qreal> X; // Матрица результатов (коэффициенты уравнения)

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
        //if (floor(fmod(i.key(), 0.5)*1000) == 0)
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

/*QList<qreal> Profile::squareMethod(const QList<qreal> &S)
{
    QMap<double, double> map;
    QListIterator<qreal> i(S);
    int n = 0;
    while (i.hasNext()) {
        map[n * 1.0 / 2] = i.next();
        n++;
    }
    return squareMethod(map);
}*/

void Profile::a_tw(double ry1, double Wi, double &x_tr, double &y_tr)
{
    double vy2, p, q, alpha_tw, phi1;
    double i21 = c_z1 / c_z2;
    double alpha_tw_n = M_PI / 180;
    double alpha_tw_k = acos((Wi/c_rb2 - sqrt(pow(Wi/c_rb2,2) - 2 * sin(2*m_E_rad) / tan(c_delta2))) / (2 * sin(m_E_rad)));
    while (alpha_tw_k - alpha_tw_n > m_eps) {
        alpha_tw = (alpha_tw_n + alpha_tw_k) / 2;
        vy2 = (Wi / (c_rb2 * sin(m_E_rad)) - (1 / (tan(m_E_rad) * tan(c_delta2) * cos(alpha_tw))) - cos(alpha_tw)) / sin(alpha_tw);
        p = cos(m_E_rad) * (cos(alpha_tw) + vy2 * sin(alpha_tw)) - sin(m_E_rad) / (cos(alpha_tw) * tan(c_delta2));
        q = sin(alpha_tw) - vy2 * cos(alpha_tw);
        if ((c_rb2 * sqrt(pow(q,2) + pow(p,2)) - ry1) > 0) {
             alpha_tw_n = alpha_tw;
        } else {
             alpha_tw_k = alpha_tw;
        }
    }
    phi1 = (alpha_tw + c_psi_b2 - vy2) / i21;
    x_tr = c_rb2 * (p * sin(phi1) - q * cos(phi1));
    y_tr = c_rb2 * (p * cos(phi1) + q * sin(phi1));
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

QString Profile::baseName()
{
    QString name = "m" + QString::number(c_m) +
                   "_" + QString::number(c_z1) +
                   "_" + QString::number(c_z2) +
                   "_" + QString::number(c_bw);
    return name;

}

QString Profile::modName()
{
    /*if (useManualXtList())
        return QString("manual");*/

    QString name1, name2, name3;
    name1 = QString::number(c_mod0 * 1000);
    name2 = QString::number(c_modCenter * 1000);
    name3 = QString::number(c_modBw * 1000);

    if (c_mod0 > 0) name1 = "+" + name1;
    if (c_modCenter > 0) name2 = "+" + name2;
    if (c_modBw > 0) name3 = "+" + name3;

    return name1 + "_" + name2 + "_" + name3;
}



void Profile::saveTrajectory()
{
	QSettings::setDefaultFormat(QSettings::IniFormat);
	QSettings settings;
	settings.beginGroup(baseName());
	settings.beginGroup(modName());
	settings.setValue("a", trajectory().at(2));
	settings.setValue("b", trajectory().at(1));
	settings.setValue("c", trajectory().at(0));
    settings.endGroup();
    settings.endGroup();
}

void Profile::saveMainSettings()
{
	c_m   = ui->spinM->value();
	c_z1  = ui->spinZ1->value();
	c_z2  = ui->spinZ2->value();
	c_bw  = ui->spinBw->value();
	c_W0  = ui->spinW0->value();
	m_E   = ui->spinE->value();
	c_x2  = ui->spinX2->value();
	c_d0  = ui->spinD0->value();
	c_ra2 = ui->spinRa2->value();
	c_rf2 = ui->spinRf2->value();

	setAlpha(ui->spinAlpha->value());
	c_c   = ui->spinC->value();
	c_ha  = ui->spinHa->value();
	c_z0  = ui->spinZ0->value();
	c_x0  = ui->spinX0->value();
	c_da0 = ui->spinDa0->value();

	QSettings::setDefaultFormat(QSettings::IniFormat);
	QSettings settings;

	settings.setValue("currentParameters", ui->cboParameters->currentText().isEmpty() ? baseName() : ui->cboParameters->currentText());
	settings.beginGroup(baseName());
	settings.setValue("m", c_m);
	settings.setValue("z1", c_z1);
	settings.setValue("z2", c_z2);
	settings.setValue("bw", c_bw);
	settings.setValue("W0", c_W0);
	settings.setValue("E", m_E);
	settings.setValue("x2", c_x2);
	settings.setValue("d0", c_d0);
	settings.setValue("ra2", c_ra2);
	settings.setValue("rf2", c_rf2);

	/*settings.setValue("useManualTr", m_useManualXtList);
    if (useManualXtList()) {
        QVariantList variantList;
        QListIterator<qreal> i(m_manualXtList);
        while (i.hasNext())
            variantList << i.next();
        settings.setValue("XtList", variantList);
    }*/

	c_mod0      = ui->spinMod0->value();
	c_modCenter = ui->spinModCenter->value();
	c_modBw     = ui->spinModBw->value();

	settings.beginGroup(ui->cboModification->currentText().isEmpty() ? modName() : ui->cboModification->currentText());
	settings.setValue("mod0", c_mod0);
	settings.setValue("modCenter", c_modCenter);
	settings.setValue("modBw", c_modBw);
	settings.endGroup();

	QStringList list = settings.childGroups();

	settings.endGroup();

	setListOfParameters();
	setModificationList(list);
}

bool Profile::areEmpty()
{
    if ( c_m == 0 ||
         c_z1 == 0 ||
         c_z2 == 0 ||
         c_bw == 0 ||
         c_W0 == 0 ||
         m_E == 0 ||
         c_x2 == 0 ||
         c_d0 == 0 ||
         c_ra2 == 0 ||
         c_rf2 == 0 ||
         m_alpha == 0 ||
         c_ha == 0 ||
         c_c == 0 ||
         c_x0 == 0 ||
         c_z0 == 0 ||
         c_da0 == 0 )
        return true;

    /*if (manualXtList().empty() && useManualXtList())
        return true;

    QListIterator<qreal> i(manualXtList());
    while (i.hasNext()) {
        if (i.next() == 0)
            return true;
    }*/

    return false;
}

bool Profile::imageExists(QString basename)
{
    basename.replace("file:///", QString());
    QFileInfo file1(basename + "_1.png");
    QFileInfo file2(basename + "_2.png");
    if (!file1.exists() || !file2.exists())
        return false;

    return true;
}

void Profile::loadSettings(const QString & value)
{
	QSettings::setDefaultFormat(QSettings::IniFormat);
	QSettings settings;

	if (!value.isEmpty())
	{
		settings.beginGroup(value);
		c_m   = settings.value("m").toDouble();
		c_z1  = settings.value("z1").toDouble();
		c_z2  = settings.value("z2").toDouble();
		c_bw  = settings.value("bw").toDouble();
		c_W0  = settings.value("W0").toDouble();
		m_E   = settings.value("E").toDouble();
		c_x2  = settings.value("x2").toDouble();
		c_d0  = settings.value("d0").toDouble();
		c_ra2 = settings.value("ra2").toDouble();
		c_rf2 = settings.value("rf2").toDouble();

		ui->spinM->setValue(c_m);
		ui->spinZ1->setValue(c_z1);
		ui->spinZ2->setValue(c_z2);
		ui->spinBw->setValue(c_bw);
		ui->spinW0->setValue(c_W0);
		ui->spinE->setValue(m_E);
		ui->spinX2->setValue(c_x2);
		ui->spinD0->setValue(c_d0);
		ui->spinRa2->setValue(c_ra2);
		ui->spinRf2->setValue(c_rf2);

		/*setUseManualXtList(settings.value("useManualTr").toBool());
    QLiyst<QVariant> variantList = settings.value("XtList").toList();
    if ( !variantList.empty() ) {
        QList<qreal> qList;
        QListIterator<QVariant> i(variantList);
        while (i.hasNext())
            qList << i.next().toDouble();

        setManualXtList(qList);
    }*/

		setModificationList(settings.childGroups());
		settings.endGroup();
	}
}

void Profile::loadModSettings(const QString & modValue)
{
	QSettings::setDefaultFormat(QSettings::IniFormat);
	QSettings settings;
	settings.beginGroup(ui->cboParameters->currentText().isEmpty() ? baseName() : ui->cboParameters->currentText());
	settings.beginGroup(modValue);
	c_mod0      = settings.value("mod0").toDouble();
	c_modCenter = settings.value("modCenter").toDouble();
	c_modBw     = settings.value("modBw").toDouble();

	ui->spinMod0->setValue(c_mod0);
	ui->spinModCenter->setValue(c_modCenter);
	ui->spinModBw->setValue(c_modBw);

	m_trajectory[2] = settings.value("a").toDouble();
	m_trajectory[1] = settings.value("b").toDouble();
    m_trajectory[0] = settings.value("c").toDouble();

    emit trajectoryChanged(m_trajectory);
    settings.endGroup();
    settings.endGroup();
}

void Profile::setListOfParameters()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    QRegExp rx("m*");
    rx.setPatternSyntax(QRegExp::Wildcard);

	ui->cboParameters->clear();
	ui->cboParameters->addItems(settings.childGroups().filter(rx));

	/*    if (m_listOfParameters != list) {
        m_listOfParameters = list;
        emit listOfParametersChanged(list);
    }*/
}

void Profile::setModificationList(QStringList arg)
{
	if (arg.isEmpty())
	{
		c_mod0      = 0.0;
		c_modCenter = 0.0;
		c_modBw     = 0.0;
	}

	ui->cboModification->clear();
	ui->cboModification->addItems(arg);

	/*if (m_modificationList != arg) {
        m_modificationList = arg;
        emit modificationListChanged(arg);
    }*/
}
