#pragma once
#include <QObject>
#include "DetectorUse.h"

class DetectorUseManager : public QObject {
    Q_OBJECT
public:
    explicit DetectorUseManager(QObject* parent = nullptr); // 👈 只保留这一个
    ~DetectorUseManager();

public slots:
    void connectDevice();
    void disconnectDevice();
    void startOffsetCalibration();
    void startGainCalibration();
    void startSingleAcquisition();
    void imageReceived();

signals:
    void logMessage(const QString& msg);
    void calibrationFinished(bool success);
    void connectionChanged(bool connected);

private:
    DetectorUse m_detectorUse;
};