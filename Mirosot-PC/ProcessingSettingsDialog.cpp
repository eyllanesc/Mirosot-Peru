/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "ProcessingSettingsDialog.h"


ProcessingSettingsDialog::ProcessingSettingsDialog(QWidget *parent) : QDialog(parent)
{
    // Setup dialog
    setupUi(this);
    // Connect GUI signals and slots
    connect(resetAllToDefaultsButton,SIGNAL(released()),SLOT(resetAllDialogToDefaults()));
    connect(resetColorToDefaultsButton,SIGNAL(released()),SLOT(resetColorDialogToDefaults()));
    connect(applyButton,SIGNAL(released()),SLOT(updateStoredSettingsFromDialog()));
    connect(ColorTypeGroup,SIGNAL(buttonReleased(QAbstractButton*)),SLOT(ColorTypeChange(QAbstractButton*)));
    // dilateIterationsEdit input string validation
    QRegExp rx("[1-9]\\d{0,2}"); // Integers 1 to 99
    QRegExpValidator *validator = new QRegExpValidator(rx, 0);
    BSIterationsEdit->setValidator(validator);
    // Set dialog values to defaults
    resetAllDialogToDefaults();
    // Update processing settings in processingSettings structure and processingThread
    updateStoredSettingsFromDialog();
} // ProcessingSettingsDialog constructor

void ProcessingSettingsDialog::updateStoredSettingsFromDialog()
{
    // Validate values in dialog before storing
    validateDialog();
    // Color
    if(ColorTypeGroup->checkedButton()==(QAbstractButton*)ColorRGBButton )
        processingSettings.ColorType=0;
    else if(ColorTypeGroup->checkedButton()==(QAbstractButton*)ColorHSVButton)
        processingSettings.ColorType=1;
    else if(ColorTypeGroup->checkedButton()==(QAbstractButton*)ColorYCrCbButton)
        processingSettings.ColorType=2;
    processingSettings.ColorParam1=ColorParam1Edit->text().toInt();
    processingSettings.ColorParam2=ColorParam2Edit->text().toInt();
    processingSettings.ColorParam3=ColorParam3Edit->text().toInt();
    processingSettings.ColorParam1_2=ColorParam1Edit_2->text().toInt();
    processingSettings.ColorParam2_2=ColorParam2Edit_2->text().toInt();
    processingSettings.ColorParam3_2=ColorParam3Edit_2->text().toInt();
    // BS
    processingSettings.BSNumberOfIterations=BSIterationsEdit->text().toInt();
    // Update processing flags in processingThread
    emit newProcessingSettings(processingSettings);
} // updateStoredSettingsFromDialog()

void ProcessingSettingsDialog::updateDialogSettingsFromStored()
{
    // Color
    if(processingSettings.ColorType==0)
        ColorRGBButton->setChecked(true);
    else if(processingSettings.ColorType==1)
        ColorHSVButton->setChecked(true);
    else if(processingSettings.ColorType==2)
        ColorYCrCbButton->setChecked(true);
    ColorParam1Edit->setText(QString::number(processingSettings.ColorParam1));
    ColorParam2Edit->setText(QString::number(processingSettings.ColorParam2));
    ColorParam3Edit->setText(QString::number(processingSettings.ColorParam3));
    ColorParam1Edit_2->setText(QString::number(processingSettings.ColorParam1_2));
    ColorParam2Edit_2->setText(QString::number(processingSettings.ColorParam2_2));
    ColorParam3Edit_2->setText(QString::number(processingSettings.ColorParam3_2));
    // BS
    BSIterationsEdit->setText(QString::number(processingSettings.BSNumberOfIterations));
    // Enable/disable appropriate Smooth parameter inputs
    ColorTypeChange(ColorTypeGroup->checkedButton());
} // updateDialogSettingsFromStored()

void ProcessingSettingsDialog::resetAllDialogToDefaults()
{
    // Smooth
    resetColorDialogToDefaults();
    resetBSDialogToDefaults();
} // resetAllDialogToDefaults()

