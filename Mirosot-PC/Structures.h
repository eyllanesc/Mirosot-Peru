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
    int TeamColorType;
    int Robot1ColorType;
    int Robot2ColorType;
    int BallColorType;

    int TeamChannel1min;
    int TeamChannel1max;
    int TeamChannel2min;
    int TeamChannel2max;
    int TeamChannel3min;
    int TeamChannel3max;

    int Robot1Channel1min;
    int Robot1Channel1max;
    int Robot1Channel2min;
    int Robot1Channel2max;
    int Robot1Channel3min;
    int Robot1Channel3max;

    int Robot2Channel1min;
    int Robot2Channel1max;
    int Robot2Channel2min;
    int Robot2Channel2max;
    int Robot2Channel3min;
    int Robot2Channel3max;

    int BallChannel1min;
    int BallChannel1max;
    int BallChannel2min;
    int BallChannel2max;
    int BallChannel3min;
    int BallChannel3max;

    int BSNumberOfIterations;
};

// ProcessingFlags structure definition
struct ProcessingFlags{
    bool playOn;
    bool rgbOn;
    bool hsvOn;
    bool ycrcbOn;

    bool bsOn;

    bool teamOn;
    bool robot1On;
    bool robot2On;
    bool ballOn;
};

// TaskData structure definition
struct TaskData{
    QRect selectionBox;
    bool setROIFlag;
    bool resetROIFlag;
};

// MouseData structure definition
struct MouseData{
    QPoint initial;
    QPoint final;
    QRect selectionBox;
    bool leftButtonRelease;
    bool rightButtonRelease;
};

struct PosData{
    QPoint initial;
    QPoint final;
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
