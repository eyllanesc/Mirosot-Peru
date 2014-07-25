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

    QRegExp rx1("[0-9]\\d{0,1}"); // Integers 1 to 99
    QRegExpValidator *validator1 = new QRegExpValidator(rx1, 0);
    ui->SmoothSizelineEdit->setValidator(validator1);

    QRegExp rx2("[0-9]\\d{0,1}"); // Integers 1 to 99
    QRegExpValidator *validator2 = new QRegExpValidator(rx2, 0);
    ui->ErodelineEdit->setValidator(validator2);

    QRegExp rx3("[0-9]\\d{0,1}"); // Integers 1 to 99
    QRegExpValidator *validator3 = new QRegExpValidator(rx3, 0);
    ui->DilatelineEdit->setValidator(validator3);

    // Set dialog values to defaults
    resetAllDialogToDefaults();
    // Update processing settings in processingSettings structure and processingThread
    updateStoredSettingsFromDialog();
    updateDialogSettingsFromStored();
    ui->radioButtonBlur->setChecked(true);
    connect(ui->TeamColorRGBButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->TeamColorHSVButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->TeamColorYCrCbButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->Robot1ColorRGBButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->Robot1ColorHSVButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->Robot1ColorYCrCbButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->Robot2ColorRGBButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->Robot2ColorHSVButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->Robot2ColorYCrCbButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->BallColorRGBButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->BallColorHSVButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->BallColorYCrCbButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->radioButtonBlur,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->radioButtonGauss,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->radioButtonMedian,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->radioButtonBilateral,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->pushButton ,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->DilateRectradioButton ,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->DilateCrossradioButton ,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->DilateEllipseradioButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->ErodeRectradioButton ,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->ErodeCrossradioButton ,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->ErodeEllipseradioButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->NoneflipradioButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->xflipradioButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->yflipradioButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->bothflipradioButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
    connect(ui->ApplyDilateErodepushButton,SIGNAL(clicked()),this,SLOT(updateStoredSettingsFromDialog()));
} // ProcessingSettingsDialog constructor

void ProcessingSettingsDialog::updateProcessingSettings(struct ProcessingSettings processingSettings)
{
    ui->TeamChannel1qxtSpanSlider->setLowerValue(processingSettings.TeamChannel1min);
    ui->TeamChannel1qxtSpanSlider->setUpperValue(processingSettings.TeamChannel1max);
    ui->TeamChannel2qxtSpanSlider->setLowerValue(processingSettings.TeamChannel2min);
    ui->TeamChannel2qxtSpanSlider->setUpperValue(processingSettings.TeamChannel2max);
    ui->TeamChannel3qxtSpanSlider->setLowerValue(processingSettings.TeamChannel3min);
    ui->TeamChannel3qxtSpanSlider->setUpperValue(processingSettings.TeamChannel3max);

    ui->TeamAreaqxtSpanSlider->setLowerValue(processingSettings.TeamAreamin);
    ui->TeamAreaqxtSpanSlider->setUpperValue(processingSettings.TeamAreamax);

    ui->Robot1Channel1qxtSpanSlider->setLowerValue(processingSettings.Robot1Channel1min);
    ui->Robot1Channel1qxtSpanSlider->setUpperValue(processingSettings.Robot1Channel1max);
    ui->Robot1Channel2qxtSpanSlider->setLowerValue(processingSettings.Robot1Channel2min);
    ui->Robot1Channel2qxtSpanSlider->setUpperValue(processingSettings.Robot1Channel2max);
    ui->Robot1Channel3qxtSpanSlider->setLowerValue(processingSettings.Robot1Channel3min);
    ui->Robot1Channel3qxtSpanSlider->setUpperValue(processingSettings.Robot1Channel3max);

    ui->Robot1AreaqxtSpanSlider->setLowerValue(processingSettings.Robot1Areamin);
    ui->Robot1AreaqxtSpanSlider->setUpperValue(processingSettings.Robot1Areamax);

    ui->Robot2Channel1qxtSpanSlider->setLowerValue(processingSettings.Robot2Channel1min);
    ui->Robot2Channel1qxtSpanSlider->setUpperValue(processingSettings.Robot2Channel1max);
    ui->Robot2Channel2qxtSpanSlider->setLowerValue(processingSettings.Robot2Channel2min);
    ui->Robot2Channel2qxtSpanSlider->setUpperValue(processingSettings.Robot2Channel2max);
    ui->Robot2Channel3qxtSpanSlider->setLowerValue(processingSettings.Robot2Channel3min);
    ui->Robot2Channel3qxtSpanSlider->setUpperValue(processingSettings.Robot2Channel3max);

    ui->Robot2AreaqxtSpanSlider->setLowerValue(processingSettings.Robot2Areamin);
    ui->Robot2AreaqxtSpanSlider->setUpperValue(processingSettings.Robot2Areamax);

    ui->BallChannel1qxtSpanSlider->setLowerValue(processingSettings.BallChannel1min);
    ui->BallChannel1qxtSpanSlider->setUpperValue(processingSettings.BallChannel1max);
    ui->BallChannel2qxtSpanSlider->setLowerValue(processingSettings.BallChannel2min);
    ui->BallChannel2qxtSpanSlider->setUpperValue(processingSettings.BallChannel2max);
    ui->BallChannel3qxtSpanSlider->setLowerValue(processingSettings.BallChannel3min);
    ui->BallChannel3qxtSpanSlider->setUpperValue(processingSettings.BallChannel3max);

    ui->BallAreaqxtSpanSlider->setLowerValue(processingSettings.BallAreamin);
    ui->BallAreaqxtSpanSlider->setUpperValue(processingSettings.BallAreamax);
}

