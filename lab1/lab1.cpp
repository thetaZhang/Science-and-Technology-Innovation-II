#include <iostream>//
#include <vector>//标准数组库
#include <fstream>//有关文件读取
#include <numeric> // 包含 accumulate 函数
using namespace std;

#define cal_window 8
#define sli_window 1

//自定义的平均值函数
float mean(const std::vector<float>& data) {
    // 使用 accumulate 计算总和
    double sum = std::accumulate(data.begin(), data.end(), 0.0);//begin是vector类的初值，end是末值
    // 返回平均值
    return sum / data.size();
}

//滑动窗口
vector<float> sliding(const std::vector<float>& data, int w_size) { //数据data，滑动窗口的大小
    vector<float> data_s(data.size() - w_size + 1); //滑动窗口后，数据size变小
    for (int i = 0; i < data.size() - w_size; ++i) { 
        // 计算滑动窗口的平均值
        const std::vector <float>& part_data =std::vector<float>(data.begin() + i, data.begin() + i + w_size);//滑动i到i+w_size的data窗口数据
        // 存储平均值
        data_s[i] = mean(part_data);
    }
    return data_s;
}

//峰值检测--最大最小值法
vector<float> find_peaks(const vector<float>& data) { //全部采用vector标准，float类型
    vector<float> peaks;                        //峰值位置暂存
    vector<float> values;                       //峰值数值暂存
    for (int i = 0; i < data.size() - 2; ++i) { //循环，考虑0-998点，因为下面用到了i+1，防止数组越界
        if (data[i + 1] > data[i] && data[i + 1] >= data[i + 2]) { //峰值检测逻辑
            peaks.push_back(i + 1);  //位置存储
            values.push_back(data[i + 1]);//数值存储
        } else {                    //代码完备性，写else
            continue;
        }
    }
    return peaks;
}

//峰值检测--阈值法
vector<float> find_peaks_threshold(const vector<float>& data, float beta) { //入参增加阈值系数
    vector<float> peaks;
    vector<float> values;
    double threshold = mean(data) + abs(mean(data)) * beta; //阈值系数的设定，数据的平均值加上平均值的绝对值✖系数beta，此项可自行修改，找到最合适的阈值系数
    for (int i = 0; i < data.size() - 2; ++i) {
        if (data[i + 1] > threshold && data[i + 1] > data[i] && data[i + 1] >= data[i + 2]) {//增加阈值判断条件，其他不变
            peaks.push_back(i + 1);
            values.push_back(data[i + 1]);
        } else {
            continue;
        }
    }
    return peaks;
}

//峰值检测--阈值法的ppg值
vector<float> find_value_threshold(const vector<float>& data, float beta) {
    vector<float> peaks;
    vector<float> values;
    double threshold = mean(data) + abs(mean(data)) * beta;
    for (int i = 0; i < data.size() - 2; ++i) {
        if (data[i + 1] > threshold && data[i + 1] > data[i] && data[i + 1] >= data[i + 2]) {
            peaks.push_back(i + 1);
            values.push_back(data[i + 1]);
        } else {
            continue;
        }
    }
    return values;
}

//峰值检测--带有心率超参数设定的方法
vector<float> find_peaks_threshold_par(const vector<float>& data, float f) {
    vector<float> result_peaks = find_peaks_threshold(data,1);
    vector<float> result_values = find_value_threshold(data,1);
    //
    return result_peaks;

}

//峰值检测--滑动窗口
vector<float> find_peaks_after_sliding(const vector<float>& data) {
    const vector<float>& slide_data=sliding(data,10); //首先对data进行10的窗口平均
    vector<float> peaks = find_peaks_threshold(slide_data,1.2);//再对平均后的数据进行峰值检测

    return peaks;
}

// 通过峰值计算心率
float cal_hr(float last_hr, std::vector<float> peaks, float f) { //引入历史心率，在峰值消失或者不满足超参数时使用
    float hr;
    if (peaks.size()>0){//防止该段信号没有峰值
    if (peaks.back() - peaks.front() != 0) {
        // 根据公式计算心率
        hr = f / (peaks.back() - peaks.front()) * (peaks.size() - 1) * 60;
    } else {
        hr = last_hr;//可增加超参数限制逻辑，心率的上升速度，最大最小心率等
    }
    }else{
        hr = last_hr;
    }

    return hr;
}

//长时间心率计算
vector<float> long_time_hr(const vector<float>& data, float f) { //入参为数据和数据的频率
    vector<float> hr;//定义心率数组
    float hr_0;
    //自行编写
    for (int i=0;i < data.size() - cal_window*f;i+=sli_window*f){
        const std::vector <float>& part_peak =std::vector<float>(data.begin() + i, data.begin() + i + cal_window*f);

        if (hr.empty()) {
            hr_0=cal_hr(70,part_peak,f);
        }
        else {
            hr_0=cal_hr(hr.back(),part_peak,f);
        }
        hr.push_back(hr_0);
    }
    return hr;
}

//误差计算
float cal_error(float hr_0, float hr_1) {
    float mae;
    //自行编写
    
    return mae;
}

int main() {
    // 打开数据文件
    std::ifstream inputFile("E:/works/Science and Technology Innovation/lab1/lab1-data/ppg_spc_8s.txt");// 通过标准库的ifstream函数读取txt文件
    if (!inputFile.is_open()) {                             //排除txt文件出现错误的情况
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }

    // 读取PPG数据
    vector<float> ppgData;                              //定义浮点数组变量ppgData
    float value;                                        //临时的浮点数，存储文件流中的每一行数据
    while (inputFile >> value) {
        ppgData.push_back(value);                       //push_back将最新的ppg数据放在数组尾部
    }
    inputFile.close();                                  //关闭txt文件，此时ppg数据以及存储在ppgData数组中
    
    // 进行峰值检测
    std::vector<float> peakPositions = find_peaks(ppgData);

    // 打印峰值位置
    std::cout << "Peak positions:" << std::endl;
    for (int position : peakPositions) {
        std::cout << position << std::endl;
    }



    return 0;
}