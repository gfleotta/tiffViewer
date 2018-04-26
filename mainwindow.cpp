#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QString>
#include <QPixmap>
#include <QList>
#include <string>
#include <QDebug>
#include "my_qlabel.h"

#include <QToolTip>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QFont>
#include <QPalette>

#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

QStringList archivos;
QString archivo;
QList<Mat> Imagenes;
QList<Mat> ImagenesShow;
QList<Mat> Imagenestemp;
Mat ResizedTemp;


//int colormap=1;
//int contador=1;

//inicializacion

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->Lbl_Image->setScaledContents(true);
    ui->Lbl_Image->setMouseTracking(false);
    connect(ui->Lbl_Image, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_current_pos()) );
    multiplicador=0.04;

    font.setFamily("Noto Sans");
    font.setBold(true);
    font.setPointSize(10);

    ui->action0_04->setFont(font);
    ui->Prev->setEnabled(false);
    ui->Next->setEnabled(false);
    ui->menuConfiguraciones->setEnabled(false);
    ui->SL_temp->setEnabled(false);
    ui->toolButton->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}


//funciones de adaptacion de mat a QLabel
QImage MainWindow::Mat2QImage(cv::Mat const& src)
{
  QImage dest(src.cols, src.rows, QImage::Format_ARGB32);

  const float scale = 255.0;

  if (src.depth() == CV_8U) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = src.at<quint8>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3b bgr = src.at<cv::Vec3b>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  } else if (src.depth() == CV_32F) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = scale * src.at<float>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3f bgr = scale * src.at<cv::Vec3f>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  }

  return dest;
}

QPixmap MainWindow::mat2Label (cv::Mat frame ) {
    QPixmap salida;
    QImage intermedio;
    intermedio=Mat2QImage(frame);
    salida=QPixmap::fromImage(intermedio);
    return salida;

}


//Funcion de conversion de tiff a escala de grises

Mat MainWindow::tiff2GrayScale(Mat ImgTTemp){
    Mat Img_A_Mostrar;
    double min=10000.0,max=0.0;
    float cuantil, minFloat;
    minMaxLoc(ImgTTemp, &min, &max);
    cuantil=float (max-min)/256.0;
    Img_A_Mostrar=Mat::zeros(ImgTTemp.size(),CV_8UC1);
    minFloat=float(min);
    for (int r=0; r<ImgTTemp.rows;++r){
        short unsigned int* ptrSrc = ImgTTemp.ptr<short unsigned int> (r);
        uchar *ptrMuestra = Img_A_Mostrar.ptr(r);
        for (int c=0; c<ImgTTemp.cols; ++c){
            float valorMat= float (ptrSrc[c]);
            ptrMuestra[c]=uchar((valorMat-minFloat)/cuantil);  // /Cuantil
        }
    }
    return Img_A_Mostrar;

}

//Funcion de conversion de tiff a escala de grises
Mat MainWindow::tiff2celcius(Mat ImgTTemp){
    Mat ImgTemperatura;
    double min=10000.0,max=0.0;
    //float  minFloat;
    minMaxLoc(ImgTTemp,&min,&max);
    min=(min*multiplicador)-273.15;
    max=(max*multiplicador)-273.15;
    float mid=(max+min)/2;
    //Cuantil=float(max-min)/256.0;
    qDebug()<<"min: "<<min<<"   Max: "<<max;
    ui->Lbl_max->setText(QString::number(max));
    ui->Lbl_min->setText(QString::number(min));
    ui->Lbl_mid->setText(QString::number(mid));
    ImgTemperatura=Mat::zeros(ImgTTemp.size(),CV_32FC1);
    //minFloat=float(min);
    for (int c=0; c<ImgTTemp.cols; ++c){
        for (int r=0; r<ImgTTemp.rows; ++r){
            float temp = ImgTemperatura.at<float>(r,c)= float(ImgTTemp.at<short unsigned int>(r,c)) * multiplicador - 273.15f;
            //cout << ImgTTemp.at<short unsigned int>(c,r) << endl;
            if (temp == min){
                qDebug() << "minimo: columna   " << c << "  fila   " << r;
            }

        }

    }





    /*
    for (int r=0; r<ImgTTemp.rows;++r){
        short unsigned int *ptrSrc = ImgTTemp.ptr<short unsigned int> (r);
        float *ptrFloat=ImgTemperatura.ptr<float>(r);

        for (int c=0; c<ImgTTemp.cols; ++c){
            float valorMat=float( ptrSrc[c]);
            ptrFloat[c] = valorMat * 0.04 - 273.15;
            //cout << ptrFloat[c] << endl;
        }
    }*/
    return ImgTemperatura;
}






//evento del Boton de Abrir archivos


