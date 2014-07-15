/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho   <e.yllanescucho@ieee.org>   */
/*                                  <e.yllanescucho@gmail.com>  */
/****************************************************************/

#include "ProcessingThread.h"

ProcessingThread::ProcessingThread(ImageBuffer *imageBuffer, int inputSourceWidth, int inputSourceHeight): QThread(),  imageBuffer(imageBuffer), inputSourceWidth(inputSourceWidth), inputSourceHeight(inputSourceHeight)
{
    // Create images
    currentFrameCopy.create(cv::Size(inputSourceWidth,inputSourceHeight),CV_8UC3);
    currentFrameCopybin.create(cv::Size(inputSourceWidth,inputSourceHeight),CV_8UC1);
    // Initialize variables
    stopped=false;
    sampleNo=0;
    fpsSum=0;
    avgFPS=0;
    fps.clear();
    // Initialize currentROI variable
    currentROI=cv::Rect(0,0,inputSourceWidth,inputSourceHeight);
    // Store original ROI
    originalROI=currentROI;
} // ProcessingThread constructor

ProcessingThread::~ProcessingThread()
{
} // ProcessingThread destructor

void ProcessingThread::run()
{
    cv::BackgroundSubtractorMOG2 bg;
    bg.set("nmixtures",5);
    bg.set("detectShadows",false);
    cv::Mat fore;
    cv::Mat foreandcolor;
    int mynumber=0;
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

        // Save processing time
        processingTime=t.elapsed();
        // Start timer (used to calculate processing rate)
        t.start();
        // Get frame from queue
        cv::Mat currentFrame=imageBuffer->getFrame();
        // Make copy of current frame (processing will be performed on this copy)
        currentFrame.copyTo(currentFrameCopy);
        // Set ROI of currentFrameCopy
        //currentFrameCopy.locateROI(frameSize,framePoint);
        //currentFrameCopy.adjustROI(-currentROI.y,-(frameSize.height-currentROI.height-currentROI.y),                           -currentROI.x,-(frameSize.width-currentROI.width-currentROI.x));

        updateMembersMutex.lock();
        ///////////////////
        // PERFORM TASKS //
        ///////////////////
        if(resetROIFlag)
            resetROI();
        else if(setROIFlag)
            setROI();
        ////////////////////////////////////
        // PERFORM IMAGE PROCESSING BELOW //
        ////////////////////////////////////
        else
        {
            if(bsOn)
            {
                if(BSNumberOfIterations)
                {
                    bg.operator ()(currentFrameCopy,fore);
                    BSNumberOfIterations--;
                }
                else
                {
                    bg.operator()(currentFrameCopy,fore,0);
                }
            }
            if(colorOn)
            {
                switch (ColorType) {
                case 0:
                    cv::inRange(currentFrameCopy,
                                cv::Scalar(ColorParam1,ColorParam2,ColorParam3),
                                cv::Scalar(ColorParam1_2,ColorParam2_2,ColorParam3_2),
                                currentFrameCopybin);
                    break;
                case 1:
                    cv::cvtColor(currentFrameCopy,
                                 currentFrameCopy,
                                 cv::COLOR_RGB2HSV);
                    cv::inRange(currentFrameCopy,
                                cv::Scalar(ColorParam1,ColorParam2,ColorParam3),
                                cv::Scalar(ColorParam1_2,ColorParam2_2,ColorParam3_2),
                                currentFrameCopybin);
                    break;
                case 2:
                    cv::cvtColor(currentFrameCopy,
                                 currentFrameCopy,
                                 cv::COLOR_RGB2YCrCb);
                    cv::inRange(currentFrameCopy,
                                cv::Scalar(ColorParam1,ColorParam2,ColorParam3),
                                cv::Scalar(ColorParam1_2,ColorParam2_2,ColorParam3_2),
                                currentFrameCopybin);
                default:
                    break;
                }
            }
        }
        ////////////////////////////////////
        // PERFORM IMAGE PROCESSING ABOVE //
        ////////////////////////////////////

        //// Convert Mat to QImage: Show grayscale frame [if either Grayscale or Canny processing modes are ON]
        if(bsOn && !colorOn)
        {
            if(BSNumberOfIterations)
                frame=MatToQImage(currentFrameCopy);
            else
                frame=MatToQImage(fore);
        }
        else if(!bsOn && colorOn)
            frame=MatToQImage(currentFrameCopybin);
        else if(bsOn && colorOn)
        {
            if(!BSNumberOfIterations)
            {
                bitwise_and(fore,currentFrameCopybin,foreandcolor);
                frame=MatToQImage(foreandcolor);
            }
            else
                frame=MatToQImage(currentFrameCopybin);
        }
        else
            frame=MatToQImage(currentFrameCopy);
        //// Convert Mat to QImage: Show BGR frame

        updateMembersMutex.unlock();

        // Update statistics
        updateFPS(processingTime);
        currentSizeOfBuffer=imageBuffer->getSizeOfImageBuffer();

        data=QString::number(mynumber);
        mynumber++;
        // Inform controller of new frame (QImage)
        emit newFrame(frame);
        emit newData(data);
    }
    qDebug() << "Stopping processing thread...";
}

