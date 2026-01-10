#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "DetectorUse.h"
class ModeSelectionDialog  : public QDialog
{
	Q_OBJECT

public:
	ModeSelectionDialog(QObject *parent);
	~ModeSelectionDialog();
	explicit ModeSelectionDialog(DetectorUse* detectorUse, QWidget* parent = nullptr);
	QString selectedSubset() const { return m_selectedSubset; }
private slots:
    void onRowDoubleClicked(int row, int column);
    void onOkClicked();

private:
    void loadModes();

    DetectorUse* m_detectorUse;
    QTableWidget* m_table;
    QPushButton* m_okButton;
    QString m_selectedSubset;
};

