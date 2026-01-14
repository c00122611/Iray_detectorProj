/**
* @brief 检测器使用类
 * @details 检测器使用类,对探测器使用的封装类，原子封装
 */
#pragma once
#include "stdafx.h"
#include "Detector.h"
#include <Windows.h>
#include <functional> 
#include <string>
#include <QVector>
#include "iniParser.h"
#include "opencv2/opencv.hpp"
#include <QThread>

// 应用模式信息（modeinfo）在 workdir文件下的配置文件中可以看到
struct ApplicationModeInfo {
    QString name;           // e.g., "ApplicationMode1"
    QString subset;         // e.g., "Mode1-2"
    QString baseMode;       // e.g., "Mode1"
    int pga = 5;
    int binning = 0;
    int zoom = 0;
    double frequency = 6.0;
};


class DetectorUse {
private:
    CDetector* m_pDetInstance;
    // 校准需要的帧数统计
    int m_TotalDarkFrames;
    int m_TotalLightFrames;
    mutable std::mutex m_grayMutex;
    int m_currentCenterGray = 0; // 缓存最新灰度值
    bool m_bError;
    bool m_bConnected = false; //连接状态
    static DetectorUse* s_Instance;
    // 日志回调 
    std::function<void(const std::string&)> m_logCallback; 
    // 静态回调
    static void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
        const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);
    int Initialize();      // 仅供 Connect 调用
    void Deinit();         // 仅供 Disconnect 调用
    void logMessage(const char* format, ...);// 内部日志函数

public:
    DetectorUse();
    ~DetectorUse();
    // 设置日志回调（供 Qt 层连接）
    void setLogCallback(std::function<void(const std::string&)> callback) {
        m_logCallback = callback;
    }
    //单步原子操作
    //1：连接
    int Connect();
    void Disconnect();
    //2：mode选择
    QVector<ApplicationModeInfo> parseApplicationModes();
    QString getBaseMode(const QString& subset);
    int setActiveSubset(const std::string& subsetName);
    //3：校正
    int InitCalibration(); // 校准初始化
    //3.1 offset校正
    //3.2 gain校正 + defect校正
    //3.3 完成校正
    int gainInit();
    int acquireLightField(int pointIndex, int framesPerPoint);
    int acquireDarkField(int framesPerPoint);
    int GenerateOffsetTemplate(); 
    int GenerateGainTemplate();
    int GenerateGainAndDefectTemplates();
    int AbortCalibration();
    int finishCalibrationProcess();
    int GetValidDarkFrames(); 
    int GetValidLightFrames();
    //4：图像获取
    int initImageBuffer();
    int startContinuousAcquisition();
    int stopContinuousAcquisition();
    std::pair<cv::Mat, int> getCurrentFrameWithIndex();
    std::tuple<cv::Mat, int, int> getCurrentFrameWithIndex_withcurGray();
    int getCurrentCenterGrayValue();

    int preAcquire(); // 新增接口
    // 获取 PreAcquire 图像（Pull 模式）
    std::pair<cv::Mat, int>getPreAcquiredFrame();
    //5：属性读取
    int getAttrInt(int attrId) {
        return m_pDetInstance ? m_pDetInstance->GetAttrInt(attrId) : 0;
    }
};