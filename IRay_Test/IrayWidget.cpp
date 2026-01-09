#include "IrayWidget.h"

IrayWidget::IrayWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//链接探测器
	connect(ui.ConnectButton, &QPushButton::clicked, [=]() {
		this->m_detectorUse.Connect();
		});
	//偏移校正
	connect(ui.offsetCalButton, &QPushButton::clicked, [=]() {
		this->m_detectorUse.runOffsetCalibration();
		});
	//增益校正+defect校正
	connect(ui.gaincalButton, &QPushButton::clicked, [=]() {
		this->m_detectorUse.runGainCalibration();
		});
	//采图
	// 1:单采 Dr
	// 2：动态采集 Ct

	
	
}

IrayWidget::~IrayWidget()
{}


