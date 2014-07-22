/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef PROCESSINGSETTINGSDIALOG_H
#define PROCESSINGSETTINGSDIALOG_H

//#include "ui_ProcessingSettingsDialog.h"

// My header
#include "Config.h"
#include "Structures.h"

// Qt header files
#include <QtGui>

#include <QDialog>
#include <qxtspanslider.h>

namespace Ui {
class ProcessingSettingsDialog;
}

class ProcessingSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessingSettingsDialog(QWidget *parent = 0);
    void updateDialogSettingsFromStored();
private:
    Ui::ProcessingSettingsDialog *ui;
    ProcessingSettings processingSettings;
    void resetColorDialogToDefaults();

public slots:
    void resetAllDialogToDefaults();
    void updateStoredSettingsFromDialog();
    void updateProcessingSettings(struct ProcessingSettings);
private slots:
    void resetTeamDialogToDefaults();
    void resetRobot1DialogToDefaults();
    void resetRobot2DialogToDefaults();
    void resetBallDialogToDefaults();
    void resetBSDialogToDefaults();
    void on_BallChannel1qxtSpanSlider_lowerValueChanged(int lower);
    void on_BallChannel1qxtSpanSlider_upperValueChanged(int upper);
    void on_BallChannel2qxtSpanSlider_lowerValueChanged(int lower);
    void on_BallChannel2qxtSpanSlider_upperValueChanged(int upper);
    void on_BallChannel3qxtSpanSlider_lowerValueChanged(int lower);
    void on_BallChannel3qxtSpanSlider_upperValueChanged(int upper);
    void on_TeamChannel1qxtSpanSlider_lowerValueChanged(int lower);
    void on_TeamChannel1qxtSpanSlider_upperValueChanged(int upper);
    void on_TeamChannel2qxtSpanSlider_lowerValueChanged(int lower);
    void on_TeamChannel2qxtSpanSlider_upperValueChanged(int upper);
    void on_TeamChannel3qxtSpanSlider_lowerValueChanged(int lower);
    void on_TeamChannel3qxtSpanSlider_upperValueChanged(int upper);
    void on_Robot1Channel1qxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot1Channel1qxtSpanSlider_upperValueChanged(int upper);
    void on_Robot1Channel2qxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot1Channel2qxtSpanSlider_upperValueChanged(int upper);
    void on_Robot1Channel3qxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot1Channel3qxtSpanSlider_upperValueChanged(int upper);
    void on_Robot2Channel1qxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot2Channel1qxtSpanSlider_upperValueChanged(int upper);
    void on_Robot2Channel2qxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot2Channel2qxtSpanSlider_upperValueChanged(int upper);
    void on_Robot2Channel3qxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot2Channel3qxtSpanSlider_upperValueChanged(int upper);
    void on_TeamColorRGBButton_clicked();
    void on_TeamColorHSVButton_clicked();
    void on_TeamColorYCrCbButton_clicked();
    void on_Robot1ColorRGBButton_clicked();
    void on_Robot1ColorHSVButton_clicked();
    void on_Robot1ColorYCrCbButton_clicked();
    void on_Robot2ColorRGBButton_clicked();
    void on_Robot2ColorHSVButton_clicked();
    void on_Robot2ColorYCrCbButton_clicked();
    void on_BallColorRGBButton_clicked();
    void on_BallColorHSVButton_clicked();
    void on_BallColorYCrCbButton_clicked();

signals:
    void newProcessingSettings(struct ProcessingSettings p_settings);
};

#endif // PROCESSINGSETTINGSDIALOG_H
