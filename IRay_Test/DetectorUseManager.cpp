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
        QMessageBox::information(nullptr, QString::fromLocal8Bit("连接成功"),
            QString::fromLocal8Bit("请选择模式..."));
        emit connectionChanged(true);
    }
    else {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("连接失败"),
            QString::fromLocal8Bit("连接失败: %1").arg(ret));
        logInfo(QString::fromLocal8Bit("连接失败: %1").arg(ret));
        emit connectionChanged(false);
    }
}
//断开连接
void DetectorUseManager::disconnectDevice() {
    m_detectorUse.Disconnect();
    logInfo(QString::fromLocal8Bit("探测器已断开"));
}
// 偏移校正逻辑实现
void DetectorUseManager::startOffsetCalibration() {
    logInfo(QString::fromLocal8Bit("开始偏移校准"));

    // 弹窗提示：确保无 X 射线
    QMessageBox::information(nullptr, QString::fromLocal8Bit("校准提示"),
        QString::fromLocal8Bit("⚠️ 请确保探测器处于完全黑暗环境（无 X 射线照射）！\n"),
        QString::fromLocal8Bit("点击“确定”开始 Offset 校准。"));

    int ret = m_detectorUse.GenerateOffsetTemplate();
    if (ret == Err_OK) {
        logInfo(QString::fromLocal8Bit("Offset 校准成功完成！"));
    }
    else {
        logInfo(QString::fromLocal8Bit("Offset 校准失败！"));
    }
    emit calibrationFinished(ret == Err_OK);
}
// 增益校正+defect校正逻辑实现
void DetectorUseManager::startGainDefectCalibration() {
    logInfo(QString::fromLocal8Bit("启动Gain + Defect 校准..."));

    // 步骤1: 初始化事务
    int ret = m_detectorUse.gainInit();
    if (ret != Err_OK) {
        logInfo(QString::fromLocal8Bit("GainInit 失败"));
        emit calibrationFinished(false);
        return;
    }

    // 步骤2: 获取参数
    int nMultiGainPoints = m_detectorUse.getAttrInt(Attr_MultiGainPointNumber);
    int nTotalFrames = m_detectorUse.getAttrInt(Attr_GainTotalFrames);
    int nFramesPerPoint = nTotalFrames / nMultiGainPoints;

    logInfo(QString::fromLocal8Bit("增益点数量: %1, 每点帧数: %2")
        .arg(nMultiGainPoints).arg(nFramesPerPoint));

    // 步骤3: 循环每个剂量点
    for (int pointIdx = 0; pointIdx < nMultiGainPoints; pointIdx++) {
        // 3.1 提示用户设置高压
        QMessageBox::information(nullptr, QString::fromLocal8Bit("校准提示"),
            QString::fromLocal8Bit("请设置高压发生器至第 %1 剂量点（参考：50kV/70kV/110kV），点击“确定”继续。")
            .arg(pointIdx + 1));

        // 3.2 采集亮场
        logInfo(QString::fromLocal8Bit("开始采集亮场（点 %1）...").arg(pointIdx + 1));
        m_detectorUse.acquireLightField(pointIdx, nFramesPerPoint);

        // 3.3 提示用户关闭 X 射线
        QMessageBox::information(nullptr, QString::fromLocal8Bit("校准提示"),
            QString::fromLocal8Bit("亮场采集完成！\n\n请关闭 X 射线源，等待 20 秒。\n点击“确定”开始暗场采集。"));

        // 3.4 采集暗场
        logInfo(QString::fromLocal8Bit("开始采集暗场（点 %1）...").arg(pointIdx + 1));
        m_detectorUse.acquireDarkField(nFramesPerPoint);
    }

    // 步骤4: 生成模板
    logInfo(QString::fromLocal8Bit("生成 Gain 和 Defect 模板..."));
    ret = m_detectorUse.GenerateGainAndDefectTemplates();
    if (ret != Err_OK) {
        logInfo(QString::fromLocal8Bit("模板生成失败"));
        emit calibrationFinished(false);
        return;
    }

    // 步骤5: 清理事务
    m_detectorUse.finishCalibrationProcess();
    logInfo(QString::fromLocal8Bit("Gain + Defect 校准成功完成！"));
    emit calibrationFinished(true);
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

