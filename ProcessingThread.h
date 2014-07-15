/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

//My header
#include "Structures.h"
#include "MatToQImage.h"
#include "Config.h"
#include "ImageBuffer.h"

// Qt header files
#include <QThread>
#include <QtGui>
#include <QDebug>

// OpenCV header files
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>

class ProcessingThread : public QThread
{
    Q_OBJECT

public:
    ProcessingThread(ImageBuffer* imageBuffer, int inputSourceWidth, int inputSourceHeight);
    ~ProcessingThread();
    void stopProcessingThread();
    int getAvgFPS();
    int getCurrentSizeOfBuffer();
    cv::Rect getCurrentROI();
private:
    void updateFPS(int);
    void setROI();
    void resetROI();
    ImageBuffer* imageBuffer;
    volatile bool stopped;
    int inputSourceWidth;
    int inputSourceHeight;
    int currentSizeOfBuffer;
    cv::Mat currentFrameCopy;
    cv::Mat currentFrameCopybin;
    cv::Rect originalROI;
    cv::Rect currentROI;
    QString data;
    QImage frame;
    QTime t;
    int processingTime;
    QQueue<int> fps;
    int fpsSum;
    int sampleNo;
    int avgFPS;
    QMutex stoppedMutex;
    QMutex updateMembersMutex;
    cv::Size frameSize;
    cv::Point framePoint;
    // Processing flags
    bool bsOn;
    bool colorOn;
    // Processing settings
    int ColorType;
    int ColorParam1;
    int ColorParam2;
    int ColorParam3;
    int ColorParam1_2;
    int ColorParam2_2;
    int ColorParam3_2;

    int BSNumberOfIterations;
    // Task data
    bool setROIFlag;
    bool resetROIFlag;
    cv::Rect selectionBox;
protected:
    void run();
private slots:
    void updateProcessingFlags(struct ProcessingFlags);
    void updateProcessingSettings(struct ProcessingSettings);
    void updateTaskData(struct TaskData);
signals:
    void newFrame(const QImage &frame);
    void newData(QString data);
};

#endif // PROCESSINGTHREAD_H
