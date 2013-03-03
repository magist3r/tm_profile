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
    QImage image3(width*2 + 50, height, QImage::Format_ARGB32_Premultiplied);

    image1.fill(0);
    image2.fill(0);
    image3.fill(0);

    QPainter painter1(&image1);
    QPainter painter2(&image2);
    QPainter painter3(&image3);

    painter1.setBrush(Qt::transparent);
    painter2.setBrush(Qt::transparent);
    qDebug() << delta << delta_s_max;

    painter1.setRenderHint(QPainter::Antialiasing);
    painter1.setRenderHint(QPainter::Qt4CompatiblePainting);
    painter2.setRenderHint(QPainter::Antialiasing);
    painter2.setRenderHint(QPainter::Qt4CompatiblePainting);

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

    double maxX;
    double maxY;

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
            maxX = qMax(i.key(), maxX);
            maxY = qMax(j.key(), maxY);

        }
    }

    // Draw border
    painter1.setPen(Qt::black);
    painter2.setPen(Qt::black);

    QPointF points[4] = {
        QPointF(maxX, 0.0),
        QPointF(maxX, image_height),
        QPointF(0.0, maxY),
        QPointF(0.0, maxY - image_height)
    };
    painter1.drawConvexPolygon(points, 4);
    painter2.drawConvexPolygon(points, 4);
    painter1.end();
    painter2.end();

    painter3.drawImage(0,0, image1);
    painter3.drawImage(width+50,0, image2);
    painter3.end();

    // Save generated images
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/";
    qDebug() << savePath;
    image1.save(savePath + image_basename + "_1.png");
    image2.save(savePath + image_basename + "_2.png");
    image3.save(savePath + image_basename + "_3.png");
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
