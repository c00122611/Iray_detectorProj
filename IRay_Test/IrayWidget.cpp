#include "IrayWidget.h"

IrayWidget::IrayWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	connect(ui.TestButton, &QPushButton::clicked, [=]() {
		this->m_detectorUse.runAutoCalibration();
		});
}

IrayWidget::~IrayWidget()
{}


