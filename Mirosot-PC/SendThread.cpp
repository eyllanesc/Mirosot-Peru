/************************************************************************/
/*Author:								*/
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>         */
/*				      <e.yllanescucho@gmail.com>        */
/************************************************************************/

#include "SendThread.h"

SendThread::SendThread(QString data) : QThread(), data(data)
{
    stopped=false;
}

bool SendThread::connectToserial(Settings settings)
{
    this->settings=&settings;
    serial = new  QSerialPort(this);
    serial->setPortName(settings.name);
    serial->setBaudRate(settings.baudRate);
    serial->setDataBits(settings.dataBits);
    serial->setParity(settings.parity);
    serial->setStopBits(settings.stopBits);
    serial->setFlowControl(settings.flowControl);
    return serial->open(QIODevice::ReadWrite);
}

void SendThread::receive(QString mydata)
{
    qbytearray=mydata.toLocal8Bit();
    serial->write(qbytearray);
}
bool SendThread::isPortConnected()
{
    return !(serial->error()==QSerialPort::ResourceError);
}

void SendThread::disconnectSerial()
{
    if(!(serial->error()==QSerialPort::ResourceError))
    {
        serial->close();
        qDebug() << "Serial successfully disconnected.";
    }

}
void SendThread::run()
{
    while(true)
    {
        /////////////////////////////////
        // Stop thread if stopped=TRUE //
        /////////////////////////////////
        stoppedMutex.lock();
        if (stopped)
        {
            stopped=false;
            stoppedMutex.unlock();
            break;
        }
        stoppedMutex.unlock();
        /////////////////////////////////
        /////////////////////////////////
        QByteArray databyte=QByteArray::number(1);
        serial->write(databyte);
    }
    qDebug() << "Stopping send thread...";
}

void SendThread::stopSendThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
}
