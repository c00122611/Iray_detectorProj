#pragma once
#include <QObject>
#include <QTimer>
#include "DetectorUse.h"
#include "ModeSelectionDialog.h"
#include "ImageUtil.h"
#include "Qdebug.h"
#include "QMessageBox"
#include "QDir"
#include <QCoreApplication>
#include <QThread>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QAbstractButton> 



enum class GainDefectStage {
    Idle,
    PreparingLight,     // 准备亮场（提示用户设 KV）
    AcquiringLight,     // 正在采集亮场
    PreparingDark,      // 提示关射线，准备暗场
    AcquiringDark,      // 正在采集暗场
    GeneratingTemplate, // 生成模板中
    Finished
};
class DetectorUseManager : public QObject {
    Q_OBJECT
public:
    explicit DetectorUseManager(QObject* parent = nullptr); 
    ~DetectorUseManager();
    int getAttrInt(int attrId);
    void stopStateMonitoring();
    void startStateMonitoring();
    QString convertStateToString(int stateEnum);
public slots:
    void connectDevice();
    void disconnectDevice();
    void startOffsetCalibration();
    void startGainDefectCalibration();
    void onSelectModeClicked();
    void onFluoroTimerTimeout();
    //实时显示
    void startFluoroDisplay();
    void stopFluoroDisplay();
    //图像采集
    void startPreAcquireAcquisition(const QString& saveDir);
    void startAveragedAcquisition(int avgFrames, int totalGroups, const QString& saveDir);
    void stopAveragedAcquisition(); 
    void onPollTimeout();

signals:
    void logMessage(const QString& msg);
    void calibrationFinished(bool success);
    void connectionChanged(bool connected);
    void applicationModeChanged(const QString& modeName, bool success);
    void newFrameReceived(const QImage& image);
    void averagedImageReady(const cv::Mat& img, int groupIndex, const QString& savePath);
    void preAcquiredImageReady(const cv::Mat& img, const QString& savePath);
    void stageChanged(GainDefectStage stage, const QString& suggestedKV, int expectedGray);
    void currentGrayUpdated(int grayValue);
    void detectorStateChanged(const QString& stateText); 


private:
    DetectorUse m_detectorUse;
    //日志函数
    void logInfo(const QString& msg);
    QTimer* m_fluoroTimer = nullptr;

    QThread* m_workerThread = nullptr;
    volatile bool m_stopRequested = false;
    int m_lastFluoroFrameNo = -1;

    GainDefectStage m_currentStage = GainDefectStage::Idle;
    QTimer* m_grayTimer = nullptr;
    QTimer* m_stateTimer = nullptr;
};