/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef STRUCTURES_H
#define STRUCTURES_H

// Qt header files
#include <QtGui>
#include <QtSerialPort/QSerialPort>

// ProcessingSettings structure definition
struct ProcessingSettings{
    int ColorType;

    int ColorParam1;
    int ColorParam2;
    int ColorParam3;
    int ColorParam1_2;
    int ColorParam2_2;
    int ColorParam3_2;

    int BSNumberOfIterations;
    int flipCode;
    double cannyThreshold1;
    double cannyThreshold2;
    int cannyApertureSize;
    bool cannyL2gradient;
};

// ProcessingFlags structure definition
struct ProcessingFlags{
    bool bsOn;
    bool colorOn;
};

// TaskData structure definition
struct TaskData{
    QRect selectionBox;
    bool setROIFlag;
    bool resetROIFlag;
};

// MouseData structure definition
struct MouseData{
    QRect selectionBox;
    bool leftButtonRelease;
    bool rightButtonRelease;
};

struct Settings {
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
    bool localEchoEnabled;
};
#endif // STRUCTURES_H
