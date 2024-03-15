#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "heartratesensor.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->resizeDisplayDataSize(1000);              // 设置波形窗口大小为1000,8s的窗口
    this->setWindowTitle("工科创II 软件实践I 示例");

    m_heartRateSensorThread = new QThread();                    // new 新线程
    m_heartRateSensor = new HeartRateSensor();                  // new 新传感器对象
    m_heartRateSensor->moveToThread(m_heartRateSensorThread);   // 将传感器对象赋给新线程

    connect(m_heartRateSensor, &HeartRateSensor::heartRateRawDataSignal, this, &MainWindow::onHeartRateRawDataSignalCb);    // 绑定原始数据回调
    connect(m_heartRateSensor, &HeartRateSensor::heartRateSignal, this, &MainWindow::onHeartRateSignalCb);                  // 绑定心率回调
    connect(ui->pushButton, &QPushButton::clicked, m_heartRateSensor, &HeartRateSensor::startSample);                       // 绑定开始采样按钮回调
    connect(ui->pushButton_2, &QPushButton::clicked, m_heartRateSensor, &HeartRateSensor::stopSample);                      // 绑定停止采样按钮回调


    m_heartRateSensorThread->start();                                                                                       // 启动传感器线程
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onHeartRateRawDataSignalCb(std::vector<float> rawdata){
    for(int i = 0; i < rawdata.size(); i++){
        ui->graphicsView->pushdata(rawdata[i]);                                                                             // push新的数据到波形显示
    }
}

void MainWindow::onHeartRateSignalCb(float heartRate){
    ui->label->setText(QString::number(heartRate));                                                                         // 设置新的心率值显示
}

