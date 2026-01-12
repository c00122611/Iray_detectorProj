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
    
    connect(m_manager, &DetectorUseManager::averagedImageReady,this, &IrayWidget::onAveragedImageReceived);
    connect(ui.btnStartAvg, &QPushButton::clicked, this, [=]() {

        int avg = ui.spinAvgFrames->value();     // 如 4
        int groups = ui.spinTotalGroups->value(); // 如 10

        // 弹出文件夹选择框
        QString saveDir = QFileDialog::getExistingDirectory(
            this, QString::fromLocal8Bit("选择平均图保存路径"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
        );  
        if (saveDir.isEmpty()) return;

        m_manager->startAveragedAcquisition(avg, groups, saveDir); });
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

void IrayWidget::onAveragedImageReceived(const cv::Mat& img, int groupIndex, const QString& saveDir)
{
    if (img.empty() || img.type() != CV_16UC1) {
        ui.logTextEdit->append(QString::fromLocal8Bit("收到无效平均图"));
        return;
    }
    // 构造文件名：Avg_Group000.raw
    QString fileName = QString("Avg_Group%1.raw").arg(groupIndex, 3, 10, QChar('0'));
    QString fullPath = saveDir + "/" + fileName;

    // 保存为 .raw（纯二进制，无头）
    QFile file(fullPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(reinterpret_cast<const char*>(img.data), img.total() * img.elemSize());
        file.close();
        ui.logTextEdit->append(QString("平均图已保存: %1").arg(fullPath));
    }
    else {
        ui.logTextEdit->append(QString("保存失败: %1").arg(fullPath));
    }
}



