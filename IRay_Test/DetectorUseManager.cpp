#include "DetectorUseManager.h"

DetectorUseManager::DetectorUseManager(QObject* parent)
    : QObject(parent)
{
    // 设置日志回调（处理 SDK 的 GBK 字符串）
    m_detectorUse.setLogCallback([this](const std::string& msg) {
        QString qMsg = QString::fromLocal8Bit(msg.c_str());
        logInfo(qMsg); 
        });
}

DetectorUseManager::~DetectorUseManager() {
    m_detectorUse.Disconnect();
}

void DetectorUseManager::logInfo(const QString& msg) {
    emit logMessage(msg); 
}
//开启连接
void DetectorUseManager::connectDevice() {
    int ret = m_detectorUse.Connect();
    if (ret == Err_OK) {
        logInfo(QString::fromUtf8(" 探测器连接成功"));
        emit connectionChanged(true);
    }
    else {
        logInfo(QString::fromLocal8Bit("连接失败: %1").arg(ret));
        emit connectionChanged(false);
    }
}
//断开连接
void DetectorUseManager::disconnectDevice() {
    m_detectorUse.Disconnect();
    logInfo(QString::fromLocal8Bit("探测器已断开"));
}
//偏移校正
void DetectorUseManager::startOffsetCalibration() {
    logInfo(QString::fromLocal8Bit("开始偏移校准"));
    m_detectorUse.runOffsetCalibration();
    // 注意：不要在这里 emit "完成"，因为 runOffsetCalibration 已经通过 logMessage 告知结果
    // 如果需要 calibrationFinished 信号，需修改 runOffsetCalibration 返回 bool
}
//增益校正
void DetectorUseManager::startGainCalibration() {
    logInfo(QString::fromLocal8Bit("开始增益校准"));
    m_detectorUse.runGainCalibration();
}
//单次采集
void DetectorUseManager::startSingleAcquisition() {
    logInfo(QString::fromLocal8Bit("开始单帧采集"));
    m_detectorUse.runSingleAcquisition();
}
// 模式选择
void DetectorUseManager::onSelectModeClicked() {
    ModeSelectionDialog dialog(&m_detectorUse, nullptr);
    if (dialog.exec() == QDialog::Accepted) {
        QString subset = dialog.selectedSubset();
        if (!subset.isEmpty()) {
            // 调用 DetectorUse 切换
            std::string subsetStr = subset.toStdString();
            int ret = m_detectorUse.setActiveSubset(subsetStr);
            if (ret == Err_OK) {
                logInfo(QString::fromLocal8Bit("已切换到模式: %1").arg(subset));
                emit applicationModeChanged(subset, true);
            }
            else {
                logInfo(QString::fromLocal8Bit("切换模式失败: %1").arg(ret));
                emit applicationModeChanged(subset, false);
            }
        }
    }
}

// DetectorUseManager.cpp
void DetectorUseManager::startFluoroDisplay() {
    if (!m_fluoroTimer) {
        m_fluoroTimer = new QTimer(this);
        connect(m_fluoroTimer, &QTimer::timeout, this, &DetectorUseManager::onFluoroTimerTimeout);
    }
    // 3帧帧率 显示 
    m_fluoroTimer->start(500);
}

void DetectorUseManager::stopFluoroDisplay() {
    if (m_fluoroTimer) {
        m_fluoroTimer->stop();
    }
}

void DetectorUseManager::onFluoroTimerTimeout() {
    cv::Mat frame = m_detectorUse.getCurrentFrame();
    if (!frame.empty()) {
        QImage img = MatToQImage(frame);
        emit newFrameReceived(img);
    }
}