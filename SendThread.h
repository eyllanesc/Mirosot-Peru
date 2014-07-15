/************************************************************************/
/*Author:								*/
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>         */
/*				      <e.yllanescucho@gmail.com>        */
/************************************************************************/

#ifndef SENDTHREAD_H
#define SENDTHREAD_H

// My header
#include "Structures.h"

// Qt header files
#include <QThread>
#include <QtSerialPort/QSerialPort>

class SendThread : public QThread
{
    Q_OBJECT
public:
    SendThread(QString);
    void receive(QString);
    bool connectToserial(Settings);
    void disconnectSerial();
    bool isPortConnected();
    void stopSendThread();
    QSerialPort *serial;
private:
    QString data;
    Settings* settings;
    volatile bool stopped;
    QMutex stoppedMutex;
protected:
    //void run();
};

#endif // SENDTHREAD_H