void ProcessingSettingsDialog::updateDialogSettingsFromStored()
{
    //SMOOTH
    if(processingSettings.SmoothType==0)
        ui->radioButtonBlur->setChecked(true);
    else if(processingSettings.SmoothType==1)
        ui->radioButtonGauss->setChecked(true);
    else if(processingSettings.SmoothType==2)
        ui->radioButtonMedian->setChecked(true);
    else if(processingSettings.SmoothType==3)
        ui->radioButtonBilateral->setChecked(true);

    //FLIP
    if(processingSettings.FlipType==NFLIP)
        ui->NoneflipradioButton->setChecked(true);
    else if(processingSettings.FlipType==XFLIP)
        ui->xflipradioButton->setChecked(true);
    else if(processingSettings.FlipType==YFLIP)
        ui->yflipradioButton->setChecked(true);
    else if(processingSettings.FlipType==BFLIP)
        ui->bothflipradioButton->setChecked(true);

    //DILATE
    if(processingSettings.DilateType==cv::MORPH_RECT)
        ui->DilateRectradioButton->setChecked(true);
    else if(processingSettings.DilateType==cv::MORPH_CROSS)
        ui->DilateCrossradioButton->setChecked(true);
    else if(processingSettings.DilateType==cv::MORPH_ELLIPSE)
        ui->DilateEllipseradioButton->setChecked(true);
    ui->DilatelineEdit->setText(QString::number(processingSettings.DilateSize));

    //ERODE
    if(processingSettings.ErodeType==cv::MORPH_RECT)
        ui->ErodeRectradioButton->setChecked(true);
    else if(processingSettings.ErodeType==cv::MORPH_CROSS)
        ui->ErodeCrossradioButton->setChecked(true);
    else if(processingSettings.ErodeType==cv::MORPH_ELLIPSE)
        ui->ErodeEllipseradioButton->setChecked(true);
    ui->ErodelineEdit->setText(QString::number(processingSettings.ErodeSize));

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

    ui->TeamAreaqxtSpanSlider->setLowerValue(processingSettings.TeamAreamin);
    ui->TeamAreaqxtSpanSlider->setUpperValue(processingSettings.TeamAreamax);

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

    ui->Robot1AreaqxtSpanSlider->setLowerValue(processingSettings.Robot1Areamin);
    ui->Robot1AreaqxtSpanSlider->setUpperValue(processingSettings.Robot1Areamax);

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

    ui->Robot2AreaqxtSpanSlider->setLowerValue(processingSettings.Robot2Areamin);
    ui->Robot2AreaqxtSpanSlider->setUpperValue(processingSettings.Robot2Areamax);

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

    ui->BallAreaqxtSpanSlider->setLowerValue(processingSettings.BallAreamin);
    ui->BallAreaqxtSpanSlider->setUpperValue(processingSettings.BallAreamax);
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

    ui->TeamAreaminLabel->setText(QString::number(ui->TeamAreaqxtSpanSlider->lowerValue()));
    ui->TeamAreamaxLabel->setText(QString::number(ui->TeamAreaqxtSpanSlider->upperValue()));
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

    ui->Robot1AreaminLabel->setText(QString::number(ui->Robot1AreaqxtSpanSlider->lowerValue()));
    ui->Robot1AreamaxLabel->setText(QString::number(ui->Robot1AreaqxtSpanSlider->upperValue()));
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

    ui->Robot2AreaminLabel->setText(QString::number(ui->Robot2AreaqxtSpanSlider->lowerValue()));
    ui->Robot2AreamaxLabel->setText(QString::number(ui->Robot2AreaqxtSpanSlider->upperValue()));
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

    ui->BallAreaminLabel->setText(QString::number(ui->BallAreaqxtSpanSlider->lowerValue()));
    ui->BallAreamaxLabel->setText(QString::number(ui->BallAreaqxtSpanSlider->upperValue()));
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
    //SMOOTH
    if(ui->radioButtonBlur->isChecked())
        processingSettings.SmoothType=0;
    else if(ui->radioButtonGauss->isChecked())
        processingSettings.SmoothType=1;
    else if(ui->radioButtonMedian->isChecked())
        processingSettings.SmoothType=2;
    else if(ui->radioButtonBilateral->isChecked())
        processingSettings.SmoothType=3;

    //FLIP
    if(ui->NoneflipradioButton->isChecked())
        processingSettings.FlipType=NFLIP;
    else if(ui->xflipradioButton->isChecked())
        processingSettings.FlipType=XFLIP;
    else if(ui->yflipradioButton->isChecked())
        processingSettings.FlipType=YFLIP;
    else if(ui->bothflipradioButton->isChecked())
        processingSettings.FlipType=BFLIP;

    //DILATE
    if(ui->DilateRectradioButton->isChecked())
        processingSettings.DilateType=cv::MORPH_RECT;
    else if(ui->DilateCrossradioButton->isChecked())
        processingSettings.DilateType=cv::MORPH_CROSS;
    else if(ui->DilateEllipseradioButton->isChecked())
        processingSettings.DilateType=cv::MORPH_ELLIPSE;
    processingSettings.DilateSize=ui->DilatelineEdit->text().toInt();

    //ERODE
    if(ui->ErodeRectradioButton->isChecked())
        processingSettings.ErodeType=cv::MORPH_RECT;
    else if(ui->ErodeCrossradioButton->isChecked())
        processingSettings.ErodeType=cv::MORPH_CROSS;
    else if(ui->ErodeEllipseradioButton->isChecked())
        processingSettings.ErodeType=cv::MORPH_ELLIPSE;
    processingSettings.ErodeSize=ui->ErodelineEdit->text().toInt();

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

    processingSettings.TeamAreamin=ui->TeamAreaqxtSpanSlider->lowerPosition();
    processingSettings.TeamAreamax=ui->TeamAreaqxtSpanSlider->upperPosition();

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

    processingSettings.Robot1Areamin=ui->Robot1AreaqxtSpanSlider->lowerPosition();
    processingSettings.Robot1Areamax=ui->Robot1AreaqxtSpanSlider->upperPosition();

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

    processingSettings.Robot2Areamin=ui->Robot2AreaqxtSpanSlider->lowerPosition();
    processingSettings.Robot2Areamax=ui->Robot2AreaqxtSpanSlider->upperPosition();

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

    processingSettings.BallAreamin=ui->BallAreaqxtSpanSlider->lowerPosition();
    processingSettings.BallAreamax=ui->BallAreaqxtSpanSlider->upperPosition();

    BSNumberOfIterations=ui->BSIterationsEdit->text().toInt();
    processingSettings.SmoothSize=ui->SmoothSizelineEdit->text().toInt();

    // Update processing flags in processingThread
    emit newProcessingSettings(processingSettings);
} // updateStoredSettingsFromDialog()

