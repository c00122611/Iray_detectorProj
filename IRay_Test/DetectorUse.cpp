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
    Deinit();
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

//校准流程
void DetectorUse::runAutoCalibration() {
    s_Instance = this; // 绑定静态实例指针

    do {
        if (Err_OK != Initialize()) {
            TRACE("Initialization failed.\n");
            break;
        }

        // 设置校准流程属性
        m_pDetInstance->SetAttr(Cfg_CalibrationFlow, 1);

        if (Err_OK != InitCalibration()) {
            TRACE("InitCalibration failed.\n");
            break;
        }

        // 1. 采集暗场
        TRACE("Starting Dark Field Acquisition...\n");
        if (Err_OK != AcquireDarkImages()) {
            TRACE("Dark Field Acquisition failed or interrupted.\n");
            break;
        }

        // 2. 生成 Offset 模板
        TRACE("Generating Offset Map...\n");
        if (Err_OK != GenerateOffsetTemplate()) {
            TRACE("Generate Offset failed.\n");
            break;
        }

        // 3. 采集亮场 - 假设外部射线源已准备好
        TRACE("Starting Light Field Acquisition...\n");
        if (Err_OK != AcquireLightImages()) {
            TRACE("Light Field Acquisition failed or interrupted.\n");
            break;
        }

        // 4. 生成 Gain 模板
        TRACE("Generating Gain Map...\n");
        if (Err_OK != GenerateGainTemplate()) {
            TRACE("Generate Gain failed.\n");
            break;
        }

        // 5. 生成坏点模板
        TRACE("Generating Defect Map...\n");
        if (Err_OK != GenerateDefectTemplate()) {
            TRACE("Generate Defect failed.\n");
            break;
        }

        TRACE("Calibration Process Completed Successfully.\n");

    } while (false);

    FinishCalibration();
    Deinit();
    s_Instance = nullptr; // 清除静态指针
}

// 单次采集
void DetectorUse::runSingleAcquisition() {
    s_Instance = this;

    if (Err_OK == Initialize()) {
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
    else {
        TRACE("Failed to initialize detector for single acquisition.\n");
    }

    Deinit();
    s_Instance = nullptr;
}
// 多次采集
void runSeqAcquisition() {

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
