#include "my_qlabel.h"
#include <QDebug>


my_qlabel::my_qlabel(QWidget *parent) :
    QLabel(parent)
{

}

void my_qlabel::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    this->globalPos = ev->globalPos();
    //qDebug()<<"pasa";
    emit Mouse_Pos();
}
