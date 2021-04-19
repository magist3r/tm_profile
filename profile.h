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
#include <QMainWindow>
#include <QDebug>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Ui
{
	class Profile;
}

class Profile : public QMainWindow
{
    Q_OBJECT

public:
	explicit Profile(QWidget * parent = nullptr);
	~Profile();

	Q_INVOKABLE bool getRadius(); // Расчет радиусов вершин и впадин колеса (ra2 и rf2)

	Q_INVOKABLE void calculate();
	Q_INVOKABLE QMap<double, double> calculateTheoretical(); // Расчет толщин зубьев теоретического профиля
	Q_INVOKABLE void calculatePractical(); // Расчет толщин зубьев практического профиля


	// Расчетные величины
	double c_i21; // Передаточное отношение
	double c_rb2; // Радиус основной окружности колеса
	double c_delta2;
	double c_psi_b2;

	QList<double> squareMethod(const QMap<double, double> & S);

	QList<double> squareMethod(const QList<qreal> & S);

	void a_tw(double ry1, double Wi, double & x_tr, double & y_tr); // Подбор угла профиля в торцовом сечении

	double det(const double A[3][3]); // Расчет определителя матрицы

	QColor getPointColor(double delta_s);

	void saveTrajectory();

	QMap<double, QMap<double, double>> m_theoreticalProfile;
	QMap<double, QMap<double, double>> m_result;


	double c_m;

	double c_z1; // Число зубьев шестерни
	double c_z2; // Число зубьев колеса
	double c_x2; // Коэффициент смещения колеса
	double c_W0; // Аппликата большего торцового сечения венца шестерни

	double c_bw; // Ширина зубчатого венца

	// Параметры исходного контура

	double c_c;  // Коэффициент радиального зазора
	double c_ha; // Коэффициент высоты зуба

	// Параметры зуборезного долбяка
	double c_z0;  // Число зубьев инструмента
	double c_x0;  // Коэффициент смещения
	double c_da0; // Диаметр вершин зубьев

	double c_d0; // Делительный диаметр фрезы

	double c_ra2; // Радиус вершин зубьев колеса
	double c_rf2; // Радиус впадин колеса

	static constexpr double m_eps = 0.000001; // Требуемая точность расчётов
	double c_nW;
	double c_nr;

	double c_delta_s_max;
	double c_delta_s_min;

	//bool m_useSmooth;
	//bool m_useOldPaintMode;
	//bool m_diagnosticMode;

	double c_mod0;
	double c_modCenter;
	double c_modBw;


	double E() const { return m_E; }
	double alpha() const { return m_alpha; }

	QList<qreal> trajectory() const { return m_trajectory; }

	Q_INVOKABLE void saveMainSettings();

	Q_INVOKABLE bool areEmpty();

	Q_INVOKABLE bool imageExists(QString basename);


	Q_INVOKABLE QString baseName();

	Q_INVOKABLE QString modName();

public slots:

	void setE(double arg)
	{
		if (m_E != arg)
		{
			m_E     = arg;
			m_E_rad = M_PI * arg / 180;
		}
	}

	void setAlpha(double arg)
	{
		if (m_alpha != arg)
		{
			m_alpha     = arg;
			m_alpha_rad = M_PI * arg / 180;
		}
	}

	void setDataLocation(QString arg)
	{
		if (m_dataLocation != arg)
		{
			m_dataLocation = arg;
			//			emit dataLocationChanged(arg);
		}
	}

	void setModificationList(QStringList arg);

	void loadSettings(const QString & value = QString());

	void loadModSettings(const QString & modValue);

	void setListOfParameters();

signals:
	void calculateFinished(QMap<double, QMap<double, double>> & _result, double delta, double _delta_s_max, double image_width,
	                       double image_height, QString image_basename);
	void trajectoryChanged(QList<qreal> arg);
	//void useManualXtListChanged(bool arg);
	//void manualXtListChanged(QList<qreal> arg);
	//void dataLocationChanged(QString arg);
	//void listOfParametersChanged(QStringList arg);
	//void modificationListChanged(QStringList arg);

private:
	double m_E; // Межосевой угол
	double m_E_rad;

	double m_alpha; // Угол профиля
	double m_alpha_rad;

	QList<qreal> m_trajectory;
	//QStringList m_listOfParameters;
	//bool m_useManualXtList;
	//QList<qreal> m_manualXtList;
	QString m_dataLocation;
	//QStringList m_modificationList;

	Ui::Profile * ui;
};

#endif // PROFILE_H
