#include "IrayWidget.h"
#include"QDebug.h"
IrayWidget::IrayWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    initWidgetState();
    // 创建工作线程和管理器
    //m_workerThread = new QThread(this);
    m_manager = new DetectorUseManager();
    //m_manager->moveToThread(m_workerThread);
    // 启动线程
    //m_workerThread->start();
    connect(ui.ConnectButton, &QPushButton::clicked, m_manager, &DetectorUseManager::connectDevice);
    connect(ui.offsetCalButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startOffsetCalibration);
    connect(m_manager, &DetectorUseManager::applicationModeChanged,this, &IrayWidget::onApplicationModeChanged);
    connect(ui.selectModeButton, &QPushButton::clicked, m_manager, &DetectorUseManager::onSelectModeClicked);
    connect(ui.gainAndDefectCalButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startGainDefectCalibration);
    // 接收日志、图像等
    connect(m_manager, &DetectorUseManager::logMessage, this, &IrayWidget::onLogMessage);
    connect(m_manager, &DetectorUseManager::connectionChanged, this,&IrayWidget::onConnectionChanged);

    //图像实时显示按钮
    connect(m_manager, &DetectorUseManager::newFrameReceived,this, &IrayWidget::onNewFrameReceived);
    connect(ui.startDisplayButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startFluoroDisplay);
    connect(ui.endDisplayButton, &QPushButton::clicked, m_manager, &DetectorUseManager::stopFluoroDisplay);


   
}
void IrayWidget::onLogMessage(const QString& msg) {
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
    ui.ConnectButton->setEnabled(!connected);
    ui.offsetCalButton->setEnabled(connected);
    ui.gainAndDefectCalButton->setEnabled(connected);
    ui.startDisplayButton->setEnabled(connected);
    ui.endDisplayButton->setEnabled(connected);
    ui.selectModeButton->setEnabled(connected);
}

void IrayWidget::onApplicationModeChanged(const QString& mode, bool success) {
    if (success) {
        ui.statusBar->showMessage(QString("当前模式: %1").arg(mode), 3000);
    }
}

void IrayWidget::onNewFrameReceived(const QImage& image) {
    ui.imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
        ui.imageLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
}

void IrayWidget::initWidgetState() {
    ui.offsetCalButton->setEnabled(FALSE);
    ui.gainAndDefectCalButton->setEnabled(FALSE);
    ui.startDisplayButton->setEnabled(FALSE);
    ui.endDisplayButton->setEnabled(FALSE);
    ui.selectModeButton->setEnabled(FALSE);
}

