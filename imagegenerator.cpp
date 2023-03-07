#include "imagegenerator.h"
#include <QPainter>
#include <QDesktopServices>
#include <QDebug>
#include <QDir>

ImageGenerator::ImageGenerator(QObject *parent, const QString &savePath) :
    QObject(parent), m_savePath(savePath)
{
}

void ImageGenerator::paint(QMap<double, QMap<double,double> > &result, double delta, double delta_s_max, double image_width, double image_height, QString image_basename)
{
    m_legendMin1 = Black;
    m_legendMin2 = Black;

    m_legendMax1 = DarkRed;
    m_legendMax2 = DarkRed;

    m_legendList1.clear();
    m_legendList2.clear();

    double width = 250;
    double height = 200;

    QImage image1(width, height, QImage::Format_ARGB32_Premultiplied);
    QImage image2(width, height, QImage::Format_ARGB32_Premultiplied);
    QImage image3(width + 100, height, QImage::Format_ARGB32_Premultiplied);
    QImage image4(width + 100, height, QImage::Format_ARGB32_Premultiplied);

    image1.fill(0);
    image2.fill(0);
    image3.fill(0);
    image4.fill(0);

    QPainter painter1(&image1);
    QPainter painter2(&image2);
    QPainter painter3(&image3);
    QPainter painter4(&image4);

    painter1.setBrush(Qt::transparent);
    painter2.setBrush(Qt::transparent);

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

    double step = delta / 3;

    m_legendList1 << delta
                  << delta - step
                  << step
                  << 0.0
                  << -step
                  << -2 * step
                  << -delta;

    m_legendList2 << delta_s_max
                  << delta_s_max - step
                  << delta_s_max - (2 * step)
                  << delta_s_max - (3 * step)
                  << delta_s_max - (4 * step)
                  << delta_s_max - (5 * step)
                  << delta_s_max - (6 * step);

    double maxX;
    double maxY;

    QMapIterator<double, QMap<double,double> > i(result);
    while (i.hasNext())
    {
        i.next();
        QMapIterator<double,double> j(i.value());
        while (j.hasNext())
        {
            j.next();

            painter1.setPen(getColor(j.value(), First));
            painter2.setPen(getColor(j.value(), Second));
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

    painter3.drawImage(0,0, drawLegend(First));
    painter3.drawImage(100,0, image1);
    painter4.drawImage(0, 0, drawLegend(Second));
    painter4.drawImage(100, 0, image2);

    painter3.end();
    painter4.end();

    image3.save(m_savePath + QDir::separator() + image_basename + "_1.png");
    image4.save(m_savePath + QDir::separator() + image_basename + "_2.png");
    emit imagesGenerated();
}

QColor ImageGenerator::getColor(double delta_s, legend l)
{
    QList<double> *list;
    switch (l) {
    case First:
        list = &m_legendList1;
        break;
    case Second:
        list = &m_legendList2;
        break;
    }

    if (delta_s < list->at(6))
        return addColorToLegend(Black, l);
    else if (delta_s < list->at(5))
        return addColorToLegend(DarkGray, l);
    else if (delta_s < list->at(4))
        return addColorToLegend(Gray, l);
    else if (delta_s < list->at(3))
        return addColorToLegend(White, l);
    else if (delta_s < list->at(2))
        return addColorToLegend(Green, l);
    else if (delta_s < list->at(1))
        return addColorToLegend(Yellow, l);
    else if (delta_s < list->at(0))
        return addColorToLegend(Red, l);
    else
        return addColorToLegend(DarkRed, l);
}

QImage ImageGenerator::drawLegend(legend l)
{
    double width = 100;
    double height = 250;
    int min, max;
    QList<double> *list;

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);

    QPainter painter(&image);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::Qt4CompatiblePainting);

    switch (l) {
    case First:
        min = m_legendMin1;
        max = m_legendMax1;
        list = &m_legendList1;
        break;
    case Second:
        min = m_legendMin2 + 1;
        max = m_legendMax2;
        list = &m_legendList2;
        break;
    }

    int n = 0;
    for (int i = min; i <= max; i++) {
        if (i != 0)
            painter.drawText(QRectF(0,n,40,20), Qt::AlignCenter, QString::number(list->at(i-1) * 1000, 'f', 1));

        if ( (i == max) && (i != 7) )
            painter.drawText(QRectF(0,n+20,40,20), Qt::AlignCenter, QString::number(list->at(i) * 1000, 'f', 1));

        painter.setBrush(getColorFromEnum(static_cast<colors>(i)));
        painter.drawRect(40,n+10,40,20);

        n+= 20;
    }

    painter.end();

    return image;
}

QColor ImageGenerator::addColorToLegend(ImageGenerator::colors c, ImageGenerator::legend l)
{
    switch (l) {
        case First:
            if (c < m_legendMin1)
                m_legendMin1 = c;
            if (c > m_legendMax1)
                m_legendMax1 = c;
            break;

        case Second:
            if (c < m_legendMin2)
                m_legendMin2 = c;
            if (c > m_legendMax2)
                m_legendMax2 = c;
            break;
    }

    return getColorFromEnum(c);
}

QColor ImageGenerator::getColorFromEnum(ImageGenerator::colors c)
{
    switch (c) {
        case DarkRed:
            return Qt::darkRed;
        case Red:
            return Qt::red;
        case Yellow:
            return Qt::yellow;
        case Green:
            return Qt::green;
        case White:
            return Qt::white;
        case Gray:
            return Qt::gray;
        case DarkGray:
            return Qt::darkGray;
        case Black:
            return Qt::black;
        default:
            return Qt::black;
    }
}
