/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "CameraConnectDialog.h"

CameraConnectDialog::CameraConnectDialog(QWidget *parent) : QDialog(parent)
{
    // Setup dialog
    setupUi(this);
    // deviceNumberEdit (device number) input string validation
    QRegExp rx1("[0-9]\\d{0,2}"); // Integers 0 to 999
    QRegExpValidator *validator1 = new QRegExpValidator(rx1, 0);
    deviceNumberEdit->setValidator(validator1);
    // imageBufferSizeEdit (image buffer size) input string validation
    QRegExp rx2("[0-9]\\d{0,2}"); // Integers 0 to 999
    QRegExpValidator *validator2 = new QRegExpValidator(rx2, 0);
    imageBufferSizeEdit->setValidator(validator2);
    // Set imageBufferSizeEdit to default value
    imageBufferSizeEdit->setText(QString::number(DEFAULT_IMAGE_BUFFER_SIZE));
    // Initially set deviceNumber and imageBufferSize to defaults
    deviceNumber=-1;
    imageBufferSize=DEFAULT_IMAGE_BUFFER_SIZE;
} // CameraConnectDialog constructor

void CameraConnectDialog::setDeviceNumber()
{
    // "Any available camera"
    if(anyCameraButton->isChecked())
        deviceNumber=-1;
    // "Device number"
    else
    {
        // Set device number to default (any available camera) if field is blank
        if(deviceNumberEdit->text().isEmpty())
        {
            QMessageBox::warning(this->parentWidget(), "WARNING:","Device Number field blank.\nAutomatically set to 'any available camera'.");
            deviceNumber=-1;
        }
        // User-specified camera
        else
            deviceNumber=deviceNumberEdit->text().toInt();
    }
} // setDeviceNumber()

void CameraConnectDialog::setImageBufferSize()
{
    // Set image buffer size to default if field is blank
    if(imageBufferSizeEdit->text().isEmpty())
    {
        QMessageBox::warning(this->parentWidget(), "WARNING:","Image Buffer Size field blank.\nAutomatically set to default value.");
        imageBufferSize=DEFAULT_IMAGE_BUFFER_SIZE;
    }
    // Set image buffer size to default if field is zero
    else if(imageBufferSizeEdit->text().toInt()==0)
    {
        QMessageBox::warning(this->parentWidget(), "WARNING:","Image Buffer Size cannot be zero.\nAutomatically set to default value.");
        imageBufferSize=DEFAULT_IMAGE_BUFFER_SIZE;;
    }
    // Use image buffer size specified by user
    else
        imageBufferSize=imageBufferSizeEdit->text().toInt();
} // setImageBufferSize()

int CameraConnectDialog::getDeviceNumber()
{
    return deviceNumber;
} // getDeviceNumber()

int CameraConnectDialog::getImageBufferSize()
{
    return imageBufferSize;
} // getImageBufferSize()
