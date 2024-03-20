#ifndef HEARTRATESENSOR_H
#define HEARTRATESENSOR_H

#include <QObject>
#include <vector>
#include <QTimer>
#include <complex>
#include <cmath>
#include <fstream>

#define delta 4096

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
    void heartRateRawDataSignal(std::vector<double> rawdata);    // 原始数据信号
    void heartRateSignal(double heartRate);                      // 计算心率后的信号

private slots:
    void onDataTimerCb();                                               // 伪数据生成定时器回调函数

private:    
    std::vector<double> m_ppgData;           // 定义浮点数组变量ppgData
    std::vector<double> m_ppgDataHRWindow;   // 8s心率计算窗口
    QTimer *m_dataTimer;                    // 伪数据产生定时器
    double m_heartRate;                      // 当前心率


    // 定义复数类型
    typedef complex<double> Complex;

    vector<complex<double>> convertToComplex(const vector<double>& signal); //double转换为complex
    vector<complex<double>> fft(vector<complex<double>> &x); // FFT计算函数
    double my_fft(double hr_last,vector<double>& data,double f); // 由FFT得到心率

    // 数字滤波
    vector<double> filter_a(const vector<double>& b, const vector<double>& a, const vector<double>& x); 

    // 滤波系数
    vector<double> a={ 1,  -7.512197136829217,  24.714495153604016, -46.51050681663608,  54.76383502861361, -41.313201762614305,  19.500377607032725, -5.2655468955662 , 0.622744822608258};
    vector<double> b={ 0.00005346553295137725, 0, -0.000213862131805509,  0, 0.0003207931977082635, 0, -0.000213862131805509,  0, 0.00005346553295137725};


    double mean(const std::vector<double>& data);                         // 自定义的平均值函数
    vector<double> sliding(const std::vector<double>& data, int w_size);  // 滑动均值滤波
    vector<double> find_peaks(const vector<double>& data);                // 峰值检测--最大最小值法
    double cal_hr(double last_hr, std::vector<double> peaks, double f);     // 通过峰值计算心率

};


#endif // HEARTRATESENSOR_H
