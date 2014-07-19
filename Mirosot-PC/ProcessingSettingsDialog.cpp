/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "ProcessingSettingsDialog.h"
#include "ui_ProcessingSettingsDialog.h"


ProcessingSettingsDialog::ProcessingSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessingSettingsDialog)
{
    // Setup dialog
    ui->setupUi(this);
    // Connect GUI signals and slots
    connect(ui->resetAllToDefaultsButton,SIGNAL(clicked()),this,SLOT(resetAllDialogToDefaults()));
    connect(ui->applyButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->resetBSToDefaultsButton,SIGNAL(clicked()),this,SLOT(resetBSDialogToDefaults()));
    connect(ui->resetTeamToDefaultsButton ,SIGNAL(clicked()),this,SLOT(resetTeamDialogToDefaults()));
    connect(ui->resetRobot1ToDefaultsButton ,SIGNAL(clicked()),this,SLOT(resetRobot1DialogToDefaults()));
    connect(ui->resetRobot2ToDefaultsButton ,SIGNAL(clicked()),this,SLOT(resetRobot2DialogToDefaults()));
    connect(ui->resetBallToDefaultsButton ,SIGNAL(clicked()),this,SLOT(resetBallDialogToDefaults()));
    //Background Substraction
    QRegExp rx("[1-9]\\d{0,2}"); // Integers 1 to 99
    QRegExpValidator *validator = new QRegExpValidator(rx, 0);
    ui->BSIterationsEdit->setValidator(validator);
    // Set dialog values to defaults
    resetAllDialogToDefaults();
    // Update processing settings in processingSettings structure and processingThread
    updateStoredSettingsFromDialog();
} // ProcessingSettingsDialog constructor

void ProcessingSettingsDialog::updateDialogSettingsFromStored()
{
    //Team
    if(processingSettings.TeamColorType==0)
        ui->TeamColorRGBButton->setChecked(true);
    else if(processingSettings.TeamColorType==1)
        ui->TeamColorHSVButton->setChecked(true);
    else if(processingSettings.TeamColorType==2)
        ui->TeamColorYCrCbButton->setChecked(true);
    ui->TeamChannel1qxtSpanSlider->setLowerValue(processingSettings.TeamChannel1min);
    ui->TeamChannel1qxtSpanSlider->setUpperValue(processingSettings.TeamChannel1max);
    ui->TeamChannel2qxtSpanSlider->setLowerValue(processingSettings.TeamChannel2min);
    ui->TeamChannel2qxtSpanSlider->setUpperValue(processingSettings.TeamChannel2max);
    ui->TeamChannel3qxtSpanSlider->setLowerValue(processingSettings.TeamChannel3min);
    ui->TeamChannel3qxtSpanSlider->setUpperValue(processingSettings.TeamChannel3max);

    //Robot1
    if(processingSettings.Robot1ColorType==0)
        ui->Robot1ColorRGBButton->setChecked(true);
    else if(processingSettings.Robot1ColorType==1)
        ui->Robot1ColorHSVButton->setChecked(true);
    else if(processingSettings.Robot1ColorType==2)
        ui->Robot1ColorYCrCbButton->setChecked(true);
    ui->Robot1Channel1qxtSpanSlider->setLowerValue(processingSettings.Robot1Channel1min);
    ui->Robot1Channel1qxtSpanSlider->setUpperValue(processingSettings.Robot1Channel1max);
    ui->Robot1Channel2qxtSpanSlider->setLowerValue(processingSettings.Robot1Channel2min);
    ui->Robot1Channel2qxtSpanSlider->setUpperValue(processingSettings.Robot1Channel2max);
    ui->Robot1Channel3qxtSpanSlider->setLowerValue(processingSettings.Robot1Channel3min);
    ui->Robot1Channel3qxtSpanSlider->setUpperValue(processingSettings.Robot1Channel3max);

    //Robot2
    if(processingSettings.Robot2ColorType==0)
        ui->Robot2ColorRGBButton->setChecked(true);
    else if(processingSettings.Robot2ColorType==1)
        ui->Robot2ColorHSVButton->setChecked(true);
    else if(processingSettings.Robot2ColorType==2)
        ui->Robot2ColorYCrCbButton->setChecked(true);
    ui->Robot2Channel1qxtSpanSlider->setLowerValue(processingSettings.Robot2Channel1min);
    ui->Robot2Channel1qxtSpanSlider->setUpperValue(processingSettings.Robot2Channel1max);
    ui->Robot2Channel2qxtSpanSlider->setLowerValue(processingSettings.Robot2Channel2min);
    ui->Robot2Channel2qxtSpanSlider->setUpperValue(processingSettings.Robot2Channel2max);
    ui->Robot2Channel3qxtSpanSlider->setLowerValue(processingSettings.Robot2Channel3min);
    ui->Robot2Channel3qxtSpanSlider->setUpperValue(processingSettings.Robot2Channel3max);

    //Ball
    if(processingSettings.BallColorType==0)
        ui->BallColorRGBButton->setChecked(true);
    else if(processingSettings.BallColorType==1)
        ui->BallColorHSVButton->setChecked(true);
    else if(processingSettings.BallColorType==2)
        ui->BallColorYCrCbButton->setChecked(true);
    ui->BallChannel1qxtSpanSlider->setLowerValue(processingSettings.BallChannel1min);
    ui->BallChannel1qxtSpanSlider->setUpperValue(processingSettings.BallChannel1max);
    ui->BallChannel2qxtSpanSlider->setLowerValue(processingSettings.BallChannel2min);
    ui->BallChannel2qxtSpanSlider->setUpperValue(processingSettings.BallChannel2max);
    ui->BallChannel3qxtSpanSlider->setLowerValue(processingSettings.BallChannel3min);
    ui->BallChannel3qxtSpanSlider->setUpperValue(processingSettings.BallChannel3max);

} // updateDialogSettingsFromStored()


