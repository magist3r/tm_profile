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

#include "paint_area.h"
#include "profile.h"
#include "math.h"
#include "mainwindow.h"
#include <QDebug>

void PaintArea::paintEvent(QPaintEvent *)
{
    if(toPaint)
    {
        QPainter painter(this);
        painter.drawImage(0,0, image);
        toPaint = false;
    }

    /*QTransform transform;
    transform.scale(40,-40);*/
   // painter.setTransform(transform);

 /*   QMap<double, QMap<double,double> > out_list;
    if (ra2 == 0)
    {
        return;
    }
    //out_list = pr_profile(ra2, rf2);


    painter.setTransform(transform);



    QMapIterator<double, QMap<double,double> > i(out_list);
    while (i.hasNext())
    {
        i.next();
        QMapIterator<double,double> j(i.value());
        while (j.hasNext())
        {
            j.next();
            if (j.value() < 0) // Условие попадания в инерционную зону
            {
                painter.setPen(Qt::white);
            }
            else if (j.value() >= 0 && j.value() <= 0.0085)
            {
                painter.setPen(Qt::black);
            }
            else
            {
                painter.setPen(Qt::red);
            }
            painter.drawPoint(QPointF(i.key(),j.key()));
        }
    }*/

}

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    toPaint = false;
}


void PaintArea::drawImage(Profile *profile)
{
    QImage image1(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image1.fill(0);
    QPainter painter(&image1);
    //  painter.begin();

    double r = (profile->rf2 - profile->c * profile->m) / cos(profile->E) - (profile->ra2 / cos(profile->E) - profile->bw * tan(profile->E));

    QTransform transform;
    painter.setRenderHint(painter.Antialiasing, true);

    double scale_x = painter.window().width() / profile->bw;
    double scale_y = painter.window().height() / r;
    double scale;
    transform.scale(1, -1);
    transform.translate(profile->bw * (scale_x - scale_y) / 2, - painter.window().height());

    if (scale_x < scale_y)
    {
        scale = scale_x;
    }
    else
    {
        scale = scale_y;
    }

    transform.scale(scale, scale);
    painter.setTransform(transform);
    QMapIterator<double, QMap<double,double> > i(profile->result);
    while (i.hasNext())
    {
        i.next();
        QMapIterator<double,double> j(i.value());
        while (j.hasNext())
        {
            j.next();
            if (j.value() < 0) // Условие попадания в инерционную зону
            {
                painter.setPen(Qt::white);
            }
            else if (j.value() >= 0 && j.value() <= 0.0085)
            {
                painter.setPen(Qt::black);
            }
            else
            {
                painter.setPen(Qt::red);
            }
            painter.drawPoint(QPointF(i.key(),j.key()));
            emit addToDebugConsole("Wi= " + QString::number(i.key()) + " | ry= " +  QString::number(j.key()) + " | delta_s = " + QString::number(j.value()));
         }
    }
    image = image1;
    toPaint = true;
    image.save("./img.png", "PNG");
    this->update();
    //QPainter painter()
}

