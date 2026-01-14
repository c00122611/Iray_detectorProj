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
        QString::fromLocal8Bit("请确保探测器处于完全黑暗环境（无 X 射线照射）！\n"),
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
void DetectorUseManager::startGainDefectCalibration()
{
    logInfo(QString::fromLocal8Bit("启动 Gain + Defect 校准（阻塞式+UI反馈）..."));

    // 启动灰度定时器（每秒读取一次）
    if (!m_grayTimer) {
        m_grayTimer = new QTimer(this);
        connect(m_grayTimer, &QTimer::timeout, this, [this]() {
            int gray = m_detectorUse.getAttrInt(Enm_ImageTag_CenterValue); // SDK 属性直接读取
            emit currentGrayUpdated(gray);
            });
    }
    m_grayTimer->start(1000);

    // 步骤1: 初始化事务
    int ret = m_detectorUse.gainInit();
    if (ret != Err_OK) {
        logInfo(QString::fromLocal8Bit("GainInit 失败"));
        m_grayTimer->stop();
        emit calibrationFinished(false);
        return;
    }

    // 步骤2: 定义校准点（严格按 NDT1717M 手册 Table 6.4.1）
    struct PointConfig {
        QString voltage;      // 推荐电压
        int expectedGray;     // 目标灰度
    };
    QList<PointConfig> points = {
        {QString::fromLocal8Bit("50kV"), 2000},   // 最低 KV
        {QString::fromLocal8Bit("110kV"), 12000}, // 最高 KV
        {QString::fromLocal8Bit("70kV"), 2000}    // 中间 KV
    };

    int nTotalFrames = m_detectorUse.getAttrInt(Attr_GainTotalFrames);
    int nFramesPerPoint = nTotalFrames / points.size();

    // 步骤3: 循环每个剂量点
    for (int i = 0; i < points.size(); ++i) {
        const auto& pt = points[i];

        // === 步骤3.1：提示用户设置高压并开启射线 ===
        m_currentStage = GainDefectStage::PreparingLight;
        emit stageChanged(m_currentStage, pt.voltage, pt.expectedGray); // 👈 新增参数

        QString prepMsg = QString::fromLocal8Bit(
            "【校准点 %1/%2】\n\n"
            "请将 X 光机设置为：\n"
            "   • 电压：%3\n"
            "   • 目标图像平均灰度：%4\n\n"
            "调整好参数后，请开启 X 射线源。\n"
            "确认射线已开启，点击“确定”开始亮场采集。")
            .arg(i + 1).arg(points.size())
            .arg(pt.voltage)
            .arg(pt.expectedGray);

        QMessageBox::information(nullptr,
            QString::fromLocal8Bit("准备亮场采集"),
            prepMsg);

        // === 步骤3.2：采集亮场 ===
        m_currentStage = GainDefectStage::AcquiringLight;
        emit stageChanged(m_currentStage, pt.voltage, pt.expectedGray);
        logInfo(QString::fromLocal8Bit("开始采集亮场（点 %1，%2）...")
            .arg(i + 1).arg(pt.voltage));
        m_detectorUse.acquireLightField(i, nFramesPerPoint);

        // === 步骤3.3：提示用户关闭 X 射线 ===
        m_currentStage = GainDefectStage::PreparingDark;
        emit stageChanged(m_currentStage, pt.voltage, pt.expectedGray);

        QMessageBox::information(nullptr,
            QString::fromLocal8Bit("亮场采集完成"),
            QString::fromLocal8Bit("亮场采集已完成！\n\n"
                "请立即关闭 X 射线源，并等待至少 20 秒确保无辐射残留。\n\n"
                "确认射线已关闭，点击“确定”开始暗场采集。"));

        // === 步骤3.4：采集暗场 ===
        m_currentStage = GainDefectStage::AcquiringDark;
        emit stageChanged(m_currentStage, pt.voltage, pt.expectedGray);
        logInfo(QString::fromLocal8Bit("开始采集暗场（点 %1）...").arg(i + 1));
        m_detectorUse.acquireDarkField(nFramesPerPoint);
    }

    // 步骤4: 生成模板
    m_currentStage = GainDefectStage::GeneratingTemplate;
    emit stageChanged(m_currentStage, "", 0);
    logInfo(QString::fromLocal8Bit("正在生成 Gain 和 Defect 校正模板..."));
    ret = m_detectorUse.GenerateGainAndDefectTemplates();
    if (ret != Err_OK) {
        logInfo(QString::fromLocal8Bit("模板生成失败"));
        m_detectorUse.finishCalibrationProcess();
        m_grayTimer->stop();
        emit calibrationFinished(false);
        return;
    }

    // 步骤5: 清理
    m_detectorUse.finishCalibrationProcess();
    m_grayTimer->stop();
    m_currentStage = GainDefectStage::Finished;
    emit stageChanged(m_currentStage, "", 0);
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
void DetectorUseManager::startFluoroDisplay()
{
    if (!m_fluoroTimer) {
        m_fluoroTimer = new QTimer(this);
        connect(m_fluoroTimer, &QTimer::timeout, this, &DetectorUseManager::onFluoroTimerTimeout);
    }
    // 启动 Fluoro 显示（例如 2fps → 500ms）
    m_fluoroTimer->start(500); // 可根据实际帧率动态设置
    logInfo("Fluoro 显示已启动");
}

void DetectorUseManager::stopFluoroDisplay()
{
    if (m_fluoroTimer && m_fluoroTimer->isActive()) {
        m_fluoroTimer->stop();
        logInfo("Fluoro 显示已停止");
    }
}

void DetectorUseManager::onFluoroTimerTimeout()
{
    auto [frame, frameNo] = m_detectorUse.getCurrentFrameWithIndex();
    if (!frame.empty() && frameNo != m_lastFluoroFrameNo) {
        m_lastFluoroFrameNo = frameNo;
        QImage img = MatToQImage(frame);
        emit newFrameReceived(img);
    }
}



// DetectorUseManager.cpp
void DetectorUseManager::startAveragedAcquisition(int avgFrames, int totalGroups, const QString& saveDir)
{
    if (avgFrames <= 0 || totalGroups <= 0) {
        emit logMessage(QString::fromLocal8Bit("错误：平均帧数和组数必须大于0"));
        return;
    }

    QDir dir(saveDir);
    if (!dir.exists()) {
        emit logMessage(QString::fromLocal8Bit("错误：保存路径不存在: %1").arg(saveDir));
        return;
    }

    // 允许中途停止
    m_stopRequested = false;

    // 创建工作线程
    m_workerThread = QThread::create([this, avgFrames, totalGroups, saveDir]() {
        int ret = m_detectorUse.initImageBuffer();
        if (ret != Err_OK) {
            emit logMessage(QString::fromLocal8Bit("初始化图像缓存失败: %1").arg(ret));
            return;
        }

        ret = m_detectorUse.startContinuousAcquisition();
        if (ret != Err_OK && ret != Err_TaskPending) {
            emit logMessage(QString::fromLocal8Bit("启动采集失败: %1").arg(ret));
            return;
        }

        emit logMessage(QString::fromLocal8Bit("开始帧平均采集：每组 %1 帧，共 %2 组").arg(avgFrames).arg(totalGroups));

        for (int group = 0; group < totalGroups && !m_stopRequested; ++group) {
            cv::Mat accumulator;
            int validCount = 0;

            while (validCount < avgFrames && !m_stopRequested) {
                auto [frame, _] = m_detectorUse.getCurrentFrameWithIndex();
                if (frame.empty()) {
                    QThread::msleep(1);
                    continue;
                }

                if (accumulator.empty()) {
                    accumulator = cv::Mat::zeros(frame.size(), CV_32F);
                }
                frame.convertTo(frame, CV_32F);
                cv::add(accumulator, frame, accumulator);
                validCount++;
            }

            // 如果被中断，不发送最后一组不完整的平均图
            if (m_stopRequested) {
                break;
            }

            // 完成一组平均
            accumulator /= static_cast<float>(avgFrames);
            cv::Mat averaged16U;
            accumulator.convertTo(averaged16U, CV_16U);
            emit averagedImageReady(averaged16U, group, saveDir);
        }

        // 停止采集 + 清空缓存（符合 SDK Guide P74）
        m_detectorUse.stopContinuousAcquisition();
        emit logMessage(QString::fromLocal8Bit("帧平均采集完成"));
        });

    connect(m_workerThread, &QThread::finished, m_workerThread, &QObject::deleteLater);
    m_workerThread->start();
}
void DetectorUseManager::stopAveragedAcquisition()
{
    m_stopRequested = true;

    if (m_workerThread && m_workerThread->isRunning()) {
        // 等待最多 2 秒让线程自然退出
        if (!m_workerThread->wait(2000)) {
            // 超时仍未退出（理论上不应发生），可考虑强制 terminate（不推荐）
            emit logMessage(QString::fromLocal8Bit("警告：采集线程未在2秒内退出"));
        }
    }
}
void DetectorUseManager::startPreAcquireAcquisition(const QString& saveDir)
{
    QDir dir(saveDir);
    if (!dir.exists()) {
        emit logMessage(QString::fromLocal8Bit("错误：保存路径不存在: %1").arg(saveDir));
        return;
    }
    QThread* worker = QThread::create([this, saveDir]() {
        int ret = m_detectorUse.initImageBuffer();
        if (ret != Err_OK) {
            emit logMessage(QString::fromLocal8Bit("PreAcquire: 初始化缓存失败 %1").arg(ret));
            return;
        }

        // 子线程调用 preAcquire
        ret = m_detectorUse.preAcquire(); // 内部调用 Invoke(Cmd_ClearAcq)
        if (ret != Err_OK && ret != Err_TaskPending) {
            emit logMessage(QString::fromLocal8Bit("PreAcquire: 启动失败 %1").arg(ret));
            return;
        }

        emit logMessage(QString::fromLocal8Bit("PreAcquire 已启动，等待图像..."));

        // 3. 等待并拉取单帧（最多5秒）
        QElapsedTimer timer;
        timer.start();
        cv::Mat frame;
        while (timer.elapsed() < 5000) {
            auto [img, _] = m_detectorUse.getCurrentFrameWithIndex();
            if (!img.empty()) {
                frame = img;
                break;
            }
            QThread::msleep(10);
        }

        if (frame.empty()) {
            emit logMessage(QString::fromLocal8Bit("PreAcquire 超时，未收到图像"));
            return;
        }

        // 4. 发送结果（与 averagedImageReady 完全一致）
        emit preAcquiredImageReady(frame,saveDir);
        emit logMessage(QString::fromLocal8Bit("PreAcquire 图像获取成功"));
        });

    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}

