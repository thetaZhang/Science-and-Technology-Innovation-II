#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <fstream>
#include "lab1.h"

using namespace std;
const double PI = 3.14159265358979323846;
// 定义复数类型
typedef complex<double> Complex;
// 将double信号转换为complex
vector<complex<double>> convertToComplex(const vector<double>& signal) {
    vector<complex<double>> complexSignal(signal.size());

    for (size_t i = 0; i < signal.size(); ++i) {
        complexSignal[i] = complex<double>(signal[i], 0.0f); // 实部为信号值，虚部为0
    }

    return complexSignal;
}

vector<complex<double>> fft(vector<complex<double>> &x) {
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

// 计算频谱函数
void calculateSpectrum(const vector<Complex>& fftResult, vector<double>& magnitude, vector<double>& phase) {
    int N = fftResult.size();
    magnitude.resize(N);
    phase.resize(N);

    for (int i = 0; i < N; ++i) {
        magnitude[i] = abs(fftResult[i]); // 计算幅度谱
        phase[i] = arg(fftResult[i]); // 计算相位谱
    }

}
void my_fft(vector<double>& data , int N ){
        // 输入信号
    vector<Complex> signal = convertToComplex(data);

    // 计算输入信号的长度
    int originalSize = signal.size();

    // 计算FFT需要的长度（补零）
    int newSize = 1;
    while (newSize < originalSize) {
        newSize *= 2;
    }
    signal.resize(N,0); // 补零
    data.resize(N,0);
    // 对信号进行FFT
   // vector<Complex> y= fft(signal);
    vector<complex<double>> tt = {1,0,2,4};
    vector<Complex> yy= fft(tt);
    vector<Complex> y=fft(signal);
    
    for (int i = 0; i < N; ++i) {
        data[i] = abs(y[i]); // 计算幅度谱
    }

}
vector<double> filter_a(const vector<double>& b, const vector<double>& a, const vector<double>& x) {
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
    return y ; // 
}

vector<double> long_time_hr_fft(const vector<double>& data, double f) {
    vector<double> hr;
    double hr_0;
    for (int i = 0; i < int(data.size() / f) ; i++) {
        const std::vector <double>& part_data =std::vector<double>(data.begin() + i*f, data.begin() + (i+8)*f);
        vector<double>signal=part_data;
        my_fft(signal,4096);//调用FFT函数
        int maxIndex = int(0.4*4096/f)+1;//初始化最大幅值的位置
        for (int i = int(0.4*4096/f)+1; i <= int(4*4096/f)+1; ++i) {
            if (signal[i] > signal[maxIndex]) {
                maxIndex = i;//更新峰值
            }
        }
        hr_0 = maxIndex*f/4096*60;
        //自行添加心率限制，连续计算的心率变化不超过5bpm
        hr.push_back(hr_0);
    }
    return hr;
}

// 主函数
int main() {
        // 打开数据文件
    std::ifstream inputFile("E:/works/Science and Technology Innovation/lab2/lab2-data/ppg_real_jz.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }

    // 读取PPG数据
    vector<double> ppgData;
    double value;
    while (inputFile >> value) {
        ppgData.push_back(value);
    }
    inputFile.close();

    //case1
    vector<double> a={ 1,  -7.512197136829217,  24.714495153604016, -46.51050681663608,  54.76383502861361, -41.313201762614305,  19.500377607032725, -5.2655468955662 , 0.622744822608258};
    vector<double> b={ 0.00005346553295137725, 0, -0.000213862131805509,  0, 0.0003207931977082635, 0, -0.000213862131805509,  0, 0.00005346553295137725};

    std::vector<double> hr_peaks = long_time_hr(ppgData,125);
    vector<double> ppgData_fliter=filter_a(b,a,ppgData);
    std::vector<double> hr_fliter = long_time_hr(ppgData_fliter,125);
    std::vector<double> hr_fft = long_time_hr_fft(ppgData_fliter,125);

    // 输出结果
    cout << "心率结果：" << endl;
    for (int i = 0; i < hr_peaks.size(); ++i) {
        cout << "HR(" << i << ") = " << hr_peaks[i] << endl;
    }
    cout << "心率结果：" << endl;
    for (int i = 0; i < hr_fliter.size(); ++i) {
        cout << "HR(" << i << ") = " << hr_fliter[i] << endl;
    }
    cout << "心率结果：" << endl;
    for (int i = 0; i < hr_fft.size(); ++i) {
        cout << "HR(" << i << ") = " << hr_fft[i] << endl;
    }

    //尝试单独输出某一段PPG信号的FFT结果并打印
    //ppgData_fliter_part=？
    // vector<double>signal_part=part_data;
    //my_fft(ppgData_fliter_part,4096); //改变点数，观察效果
    //打印ppgData_fliter_part


    return 0;
}
