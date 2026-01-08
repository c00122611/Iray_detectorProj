#pragma once

#include <QMainWindow>
#include "ui_IrayWidget.h"
#include"calibration.h"
#include<qpushbutton.h>

class IrayWidget : public QMainWindow
{
	Q_OBJECT

public:
	IrayWidget(QWidget *parent = nullptr);
	~IrayWidget();

private:
	Ui::IrayWidgetClass ui;
};

