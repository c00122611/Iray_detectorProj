// ModeSelectionDialog.h
#ifndef MODESELECTIONDIALOG_H
#define MODESELECTIONDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "DetectorUse.h"

class ModeSelectionDialog : public QDialog {
    Q_OBJECT

public:
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

#endif // MODESELECTIONDIALOG_H