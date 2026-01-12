#pragma once
#include <QObject>
#include <QTimer>
#include "DetectorUse.h"
#include "ModeSelectionDialog.h"
#include "ImageUtil.h"
#include "Qdebug.h"
#include "QMessageBox"
class DetectorUseManager : public QObject {
    Q_OBJECT
public:
    explicit DetectorUseManager(QObject* parent = nullptr); 
    ~DetectorUseManager();
    
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

signals:
    void logMessage(const QString& msg);
    void calibrationFinished(bool success);
    void connectionChanged(bool connected);
    void applicationModeChanged(const QString& modeName, bool success);
    void newFrameReceived(const QImage& image);
private:
    DetectorUse m_detectorUse;
    //日志函数
    void logInfo(const QString& msg);
    QTimer* m_fluoroTimer = nullptr;
};