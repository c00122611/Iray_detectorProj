#include "DetectorUse.h"
#include <cstdarg>   // 
#include <cstdio>    // 

// 静态成员变量定义
DetectorUse* DetectorUse::s_Instance = nullptr;

DetectorUse::DetectorUse() {
    m_pDetInstance = nullptr;
    //默认 采集128张图片做校正
    m_TotalDarkFrames = 128;
    m_TotalLightFrames = 128;
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

QVector<ApplicationModeInfo> DetectorUse::parseApplicationModes(){
    QVector<ApplicationModeInfo> modes;
    std::string workDir = GetWorkDirPath();
    std::string iniPath = workDir + "/DynamicApplicationMode.ini";

    CIniParser ini;
    if (!ini.ReadFile(iniPath)) {
        logMessage("Warning: Failed to read DynamicApplicationMode.ini\n");
        return modes;
    }

    int index = 1;
    while (true) {
        char sectionName[32];
        snprintf(sectionName, sizeof(sectionName), "ApplicationMode%d", index);

        // 尝试读取一个必有字段（如 PGA）来判断节是否存在
        int dummyPga = 0;
        if (!ini.GetItemValueI(sectionName, "PGA", dummyPga)) {
            break; // 节不存在，结束循环
        }

        ApplicationModeInfo mode;
        mode.name = QString("ApplicationMode%1").arg(index);

        std::string subsetStr;
        //默认值
        int pga = 5, binning = 0, zoom = 0;
        double freq = 6.0;

        // 安全读取，失败则保留默认值
        ini.GetItemValueS(sectionName, "subset", subsetStr);
        ini.GetItemValueI(sectionName, "PGA", pga);
        ini.GetItemValueI(sectionName, "Binning", binning);
        ini.GetItemValueI(sectionName, "Zoom", zoom);
        ini.GetItemValueF(sectionName, "Frequency", freq);

        mode.subset = QString::fromStdString(subsetStr.empty() ? "Mode1" : subsetStr);
        mode.pga = pga;
        mode.binning = binning;
        mode.zoom = zoom;
        mode.frequency = freq;

        modes.append(mode);
        index++;
    }

    if (modes.isEmpty()) {
        // 保底一个 Mode1
        ApplicationModeInfo fallback;
        fallback.name = "ApplicationMode1";
        fallback.subset = "Mode1";
        fallback.pga = 5;
        fallback.binning = 0;
        fallback.zoom = 0;
        fallback.frequency = 6.0;
        modes.append(fallback);
    }

    return modes;
}

int DetectorUse::setActiveSubset(const std::string& subsetName) {
    if (!m_pDetInstance) {
        logMessage("Error: Detector not connected.\n");
        return Err_NotInitialized;
    }

    logMessage("Switching to subset: %s\n", subsetName.c_str());
    int ret = m_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, subsetName.c_str(), 5000);
    if (ret == Err_OK) {
        logMessage("Subset switched successfully.\n");
    }
    else {
        logMessage("Failed to switch subset: %s\n", m_pDetInstance->GetErrorInfo(ret).c_str());
    }
    return ret;
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

// DetectorUse.cpp
int DetectorUse::initImageBuffer() {
    // 获取单帧大小
    int width = m_pDetInstance->GetAttrInt(Attr_Width);
    int height = m_pDetInstance->GetAttrInt(Attr_Height);
    //int bpp = m_pDetInstance->GetAttrInt(); // 通常为 2（16-bit）
    //16位图像，两字节 TODO
    size_t frameSize = width * height * 2;

    // 预分配 10 帧缓存
    return m_pDetInstance->UseImageBuf(static_cast<unsigned long long>(frameSize * 10));
}
int DetectorUse::startContinuousAcquisition() {
    if (!m_pDetInstance) return Err_NotInitialized;

    // 确保校正已启用
    int correctOpt = Enm_CorrectOp_SW_PreOffset |
        Enm_CorrectOp_SW_Gain |
        Enm_CorrectOp_SW_Defect;
    m_pDetInstance->SetAttr(Attr_CurrentCorrectOption, correctOpt);

    // 启动连续采集
    int ret = m_pDetInstance->Invoke(Cmd_StartAcq);
    if (ret != Err_OK && ret != Err_TaskPending) {
        logMessage("StartAcq failed: %s\n", m_pDetInstance->GetErrorInfo(ret).c_str());
        return ret;
    }
    logMessage("Continuous acquisition started.\n");
    return Err_OK;
}
int DetectorUse::stopContinuousAcquisition() {
    return m_pDetInstance->Abort(); // 终止当前任务
}
cv::Mat DetectorUse::getCurrentFrame() {
    if (!m_pDetInstance) return cv::Mat();

    int nFrameNum, nImageSize, nPropSize;
    if (Err_OK != m_pDetInstance->QueryImageBuf(nFrameNum, nImageSize, nPropSize)) {
        return cv::Mat();
    }

    std::vector<uchar> buffer(nImageSize);
    int frameIndex;
    if (Err_OK != m_pDetInstance->GetImageFromBuf(buffer.data(), nImageSize, nPropSize, frameIndex)) {
        return cv::Mat();
    }

    int width = m_pDetInstance->GetAttrInt(Attr_Width);
    int height = m_pDetInstance->GetAttrInt(Attr_Height);
    // 数据clone，避免buffer指针重置导致数据丢失
    return cv::Mat(height, width, CV_16UC1, buffer.data()).clone(); 
}