void ProcessingSettingsDialog::resetTeamDialogToDefaults()
{
    if(DEFAULT_COLOR_TYPE==0)
        ui->TeamColorRGBButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==1)
        ui->TeamColorHSVButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==2)
        ui->TeamColorYCrCbButton->setChecked(true);

    ui->TeamChannel1qxtSpanSlider->setLowerPosition(0);
    ui->TeamChannel1qxtSpanSlider->setUpperPosition(255);
    ui->TeamChannel2qxtSpanSlider->setLowerPosition(0);
    ui->TeamChannel2qxtSpanSlider->setUpperPosition(255);
    ui->TeamChannel3qxtSpanSlider->setLowerPosition(0);
    ui->TeamChannel3qxtSpanSlider->setUpperPosition(255);

    ui->TeamChannel1min->setText(QString::number(ui->TeamChannel1qxtSpanSlider->lowerValue()));
    ui->TeamChannel1max->setText(QString::number(ui->TeamChannel1qxtSpanSlider->upperValue()));
    ui->TeamChannel2min->setText(QString::number(ui->TeamChannel2qxtSpanSlider->lowerValue()));
    ui->TeamChannel2max->setText(QString::number(ui->TeamChannel2qxtSpanSlider->upperValue()));
    ui->TeamChannel3min->setText(QString::number(ui->TeamChannel3qxtSpanSlider->lowerValue()));
    ui->TeamChannel3max->setText(QString::number(ui->TeamChannel3qxtSpanSlider->upperValue()));
}

void ProcessingSettingsDialog::resetRobot1DialogToDefaults()
{
    if(DEFAULT_COLOR_TYPE==0)
        ui->Robot1ColorRGBButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==1)
        ui->Robot1ColorHSVButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==2)
        ui->Robot1ColorYCrCbButton->setChecked(true);

    ui->Robot1Channel1qxtSpanSlider->setLowerPosition(0);
    ui->Robot1Channel1qxtSpanSlider->setUpperPosition(255);
    ui->Robot1Channel2qxtSpanSlider->setLowerPosition(0);
    ui->Robot1Channel2qxtSpanSlider->setUpperPosition(255);
    ui->Robot1Channel3qxtSpanSlider->setLowerPosition(0);
    ui->Robot1Channel3qxtSpanSlider->setUpperPosition(255);

    ui->Robot1Channel1min->setText(QString::number(ui->Robot1Channel1qxtSpanSlider->lowerValue()));
    ui->Robot1Channel1max->setText(QString::number(ui->Robot1Channel1qxtSpanSlider->upperValue()));
    ui->Robot1Channel2min->setText(QString::number(ui->Robot1Channel2qxtSpanSlider->lowerValue()));
    ui->Robot1Channel2max->setText(QString::number(ui->Robot1Channel2qxtSpanSlider->upperValue()));
    ui->Robot1Channel3min->setText(QString::number(ui->Robot1Channel3qxtSpanSlider->lowerValue()));
    ui->Robot1Channel3max->setText(QString::number(ui->Robot1Channel3qxtSpanSlider->upperValue()));
}

