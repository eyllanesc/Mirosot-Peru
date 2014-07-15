/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "Controller.h"

Controller::Controller()
{
} // Controller constructor

Controller::~Controller()
{
} // Controller destructor

bool Controller::connectToSerial(Settings settings)
{
    bool isOpened;
    sendThread = new SendThread(data);
    isOpened=sendThread->connectToserial(settings);
    return isOpened;
}
bool Controller::connectToCamera(int deviceNumber, int imageBufferSize)
{
    // Local variable
    bool isOpened;
    // Store imageBufferSize in private member
    imageBufferSizeStore=imageBufferSize;
    // Create image buffer with user-defined size
    imageBuffer = new ImageBuffer(imageBufferSize);
    // Create capture thread
    captureThread = new CaptureThread(imageBuffer);
    // Connect to camera
    isOpened=captureThread->connectToCamera(deviceNumber);
    // Create processing thread (if camera open was successful)
    if(isOpened)
        processingThread = new ProcessingThread(imageBuffer,captureThread->getInputSourceWidth(),captureThread->getInputSourceHeight());
    // Return camera open result
    return isOpened;
} // connectToCamera()

void Controller::disconnectCamera()
{
    captureThread->disconnectCamera();
} // disconnectCamera()

void Controller::disconnectSerial()
{
    sendThread->disconnectSerial();
}

void Controller::stopSendThread()
{
    qDebug() << "About to stop send thread...";
    sendThread->stopSendThread();
    sendThread->wait();
    qDebug() << "Send thread successfully stopped.";
}
void Controller::stopCaptureThread()
{
    qDebug() << "About to stop capture thread...";
    captureThread->stopCaptureThread();
    // Take one frame off a FULL queue to allow the capture thread to finish
    if(imageBuffer->getSizeOfImageBuffer()==imageBufferSizeStore)
    {
        cv::Mat temp;
        temp=imageBuffer->getFrame();
    }
    captureThread->wait();
    qDebug() << "Capture thread successfully stopped.";
} // stopCaptureThread()

void Controller::stopProcessingThread()
{
    qDebug() << "About to stop processing thread...";
    processingThread->stopProcessingThread();
    processingThread->wait();
    qDebug() << "Processing thread successfully stopped.";
} // stopProcessingThread()

void Controller::deleteSendThread()
{
    delete sendThread;
}
void Controller::deleteCaptureThread()
{
    // Delete thread
    delete captureThread;
} // deleteCaptureThread()

void Controller::deleteProcessingThread()
{
    // Delete thread
    delete processingThread;
} // deleteProcessingThread()


void Controller::clearImageBuffer()
{
    imageBuffer->clearBuffer();
} // clearImageBuffer()

void Controller::deleteImageBuffer()
{
    // Delete image buffer
    delete imageBuffer;
} // deleteImageBuffer()

