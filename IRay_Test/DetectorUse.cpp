#include "DetectorUse.h"

// 静态成员变量定义，保证全局单例，唯一s_Instance
DetectorUse* DetectorUse::s_Instance = nullptr;

DetectorUse::DetectorUse() {
    m_pDetInstance = nullptr;
    m_TotalDarkFrames = 0;
    m_TotalLightFrames = 0;
    m_bError = false;
}

DetectorUse::~DetectorUse() {
    // 析构时自动断开，防止忘记调用 Disconnect
    Disconnect();
}

// SDK回调函数
void DetectorUse::SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
    const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam) {

    if (s_Instance != nullptr) {
        // 转发回调给底层SDK实例
        s_Instance->m_pDetInstance->SDKCallback(nDetectorID, nEventID, nEventLevel, pszMsg, nParam1, nParam2, nPtrParamLen, pParam);

        switch (nEventID) {
        case Evt_ConnectProcess:
            // TRACE("%s\n", pszMsg);
            break;
        case Evt_TaskResult_Failed:
            // 设置错误标志，以便主循环退出
            if (nParam1 == Cmd_ForceDarkContinuousAcq) {
                TRACE("Acquisition Error: %s\n", s_Instance->m_pDetInstance->GetErrorInfo(nParam2).c_str());
                s_Instance->m_bError = true;
            }
            break;
        case Evt_Image:
            // TODO 图像处理、存储等逻辑
            break;
        default:
            break;
        }
    }
}

int DetectorUse::Initialize() {
    m_pDetInstance = new CDetector();

    TRACE("Loading Library...");
    int ret = m_pDetInstance->LoadIRayLibrary();
    if (Err_OK != ret) return ret;
    TRACE("[OK]\n");

    TRACE("Creating Instance...");
    ret = m_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
    if (Err_OK != ret) return ret;
    TRACE("[OK]\n");

    TRACE("Connecting Device...");
    ret = m_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
    if (Err_OK != ret) return ret;
    TRACE("[OK]\n");

    TRACE("Setting Application Mode...");
    ret = m_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode1", 5000);
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
    // 轮询直到采集完成或出错
    while (nValid < m_TotalDarkFrames && !m_bError) {
        nValid = GetValidDarkFrames();
        Sleep(100); // 避免 CPU 占用过高
    }

    return m_bError ? Err_Unknown : Err_OK;
}
int DetectorUse::AcquireLightImages() {
    m_bError = false;
    m_pDetInstance->Invoke(Cmd_StartAcq);

    int nValid = 0;
    // 轮询直到采集完成或出错
    while (nValid < m_TotalLightFrames && !m_bError) {
        nValid = GetValidLightFrames();
        Sleep(100); // 避免 CPU 占用过高
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

// 连接 
int DetectorUse::Connect() {
    if (m_pDetInstance != nullptr) {
        TRACE("Already connected.\n");
        return Err_OK; // 或者返回错误，视需求而定
    }

    TRACE("Initializing Detector...\n");
    int ret = Initialize();

    if (Err_OK == ret) {
        s_Instance = this; // 连接成功后绑定静态指针
        TRACE("Detector Connected Successfully.\n");
    }
    else {
        TRACE("Connection Failed.\n");
    }

    return ret;
}

// 断开 
void DetectorUse::Disconnect() {
    if (m_pDetInstance != nullptr) {
        s_Instance = nullptr; // 断开前清除静态指针
        Deinit();
        TRACE("Detector Disconnected.\n");
    }
}

// 偏移校正
void DetectorUse::runOffsetCalibration() {
    // 安全检查：必须先连接
    if (m_pDetInstance == nullptr) {
        TRACE("Error: Detector not connected. Please call Connect() first.\n");
        return;
    }

    do {
        m_pDetInstance->SetAttr(Cfg_CalibrationFlow, 1);

        if (Err_OK != InitCalibration()) {
            TRACE("InitCalibration failed.\n");
            break;
        }

        TRACE("Starting Dark Field Acquisition...\n");
        if (Err_OK != AcquireDarkImages()) {
            TRACE("Dark Field Acquisition failed.\n");
            break;
        }

        TRACE("Generating Offset Map...\n");
        if (Err_OK != GenerateOffsetTemplate()) {
            TRACE("Generate Offset failed.\n");
            break;
        }

        TRACE("Offset Calibration Completed Successfully.\n");

    } while (false);

    FinishCalibration(); // 每次校准结束都通知SDK完成流程
}

// === 内部实现：增益校正 ===
void DetectorUse::runGainCalibration() {
    if (m_pDetInstance == nullptr) {
        TRACE("Error: Detector not connected. Please call Connect() first.\n");
        return;
    }

    do {
        m_pDetInstance->SetAttr(Cfg_CalibrationFlow, 1);

        if (Err_OK != InitCalibration()) {
            TRACE("InitCalibration failed.\n");
            break;
        }

        TRACE("Starting Light Field Acquisition...\n");
        if (Err_OK != AcquireLightImages()) {
            TRACE("Light Field Acquisition failed.\n");
            break;
        }

        TRACE("Generating Gain Map...\n");
        if (Err_OK != GenerateGainTemplate()) {
            TRACE("Generate Gain failed.\n");
            break;
        }

        TRACE("Generating Defect Map...\n");
        if (Err_OK != GenerateDefectTemplate()) {
            TRACE("Generate Defect failed.\n");
            break;
        }

        TRACE("Gain Calibration Completed Successfully.\n");

    } while (false);

    FinishCalibration();
}

// === 内部实现：单次采集 ===
void DetectorUse::runSingleAcquisition() {
    if (m_pDetInstance == nullptr) {
        TRACE("Error: Detector not connected. Please call Connect() first.\n");
        return;
    }

    TRACE("Starting Single Acquisition...\n");

    // 设置同步输出模式
    m_pDetInstance->SetAttr(Attr_UROM_FluroSync_W, Enm_FluroSync_SyncOut);
    m_pDetInstance->SyncInvoke(Cmd_WriteUserRAM, 4000);

    int nExposeWindowTime = 5000;
    int nTimeOut = nExposeWindowTime + 2000;

    m_pDetInstance->SetAttr(Cfg_ClearAcqParam_DelayTime, nExposeWindowTime);
    m_pDetInstance->SyncInvoke(Cmd_ClearAcq, nTimeOut);

    TRACE("Single Acquisition Finished.\n");
}

// 多次采集
void runSeqAcquisition() {

}