void ProcessingSettingsDialog::resetRobot2DialogToDefaults()
{
    if(DEFAULT_COLOR_TYPE==0)
        ui->Robot2ColorRGBButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==1)
        ui->Robot2ColorHSVButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==2)
        ui->Robot2ColorYCrCbButton->setChecked(true);

    ui->Robot2Channel1qxtSpanSlider->setLowerPosition(0);
    ui->Robot2Channel1qxtSpanSlider->setUpperPosition(255);
    ui->Robot2Channel2qxtSpanSlider->setLowerPosition(0);
    ui->Robot2Channel2qxtSpanSlider->setUpperPosition(255);
    ui->Robot2Channel3qxtSpanSlider->setLowerPosition(0);
    ui->Robot2Channel3qxtSpanSlider->setUpperPosition(255);

    ui->Robot2Channel1min->setText(QString::number(ui->Robot2Channel1qxtSpanSlider->lowerValue()));
    ui->Robot2Channel1max->setText(QString::number(ui->Robot2Channel1qxtSpanSlider->upperValue()));
    ui->Robot2Channel2min->setText(QString::number(ui->Robot2Channel2qxtSpanSlider->lowerValue()));
    ui->Robot2Channel2max->setText(QString::number(ui->Robot2Channel2qxtSpanSlider->upperValue()));
    ui->Robot2Channel3min->setText(QString::number(ui->Robot2Channel3qxtSpanSlider->lowerValue()));
    ui->Robot2Channel3max->setText(QString::number(ui->Robot2Channel3qxtSpanSlider->upperValue()));
}

void ProcessingSettingsDialog::resetBallDialogToDefaults()
{
    if(DEFAULT_COLOR_TYPE==0)
        ui->BallColorRGBButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==1)
        ui->BallColorHSVButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==2)
        ui->BallColorYCrCbButton->setChecked(true);

    ui->BallChannel1qxtSpanSlider->setLowerPosition(0);
    ui->BallChannel1qxtSpanSlider->setUpperPosition(255);
    ui->BallChannel2qxtSpanSlider->setLowerPosition(0);
    ui->BallChannel2qxtSpanSlider->setUpperPosition(255);
    ui->BallChannel3qxtSpanSlider->setLowerPosition(0);
    ui->BallChannel3qxtSpanSlider->setUpperPosition(255);

    ui->BallChannel1min->setText(QString::number(ui->BallChannel1qxtSpanSlider->lowerValue()));
    ui->BallChannel1max->setText(QString::number(ui->BallChannel1qxtSpanSlider->upperValue()));
    ui->BallChannel2min->setText(QString::number(ui->BallChannel2qxtSpanSlider->lowerValue()));
    ui->BallChannel2max->setText(QString::number(ui->BallChannel2qxtSpanSlider->upperValue()));
    ui->BallChannel3min->setText(QString::number(ui->BallChannel3qxtSpanSlider->lowerValue()));
    ui->BallChannel3max->setText(QString::number(ui->BallChannel3qxtSpanSlider->upperValue()));
}

void ProcessingSettingsDialog::resetColorDialogToDefaults()
{
    resetTeamDialogToDefaults();
    resetRobot1DialogToDefaults();
    resetRobot2DialogToDefaults();
    resetBallDialogToDefaults();
}

