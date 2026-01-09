#pragma once
#include "stdafx.h"
#include "Detector.h"
#include <calibration.h>
#include <Windows.h>

class DetectorUse {
private:
    CDetector* m_pDetInstance;

    // 校准需要的帧数统计
    int m_TotalDarkFrames;
    int m_TotalLightFrames;

    // 错误标志，用于异步操作中断
    bool m_bError;

    // 静态成员变量用于回调函数访问
    static DetectorUse* s_Instance;

    // 私有辅助函数
    int Initialize();
    void Deinit();
    int InitCalibration();
    int AcquireDarkImages();
    int AcquireLightImages();
    int GenerateOffsetTemplate();
    int GenerateGainTemplate();
    int GenerateDefectTemplate();
    int AbortCalibration();
    void FinishCalibration();
    int GetValidDarkFrames();
    int GetValidLightFrames();

    // 静态回调函数 (精简版)
    static void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
        const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);

public:
    DetectorUse();
    ~DetectorUse();

    // 公共接口函数
    // 自动执行完整的连接和校准流程
    void runAutoCalibration();
    // 执行单次采集
    void runSingleAcquisition();
};
