#ifndef MY_QLABEL_H
#define MY_QLABEL_H

//#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>


class my_qlabel : public QLabel
{
    Q_OBJECT
public:
    explicit my_qlabel(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *ev);

    int x,y;
    QPoint globalPos;
signals:
    void Mouse_Pos();


};

#endif // MY_QLABEL_H
