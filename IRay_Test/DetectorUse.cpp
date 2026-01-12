#include "DetectorUse.h"
#include <cstdarg>   // 
#include <cstdio>    // 
#include <QRegularExpression>

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
    if (m_pDetInstance) {
        m_pDetInstance->Destroy(); // 最终释放
        delete m_pDetInstance;
    }
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
    // 1. 加载 DLL（必须在 Create 之前）
    logMessage("Loading Library...");
    int ret = m_pDetInstance->LoadIRayLibrary();
    if (ret != Err_OK) {
        logMessage("FAILED\n");
        return ret;
    }
    logMessage("[OK]\n");

    // 2. 创建探测器实例
    logMessage("Creating Instance...");
    ret = m_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
    if (ret != Err_OK) {
        logMessage("FAILED\n");
        return ret;
    }
    logMessage("[OK]\n");

    // 3. 连接设备
    logMessage("Connecting Device...");
    ret = m_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
    if (ret != Err_OK) {
        logMessage("FAILED\n");
        return ret;
    }
    logMessage("[OK]\n");

    return ret;
}

void DetectorUse::Deinit() {
    if (m_pDetInstance) {
        m_pDetInstance->Destroy();
        // 关键：在 Destroy 后释放 DLL
        m_pDetInstance->FreeIRayLibrary();
        delete m_pDetInstance;
        m_pDetInstance = nullptr;
    }
}

int DetectorUse::InitCalibration() {
    // 校正初始化
    int ret = m_pDetInstance->SyncInvoke(Cmd_CalibrationInit, 5000);
    if (Err_OK == ret) {
        m_TotalDarkFrames = m_pDetInstance->GetAttrInt(Attr_OffsetTotalFrames);
        m_TotalLightFrames = m_pDetInstance->GetAttrInt(Attr_GainTotalFrames);
    }
    logMessage("m_TotalDarkFrames:"+ m_TotalDarkFrames);
    logMessage("m_TotalLightFrames:"+ m_TotalLightFrames);
    return ret;
}
int DetectorUse::GenerateOffsetTemplate() {
    logMessage("开始 Offset 校准...\n");
    int ret = m_pDetInstance->SyncInvoke(Cmd_OffsetGeneration, 10000);
    if (ret == Err_OK) {
        logMessage("Offset 校准成功\n");
    }
    else {
        logMessage("Offset 校准失败: %s\n",
            m_pDetInstance->GetErrorInfo(ret).c_str());
    }
    return ret;
}
int DetectorUse::GenerateGainTemplate() {
    return m_pDetInstance->Invoke(Cmd_GainGeneration);
}
int DetectorUse::AbortCalibration() {
    return m_pDetInstance->Abort();
}
int DetectorUse::GetValidDarkFrames() {
    return m_pDetInstance->GetAttrInt(Attr_OffsetValidFrames);
}

int DetectorUse::GetValidLightFrames() {
    return m_pDetInstance->GetAttrInt(Attr_GainValidFrames);
}
//
int DetectorUse::Connect() {

    if (m_bConnected) {
        logMessage("Already connected.\n");
        return Err_OK;
    }

    // 如果已有实例，直接重试连接
    if (!m_pDetInstance) {
        // 首次连接：创建实例
        m_pDetInstance = new CDetector();
        s_Instance = this;

        // 加载 DLL + 创建
        logMessage("Loading Library...");
        int ret = m_pDetInstance->LoadIRayLibrary();
        if (ret != Err_OK) {
            logMessage("FAILED\n");
            return ret;
        }
        logMessage("[OK]\n");

        // 2. 创建探测器实例
        logMessage("Creating Instance...");
        ret = m_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
        if (ret != Err_OK) {
            logMessage("FAILED\n");
            return ret;
        }
        logMessage("[OK]\n");
    }

    // 尝试连接（可多次调用）
    logMessage("Connecting Device...\n");
    int ret = m_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
    if (ret == Err_OK) {
        m_bConnected = true;
        logMessage("Detector Connected Successfully.\n");
    }
    else {
        logMessage("Connection Failed: %s\n",
            m_pDetInstance->GetErrorInfo(ret).c_str());
    }
    return ret;
}
void DetectorUse::Disconnect() {
    if (m_bConnected) {
        // 仅断开连接，不销毁实例
        m_pDetInstance->SyncInvoke(Cmd_Disconnect, 5000);
        m_bConnected = false;
        logMessage("Detector Disconnected.\n");
    }
}

int DetectorUse::finishCalibrationProcess()
{
    return m_pDetInstance->SyncInvoke(Cmd_FinishGenerationProcess, 3000);
}

int DetectorUse::gainInit()
{
    return m_pDetInstance->SyncInvoke(Cmd_GainInit, 5000);
}
int DetectorUse::acquireLightField(int pointIndex, int framesPerPoint)
{
    logMessage("请开启射线（增益点 %d），开始采集亮场...\n", pointIndex + 1);
    // 启动亮场采集
    m_pDetInstance->Invoke(Cmd_StartAcq);

    // 等待用户操作 + 采集完成
    int nValid = 0;
    while (nValid < framesPerPoint && !m_bError) {
        nValid = m_pDetInstance->GetAttrInt(Attr_GainValidFrames);
        Sleep(100);
    }
    return m_bError ? Err_Unknown : Err_OK;
}
int DetectorUse::acquireDarkField(int framesPerPoint)
{
    logMessage("关闭射线，开始采集暗场...\n");

    // 启动暗场采集
    m_pDetInstance->Invoke(Cmd_ForceDarkContinuousAcq, 0);

    // 等待采集完成
    int nValid = 0;
    while (nValid < framesPerPoint && !m_bError) {
        nValid = m_pDetInstance->GetAttrInt(Attr_OffsetValidFrames); // 注意：暗场用 OffsetValidFrames
        Sleep(100);
    }
    return m_bError ? Err_Unknown : Err_OK;
}

