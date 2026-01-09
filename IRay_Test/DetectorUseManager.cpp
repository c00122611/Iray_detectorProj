#include "DetectorUseManager.h"

DetectorUseManager::DetectorUseManager(QObject* parent)
    : QObject(parent)
{
    // 设置日志回调（处理 SDK 的 GBK 字符串）
    m_detectorUse.setLogCallback([this](const std::string& msg) {
        QString qMsg = QString::fromLocal8Bit(msg.c_str());
        emit logMessage(qMsg);
        });
}

DetectorUseManager::~DetectorUseManager() {
    m_detectorUse.Disconnect();
}

void DetectorUseManager::connectDevice() {
    int ret = m_detectorUse.Connect();
    if (ret == Err_OK) {
        emit logMessage(" 探测器连接成功");
        emit connectionChanged(true);
    }
    else {
        emit logMessage(QString("连接失败: %1").arg(ret));
        emit connectionChanged(false);
    }
}

void DetectorUseManager::disconnectDevice() {
    m_detectorUse.Disconnect();
    emit logMessage("探测器已断开");
}

void DetectorUseManager::startOffsetCalibration() {
    emit logMessage("开始偏移校准...");
    m_detectorUse.runOffsetCalibration(); // 依赖内部 logMessage 输出结果
    // 注意：不要在这里 emit "完成"，因为 runOffsetCalibration 已经通过 logMessage 告知结果
    // 如果需要 calibrationFinished 信号，需修改 runOffsetCalibration 返回 bool
}

void DetectorUseManager::startGainCalibration() {
    emit logMessage("开始增益校准...");
    m_detectorUse.runGainCalibration();
}

void DetectorUseManager::startSingleAcquisition() {
    emit logMessage("开始单帧采集...");
    m_detectorUse.runSingleAcquisition();
}