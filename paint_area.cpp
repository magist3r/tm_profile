#include "paint_area.h"
#include "profile.h"
#include <QDateTime>
#include "math.h"
#include "QDebug"

void PaintArea::paintEvent(QPaintEvent *)
{
    QMap<double, QMap<double,double> > out_list;
    //load_vars();
    out_list = pr_profile(ra2, rf2);

    double r = (rf2 - ra2 - c * m);

    QTransform transform;
    transform.scale(40, -40);
    transform.translate(0, -3.7);
    //transform.rotate(90);


    QPointF points[4] =
    {
        QPointF(bw, 0),
        QPointF(bw, r),
        QPointF(0, r + bw * tan(E)),
        QPointF(0, bw * tan(E))
    };
    QPainter painter(this);
    painter.setTransform(transform);
    painter.setPen(Qt::black);

    //painter.setFont(QFont("Arial", 30));
   // painter.drawText(rect(), Qt::AlignCenter, QTime::currentTime().toString());
    painter.drawConvexPolygon(points,4);
    //painter.d;
    painter.setRenderHint(painter.Antialiasing, true);
    //painter.


    QMapIterator<double, QMap<double,double> > i(out_list);
    while (i.hasNext())
    {
        i.next();
        QMapIterator<double,double> j(i.value());
        while (j.hasNext())
        {
            j.next();
            if (j.value() >= 0 && j.value() <= 0.0085)
            {
                painter.setPen(Qt::green);
            }
            else
            {
                painter.setPen(Qt::red);
            }
            painter.drawPoint(QPointF(i.key(),j.key()));
            qDebug() << i.key() << j.key() << j.value();
        }
    }

}

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{

}

