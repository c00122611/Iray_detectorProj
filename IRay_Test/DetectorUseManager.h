#pragma once
#include <QObject>
#include "DetectorUse.h"
#include "ModeSelectionDialog.h"
class DetectorUseManager : public QObject {
    Q_OBJECT
public:
    explicit DetectorUseManager(QObject* parent = nullptr); 
    ~DetectorUseManager();

public slots:
    void connectDevice();
    void disconnectDevice();
    void startOffsetCalibration();
    void startGainCalibration();
    void startSingleAcquisition();
    void onSelectModeClicked();

signals:
    void logMessage(const QString& msg);
    void calibrationFinished(bool success);
    void connectionChanged(bool connected);
    void applicationModeChanged(const QString& modeName, bool success);

private:
    DetectorUse m_detectorUse;
    //日志函数
    void logInfo(const QString& msg);
};