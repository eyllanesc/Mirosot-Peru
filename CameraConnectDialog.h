/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef CAMERACONNECTDIALOG_H
#define CAMERACONNECTDIALOG_H

// My header
#include "ui_CameraConnectDialog.h"
#include "Config.h"

// Qt header files
#include <QtGui>

class CameraConnectDialog : public QDialog, private Ui::CameraConnectDialog
{
    Q_OBJECT

public:
    CameraConnectDialog(QWidget *parent = 0);
    void setDeviceNumber();
    void setImageBufferSize();
    int getDeviceNumber();
    int getImageBufferSize();
private:
    int deviceNumber;
    int imageBufferSize;
};

#endif // CAMERACONNECTDIALOG_H
