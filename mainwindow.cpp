#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port_init();
    camera_init();
    //这里应该是相对路径才好，懒得改了，用之前把maptest.html的路径更改一下
    QUrl url("D:/Desktop/maptest.html");
    ui->wgt_map->setUrl(url);
    connect(ui->wgt_map, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*---------------------------------------
 *  function
 *--------------------------------------*/
void MainWindow::port_init()
{
    //port init
    global_port.setParity(QSerialPort::NoParity);   //无校验位
    global_port.setDataBits(QSerialPort::Data8);    //数据位为8位
    global_port.setStopBits(QSerialPort::OneStop);  //一位停止位

    //connect
    //连接信号槽
    QObject::connect(ui->btn_open,&QPushButton::clicked,this,&MainWindow::btn_open_port);
    QObject::connect(ui->btn_close,&QPushButton::clicked,this,&MainWindow::btn_close_port);
    QObject::connect(ui->btn_send,&QPushButton::clicked,this,&MainWindow::btn_send_data);
    QObject::connect(&global_port,&QSerialPort::readyRead,this,&MainWindow::receive_data );
}

void MainWindow::camera_init()
{
    QList<QCameraInfo> list;
    list = QCameraInfo::availableCameras();
    for (int i = 0; i < list.size(); i++){
        qDebug() << list.at(i).deviceName(); //获取设备名称
        qDebug() << list.at(i).description();
        qDebug() << list.at(i).position();
    }
    //
    camera = new QCamera("@device:pnp:\\\\?\\usb#vid_0408&pid_1040&mi_00#7&385b7d3c&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global",this);
    viewfinder = new QCameraViewfinder(this);
    imageCapture = new QCameraImageCapture(camera);

    ui->hlt_view->addWidget(viewfinder);
    camera->setViewfinder(viewfinder);
    camera->start();
}


void MainWindow::btn_open_port(bool)
{
    qDebug()<<ui->cmb_port_name->currentIndex();
    //port name
    switch (ui->cmb_port_name->currentIndex()) {
    case 0:
        global_port.setPortName("COM1");
        break;
    case 1:
        global_port.setPortName("COM2");
        break;
    case 2:
        global_port.setPortName("COM3");
        break;
    case 3:
        global_port.setPortName("COM4");
        break;
    case 4:
        global_port.setPortName("COM5");
        break;
    case 5:
        global_port.setPortName("COM6");
        break;
    case 6:
        global_port.setPortName("COM7");
        break;
    case 7:
        global_port.setPortName("COM8");
        break;
    default:
        break;
    }
    //baud rate
    switch (ui->cmb_baud_rate->currentIndex()) {
    case 0:
        global_port.setBaudRate(QSerialPort::Baud115200);
        break;
    case 1:
        global_port.setBaudRate(QSerialPort::Baud57600);
        break;
    case 2:
        global_port.setBaudRate(QSerialPort::Baud38400);
        break;
    case 3:
        global_port.setBaudRate(QSerialPort::Baud19200);
        break;
    case 4:
        global_port.setBaudRate(QSerialPort::Baud9600);
        break;
    case 5:
        global_port.setBaudRate(QSerialPort::Baud4800);
        break;
    case 6:
        global_port.setBaudRate(QSerialPort::Baud2400);
        break;
    case 7:
        global_port.setBaudRate(QSerialPort::Baud1200);
        break;
    default:
        break;
    }
    //open
    global_port.open(QIODevice::ReadWrite);//用ReadWrite 的模式尝试打开串口
    ui->lab_status->setText("Connected");
}

void MainWindow::btn_close_port(bool)
{
    global_port.close();
    ui->lab_status->setText("Disconnected");

}

void MainWindow::btn_send_data(bool)
{
    QString data = ui->lint_send_data->text();
    QByteArray array = data.toLatin1();//QString-->QByteArray
    global_port.write(array);

}

void MainWindow::receive_data()
{
    QByteArray array = global_port.readAll();
    qDebug()<<array;
    ui->plainTextEdit->moveCursor(QTextCursor::End);//滚动条自动往下滚

    if(ui->chb_hex->checkState() == Qt::Checked){
        ui->plainTextEdit->insertPlainText(QString(array.toHex(' ')));
    }else{
        ui->plainTextEdit->insertPlainText(QString(array));
    }
}

//ak=2MGelREmECQIlwa6YuZihdtlPDhIK05K
void MainWindow::loadFinished(bool a)
{

    if (a == true)
    {
        double lon, lat;
        lon = 121.50866;
        lat = 31.28867;
        QString command = QString("markpoint(%1,%2)").arg(lon).arg(lat);
        ui->wgt_map->page()->runJavaScript(command);
    }
}
