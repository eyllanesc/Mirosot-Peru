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
    int BSNumberOfIterations;
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
    void resetSmoothsizeDefaults();
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
    void on_TeamAreaqxtSpanSlider_lowerValueChanged(int lower);
    void on_TeamAreaqxtSpanSlider_upperValueChanged(int upper);
    void on_Robot1AreaqxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot1AreaqxtSpanSlider_upperValueChanged(int upper);
    void on_Robot2AreaqxtSpanSlider_lowerValueChanged(int lower);
    void on_Robot2AreaqxtSpanSlider_upperValueChanged(int upper);
    void on_BallAreaqxtSpanSlider_lowerValueChanged(int lower);
    void on_BallAreaqxtSpanSlider_upperValueChanged(int upper);
    void on_BSpushButton_clicked();
signals:
    void newProcessingSettings(struct ProcessingSettings p_settings);
    void newNumberBS(int);
    void resetSteam();
    void resetSrobot1();
    void resetSrobot2();
    void resetSball();
};

#endif // PROCESSINGSETTINGSDIALOG_H