void ProcessingSettingsDialog::updateStoredSettingsFromDialog()
{

    //COLOR:

    //Team
    if(ui->TeamColorRGBButton->isChecked())
        processingSettings.TeamColorType=0;
    else if(ui->TeamColorHSVButton->isChecked())
        processingSettings.TeamColorType=1;
    else if(ui->TeamColorYCrCbButton->isChecked())
        processingSettings.TeamColorType=2;

    processingSettings.TeamChannel1min = ui->TeamChannel1qxtSpanSlider->lowerPosition();
    processingSettings.TeamChannel1max = ui->TeamChannel1qxtSpanSlider->upperPosition();
    processingSettings.TeamChannel2min = ui->TeamChannel2qxtSpanSlider->lowerPosition();
    processingSettings.TeamChannel2max = ui->TeamChannel2qxtSpanSlider->upperPosition();
    processingSettings.TeamChannel3min = ui->TeamChannel3qxtSpanSlider->lowerPosition();
    processingSettings.TeamChannel3max = ui->TeamChannel3qxtSpanSlider->upperPosition();

    //Robot1
    if(ui->Robot1ColorRGBButton->isChecked())
        processingSettings.Robot1ColorType=0;
    else if(ui->Robot1ColorHSVButton->isChecked())
        processingSettings.Robot1ColorType=1;
    else if(ui->Robot1ColorYCrCbButton)
        processingSettings.Robot1ColorType=2;

    processingSettings.Robot1Channel1min = ui->Robot1Channel1qxtSpanSlider->lowerPosition();
    processingSettings.Robot1Channel1max = ui->Robot1Channel1qxtSpanSlider->upperPosition();
    processingSettings.Robot1Channel2min = ui->Robot1Channel2qxtSpanSlider->lowerPosition();
    processingSettings.Robot1Channel2max = ui->Robot1Channel2qxtSpanSlider->upperPosition();
    processingSettings.Robot1Channel3min = ui->Robot1Channel3qxtSpanSlider->lowerPosition();
    processingSettings.Robot1Channel3max = ui->Robot1Channel3qxtSpanSlider->upperPosition();

    //Robot2
    if(ui->Robot2ColorRGBButton->isChecked())
        processingSettings.Robot2ColorType=0;
    else if(ui->Robot2ColorHSVButton->isChecked())
        processingSettings.Robot2ColorType=1;
    else if(ui->Robot2ColorYCrCbButton)
        processingSettings.Robot2ColorType=2;

    processingSettings.Robot2Channel1min = ui->Robot2Channel1qxtSpanSlider->lowerPosition();
    processingSettings.Robot2Channel1max = ui->Robot2Channel1qxtSpanSlider->upperPosition();
    processingSettings.Robot2Channel2min = ui->Robot2Channel2qxtSpanSlider->lowerPosition();
    processingSettings.Robot2Channel2max = ui->Robot2Channel2qxtSpanSlider->upperPosition();
    processingSettings.Robot2Channel3min = ui->Robot2Channel3qxtSpanSlider->lowerPosition();
    processingSettings.Robot2Channel3max = ui->Robot2Channel3qxtSpanSlider->upperPosition();

    //Ball
    if(ui->BallColorRGBButton->isChecked())
        processingSettings.BallColorType=0;
    else if(ui->BallColorHSVButton->isChecked())
        processingSettings.BallColorType=1;
    else if(ui->BallColorYCrCbButton)
        processingSettings.BallColorType=2;

    processingSettings.BallChannel1min = ui->BallChannel1qxtSpanSlider->lowerPosition();
    processingSettings.BallChannel1max = ui->BallChannel1qxtSpanSlider->upperPosition();
    processingSettings.BallChannel2min = ui->BallChannel2qxtSpanSlider->lowerPosition();
    processingSettings.BallChannel2max = ui->BallChannel2qxtSpanSlider->upperPosition();
    processingSettings.BallChannel3min = ui->BallChannel3qxtSpanSlider->lowerPosition();
    processingSettings.BallChannel3max = ui->BallChannel3qxtSpanSlider->upperPosition();

    processingSettings.BSNumberOfIterations=ui->BSIterationsEdit->text().toInt();
    // Update processing flags in processingThread
    emit newProcessingSettings(processingSettings);
} // updateStoredSettingsFromDialog()

void ProcessingSettingsDialog::resetBSDialogToDefaults()
{
    ui->BSIterationsEdit->setText(QString::number(DEFAULT_BS_ITERATIONS));
} //resetBSDialogToDefaults()

void ProcessingSettingsDialog::resetAllDialogToDefaults()
{
    // Smooth
    resetColorDialogToDefaults();
    resetBSDialogToDefaults();
} // resetAllDialogToDefaults()

//Team
void ProcessingSettingsDialog::on_TeamChannel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamChannel1min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_TeamChannel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamChannel1max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_TeamChannel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamChannel2min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_TeamChannel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamChannel2max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_TeamChannel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamChannel3min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_TeamChannel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamChannel3max->setText(QString::number(upper));
}

//Robot2
void ProcessingSettingsDialog::on_Robot2Channel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2Channel1min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_Robot2Channel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2Channel1max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_Robot2Channel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2Channel2min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_Robot2Channel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2Channel2max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_Robot2Channel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2Channel3min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_Robot2Channel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2Channel3max->setText(QString::number(upper));
}

//Robot1
void ProcessingSettingsDialog::on_Robot1Channel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1Channel1min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_Robot1Channel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1Channel1max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_Robot1Channel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1Channel2min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_Robot1Channel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1Channel2max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_Robot1Channel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1Channel3min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_Robot1Channel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1Channel3max->setText(QString::number(upper));
}

//Ball
void ProcessingSettingsDialog::on_BallChannel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallChannel1min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_BallChannel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallChannel1max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_BallChannel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallChannel2min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_BallChannel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallChannel2max->setText(QString::number(upper));
}

void ProcessingSettingsDialog::on_BallChannel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallChannel3min->setText(QString::number(lower));
}

void ProcessingSettingsDialog::on_BallChannel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallChannel3max->setText(QString::number(upper));
}
