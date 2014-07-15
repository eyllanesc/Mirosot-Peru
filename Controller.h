/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef CONTROLLER_H
#define CONTROLLER_H

// My header
#include "CaptureThread.h"
#include "ProcessingThread.h"
#include "SendThread.h"

// OpenCV header files
#include <opencv2/highgui/highgui.hpp>

class ImageBuffer;

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();
    ~Controller();

    ImageBuffer *imageBuffer;
    ProcessingThread *processingThread;
    CaptureThread *captureThread;
    SendThread *sendThread;

    QString data;
    bool connectToCamera(int,int);
    bool connectToSerial(Settings);
    void disconnectCamera();
    void disconnectSerial();
    void stopCaptureThread();
    void stopSendThread();
    void stopProcessingThread();
    void deleteCaptureThread();
    void deleteProcessingThread();
    void deleteSendThread();

    void deleteData();
    void clearImageBuffer();
    void deleteImageBuffer();
private:
    int imageBufferSizeStore;
};

#endif // CONTROLLER_H