void ProcessingSettingsDialog::ColorTypeChange(QAbstractButton *input)
{
    if(input==(QAbstractButton*)ColorRGBButton)
    {
        // ColorParam1Edit input string validation
        QRegExp rx1("[1-9]\\d{0,2}"); // Integers 1 to 99
        QRegExpValidator *validator1 = new QRegExpValidator(rx1, 0);
        ColorParam1Edit->setValidator(validator1);
        // ColorParam2Edit input string validation
        QRegExp rx2("[1-9]\\d{0,2}"); // Integers 1 to 99
        QRegExpValidator *validator2 = new QRegExpValidator(rx2, 0);
        ColorParam2Edit->setValidator(validator2);
        // ColorParam1Edit input string validation
        QRegExp rx3("[1-9]\\d{0,2}"); // Integers 1 to 99
        QRegExpValidator *validator3 = new QRegExpValidator(rx1, 0);
        ColorParam3Edit->setValidator(validator3);
        // ColorParam2Edit input string validation
        QRegExp rx1_2("[1-9]\\d{0,2}"); // Integers 1 to 99
        QRegExpValidator *validator1_2 = new QRegExpValidator(rx1_2, 0);
        ColorParam1Edit_2->setValidator(validator1_2);
        QRegExp rx2_2("[1-9]\\d{0,2}"); // Integers 1 to 99
        QRegExpValidator *validator2_2 = new QRegExpValidator(rx2_2, 0);
        ColorParam2Edit_2->setValidator(validator2_2);
        QRegExp rx3_2("[1-9]\\d{0,2}"); // Integers 1 to 99
        QRegExpValidator *validator3_2 = new QRegExpValidator(rx3_2, 0);
        ColorParam3Edit_2->setValidator(validator3_2);
        // Enable/disable appropriate parameter inputs
        ColorParam1Edit->setEnabled(true);
        ColorParam2Edit->setEnabled(true);
        ColorParam3Edit->setEnabled(true);
        ColorParam1Edit_2->setEnabled(true);
        ColorParam2Edit_2->setEnabled(true);
        ColorParam3Edit_2->setEnabled(true);
        // Set parameter range labels
        //smoothParam1RangeLabel->setText("[1-99]");
        //smoothParam2RangeLabel->setText("[1-99]");
        //smoothParam3RangeLabel->setText("");
        //smoothParam4RangeLabel->setText("");
        // Set parameter labels
        ColorParam1Label->setText("R min:");
        ColorParam2Label->setText("G min:");
        ColorParam3Label->setText("B min:");
        ColorParam1Label_2->setText("R max:");
        ColorParam2Label_2->setText("G max:");
        ColorParam3Label_2->setText("B max:");
    }
    else if(input==(QAbstractButton*)ColorHSVButton)
    {
        // ColorParam1Edit input string validation
        QRegExp rx1("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator1 = new QRegExpValidator(rx1, 0);
        ColorParam1Edit->setValidator(validator1);
        // ColorParam2Edit input string validation
        QRegExp rx2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator2 = new QRegExpValidator(rx2, 0);
        ColorParam2Edit->setValidator(validator2);
        // ColorParam1Edit input string validation
        QRegExp rx3("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator3 = new QRegExpValidator(rx1, 0);
        ColorParam3Edit->setValidator(validator3);
        // ColorParam2Edit input string validation
        QRegExp rx1_2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator1_2 = new QRegExpValidator(rx1_2, 0);
        ColorParam1Edit_2->setValidator(validator1_2);
        QRegExp rx2_2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator2_2 = new QRegExpValidator(rx2_2, 0);
        ColorParam2Edit_2->setValidator(validator2_2);
        QRegExp rx3_2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator3_2 = new QRegExpValidator(rx3_2, 0);
        ColorParam3Edit_2->setValidator(validator3_2);
        // Enable/disable appropriate parameter inputs
        ColorParam1Edit->setEnabled(true);
        ColorParam2Edit->setEnabled(true);
        ColorParam3Edit->setEnabled(true);
        ColorParam1Edit_2->setEnabled(true);
        ColorParam2Edit_2->setEnabled(true);
        ColorParam3Edit_2->setEnabled(true);
        // Set parameter range labels
        //smoothParam1RangeLabel->setText("[1-99]");
        //smoothParam2RangeLabel->setText("[1-99]");
        //smoothParam3RangeLabel->setText("");
        //smoothParam4RangeLabel->setText("");
        // Set parameter labels
        ColorParam1Label->setText("H min:");
        ColorParam2Label->setText("S min:");
        ColorParam3Label->setText("V min:");
        ColorParam1Label_2->setText("H max:");
        ColorParam2Label_2->setText("S max:");
        ColorParam3Label_2->setText("V max:");
    }
    else if(input==(QAbstractButton*)ColorYCrCbButton)
    {
        // ColorParam1Edit input string validation
        QRegExp rx1("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator1 = new QRegExpValidator(rx1, 0);
        ColorParam1Edit->setValidator(validator1);
        // ColorParam2Edit input string validation
        QRegExp rx2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator2 = new QRegExpValidator(rx2, 0);
        ColorParam2Edit->setValidator(validator2);
        // ColorParam1Edit input string validation
        QRegExp rx3("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator3 = new QRegExpValidator(rx1, 0);
        ColorParam3Edit->setValidator(validator3);
        // ColorParam2Edit input string validation
        QRegExp rx1_2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator1_2 = new QRegExpValidator(rx1_2, 0);
        ColorParam1Edit_2->setValidator(validator1_2);
        QRegExp rx2_2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator2_2 = new QRegExpValidator(rx2_2, 0);
        ColorParam2Edit_2->setValidator(validator2_2);
        QRegExp rx3_2("[1-9]\\d{0,1}"); // Integers 1 to 99
        QRegExpValidator *validator3_2 = new QRegExpValidator(rx3_2, 0);
        ColorParam3Edit_2->setValidator(validator3_2);
        // Enable/disable appropriate parameter inputs
        ColorParam1Edit->setEnabled(true);
        ColorParam2Edit->setEnabled(true);
        ColorParam3Edit->setEnabled(true);
        ColorParam1Edit_2->setEnabled(true);
        ColorParam2Edit_2->setEnabled(true);
        ColorParam3Edit_2->setEnabled(true);
        // Set parameter range labels
        //smoothParam1RangeLabel->setText("[1-99]");
        //smoothParam2RangeLabel->setText("[1-99]");
        //smoothParam3RangeLabel->setText("");
        //smoothParam4RangeLabel->setText("");
        // Set parameter labels
        ColorParam1Label->setText("Y min:");
        ColorParam2Label->setText("Cr min:");
        ColorParam3Label->setText("Cb min:");
        ColorParam1Label_2->setText("Y max:");
        ColorParam2Label_2->setText("Cr max:");
        ColorParam3Label_2->setText("Cb max:");
    }
} // ColorTypeChange()

void ProcessingSettingsDialog::validateDialog()
{
    // Local variables
    bool inputEmpty=false;

    /*// If value of Smooth parameter 1 is EVEN (and not zero), convert to ODD by adding 1
    if(((smoothParam1Edit->text().toInt()%2)==0)&&(smoothParam1Edit->text().toInt()!=0))
    {
        smoothParam1Edit->setText(QString::number(smoothParam1Edit->text().toInt()+1));
        QMessageBox::information(this->parentWidget(),"NOTE:","Smooth parameter 1 must be an ODD number.\n\nAutomatically set to (inputted value+1).");
    }
    // If value of Smooth parameter 2 is EVEN (and not zero), convert to ODD by adding 1
    if(((smoothParam2Edit->text().toInt()%2)==0)&&(smoothParam2Edit->text().toInt()!=0))
    {
        smoothParam2Edit->setText(QString::number(smoothParam2Edit->text().toInt()+1));
        QMessageBox::information(this->parentWidget(),"NOTE:","Smooth parameter 2 must be an ODD number (or zero).\n\nAutomatically set to (inputted value+1).");
    }
*/
    // Check for empty inputs: if empty, set to default values
    if(ColorParam1Edit->text().isEmpty())
    {
        ColorParam1Edit->setText(QString::number(DEFAULT_COLOR_PARAM_1));
    }
    if(ColorParam2Edit->text().isEmpty())
    {
        ColorParam2Edit->setText(QString::number(DEFAULT_COLOR_PARAM_2));
    }
    if(ColorParam3Edit->text().isEmpty())
    {
        ColorParam1Edit->setText(QString::number(DEFAULT_COLOR_PARAM_3));
    }
    if(ColorParam1Edit_2->text().isEmpty())
    {
        ColorParam1Edit_2->setText(QString::number(DEFAULT_COLOR_PARAM_1_2));
    }
    if(ColorParam2Edit_2->text().isEmpty())
    {
        ColorParam2Edit_2->setText(QString::number(DEFAULT_COLOR_PARAM_2_2));
    }
    if(ColorParam3Edit_2->text().isEmpty())
    {
        ColorParam1Edit_2->setText(QString::number(DEFAULT_COLOR_PARAM_3_2));
    }
    if(BSIterationsEdit->text().isEmpty())
    {
        BSIterationsEdit->setText(QString::number(DEFAULT_BS_ITERATIONS));
        inputEmpty=true;
    }
    // Check if any of the inputs were empty
    if(inputEmpty)
        QMessageBox::warning(this->parentWidget(),"WARNING:","One or more inputs empty.\n\nAutomatically set to default values.");

    // Check for special parameter cases when smoothing type is GAUSSIAN
    /*if((smoothTypeGroup->checkedButton()==(QAbstractButton*)smoothGaussianButton)&&
       (smoothParam1Edit->text().toInt()==0)&&(smoothParam3Edit->text().toDouble()==0.00))
    {
        smoothParam1Edit->setText(QString::number(DEFAULT_SMOOTH_PARAM_1));
        smoothParam3Edit->setText(QString::number(DEFAULT_SMOOTH_PARAM_3));
        QMessageBox::warning(this->parentWidget(),"ERROR:","Parameters 1 and 3 cannot BOTH be zero when the smoothing type is GAUSSIAN.\n\nAutomatically set to default values.");
    }
    if((smoothTypeGroup->checkedButton()==(QAbstractButton*)smoothGaussianButton)&&
       (smoothParam2Edit->text().toInt()==0)&&(smoothParam4Edit->text().toDouble()==0.00))
    {
        smoothParam2Edit->setText(QString::number(DEFAULT_SMOOTH_PARAM_2));
        smoothParam4Edit->setText(QString::number(DEFAULT_SMOOTH_PARAM_4));
        QMessageBox::warning(this->parentWidget(),"ERROR:","Parameters 2 and 4 cannot BOTH be zero when the smoothing type is GAUSSIAN.\n\nAutomatically set to default values.");
    }
    */
    // Ensure neither smoothing parameters 1 or 2 are ZERO (except in the GAUSSIAN case)
    /*if((smoothTypeGroup->checkedButton()!=(QAbstractButton*)smoothGaussianButton)&&
       ((smoothParam1Edit->text().toInt()==0)||(smoothParam2Edit->text().toDouble()==0)))
    {
        smoothParam1Edit->setText(QString::number(DEFAULT_SMOOTH_PARAM_1));
        smoothParam2Edit->setText(QString::number(DEFAULT_SMOOTH_PARAM_2));
        QMessageBox::warning(this->parentWidget(),"ERROR:","Parameters 1 or 2 cannot be zero for the current smoothing type.\n\nAutomatically set to default values.");
    }*/
} // validateDialog()

void ProcessingSettingsDialog::resetColorDialogToDefaults()
{
    if(DEFAULT_COLOR_TYPE==0)
        ColorRGBButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==1)
        ColorHSVButton->setChecked(true);
    else if(DEFAULT_COLOR_TYPE==2)
        ColorYCrCbButton->setChecked(true);
    ColorParam1Edit->setText(QString::number(DEFAULT_COLOR_PARAM_1));
    ColorParam2Edit->setText(QString::number(DEFAULT_COLOR_PARAM_2));
    ColorParam3Edit->setText(QString::number(DEFAULT_COLOR_PARAM_3));
    ColorParam1Edit_2->setText(QString::number(DEFAULT_COLOR_PARAM_1_2));
    ColorParam2Edit_2->setText(QString::number(DEFAULT_COLOR_PARAM_2_2));
    ColorParam3Edit_2->setText(QString::number(DEFAULT_COLOR_PARAM_3_2));
    // Enable/disable appropriate Smooth parameter inputs
    ColorTypeChange(ColorTypeGroup->checkedButton());
} // resetSmoothDialogToDefaults()

void ProcessingSettingsDialog::resetBSDialogToDefaults()
{
    BSIterationsEdit->setText(QString::number(DEFAULT_BS_ITERATIONS));
} // resetDilateDialogToDefaults()
