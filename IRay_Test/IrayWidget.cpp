#include "IrayWidget.h"

IrayWidget::IrayWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    // 创建工作线程和管理器
    //m_workerThread = new QThread(this);
    m_manager = new DetectorUseManager();
    //m_manager->moveToThread(m_workerThread);
    // 启动线程
    //m_workerThread->start();
    connect(ui.ConnectButton, &QPushButton::clicked, m_manager, &DetectorUseManager::connectDevice);
    connect(ui.offsetCalButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startOffsetCalibration);
    connect(ui.gaincalButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startGainCalibration);
    connect(m_manager, &DetectorUseManager::applicationModeChanged,this, &IrayWidget::onApplicationModeChanged);
    connect(ui.selectModeButton, &QPushButton::clicked, m_manager, &DetectorUseManager::onSelectModeClicked);
    // 接收日志、图像等
    connect(m_manager, &DetectorUseManager::logMessage, this, &IrayWidget::onLogMessage);
    connect(m_manager, &DetectorUseManager::connectionChanged, this, [this](bool connected) {
        ui.ConnectButton->setEnabled(!connected);
        ui.offsetCalButton->setEnabled(connected);
        ui.gaincalButton->setEnabled(connected);
        });

	
	
}
void IrayWidget::onLogMessage(const QString& msg) {
    // 假设你有一个 QTextEdit 叫 logTextEdit
    ui.logTextEdit->append(msg);
}
IrayWidget::~IrayWidget() {
    //m_workerThread->quit();
    //m_workerThread->wait();
    delete m_manager; // 线程结束后删除
}
// IrayWidget.cpp
void IrayWidget::onConnectionChanged(bool connected)
{
    // 你的逻辑，例如启用/禁用按钮
    ui.ConnectButton->setEnabled(!connected);
    ui.offsetCalButton->setEnabled(connected);
    ui.gaincalButton->setEnabled(connected);
}

void IrayWidget::onApplicationModeChanged(const QString& mode, bool success) {
    if (success) {
        ui.statusBar->showMessage(QString("当前模式: %1").arg(mode), 3000);
    }
}


