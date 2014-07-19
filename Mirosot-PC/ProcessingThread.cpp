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
    isPos=false;
    initial.x=-10;
    initial.y=-10;
    final.x=-10;
    final.y=-10;
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
    float angle=0;
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

        line(currentFrameCopy, initial, final, CV_RGB(255, 255, 0), 2, CV_AA);
        circle(currentFrameCopy , initial, 3, CV_RGB(255, 0, 255), 2, CV_AA);
        circle(currentFrameCopy , final, 3, CV_RGB(0, 255, 255), 2, CV_AA);
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
            //if(colorOn)
            //{
                /*switch (ColorType) {
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
                }*/
            //}
        }
        ////////////////////////////////////
        // PERFORM IMAGE PROCESSING ABOVE //
        ////////////////////////////////////

        //// Convert Mat to QImage: Show grayscale frame [if either Grayscale or Canny processing modes are ON]
        /*if(bsOn && !colorOn)
        {
          //  if(BSNumberOfIterations)
              //  frame=MatToQImage(currentFrameCopy);
            //else
                //frame=MatToQImage(fore);
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
*/
        updateMembersMutex.unlock();

        // Update statistics
        updateFPS(processingTime);
        currentSizeOfBuffer=imageBuffer->getSizeOfImageBuffer();
        if(isPos)
        {
            angle=cv::fastAtan2(final.y-initial.y,final.x-initial.x);
            data=QString::number(angle-180);
            emit newData(data);
        }
        emit newFrame(frame);

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
    playOn=processingFlags.playOn;
    bsOn=processingFlags.bsOn;
    rgbOn=processingFlags.rgbOn;
    hsvOn=processingFlags.hsvOn;
    ycrcbOn=processingFlags.ycrcbOn;

} // updateProcessingFlags()

void ProcessingThread::updateProcessingSettings(struct ProcessingSettings processingSettings)
{
    QMutexLocker locker(&updateMembersMutex);

    Teammin=cv::Scalar(processingSettings.TeamChannel1min,processingSettings.TeamChannel2min,processingSettings.TeamChannel3min);
    Teammax=cv::Scalar(processingSettings.TeamChannel1max,processingSettings.TeamChannel2max,processingSettings.TeamChannel3max);
    Robot1min=cv::Scalar(processingSettings.Robot1Channel1min,processingSettings.Robot1Channel2min,processingSettings.Robot1Channel3min);
    Robot1max=cv::Scalar(processingSettings.Robot1Channel1max,processingSettings.Robot1Channel2max,processingSettings.Robot1Channel3max);
    Robot2min=cv::Scalar(processingSettings.Robot2Channel1min,processingSettings.Robot2Channel2min,processingSettings.Robot2Channel3min);
    Robot2max=cv::Scalar(processingSettings.Robot2Channel1max,processingSettings.Robot2Channel2max,processingSettings.Robot2Channel3max);
    Ballmin=cv::Scalar(processingSettings.BallChannel1min,processingSettings.BallChannel2min,processingSettings.BallChannel3min);
    Ballmax=cv::Scalar(processingSettings.BallChannel1max,processingSettings.BallChannel2max,processingSettings.BallChannel3max);

    BSNumberOfIterations=processingSettings.BSNumberOfIterations;
} // updateProcessingSettings()

void ProcessingThread::updatePosData(struct PosData posData)
{
    QMutexLocker locker(&updateMembersMutex);
    initial.x=posData.initial.x();
    initial.y=posData.initial.y();
    final.x=posData.final.x();
    final.y=posData.final.y();
    isPos=true;
}

void ProcessingThread::updateTaskData(struct TaskData taskData)
{
    QMutexLocker locker(&updateMembersMutex);
    setROIFlag=taskData.setROIFlag;
    resetROIFlag=taskData.resetROIFlag;
    selectionBox.x=taskData.selectionBox.left();
    //qDebug()<<taskData.selectionBox.left();
    //qDebug()<<taskData.selectionBox.right();
    //qDebug()<<taskData.selectionBox.bottom();
    //qDebug()<<taskData.selectionBox.top();
    selectionBox.y=taskData.selectionBox.top();
    selectionBox.width=taskData.selectionBox.width();
    selectionBox.height=taskData.selectionBox.height();
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
