#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <QObject>
#include <QMap>
#include <QImage>

class ImageGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ImageGenerator(QObject *parent = 0);

    
signals:
    
public slots:
    void paint(QMap<double, QMap<double,double> > &result, double delta, double delta_s_max, double image_width, double image_height);

private:
    QColor getColor(double delta_s, double delta, double min, double max);
};

#endif // IMAGEGENERATOR_H
