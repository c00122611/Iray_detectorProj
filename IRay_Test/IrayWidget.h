#pragma once

#include <QMainWindow>
#include "ui_IrayWidget.h"
#include "DetectorUseManager.h"
#include<qpushbutton.h>
#include <QThread>
#include<QTextedit>
#include<opencv2/opencv.hpp>
#include <QFileDialog>
#include <QStandardPaths>

class IrayWidget : public QMainWindow
{
	Q_OBJECT

public:
	IrayWidget(QWidget *parent = nullptr);
	~IrayWidget();
private slots:
	void onLogMessage(const QString& msg);
	void onConnectionChanged(bool connected);
	void onApplicationModeChanged(const QString& mode, bool success);
	void onNewFrameReceived(const QImage& image);
	void onAveragedImageReceived(const cv::Mat& img, int groupIndex, const QString& saveDir);
	void onPreAcqImageReceived(const cv::Mat& img, const QString& saveDir);
	void onCurrentGrayUpdated(int grayValue);
	void onStageChanged(GainDefectStage stage, const QString& suggestedKV, int expectedGray);
private:
	Ui::IrayWidgetClass ui;
	DetectorUseManager* m_manager;
	QThread* m_workerThread;

	void initWidgetState();
};

