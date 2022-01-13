#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/*------------------user----------------*/
//debug
#include <QDebug>
#include <QFileDialog>
//port
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
//camera
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
//web
#include<QWebEngineView>
/*--------------------------------------*/

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
    void btn_open_port(bool);
    void btn_close_port(bool);
    void btn_send_data(bool);
    void receive_data();
    void loadFinished(bool a);

private:
    Ui::MainWindow *ui;
    /*-------------function-----------------*/
    void port_init();
    void camera_init();
    /*-------------variable-----------------*/
    //串口类     类的对象
    QSerialPort global_port;//实例化串口类一个对象
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
};
#endif // MAINWINDOW_H
