/*#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QImage>

class PaintArea : public QImage
{
    Q_OBJECT
public:
    explicit PaintArea(QObject *parent = 0);
    void paint(QMap<double, QMap<double,double> > *points, double scale_x, double scale_y, QImage *image);
signals:
    
public slots:

private:
    QImage *m_image1;
    QImage *m_image2;
    
};

#endif // PAINTAREA_H*/
