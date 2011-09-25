#include "paint_area.h"
#include "profile.h"
#include "math.h"
#include <QDebug>

void PaintArea::paintEvent(QPaintEvent *)
{
    QMap<double, QMap<double,double> > out_list;
    if (ra2 == 0)
    {
        return;
    }
    out_list = pr_profile(ra2, rf2);

    double r = (rf2 - c * m) / cos(E) - (ra2 / cos(E) - bw * tan(E));
    qDebug() << (rf2 - c * m) / cos(E);
    //double r = (rf2 - ra2 - c * m);

    QPainter painter(this);
    qDebug() << painter.window().height();
    QTransform transform;
    double scale_x = painter.window().width() / bw;
    double scale_y = painter.window().height() / r;
    qDebug() << scale_x << scale_y;
    transform.scale(scale_y, -scale_y);
    transform.translate(bw * (scale_x - scale_y) / (2 * scale_y), - painter.window().height()/scale_y);

 //   qDebug() << min_r;
   // qDebug() << "Hey wazzup????????????????????????/";
    painter.setTransform(transform);

    for (int i = 0; i <= bw; i++)
    {
        painter.drawLine(i,0,i,painter.window().height()/scale_y + bw * tan(E));
    }

    for (int i = 0; i <= r + bw * tan(E); i++)
    {
        painter.drawLine(0,i,bw,i);
    }

    painter.setPen(Qt::gray);
    painter.setRenderHint(painter.Antialiasing, true);

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
     //       qDebug() << i.key() << j.key();
        }
    }
}

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{

}

