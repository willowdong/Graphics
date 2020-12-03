#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QDebug>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    void paintEvent(QPaintEvent *) override;
    int DDA=0;
    int Bresen=0;
protected:
    void mousePressEvent(QMouseEvent *event) override;          //鼠标摁下
    void mouseReleaseEvent(QMouseEvent *event) override;           //鼠标松开
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint start;
    QPoint end;

};

#endif // MYLABEL_H
