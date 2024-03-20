#include "heartratesensor.h"
#include <iostream>
#include <fstream>//?????$)A;6h/;e?
#include <vector>//????$)A0g?e:?
#include <complex>
#include <cmath>

using namespace std;
const double PI = 3.14159265358979323846;


/*
 * $)Ae???d<????
 * $)Ae/9h1!?????
*/
HeartRateSensor::HeartRateSensor()
    : QObject{nullptr}
{
    // ????$)A0f???;6
    std::ifstream inputFile("./lab3-data/ppg_real_jz.txt");// ??????$)Ae:??ifstream?=f?h/;e?txt??;6
    if (!inputFile.is_open()) {                             //???txt??$)A;6?:g?????????
        cerr << "Unable to open file!" << endl;
        exit(-1);
    }

    // $)Ah/;e?PPG?0f?
    double value;                                        //$)Ad84f???5.?9f?o<???(f?d;6f?d8??f/??h!????
    while (inputFile >> value) {
        m_ppgData.push_back(value*100);                       //push_back$)Ae0???0g?ppg?0f??>e??0g?e0>i?
    }
    inputFile.close();                                  //?$)A3i?txt??;6o<????pg?0f?d;%e?e-????pgData?0g?d8?

    // ?????s$)Ae???h.!g?g*??
    m_ppgDataHRWindow.assign(1000,0);
    // ???$)Ae???d8?0
    m_heartRate = 70;


    // $)Ad<?????????6e?
    m_dataTimer = new QTimer(this);
    m_dataTimer->setInterval(2000);     // ?$)A0f?????4i?d8?s
    m_dataTimer->setSingleShot(false);  // $)Ah??;-h'??
    connect(m_dataTimer, &QTimer::timeout, this, &HeartRateSensor::onDataTimerCb);  // $)Ag;??e.???(h??6e?h0????
}

/*
 * $)Ae93e??=f?
*/
double HeartRateSensor::mean(const std::vector<double>& data) {
    // $)Ad=?g? accumulate h.!g??;e?
    double sum = std::accumulate(data.begin(), data.end(), 0.0);//begin??ector$)Ag1;g????<o?end?????
    // $)Ah???e93e???
    return sum / data.size();
}

/*
 * $)Af;?????<f;$f3?
*/
vector<double> HeartRateSensor::sliding(const std::vector<double>& data, int w_size){ //?$)A0f?datao<???(g??#g?e$'e?
    vector<double> data_s(data.size() - w_size + 1); //$)Af;??g*??????0f?size???
    for (int i = 0; i < data.size() - w_size; ++i) {
        // $)Ah.!g?f;??g*????93????
        const std::vector <double>& part_data =std::vector<double>(data.begin() + i, data.begin() + i + w_size);//$)Af;??i??+w_size??atag*???0f?
        // $)Ae-??e93e???
        data_s[i] = mean(part_data);
    }
    return data_s;
}

/*
 * $)Ae30e?<f?f5?
 * ??$)Ae$'f?e0??<f?
*/
vector<double> HeartRateSensor::find_peaks(const vector<double>& data){   //?$)A(i????vector???o<?loatg1;e?
    vector<double> peaks;                        //$)Ae30e?<d?g=??e-?
    vector<double> values;                       //$)Ae30e?<f??<f?e-?
    for (int i = 0; i < data.size() - 2; ++i) { //$)Ae>??o<????0-998?9o???8:d8???(e?d:?+1o<??f-"f?g;????
        if (data[i + 1] > data[i] && data[i + 1] >= data[i + 2]) { //$)Ae30e?<f?f5??;h?
            peaks.push_back(i + 1);  //$)Ad=?=.e-??
            values.push_back(data[i + 1]);//?$)A0e?<e???
        } else {                    //$)Ad;#g?e.???'o???lse
            continue;
        }
    }
    return peaks;
}

/*
 * ???$)Ae30e?<h?g.????
*/
double HeartRateSensor::cal_hr(double last_hr, std::vector<double> peaks, double f){    //$)Ae<?????e???o<??e30e?<f?e$1f????f;!h63h6???0f?d=?g?
    double hr;
    if (peaks.size()>0){//?$)A2f?h/%f?d?!e?f2!f?e30e??
        if (peaks.back() - peaks.front() != 0) {
            // ?$)A9f????h.!g?e???
            hr = f / (peaks.back() - peaks.front()) * (peaks.size() - 1) * 60;
        } else {
            hr = last_hr;//?????????????$)A;h?o<?????d8????:&o<??e$'f?e0?????
        }
    }else{
        hr = last_hr;
    }

    return hr;
}

/**
 * @brief double转换为complex 
 * 
 * @param signal 输入的doublePPG信号 
 * @return vector<complex<double>> 返回的complexPPG信号 
 */
vector<complex<double>> HeartRateSensor::convertToComplex(const vector<double>& signal){
    vector<complex<double>> complexSignal(signal.size());

    for (size_t i = 0; i < signal.size(); ++i) {
        complexSignal[i] = complex<double>(signal[i], 0.0f); // 实部为信号值，虚部为0
    }

    return complexSignal;
}

/**
 * @brief 对输入的一组complex信号作FFT处理,得到频谱数据 
 * 
 * @param x 输入的信号
 * @return vector<complex<double>> 与输入数据数量相同的complex频谱数据
 */
