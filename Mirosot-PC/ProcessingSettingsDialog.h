/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef PROCESSINGSETTINGSDIALOG_H
#define PROCESSINGSETTINGSDIALOG_H

#include "ui_ProcessingSettingsDialog.h"

// My header
#include "Config.h"
#include "Structures.h"

// Qt header files
#include <QtGui>


class ProcessingSettingsDialog : public QDialog, private Ui::ProcessingSettingsDialog
{
    Q_OBJECT

public:
    ProcessingSettingsDialog(QWidget *parent = 0);
    void updateDialogSettingsFromStored();
private:
    ProcessingSettings processingSettings;
public slots:
    void resetAllDialogToDefaults();
    void updateStoredSettingsFromDialog();
private slots:
    void resetColorDialogToDefaults();
    void resetBSDialogToDefaults();
    void validateDialog();
    void ColorTypeChange(QAbstractButton*);
signals:
    void newProcessingSettings(struct ProcessingSettings p_settings);
};

#endif // PROCESSINGSETTINGSDIALOG_H