void MainWindow::on_actionAbrir_triggered()
{
    archivos.clear();
    archivos << QFileDialog::getOpenFileNames(this, "abrir archivos", "", "Radiometria Tiff *.tiff (*.tiff)");

    if (archivos.size() !=0){
        int cantidad=archivos.size();
        contador=0;
        colormap=1;
        Imagenes.clear();
        ImagenesShow.clear();
        Imagenestemp.clear();




        ui->LblTotal->setText(QString::number(cantidad));
        string nombre="";
        for (int i=0; i<cantidad; ++i){


            qDebug()<< "cargando imagen: " << archivos.at(i);
            Mat image;
            QString a=archivos.at(i);
            nombre = a.toStdString();
            try
            {
                image=imread(nombre, CV_LOAD_IMAGE_ANYDEPTH);
            }
            catch(cv::Exception& e){
                qDebug()<< e.what() ;
            }

            Imagenes<<image;
            Mat imageS=tiff2GrayScale(image);
            ImagenesShow<<imageS;
            Mat imageSS=tiff2celcius(image);
            qDebug() << "ancho Imagenes Temp:  " <<imageSS.cols;
            qDebug()<< "alto Imagenes Temp:  " << imageSS.rows << endl;

            Imagenestemp<<imageSS;
        }
        Mat auxx= ImagenesShow.at(0);
        setImage(auxx);
        ui->Lbl_Image->setMouseTracking(true);
        ui->Lbl_color->setPixmap(QPixmap ("./color_gray.jpg"));
        ui->menuConfiguraciones->setEnabled(true);
        ui->toolButton->setVisible(true);

        double min=1000000,max=0.0;
        Mat imagn=Imagenestemp.at(contador);
        minMaxLoc(imagn,&min,&max);
        minimo=min;
        maximo=max;
        double mid=(max+min)/2;
        ui->Lbl_max->setText(QString::number(max,'f',2));
        ui->Lbl_min->setText(QString::number(min,'f',2));
        ui->Lbl_mid->setText(QString::number(mid,'f',2));

        min*=1000;
        max*=1000;
        ui->SL_temp->setRange(int(min),int(max));
        if (ImagenesShow.size()>1)
            ui->Next->setEnabled(true);
        else
            ui->Next->setEnabled(false);
    }
}



void MainWindow::setImage(Mat imagen)
{
    Mat imagens;
    applyColorMap(imagen, imagens, colormap);
    QPixmap foto = MainWindow::mat2Label(imagens);

    Size tamano;
    tamano.height=ui->Lbl_Image->height();
    tamano.width=ui->Lbl_Image->width();
    ui->Lbl_Image->setPixmap(foto);
    ui->LblNumber->setText(QString::number(contador+1));
    QString title = "visorTiff ";
    title= title + ": ("+archivos.at(contador)+" )";
    qDebug()<<title;
    this->setWindowTitle(title);
    imagenLabel=imagens;
}
void MainWindow::setBinImage(Mat imagen)
{
    QPixmap foto = MainWindow::mat2Label(imagen);
    Size tamano;
    tamano.height=ui->Lbl_Image->height();
    tamano.width=ui->Lbl_Image->width();
    ui->Lbl_Image->setPixmap(foto);
    imagenLabel = imagen;

}



void MainWindow::on_Next_clicked()
{
    if (contador<(archivos.size()-1)){
        ui->Next->setEnabled(true);
        ui->Prev->setEnabled(true);
        contador++;

        Mat imagen=ImagenesShow.at(contador);
        setImage(imagen);
        if (contador==(archivos.size()-1))
            ui->Next->setEnabled(false);
    }
    else
        ui->Next->setEnabled(false);
    if (contador==0)
        ui->Prev->setEnabled(false);

    double min=1000000,max=0.0;
    Mat imagn=Imagenestemp.at(contador);
    minMaxLoc(imagn,&min,&max);
    minimo=min;
    maximo=max;
    double mid=(max+min)/2;
    ui->Lbl_max->setText(QString::number(max,'f',2));
    ui->Lbl_min->setText(QString::number(min,'f',2));
    ui->Lbl_mid->setText(QString::number(mid,'f',2));

    min*=1000;
    max*=1000;
    ui->SL_temp->setRange(int(min),int(max));


}

void MainWindow::on_Prev_clicked()
{
    if (contador>0){
        ui->Prev->setEnabled(true);
        ui->Next->setEnabled(true);
        contador--;

        Mat imagen=ImagenesShow.at(contador);
        setImage(imagen);
        if (contador==0)
            ui->Prev->setEnabled(false);
    }
    else if (contador == 0)
        ui->Prev->setEnabled(false);
    double min=1000000,max=0.0;
    Mat imagn=Imagenestemp.at(contador);
    minMaxLoc(imagn,&min,&max);
    minimo=min;
    maximo=max;
    double mid=(max+min)/2;
    ui->Lbl_max->setText(QString::number(max,'f',2));
    ui->Lbl_min->setText(QString::number(min,'f',2));
    ui->Lbl_mid->setText(QString::number(mid,'f',2));

    min*=1000;
    max*=1000;
    ui->SL_temp->setRange(int(min),int(max));


}