vector<complex<double>> HeartRateSensor::fft(vector<complex<double>> &x){
    int N = x.size();
    if (N <= 1)
        return x;

    vector<complex<double>> even, odd;
    for (int i = 0; i < N; i += 2)
        even.push_back(x[i]);
    for (int i = 1; i < N; i += 2)
        odd.push_back(x[i]);

    even = fft(even);
    odd = fft(odd);

    vector<complex<double>> T(N / 2);
    for (int k = 0; k < N / 2; ++k)
        T[k] = polar(double(1),-2*PI*k/N)* odd[k];

    vector<complex<double>> result(N);
    for (int k = 0; k < N / 2; ++k) {
        result[k] = even[k] + T[k];
        result[k + N / 2] = even[k] - T[k];
    }

    return result;
}

/**
 * @brief 对输入信号调用fft函数由频谱法计算心率
 * 
 * @param last_hr 上一次心率,用于超参数限制
 * @param data 输入PPG信号数据
 * @param f 采样率
 * @return double 
 */
double HeartRateSensor::my_fft(double hr_last,vector<double>& data,double f){
    // 输入信号
    vector<Complex> signal = convertToComplex(data);

    // 补零
    signal.resize(delta,0); 
    data.resize(delta,0);

    // 对信号进行FFT
    vector<Complex> y=fft(signal);
    
    for (int i = 0; i < delta; ++i) {
        data[i] = abs(y[i]); // 计算幅度谱
    }

    double hr_0; // 本次计算心率
    static int count=0;
    int maxIndex = int(0.4*delta/f)+1;//初始化最大幅值的位置
        for (int i = int(0.4*delta/f)+1; i <= int(4*delta/f)+1; ++i) {
            if (data[i] > data[maxIndex]) {
                maxIndex = i;//更新峰值
            }
        }
        hr_0 = maxIndex*f/delta*60;
        //心率限制，连续计算的心率变化不超过10bpm(2s窗口)
        int first=int(0.4*delta/f)+1,last=int(4*delta/f)+1;
        while (count!=0&&hr_0-hr_last>=10.0||count!=0&&hr_last-hr_0>=10.0) {
        if (hr_0-hr_last>=10.0) {
            maxIndex--;
            last=maxIndex;
            for (int j=maxIndex;j>= first; j--) {
                if (data[j] > data[maxIndex]) {
                    maxIndex = j;
                }
            }
            hr_0=maxIndex*f/delta*60;
            
        }
        else if (hr_last-hr_0>=10.0) {
            maxIndex++;
            first=maxIndex;
            for (int k=maxIndex;k<= last; k++) {
                if (data[k] > data[maxIndex]) {
                    maxIndex = k;
                }
            }
            hr_0=maxIndex*f/delta*60;
        }
        }
    count++;   
    return hr_0;
}

/**
 * @brief 对PPG信号进行数字滤波 
 * 
 * @param b 滤波系数分子
 * @param a 滤波系数分母
 * @param x 输入数据
 * @return vector<double> 滤波后的信号输出
 */
vector<double> HeartRateSensor::filter_a(const vector<double>& b, const vector<double>& a, const vector<double>& x){
    vector<double> y; // 用于存储滤波器输出的向量
    y.push_back(b[0] * x[0]); // 初始条件，计算输出y的第一个样本值
    // 外层循环遍历输入信号x中的每个样本
    for (int i = 1; i < x.size(); i++) {
        y.push_back(0); // 初始化输出y的当前样本值为0
        // 计算当前输出y的样本值
        for (int j = 0; j < b.size(); j++) {
            if (i >= j) {
                // 根据差分方程的前向部分，对应相乘并累加
                y[i] = y[i] + b[j] * x[i - j];
            }
        }
        // 计算当前输出y的样本值，根据差分方程的反馈部分
        for (int l = 0; l < a.size() - 1; l++) {
            if (i > l) {
                // 对应相乘并减去前一时刻的反馈
                y[i] = (y[i] - a[l + 1] * y[i - l - 1]);
            }
        }
    }
    return y ; 
}


/*
 *
*/
void HeartRateSensor::onDataTimerCb(){
    static int ppgDataWindow_index = 0;         // ?????$)A4"e<???
//    std::cout << "onDataTimerCb" << std::endl;
    std::vector<double> tempVector (m_ppgDataHRWindow.begin()+250,m_ppgDataHRWindow.end());                                          // $)Ae0?se???g*????sd???
    std::vector<double> addVector (m_ppgData.begin()+ppgDataWindow_index*250, m_ppgData.begin()+ppgDataWindow_index*250+250);        // 2s?$)A0f???
    emit heartRateRawDataSignal(addVector);                                                                                         // ????$)A0f?d?!e?
    tempVector.insert(tempVector.end(), addVector.begin(),addVector.end());                                                         // 8s$)Ae???g*??f7;e?2s?0g??0f?
    m_ppgDataHRWindow = tempVector;                                                                                                 // $)Ad84f?g*??h5??e???g*??

    std::vector<double> filter_data=filter_a(b,a,m_ppgDataHRWindow);                                                             // $)Ai&??e/?atah???10????#e93??                                                                                 // ???$)Ae93e?????0f?h???e30e?<f?f5?
    m_heartRate = my_fft(m_heartRate,filter_data,125);                                                                                    // $)Ah.!g?e???
    emit heartRateSignal(m_heartRate);                                                                                              // $)Ae????>g$:d?!e?

//    std::cout << "heart rate: " << m_heartRate << std::endl;
    ppgDataWindow_index++;
    if(ppgDataWindow_index == 30){
        ppgDataWindow_index = 0;
    }

}




