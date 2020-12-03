#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    int drawRect=0;
    int drawPoint=0;
    int getPoint=0;
    int drawCurve=0;
    int numP=0;
    QPointF intersect(int x1, int y1, int x2, int y2, int sig,int base);
    void curve();
    void fresh();

private slots:

signals:


private:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void paintEvent(QPaintEvent *) override;


    int rect[4];
    int pointx[1024];
    int pointy[1024];
    int pointx_curve[10][100];
    int pointy_curve[10][100];
    int nums=0;
    int _nums=0;
};

#endif // MYLABEL_H