void MainWindow::Mouse_current_pos()
{
    qDebug()<< "Mouse Position Temperature";
    qDebug()<< "X= " << ui->Lbl_Image->x;
    qDebug()<< "Y= " << ui->Lbl_Image->y;
    QPoint mousePos(ui->Lbl_Image->x, ui->Lbl_Image->y);
    ostringstream posit;
    posit << "X= " << ui->Lbl_Image->x << "\nY= " << ui->Lbl_Image->y;

    ui->Lbl_x->setText(QString::number(ui->Lbl_Image->x));
    ui->Lbl_y->setText(QString::number(ui->Lbl_Image->y));
    Mat image=Imagenestemp.at(contador);
    if (!image.empty()){

        qDebug()<< "Temperatura  " << image.at<float>(ui->Lbl_Image->y,ui->Lbl_Image->x);
        QToolTip::showText(ui->Lbl_Image->globalPos,"<font color = 'black'>" + (QString::number(image.at<float>(ui->Lbl_Image->y,ui->Lbl_Image->x))) + "</font>");
        //QToolTip::setPalette(QPalette::ToolTipBase)
        ui->Lbl_t->setText(QString::number(image.at<float>(ui->Lbl_Image->y,ui->Lbl_Image->x)));
    }
    qDebug() << "";

}

void MainWindow::on_actionGray_triggered()
{
    colormap = 1;
    Mat imagen=ImagenesShow.at(contador);
    setImage(imagen);
    ui->Lbl_color->setPixmap(QPixmap ("./color_gray.jpg"));
}
void MainWindow::on_actionJet_triggered()
{
    colormap = 2;
    Mat imagen=ImagenesShow.at(contador);
    setImage(imagen);
    ui->Lbl_color->setPixmap(QPixmap ("./color_jet.jpg"));
}


void MainWindow::on_actionHot_triggered()
{
    colormap = 11;
    Mat imagen=ImagenesShow.at(contador);
    setImage(imagen);
    QPixmap color();
    ui->Lbl_color->setPixmap(QPixmap ("./color_hot.jpg"));
}

void MainWindow::on_action0_04_triggered()
{

    font.setFamily("Noto Sans");

    font.setBold(true);
    font.setPointSize(10);

    ui->action0_04->setFont(font);
    font.setBold(false);
    ui->action0_4->setFont(font);

    multiplicador = 0.04;

    Mat image;
    int cantidad=Imagenes.size();
    Imagenestemp.clear();
    for (int i=0; i<cantidad;++i){
        image=Imagenes.at(i);
        Mat aux=tiff2celcius(image);
        Imagenestemp<<aux;
    }


}

void MainWindow::on_action0_4_triggered()
{
    font.setFamily("Noto Sans");

    font.setBold(true);
    font.setPointSize(10);

    ui->action0_4->setFont(font);
    font.setBold(false);
    ui->action0_04->setFont(font);

    multiplicador = 0.4;

    Mat image;
    int cantidad=this->Imagenes.size();
    Imagenestemp.clear();
    for (int i=0; i<cantidad;++i){
        image=Imagenes.at(i);
        Mat aux=tiff2celcius(image);
        Imagenestemp<<aux;
    }

}
Mat MainWindow::binarizar(Mat iTemp){

    Mat Img_A_Mostrar;
    Mat ImgTemperatura;
    double min=10000.0,max=0.0;
    //float cuantil, minFloat;
    minMaxLoc(iTemp, &min, &max);
    //cuantil=float (max-min)/256.0;
    Img_A_Mostrar=Mat::zeros(iTemp.size(),CV_8UC1);
    ImgTemperatura=Mat::zeros(iTemp.size(),CV_32FC1);
    //minFloat=float(min);
    for (int r=0; r<iTemp.rows;++r){
        short unsigned int* ptrSrc = iTemp.ptr<short unsigned int> (r);
        float *ptrFloat=ImgTemperatura.ptr<float>(r);
        uchar *ptrMuestra = Img_A_Mostrar.ptr(r);
        for (int c=0; c<iTemp.cols; ++c){
            float valorMat= float (ptrSrc[c]);

            ptrFloat[c] = valorMat * multiplicador - 273.15;
            if (ptrFloat[c]<umbral)
            {
                ptrMuestra[c]=1;
            }
            else if (ptrFloat[c]>=umbral)
            {
                ptrMuestra[c]=250;
            }

            //ptrMuestra[c]=uchar((valorMat-minFloat)/cuantil);  // /Cuantil
        }
    }



    return Img_A_Mostrar;

}


void MainWindow::on_toolButton_clicked()
{

    if (ui->toolButton->isChecked())
    {
        ui->SL_temp->setEnabled(true);
    }
    if (!ui->toolButton->isChecked())
    {
        ui->SL_temp->setEnabled(false);
        setBinImage(ImagenesShow.at(contador));
    }
}

void MainWindow::on_SL_temp_actionTriggered()
{
    double valor=ui->SL_temp->value();

    umbral=valor/1000;
    ui->Lbl_binTemp->setText(QString::number(umbral,'f',3));

    Binary=binarizar(Imagenes.at(contador));
    setBinImage(Binary);
}

void MainWindow::on_actionGuardar_triggered()
{
    QString Guardar;
    Guardar=QFileDialog::getSaveFileName(this,tr("Guardar Archivo"),"termica.png",tr("Imagenes (*.jpg *.png)"));
    if (Guardar!="")
        imwrite(Guardar.toStdString(),imagenLabel);

}
