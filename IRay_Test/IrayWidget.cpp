#include "IrayWidget.h"

IrayWidget::IrayWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    // 创建工作线程和管理器
    m_workerThread = new QThread(this);
    m_manager = new DetectorUseManager();
    m_manager->moveToThread(m_workerThread);
    // 启动线程
    m_workerThread->start();

    // 连接 UI 按钮到 manager 的槽（通过 queued connection 自动跨线程）
    connect(ui.ConnectButton, &QPushButton::clicked, m_manager, &DetectorUseManager::connectDevice);
    connect(ui.offsetCalButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startOffsetCalibration);
    connect(ui.gaincalButton, &QPushButton::clicked, m_manager, &DetectorUseManager::startGainCalibration);

    // 接收日志、图像等
    connect(m_manager, &DetectorUseManager::logMessage, this, &IrayWidget::onLogMessage);
    //图像相关
    connect(m_manager, &DetectorUseManager::imageReceived, this, &IrayWidget::onImageReceived);
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
//
//void IrayWidget::onImageReceived(const cv::Mat& img) {
//    // 转 QImage 并显示（参考之前代码）
//    if (img.empty()) return;
//    cv::Mat displayImg;
//    if (img.channels() == 1) {
//        cv::normalize(img, displayImg, 0, 255, cv::NORM_MINMAX, CV_8UC1);
//        cv::cvtColor(displayImg, displayImg, cv::COLOR_GRAY2RGB);
//    }
//    else {
//        displayImg = img.clone();
//    }
//    QImage qimg(displayImg.data, displayImg.cols, displayImg.rows, displayImg.step, QImage::Format_RGB888);
//    ui.imageViewLabel->setPixmap(QPixmap::fromImage(qimg).scaled(
//        ui.imageViewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
//}

IrayWidget::~IrayWidget() {
    m_workerThread->quit();
    m_workerThread->wait();
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