int DetectorUse::GenerateGainAndDefectTemplates()
{
    logMessage("生成 Gain 模板...\n");
    int ret = m_pDetInstance->Invoke(Cmd_GainGeneration);
    if (ret != Err_OK) return ret;

    logMessage("生成 Defect 模板...\n");
    ret = m_pDetInstance->Invoke(Cmd_DefectGeneration);
    return ret;
}
QString DetectorUse::getBaseMode(const QString& subset) {
    // 使用正则提取 "ModeX" 部分（支持 Mode1, Mode1-2, ModeFluoro-10 等）
    QRegularExpression re(R"(^([a-zA-Z]+\d+))");
    QRegularExpressionMatch match = re.match(subset);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    return subset; // fallback
}
QVector<ApplicationModeInfo> DetectorUse::parseApplicationModes() {
    QVector<ApplicationModeInfo> modes;
    std::string workDir = GetWorkDirPath();
    std::string iniPath = workDir + "\\DynamicApplicationMode.ini";

    CIniParser ini;
    if (!ini.ReadFile(iniPath)) {
        logMessage("Warning: Failed to read DynamicApplicationMode.ini at %s\n", iniPath.c_str());
        return modes;
    }

    // 安全扫描：最多尝试 20 个 ApplicationMode
    for (int index = 1; index <= 20; ++index) {
        char sectionName[32];
        snprintf(sectionName, sizeof(sectionName), "ApplicationMode%d", index);

        if (!ini.IsSectionExists(sectionName)) {
            continue; // 跳过不存在的节
        }

        std::string subsetStr;
        int pga = 5;
        int binning = 0;
        int zoom = 0;
        double freq = 6.0;

        // 读取字段（失败则保留默认值）
        ini.GetItemValueS(sectionName, "subset", subsetStr);
        ini.GetItemValueI(sectionName, "PGA", pga);
        ini.GetItemValueI(sectionName, "Binning", binning);
        ini.GetItemValueI(sectionName, "Zoom", zoom);
        ini.GetItemValueF(sectionName, "Frequency", freq);

        // 如果 subset 为空，用默认值
        if (subsetStr.empty()) {
            char defaultSubset[32];
            snprintf(defaultSubset, sizeof(defaultSubset), "Mode%d", index);
            subsetStr = defaultSubset;
        }

        ApplicationModeInfo mode;
        mode.name = QString("ApplicationMode%1").arg(index);
        mode.subset = QString::fromStdString(subsetStr);
        mode.baseMode = getBaseMode(mode.subset);
        mode.pga = pga;
        mode.binning = binning;
        mode.zoom = zoom;
        mode.frequency = freq;

        modes.append(mode);
    }

    // 保底机制
    if (modes.isEmpty()) {
        ApplicationModeInfo fallback;
        fallback.name = "ApplicationMode1";
        fallback.subset = "Mode1";
        fallback.baseMode = "Mode1";
        fallback.pga = 5;
        fallback.binning = 0;
        fallback.zoom = 0;
        fallback.frequency = 6.0;
        modes.append(fallback);
        logMessage("No valid ApplicationMode found. Using fallback mode.\n");
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
    if (!m_pDetInstance) return Err_NotInitialized;

    int ret = m_pDetInstance->SyncInvoke(Cmd_StopAcq, 2000);
    if (ret == Err_OK || ret == Err_TaskPending) {
        m_pDetInstance->ClearImageBuf(); // 清空残留图像
        logMessage("Continuous acquisition stopped.\n");
        return Err_OK;
    }
    logMessage("StopAcq failed: %s\n", m_pDetInstance->GetErrorInfo(ret).c_str());
    return ret;
}

//每个获取的图像数据赋予一个 帧号，确保图像数据不重复
std::pair<cv::Mat, int> DetectorUse::getCurrentFrameWithIndex() {
    if (!m_pDetInstance) return { cv::Mat(), -1 };

    int nFrameNum, nImageSize, nPropSize;
    if (Err_OK != m_pDetInstance->QueryImageBuf(nFrameNum, nImageSize, nPropSize)) {
        return { cv::Mat(), -1 };
    }

    std::vector<uchar> buffer(nImageSize);
    int frameIndex;
    if (Err_OK != m_pDetInstance->GetImageFromBuf(buffer.data(), nImageSize, nPropSize, frameIndex)) {
        return { cv::Mat(), -1 };
    }

    int width = m_pDetInstance->GetAttrInt(Attr_Width);
    int height = m_pDetInstance->GetAttrInt(Attr_Height);
    cv::Mat img = cv::Mat(height, width, CV_16UC1, buffer.data()).clone();
    return { img, frameIndex };   
}
    