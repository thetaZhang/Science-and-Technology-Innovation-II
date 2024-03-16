#include <iostream>
#include <vector>
#include <fstream>
#include <numeric> // 包含 accumulate 函数
using namespace std;
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
//#include "matplotlibcpp.h"
//namespace plt = matplotlibcpp;

double mean(const std::vector<double>& data) {
    // 使用 accumulate 计算总和
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    // 返回平均值
    return sum / data.size();
}
vector<double> sliding(const std::vector<double>& data, int w_size) {
    vector<double> data_s(data.size() - w_size + 1);
    for (int i = 0; i < data.size() - w_size; ++i) {
        const std::vector <double>& part_data =std::vector<double>(data.begin() + i, data.begin() + i + w_size);
        data_s[i] = mean(part_data);
    }
    return data_s;
}
vector<double> find_peaks(const vector<double>& data) {
    vector<double> peaks;
    vector<double> values;
    for (int i = 0; i < data.size() - 2; ++i) {
        if (data[i + 1] > data[i] && data[i + 1] >= data[i + 2]) {
            peaks.push_back(i + 1);
            values.push_back(data[i + 1]);
        } else {
            continue;
        }
    }
    return peaks;
}
vector<double> find_peaks_threshold(const vector<double>& data, double beta) {
    vector<double> peaks;
    vector<double> values;
    double threshold = mean(data) + abs(mean(data)) * beta;
    for (int i = 0; i < data.size() - 2; ++i) {
        if (data[i + 1] > threshold && data[i + 1] > data[i] && data[i + 1] >= data[i + 2]) {
            peaks.push_back(i + 1);
            values.push_back(data[i + 1]);
        } else {
            continue;
        }
    }
    return peaks;
}
vector<double> find_value_threshold(const vector<double>& data, double beta) {
    vector<double> peaks;
    vector<double> values;
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
vector<double> find_peaks_threshold_par(const vector<double>& data, double f) {
    vector<double> result_peaks = find_peaks_threshold(data,1);
    vector<double> result_values = find_value_threshold(data,1);
    int high_hr=4;
    int i = 0;
    while (i < result_peaks.size() - 2) {
        for (int j = i + 1; j < result_peaks.size() - 1; j++) {
            int delta_peaks = result_peaks[j] - result_peaks[i];
            if (f / delta_peaks > high_hr) {
                if (result_values[j] > result_values[i]) {
                    result_peaks.erase(result_peaks.begin() + i);
                    result_values.erase(result_values.begin() + i);
                } else {
                    result_peaks.erase(result_peaks.begin() + j);
                    result_values.erase(result_values.begin() + j);
                }
                i--;
                break;
            }
        }
        i++;
    }
    return result_peaks;

}
vector<double> find_peaks_after_sliding(const vector<double>& data) {
    const vector<double>& slide_data=sliding(data,20);  
    vector<double> peaks = find_peaks_threshold_par(slide_data,125);

    return peaks;
}
double cal_hr(double last_hr, std::vector<double> peaks, double f) {
    double hr;
    if (peaks.size()>0){
    if (peaks.back() - peaks.front() != 0) {
        hr = f / (peaks.back() - peaks.front()) * (peaks.size() - 1) * 60;
    } else {
        hr = last_hr;
    }
    }else{
        hr = last_hr;
    }

    return hr;
}
vector<double> long_time_hr(const vector<double>& data, double f) {
    vector<double> hr;
    double hr_0;
    for (int i = 0; i < int(data.size() / f) ; i++) {
        const std::vector <double>& part_data =std::vector<double>(data.begin() + i*f, data.begin() + (i+8)*f);
        vector<double> peaks = find_peaks_after_sliding(part_data);
        if (hr.empty()) {
            hr_0 = cal_hr(70, peaks, f);
        } else {
            hr_0 = cal_hr(hr.back(), peaks, f);
        }
        hr.push_back(hr_0);
    }
    return hr;
}
