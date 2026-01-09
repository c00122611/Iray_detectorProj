#pragma once

#include <QMainWindow>
#include "ui_IrayWidget.h"
#include "DetectorUseManager.h"
#include<qpushbutton.h>
#include <QThread>
#include<QTextedit>

class IrayWidget : public QMainWindow
{
	Q_OBJECT

public:
	IrayWidget(QWidget *parent = nullptr);
	~IrayWidget();
private slots:
	void onLogMessage(const QString& msg);
	//TODO Í¼ÏñÏà¹Ø
	//void onImageReceived(const cv::Mat& img);
	void onConnectionChanged(bool connected);
private:
	Ui::IrayWidgetClass ui;
	DetectorUseManager* m_manager;
	QThread* m_workerThread;
};

