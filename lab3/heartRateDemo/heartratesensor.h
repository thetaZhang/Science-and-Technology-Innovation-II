#ifndef HEARTRATESENSOR_H
#define HEARTRATESENSOR_H

#include <QObject>
#include <vector>
#include <QTimer>

using namespace std;

class HeartRateSensor : public QObject
{
Q_OBJECT
public:
    HeartRateSensor();

public slots:
    void startSample(){m_dataTimer->start();}   // 开始产生数据
    void stopSample(){m_dataTimer->stop();}     // 停止产生数据

signals:
    void heartRateRawDataSignal(std::vector<float> rawdata);    // 原始数据信号
    void heartRateSignal(float heartRate);                      // 计算心率后的信号

private slots:
    void onDataTimerCb();                                               // 伪数据生成定时器回调函数

private:
    std::vector<float> m_ppgData;           // 定义浮点数组变量ppgData
    std::vector<float> m_ppgDataHRWindow;   // 8s心率计算窗口
    QTimer *m_dataTimer;                    // 伪数据产生定时器
    float m_heartRate;                      // 当前心率

    float mean(const std::vector<float>& data);                         // 自定义的平均值函数
    vector<float> sliding(const std::vector<float>& data, int w_size);  // 滑动均值滤波
    vector<float> find_peaks(const vector<float>& data);                // 峰值检测--最大最小值法
    float cal_hr(float last_hr, std::vector<float> peaks, float f);     // 通过峰值计算心率

};


#endif // HEARTRATESENSOR_H
