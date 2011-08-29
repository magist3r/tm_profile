#ifndef PAINT_AREA_H
#define PAINT_AREA_H

#include <QPainter>
#include <QWidget>

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    PaintArea(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // PAINT_AREA_H
