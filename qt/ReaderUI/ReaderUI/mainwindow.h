#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h" // settings
#include <QMainWindow>
#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <qdebug.h>
#include <qlabel.h>


using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSettings_triggered();

    void on_actionOpen_triggered();

    //void on_actionOpen_a_Folder_triggered();

    void on_actionOpen_a_Image_triggered();

    void on_actionScan_triggered();

private:
    Ui::MainWindow *ui;
    Mat src;
    QImage img;
    QString img_name;
    //Mat cvImg;
    QImage prsdImg;

};
#endif // MAINWINDOW_H
