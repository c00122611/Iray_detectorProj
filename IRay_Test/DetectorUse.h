#pragma once
#include "stdafx.h"
#include "Detector.h"
#include <Windows.h>
#include <functional> 
#include <string>     

class DetectorUse {
private:
    CDetector* m_pDetInstance;

    // 校准需要的帧数统计
    int m_TotalDarkFrames;
    int m_TotalLightFrames;

    // 错误标志
    bool m_bError;

    // 静态成员变量
    static DetectorUse* s_Instance;

    // === 日志回调 ===
    std::function<void(const std::string&)> m_logCallback; // 👈 新增

    // === 内部辅助函数 ===
    int Initialize();      // 仅供 Connect 调用
    void Deinit();         // 仅供 Disconnect 调用
    int InitCalibration(); // 供各校准步骤调用
    int AcquireDarkImages();
    int AcquireLightImages();
    int GenerateOffsetTemplate();
    int GenerateGainTemplate();
    int GenerateDefectTemplate();
    void FinishCalibration();
    int AbortCalibration();
    int GetValidDarkFrames();
    int GetValidLightFrames();

    // 内部日志函数
    void logMessage(const char* format, ...); 

    // 静态回调
    static void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
        const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);

public:
    DetectorUse();
    ~DetectorUse();

    // 设置日志回调（供 Qt 层连接）
    void setLogCallback(std::function<void(const std::string&)> callback) {
        m_logCallback = callback;
    }

    // === 连接管理 ===
    int Connect();
    void Disconnect();

    // === 校准流程 ===
    void runOffsetCalibration();
    void runGainCalibration();

    // === 采集 ===
    void runSingleAcquisition();
    void runSeqAcquisition();
};