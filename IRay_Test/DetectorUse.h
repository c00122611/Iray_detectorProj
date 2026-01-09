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

    // 错误标志
    bool m_bError;

    // 静态成员变量
    static DetectorUse* s_Instance;

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
    // 静态回调
    static void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
        const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);

public:
    DetectorUse();
    ~DetectorUse();

    // === 连接管理 ===
    // 返回 Err_OK 表示成功，其他值请参考 GetErrorInfo
    int Connect();
    void Disconnect();

    // === 校准流程 ===
    // 偏移校正 (需先 Connect)
    void runOffsetCalibration();

    // 增益校正 (需先 Connect)
    void runGainCalibration();

    // === 单次采集 ===
    void runSingleAcquisition();
};
