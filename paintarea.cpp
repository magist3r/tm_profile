/*#include "paintarea.h"

PaintArea::PaintArea(QObject *parent) :
    QImage(parent)
{
}

void PaintArea::paintFirstImage(QMap<double, QMap<double, double> > *points, double scale_x, double scale_y, double delta)
{
    m_image1 = new QImage(320, 240, QImage::Format_ARGB32_Premultiplied);
    m_image2 = new QImage(320, 240, QImage::Format_ARGB32_Premultiplied);

    m_image1->fill(0);
    m_image2->fill(0);

    //double delta_image = delta * sqrt(profile->m_m);
    QPainter painter1(m_image1);
    QPainter painter2(m_image2);

    painter1.setBrush(Qt::white);
    painter2.setBrush(Qt::white);

    painter1.setRenderHint(painter.Antialiasing, true);
    painter2.setRenderHint(painter.Antialiasing, true);
    //    painter.drawRect(width(), 0, -10, 10);

        double r = (profile->m_rf2 - profile->m_c * profile->m_m) / cos(profile->m_E) - (profile->m_ra2 / cos(profile->m_E) - profile->m_bw * tan(profile->m_E));

        QTransform transform;


        double scale_x = painter.window().width() / profile->m_bw;
        double scale_y = painter.window().height() / r;
        double scale;
        transform.scale(1, -1);
        transform.translate(profile->m_bw * (scale_x - scale_y) / 2, - painter.window().height());

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

        if (profile->m_useOldPaintMode){
            max_value = delta_image;
            min_value = 0;
        }
        else{
            max_value = profile->m_delta_s_max;
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

                if (profile->m_useSmooth)
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
http://translate.google.ru/#en/ru/Scales%20the%20coordinate%20system%20by%20sx%20horizontally%20and%20sy%20vertically%2C%20and%20returns%20a%20reference%20to%20the%20matrix.
                painter.drawPoint(QPointF(i.key(),j.key()));

             }
        }
      //   qDebug() << max_value << max_value / 3 << max_value * 2 / 3;
        image.save("./img.png", "PNG");
        this->update();*/
//}*/
