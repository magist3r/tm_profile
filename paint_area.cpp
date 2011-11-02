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
   QPainter painter(this);
   painter.drawImage(0,0, image);
}

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{

}


void PaintArea::drawImage(Profile *profile)
{
    image = QImage(321, 171, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);

    double delta_image = delta * sqrt(profile->m);

    QPainter painter(&image);

    painter.setBrush(Qt::white);
//    painter.drawRect(width(), 0, -10, 10);

    double r = (profile->rf2 - profile->c * profile->m) / cos(profile->E) - (profile->ra2 / cos(profile->E) - profile->bw * tan(profile->E));

    QTransform transform;
    painter.setRenderHint(painter.Antialiasing, true);

    double scale_x = painter.window().width() / profile->bw;
    double scale_y = painter.window().height() / r;
    double scale;
    transform.scale(1, -1);
    transform.translate(profile->bw * (scale_x - scale_y) / 2, - painter.window().height());

    if (scale_x < scale_y){
        scale = scale_x;
    }
    else{
        scale = scale_y;
    }

    transform.scale(scale, scale);
    painter.setTransform(transform);

    double max_value;
    double min_value;

    if (profile->useOldPaintMode){
        max_value = delta_image;
        min_value = 0;
    }
    else{
        max_value = profile->delta_s_max;
        min_value = max_value - delta_image;
    }
    double delta_1_3 = min_value + delta_image / 3;
    double delta_2_3 = min_value + delta_image * (2 / 3);
    QMapIterator<double, QMap<double,double> > i(profile->result);
    while (i.hasNext())
    {
        i.next();
        QMapIterator<double,double> j(i.value());
        while (j.hasNext())
        {
            j.next();
            if (j.value() < min_value) // Условие попадания в инерционную зону
            {
                painter.setPen(Qt::white);
            }
            else if (j.value() > max_value)
            {
                painter.setPen(Qt::red);
            }

            if (profile->useSmooth)
            {
                if (j.value() >= min_value && j.value() <= max_value)
                {
                    painter.setPen(QColor(255 - (255/delta_image) * (j.value() - min_value),255 - (255/delta_image) * (j.value() - min_value),255));
                }
            }
            else
            {
                if (j.value() >= min_value && j.value() <= delta_1_3)
                {
                    painter.setPen(QColor(160,160,255));
                }
                else if (j.value() >= delta_1_3 && j.value() < delta_2_3)
                {
                    painter.setPen(QColor(70,70,255));
                }
                else if (j.value() >= delta_2_3 && j.value() <= max_value)
                {
                    painter.setPen(QColor(0,0,255));
                }
            }

            painter.drawPoint(QPointF(i.key(),j.key()));
         }
    }
  //   qDebug() << max_value << max_value / 3 << max_value * 2 / 3;
    image.save("./img.png", "PNG");
    this->update();
}

