#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <QObject>
#include <QMap>
#include <QImage>

class ImageGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ImageGenerator(QObject *parent = 0, const QString & savePath = "images");
    
signals:
    void imagesGenerated();
    
public slots:
    void paint(QMap<double, QMap<double,double> > &result, double delta, double delta_s_max, double image_width, double image_height, QString image_basename);

private:
    enum colors {
        DarkRed,
        Red,
        Yellow,
        Green,
        White,
        Gray,
        DarkGray,
        Black
    };
    enum legend {
        First,
        Second
    };

    QColor getColor(double delta_s, legend l);
    QImage drawLegend(legend l);
    QColor addColorToLegend(colors c, legend l);
    QColor getColorFromEnum(colors c);


    colors m_legendMin1, m_legendMax1, m_legendMin2, m_legendMax2;
    QList<double> m_legendList1;
    QList<double> m_legendList2;
    QString m_savePath;
};

#endif // IMAGEGENERATOR_H
