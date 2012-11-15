#include "imagegenerator.h"
#include <QPainter>
#include <QDesktopServices>
#include <QDebug>

ImageGenerator::ImageGenerator(QObject *parent) :
    QObject(parent)
{
}

void ImageGenerator::paint(QMap<double, QMap<double,double> > &result, double delta, double delta_s_max, double image_width, double image_height, QString image_basename)
{
    double width = 250;
    double height = 200;

    QImage image1(width, height, QImage::Format_ARGB32_Premultiplied);
    QImage image2(width, height, QImage::Format_ARGB32_Premultiplied);

    image1.fill(0);
    image2.fill(0);

    QPainter painter1(&image1);
    QPainter painter2(&image2);

    painter1.setBrush(Qt::white);
    painter2.setBrush(Qt::white);
    qDebug() << delta << delta_s_max;

    painter1.setRenderHint(QPainter::Antialiasing);
    painter2.setRenderHint(QPainter::Antialiasing);

    QTransform transform;
    transform.translate(0, height); // move (0,0) to bottom left corner
    transform.scale(1, -1);

    double scale_x = width / image_width;
    double scale_y = height / image_height;
    double scale = qMin(scale_x, scale_y);

    transform.scale(scale, scale);

    painter1.setTransform(transform);
    painter2.setTransform(transform);

    double max_value1 = delta;
    double min_value1 = 0;

    double max_value2 = delta_s_max;
    double min_value2 = delta_s_max - delta;

    QMapIterator<double, QMap<double,double> > i(result);
    while (i.hasNext())
    {
        i.next();
      //  qDebug() << i.key();
        QMapIterator<double,double> j(i.value());
        while (j.hasNext())
        {
            j.next();

            painter1.setPen(getColor(j.value(), min_value1, max_value1));
            painter2.setPen(getColor(j.value(), min_value2, max_value2));
            painter1.drawPoint(QPointF(i.key(), j.key()));
            painter2.drawPoint(QPointF(i.key(), j.key()));
        }
    }
       //   qDebug() << max_value << max_value / 3 << max_value * 2 / 3;
    QString savePath = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/";
    qDebug() << savePath;
    image1.save(savePath + image_basename + "_1.png");
    image2.save(savePath + image_basename + "_2.png");
    emit imagesGenerated(savePath + image_basename + "_1.png", savePath + image_basename + "_2.png");
}

QColor ImageGenerator::getColor(double delta_s, double min, double max)
{
    double delta = max - min;
    double step = delta / 3;
    double min2 = min - delta;

    if (delta_s < min2)
        return Qt::black;
    else if (delta_s < (min2 + step))
        return Qt::darkGray;
    else if (delta_s < (min2 + 2 * step))
        return Qt::gray;
    else if (delta_s < min)
        return Qt::white;
    else if (delta_s < (min + step))
        return Qt::green;
    else if (delta_s < (min + 2 * step))
        return Qt::yellow;
    else if (delta_s < max)
        return Qt::red;
    else
        return Qt::darkRed;
}
