#include "DetectorUseManager.h"
#include <QDebug>
DetectorUseManager::DetectorUseManager(QObject *parent)
	: QObject(parent)
{}

DetectorUseManager::~DetectorUseManager()
{
    m_detectorUse.Disconnect();
}

void DetectorUseManager::connectDevice() {
    int ret = m_detectorUse.Connect();
    if (ret == Err_OK) {
        emit logMessage("探测器连接成功");
        emit connectionChanged(true);
    }
    else {
        emit logMessage(QString("连接失败: %1").arg(ret));
        emit connectionChanged(false);
    }
}

void DetectorUseManager::startOffsetCalibration() {
    emit logMessage("开始偏移校准...");
    m_detectorUse.runOffsetCalibration();
    emit logMessage("偏移校准完成");
    emit calibrationFinished(true);
}

void DetectorUseManager::startGainCalibration() {
    emit logMessage("开始偏移校准...");
    m_detectorUse.runGainCalibration();
    emit logMessage("偏移校准完成");
    emit calibrationFinished(true);
}

