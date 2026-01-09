#pragma once

#include <QObject>
#include "DetectorUse.h"
//#include <opencv2/opencv.hpp>

class DetectorUseManager  : public QObject
{
	Q_OBJECT

public:
	DetectorUseManager(QObject *parent);
	~DetectorUseManager();
public slots:
    void connectDevice();
    void disconnectDevice();
    void startOffsetCalibration();
    void startGainCalibration();
    void startSingleAcquisition();

signals:
    void logMessage(const QString& msg);
    //void imageReceived(const cv::Mat& image);
    void calibrationFinished(bool success);
    void connectionChanged(bool connected);

private:
    DetectorUse m_detectorUse;
};

