#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core.hpp>
#include <QList>


using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Mat tiff2celcius(Mat ImgTTemp);
    Mat tiff2GrayScale(Mat ImgTTemp);
    void setImage(Mat imagen);
    ~MainWindow();

    QImage Mat2QImage(cv::Mat const& src);
    QPixmap mat2Label (cv::Mat frame );





private slots:
    void on_actionSalir_triggered();

    void on_actionAbrir_triggered();

    void on_Next_clicked();

    void on_Prev_clicked();

    void Mouse_current_pos();



    void on_actionGray_triggered();

    void on_actionJet_triggered();

    void on_actionHot_triggered();

    void on_action0_04_triggered();

    void on_action0_4_triggered();

    void on_toolButton_clicked();

    void setBinImage(Mat imagen);
    Mat binarizar(Mat iTemp);

    void on_SL_temp_actionTriggered();

    void on_actionGuardar_triggered();

private:
    Ui::MainWindow *ui;



    QStringList archivos;
    QString archivo;
    QList<Mat> Imagenes;
    QList<Mat> ImagenesShow;
    QList<Mat> Imagenestemp;
    Mat ResizedTemp;
    int colormap=1;
    int contador=0;

    float multiplicador;
    QFont font;
    double umbral;
    Mat Binary;

    double minimo;
    double maximo;

    Mat imagenLabel;
};

#endif // MAINWINDOW_H
