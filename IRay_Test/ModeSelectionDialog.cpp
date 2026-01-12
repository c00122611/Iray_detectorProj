// ModeSelectionDialog.cpp
#include "ModeSelectionDialog.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
ModeSelectionDialog::ModeSelectionDialog(DetectorUse* detectorUse, QWidget* parent)
    : QDialog(parent)
    , m_detectorUse(detectorUse)
    , m_selectedSubset("")
{
    setWindowTitle("Select Application Mode");
    resize(750, 400);

    m_table = new QTableWidget(this);
    // 新增 "复用组" 列
    m_table->setColumnCount(6);
    m_table->setHorizontalHeaderLabels({ "Mode", "Subset", "PGA", "Binning", "FPS", "root" });
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_okButton = new QPushButton("OK", this);
    m_okButton->setEnabled(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->addWidget(m_okButton);

    connect(m_table, &QTableWidget::cellDoubleClicked, this, &ModeSelectionDialog::onRowDoubleClicked);
    connect(m_okButton, &QPushButton::clicked, this, &ModeSelectionDialog::onOkClicked);

    loadModes();
}

void ModeSelectionDialog::loadModes() {
    auto modes = m_detectorUse->parseApplicationModes();
    m_table->setRowCount(modes.size());

    for (int i = 0; i < modes.size(); ++i) {
        const auto& mode = modes[i];
        m_table->setItem(i, 0, new QTableWidgetItem(mode.name));
        m_table->setItem(i, 1, new QTableWidgetItem(mode.subset));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(mode.pga)));
        m_table->setItem(i, 3, new QTableWidgetItem(QString::number(mode.binning)));
        m_table->setItem(i, 4, new QTableWidgetItem(QString::number(mode.frequency, 'f', 1)));
        m_table->setItem(i, 5, new QTableWidgetItem(mode.baseMode)); // 👈 显示复用组
    }

    if (modes.size() > 0) {
        m_table->selectRow(0);
        m_okButton->setEnabled(true);
    }
}

void ModeSelectionDialog::onRowDoubleClicked(int row, int /*column*/) {
    m_selectedSubset = m_table->item(row, 1)->text(); // subset 列
    accept();
}

void ModeSelectionDialog::onOkClicked() {
    auto selected = m_table->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        m_selectedSubset = m_table->item(row, 1)->text();
        accept();
    }
}