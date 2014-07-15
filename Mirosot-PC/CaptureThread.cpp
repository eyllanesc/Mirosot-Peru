/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "CaptureThread.h"

CaptureThread::CaptureThread(ImageBuffer *imageBuffer) : QThread(), imageBuffer(imageBuffer)
{
    // Initialize variables
    stopped=false;
    sampleNo=0;
    fpsSum=0;
    avgFPS=0;
    fps.clear();
} // CaptureThread constructor

void CaptureThread::run()
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
        // Save capture time
        captureTime=t.elapsed();
        // Start timer (used to calculate capture rate)
        t.start();
        // Capture and add frame to buffer
        cap>>grabbedFrame;
        imageBuffer->addFrame(grabbedFrame);
        // Update statistics
        updateFPS(captureTime);
    }
    qDebug() << "Stopping capture thread...";
} // run()

bool CaptureThread::connectToCamera(int deviceNumber)
{
    // Open camera and return result
    return cap.open(deviceNumber);
} // connectToCamera()

void CaptureThread::disconnectCamera()
{
    // Check if camera is connected
    if(cap.isOpened())
    {
        // Disconnect camera
        cap.release();
        qDebug() << "Camera successfully disconnected.";
    }
} // disconnectCamera()

void CaptureThread::updateFPS(int timeElapsed)
{
    // Add instantaneous FPS value to queue
    if(timeElapsed>0)
    {
        fps.enqueue((int)1000/timeElapsed);
        // Increment sample number
        sampleNo++;
    }
    // Maximum size of queue is 16
    if(fps.size() > 16)
        fps.dequeue();
    // Update FPS value every 16 samples
    if((fps.size()==16)&&(sampleNo==16))
    {
        // Empty queue and store sum
        while(!fps.empty())
            fpsSum+=fps.dequeue();
        avgFPS=fpsSum/16; // Calculate average FPS
        fpsSum=0; // Reset sum
        sampleNo=0; // Reset sample number
    }
} // updateFPS()

void CaptureThread::stopCaptureThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
} // stopCaptureThread()

int CaptureThread::getAvgFPS()
{
    return avgFPS;
} // getAvgFPS()

bool CaptureThread::isCameraConnected()
{
    if(cap.isOpened())
        return true;
    else
        return false;
} // isCameraConnected()

int CaptureThread::getInputSourceWidth()
{
    return cap.get(CV_CAP_PROP_FRAME_WIDTH);
} // getInputSourceWidth()

int CaptureThread::getInputSourceHeight()
{
    return cap.get(CV_CAP_PROP_FRAME_HEIGHT);
} // getInputSourceHeight()
