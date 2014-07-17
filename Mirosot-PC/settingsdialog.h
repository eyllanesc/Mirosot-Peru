
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// My header
#include "ui_settingsdialog.h"
#include "Structures.h"

// Qt header files
#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:

    SettingsDialog(QWidget *parent = 0);
    Settings settings() const;

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::SettingsDialog *ui;
    Settings currentSettings;
    QIntValidator *intValidator;
};

#endif // SETTINGSDIALOG_H
