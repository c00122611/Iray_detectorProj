#include "DetectorUse.h"
#include <cstdarg>   // 
#include <cstdio>    // 

// 静态成员变量定义
DetectorUse* DetectorUse::s_Instance = nullptr;

DetectorUse::DetectorUse() {
    m_pDetInstance = nullptr;
    m_TotalDarkFrames = 0;
    m_TotalLightFrames = 0;
    m_bError = false;
}

DetectorUse::~DetectorUse() {
    Disconnect();
}

// === 新增：内部日志函数 ===
void DetectorUse::logMessage(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (m_logCallback) {
        m_logCallback(buffer);
    }
    else {
        // 回退到控制台（保留原有 TRACE 行为）
        printf("%s", buffer);
    }
}

// SDK回调函数
void DetectorUse::SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
    const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam) {

    if (s_Instance != nullptr) {
        s_Instance->m_pDetInstance->SDKCallback(nDetectorID, nEventID, nEventLevel, pszMsg, nParam1, nParam2, nPtrParamLen, pParam);

        switch (nEventID) {
        case Evt_ConnectProcess:
            // 通过日志回调输出
            if (pszMsg && s_Instance->m_logCallback) {
                s_Instance->m_logCallback(pszMsg);
            }
            break;
        case Evt_TaskResult_Failed:
            if (nParam1 == Cmd_ForceDarkContinuousAcq) {
                std::string errMsg = "Acquisition Error: " + s_Instance->m_pDetInstance->GetErrorInfo(nParam2);
                s_Instance->m_logCallback(errMsg);
                s_Instance->m_bError = true;
            }
            break;
        case Evt_Image:
            // TODO: 可在此处 emit 图像
            break;
        default:
            break;
        }
    }
}

int DetectorUse::Initialize() {
    m_pDetInstance = new CDetector();

    logMessage("Loading Library...");
    int ret = m_pDetInstance->LoadIRayLibrary();
    if (Err_OK != ret) {
        logMessage("FAILED\n");
        return ret;
    }
    logMessage("[OK]\n");

    logMessage("Creating Instance...");
    ret = m_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
    if (Err_OK != ret) {
        logMessage("FAILED\n");
        return ret;
    }
    logMessage("[OK]\n");

    logMessage("Connecting Device...");
    ret = m_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
    if (Err_OK != ret) {
        logMessage("FAILED\n");
        return ret;
    }
    logMessage("[OK]\n");

    logMessage("Setting Application Mode...");
    ret = m_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode1", 5000);
    if (Err_OK != ret) {
        logMessage("FAILED\n");
    }
    else {
        logMessage("[OK]\n");
    }
    return ret;
}

void DetectorUse::Deinit() {
    if (m_pDetInstance) {
        m_pDetInstance->Destroy();
        m_pDetInstance->FreeIRayLibrary();
        delete m_pDetInstance;
        m_pDetInstance = nullptr;
    }
}

int DetectorUse::InitCalibration() {
    int ret = m_pDetInstance->SyncInvoke(Cmd_CalibrationInit, 5000);
    if (Err_OK == ret) {
        m_TotalDarkFrames = m_pDetInstance->GetAttrInt(Attr_OffsetTotalFrames);
        m_TotalLightFrames = m_pDetInstance->GetAttrInt(Attr_GainTotalFrames);
    }
    return ret;
}

int DetectorUse::AcquireDarkImages() {
    m_bError = false;
    m_pDetInstance->Invoke(Cmd_ForceDarkContinuousAcq, 0);

    int nValid = 0;
    while (nValid < m_TotalDarkFrames && !m_bError) {
        nValid = GetValidDarkFrames();
        Sleep(100);
    }

    return m_bError ? Err_Unknown : Err_OK;
}

int DetectorUse::AcquireLightImages() {
    m_bError = false;
    m_pDetInstance->Invoke(Cmd_StartAcq);

    int nValid = 0;
    while (nValid < m_TotalLightFrames && !m_bError) {
        nValid = GetValidLightFrames();
        Sleep(100);
    }

    return m_bError ? Err_Unknown : Err_OK;
}

int DetectorUse::GenerateOffsetTemplate() {
    return m_pDetInstance->Invoke(Cmd_OffsetGeneration);
}

int DetectorUse::GenerateGainTemplate() {
    return m_pDetInstance->Invoke(Cmd_GainGeneration);
}

int DetectorUse::GenerateDefectTemplate() {
    return m_pDetInstance->Invoke(Cmd_DefectGeneration);
}

