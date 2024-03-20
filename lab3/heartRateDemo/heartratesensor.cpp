#include "heartratesensor.h"
#include <iostream>
#include <fstream>//?????$)A;6h/;e?
#include <vector>//????$)A0g?e:?

using namespace std;

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
    float value;                                        //$)Ad84f???5.?9f?o<???(f?d;6f?d8??f/??h!????
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
float HeartRateSensor::mean(const std::vector<float>& data) {
    // $)Ad=?g? accumulate h.!g??;e?
    double sum = std::accumulate(data.begin(), data.end(), 0.0);//begin??ector$)Ag1;g????<o?end?????
    // $)Ah???e93e???
    return sum / data.size();
}

/*
 * $)Af;?????<f;$f3?
*/
vector<float> HeartRateSensor::sliding(const std::vector<float>& data, int w_size){ //?$)A0f?datao<???(g??#g?e$'e?
    vector<float> data_s(data.size() - w_size + 1); //$)Af;??g*??????0f?size???
    for (int i = 0; i < data.size() - w_size; ++i) {
        // $)Ah.!g?f;??g*????93????
        const std::vector <float>& part_data =std::vector<float>(data.begin() + i, data.begin() + i + w_size);//$)Af;??i??+w_size??atag*???0f?
        // $)Ae-??e93e???
        data_s[i] = mean(part_data);
    }
    return data_s;
}

/*
 * $)Ae30e?<f?f5?
 * ??$)Ae$'f?e0??<f?
*/
vector<float> HeartRateSensor::find_peaks(const vector<float>& data){   //?$)A(i????vector???o<?loatg1;e?
    vector<float> peaks;                        //$)Ae30e?<d?g=??e-?
    vector<float> values;                       //$)Ae30e?<f??<f?e-?
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
float HeartRateSensor::cal_hr(float last_hr, std::vector<float> peaks, float f){    //$)Ae<?????e???o<??e30e?<f?e$1f????f;!h63h6???0f?d=?g?
    float hr;
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

/*
 *
*/
void HeartRateSensor::onDataTimerCb(){
    static int ppgDataWindow_index = 0;         // ?????$)A4"e<???
//    std::cout << "onDataTimerCb" << std::endl;
    std::vector<float> tempVector (m_ppgDataHRWindow.begin()+250,m_ppgDataHRWindow.end());                                          // $)Ae0?se???g*????sd???
    std::vector<float> addVector (m_ppgData.begin()+ppgDataWindow_index*250, m_ppgData.begin()+ppgDataWindow_index*250+250);        // 2s?$)A0f???
    emit heartRateRawDataSignal(addVector);                                                                                         // ????$)A0f?d?!e?
    tempVector.insert(tempVector.end(), addVector.begin(),addVector.end());                                                         // 8s$)Ae???g*??f7;e?2s?0g??0f?
    m_ppgDataHRWindow = tempVector;                                                                                                 // $)Ad84f?g*??h5??e???g*??

    const std::vector<float> &slide_data=sliding(m_ppgDataHRWindow,10);                                                             // $)Ai&??e/?atah???10????#e93??
    vector<float> peaks = find_peaks(slide_data);                                                                                   // ???$)Ae93e?????0f?h???e30e?<f?f5?
    m_heartRate = cal_hr(m_heartRate,peaks,125);                                                                                    // $)Ah.!g?e???
    emit heartRateSignal(m_heartRate);                                                                                              // $)Ae????>g$:d?!e?

//    std::cout << "heart rate: " << m_heartRate << std::endl;
    ppgDataWindow_index++;
    if(ppgDataWindow_index == 30){
        ppgDataWindow_index = 0;
    }

}