void ProcessingSettingsDialog::resetSmoothsizeDefaults()
{
    ui->SmoothSizelineEdit->setText(QString::number(DEFAULT_SMOOTH_ITERATIONS));
}

void ProcessingSettingsDialog::resetBSDialogToDefaults()
{
    ui->BSIterationsEdit->setText(QString::number(DEFAULT_BS_ITERATIONS));
} //resetBSDialogToDefaults()

void ProcessingSettingsDialog::resetAllDialogToDefaults()
{
    // Smooth
    resetColorDialogToDefaults();
    resetBSDialogToDefaults();
    resetSmoothsizeDefaults();
} // resetAllDialogToDefaults()

//Team
void ProcessingSettingsDialog::on_TeamChannel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamChannel1min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_TeamChannel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamChannel1max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_TeamChannel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamChannel2min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_TeamChannel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamChannel2max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_TeamChannel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamChannel3min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_TeamChannel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamChannel3max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

//Robot2
void ProcessingSettingsDialog::on_Robot2Channel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2Channel1min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2Channel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2Channel1max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2Channel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2Channel2min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2Channel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2Channel2max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2Channel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2Channel3min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2Channel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2Channel3max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

//Robot1
void ProcessingSettingsDialog::on_Robot1Channel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1Channel1min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1Channel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1Channel1max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1Channel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1Channel2min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1Channel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1Channel2max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1Channel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1Channel3min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1Channel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1Channel3max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

//Ball
void ProcessingSettingsDialog::on_BallChannel1qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallChannel1min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallChannel1qxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallChannel1max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallChannel2qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallChannel2min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallChannel2qxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallChannel2max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallChannel3qxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallChannel3min->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallChannel3qxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallChannel3max->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BSpushButton_clicked()
{
    emit newNumberBS(ui->BSIterationsEdit->text().toInt());
}

void ProcessingSettingsDialog::on_TeamAreaqxtSpanSlider_lowerValueChanged(int lower)
{
    ui->TeamAreaminLabel->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_TeamAreaqxtSpanSlider_upperValueChanged(int upper)
{
    ui->TeamAreamaxLabel->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1AreaqxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot1AreaminLabel->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot1AreaqxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot1AreamaxLabel->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2AreaqxtSpanSlider_lowerValueChanged(int lower)
{
    ui->Robot2AreaminLabel->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_Robot2AreaqxtSpanSlider_upperValueChanged(int upper)
{
    ui->Robot2AreamaxLabel->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallAreaqxtSpanSlider_lowerValueChanged(int lower)
{
    ui->BallAreaminLabel->setText(QString::number(lower));
    updateStoredSettingsFromDialog();
}

void ProcessingSettingsDialog::on_BallAreaqxtSpanSlider_upperValueChanged(int upper)
{
    ui->BallAreamaxLabel->setText(QString::number(upper));
    updateStoredSettingsFromDialog();
}