int DetectorUse::AbortCalibration() {
    return m_pDetInstance->Abort();
}

void DetectorUse::FinishCalibration() {
    m_pDetInstance->SyncInvoke(Cmd_FinishGenerationProcess, 3000);
}

int DetectorUse::GetValidDarkFrames() {
    return m_pDetInstance->GetAttrInt(Attr_OffsetValidFrames);
}

int DetectorUse::GetValidLightFrames() {
    return m_pDetInstance->GetAttrInt(Attr_GainValidFrames);
}

int DetectorUse::Connect() {
    if (m_pDetInstance != nullptr) {
        logMessage("Already connected.\n");
        return Err_OK;
    }

    logMessage("开始初始化 Initializing Detector...\n");
    int ret = Initialize();

    if (Err_OK == ret) {
        s_Instance = this;
        logMessage("Detector Connected Successfully.\n");
    }
    else {
        logMessage("Connection Failed.\n");
    }

    return ret;
}

void DetectorUse::Disconnect() {
    if (m_pDetInstance != nullptr) {
        s_Instance = nullptr;
        Deinit();
        logMessage("Detector Disconnected.\n");
    }
}

void DetectorUse::runOffsetCalibration() {
    if (m_pDetInstance == nullptr) {
        logMessage("Error: Detector not connected. Please call Connect() first.\n");
        return;
    }

    bool bSuccess = false;
    do {
        m_pDetInstance->SetAttr(Cfg_CalibrationFlow, 1);

        if (Err_OK != InitCalibration()) {
            logMessage("InitCalibration failed.\n");
            break;
        }

        logMessage("Starting Dark Field Acquisition...\n");
        if (Err_OK != AcquireDarkImages()) {
            logMessage("Dark Field Acquisition failed.\n");
            break;
        }

        logMessage("Generating Offset Map...\n");
        if (Err_OK != GenerateOffsetTemplate()) {
            logMessage("Generate Offset failed.\n");
            break;
        }

        bSuccess = true;

    } while (false);

    FinishCalibration();

    if (bSuccess) {
        logMessage("Offset Calibration Completed Successfully.\n");
    }
    else {
        logMessage("Offset Calibration FAILED.\n");
    }
}

void DetectorUse::runGainCalibration() {
    if (m_pDetInstance == nullptr) {
        logMessage("Error: Detector not connected. Please call Connect() first.\n");
        return;
    }

    bool bSuccess = false;
    do {
        m_pDetInstance->SetAttr(Cfg_CalibrationFlow, 1);

        if (Err_OK != InitCalibration()) {
            logMessage("InitCalibration failed.\n");
            break;
        }

        logMessage("Starting Light Field Acquisition...\n");
        if (Err_OK != AcquireLightImages()) {
            logMessage("Light Field Acquisition failed.\n");
            break;
        }

        logMessage("Generating Gain Map...\n");
        if (Err_OK != GenerateGainTemplate()) {
            logMessage("Generate Gain failed.\n");
            break;
        }

        logMessage("Generating Defect Map...\n");
        if (Err_OK != GenerateDefectTemplate()) {
            logMessage("Generate Defect failed.\n");
            break;
        }

        bSuccess = true;

    } while (false);

    FinishCalibration();

    if (bSuccess) {
        logMessage("Gain Calibration Completed Successfully.\n");
    }
    else {
        logMessage("Gain Calibration FAILED.\n");
    }
}

void DetectorUse::runSingleAcquisition() {
    if (m_pDetInstance == nullptr) {
        logMessage("Error: Detector not connected. Please call Connect() first.\n");
        return;
    }

    logMessage("Starting Single Acquisition...\n");

    m_pDetInstance->SetAttr(Attr_UROM_FluroSync_W, Enm_FluroSync_SyncOut);
    m_pDetInstance->SyncInvoke(Cmd_WriteUserRAM, 4000);

    int nExposeWindowTime = 5000;
    int nTimeOut = nExposeWindowTime + 2000;

    m_pDetInstance->SetAttr(Cfg_ClearAcqParam_DelayTime, nExposeWindowTime);
    int ret = m_pDetInstance->SyncInvoke(Cmd_ClearAcq, nTimeOut);

    if (ret == Err_OK || ret == Err_TaskPending) {
        logMessage("Single Acquisition command sent.\n");
    }
    else {
        logMessage("Single Acquisition failed: %s\n", m_pDetInstance->GetErrorInfo(ret).c_str());
    }
}

void DetectorUse::runSeqAcquisition() {
    logMessage("Sequential acquisition not implemented yet.\n");
}