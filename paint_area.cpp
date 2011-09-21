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

    double r = (rf2 - ra2 - c * m);

    QTransform transform;
    transform.scale(40, -40);
    transform.translate(1.7, -3.7);

    QPainter painter(this);
    painter.setTransform(transform);
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
            if (j.value() >= 0 && j.value() <= 0.0085) // Условие попадания в инерционную зону
            {
                painter.setPen(Qt::black);
            }
            else
            {
                painter.setPen(Qt::white);
            }
            painter.drawPoint(QPointF(i.key(),j.key()));
     //       qDebug() << i.key() << j.key();
        }
    }
}

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{

}