void ProcessingThread::updateFPS(int timeElapsed)
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

void ProcessingThread::stopProcessingThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
} // stopProcessingThread()

void ProcessingThread::setROI()
{
    // Save selection as new (current) ROI
    currentROI=selectionBox;
    qDebug() << "ROI successfully SET.";
    // Reset setROIOn flag to FALSE
    setROIFlag=false;
} // setROI()

void ProcessingThread::resetROI()
{
    // Reset ROI to original
    currentROI=originalROI;
    qDebug() << "ROI successfully RESET.";
    // Reset resetROIOn flag to FALSE
    resetROIFlag=false;
} // resetROI()

void ProcessingThread::updateProcessingFlags(struct ProcessingFlags processingFlags)
{
    QMutexLocker locker(&updateMembersMutex);
    this->bsOn=processingFlags.bsOn;
    this->colorOn=processingFlags.colorOn;
} // updateProcessingFlags()

void ProcessingThread::updateProcessingSettings(struct ProcessingSettings processingSettings)
{
    QMutexLocker locker(&updateMembersMutex);
    this->ColorType=processingSettings.ColorType;
    this->ColorParam1 = processingSettings.ColorParam1;
    this->ColorParam2 = processingSettings.ColorParam2;
    this->ColorParam3 = processingSettings.ColorParam3;
    this->ColorParam1_2 = processingSettings.ColorParam1_2;
    this->ColorParam2_2 = processingSettings.ColorParam2_2;
    this->ColorParam3_2 = processingSettings.ColorParam3_2;
    this->BSNumberOfIterations=processingSettings.BSNumberOfIterations;
} // updateProcessingSettings()

void ProcessingThread::updateTaskData(struct TaskData taskData)
{
    QMutexLocker locker(&updateMembersMutex);
    this->setROIFlag=taskData.setROIFlag;
    this->resetROIFlag=taskData.resetROIFlag;
    this->selectionBox.x=taskData.selectionBox.left();
    qDebug()<<taskData.selectionBox.left();
    qDebug()<<taskData.selectionBox.right();
    qDebug()<<taskData.selectionBox.bottom();
    qDebug()<<taskData.selectionBox.top();
    this->selectionBox.y=taskData.selectionBox.top();
    this->selectionBox.width=taskData.selectionBox.width();
    this->selectionBox.height=taskData.selectionBox.height();
} // updateTaskData()

int ProcessingThread::getAvgFPS()
{
    return avgFPS;
} // getAvgFPS()

int ProcessingThread::getCurrentSizeOfBuffer()
{
    return currentSizeOfBuffer;
} // getCurrentSizeOfBuffer()

cv::Rect ProcessingThread::getCurrentROI()
{
    return currentROI;
} // getCurrentROI();
