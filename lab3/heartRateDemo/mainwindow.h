#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "heartratesensor.h"

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
//    void on_pushButton_clicked(){
//        m_heartRateSensor->startSample();
//    }

//    void on_pushButton_2_clicked(){
//        m_heartRateSensor->stopSample();
//    }

    void onHeartRateRawDataSignalCb(std::vector<double> rawdata);    // 原始数据回调

    void onHeartRateSignalCb(double heartRate);                      // 心率回调

private:
    Ui::MainWindow *ui;                                          // ui
    HeartRateSensor *m_heartRateSensor;                          // 心率传感器对象
    QThread *m_heartRateSensorThread = nullptr;                  // 心率传感器线程
};
#endif // MAINWINDOW_H
