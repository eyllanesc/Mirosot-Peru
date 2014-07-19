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
    bool playOn;
    bool bsOn;
    bool rgbOn;
    bool hsvOn;
    bool ycrcbOn;
    // Processing settings
    //Team
    int TeamColorType;
    cv::Scalar Teammin;
    cv::Scalar Teammax;
    //Robot1
    int Robot1ColorType;
    cv::Scalar Robot1min;
    cv::Scalar Robot1max;
    //Robot2
    int Robot2ColorType;
    cv::Scalar Robot2min;
    cv::Scalar Robot2max;
    //Ball
    int BallColorType;
    cv::Scalar Ballmin;
    cv::Scalar Ballmax;

    int BSNumberOfIterations;
    // Task data
    bool setROIFlag;
    bool resetROIFlag;
    cv::Rect selectionBox;

    cv::Point initial;
    cv::Point final;
    bool isPos;

protected:
    void run();
private slots:
    void updateProcessingFlags(struct ProcessingFlags);
    void updateProcessingSettings(struct ProcessingSettings);
    void updateTaskData(struct TaskData);
    void updatePosData(struct PosData);
signals:
    void newFrame(const QImage &frame);
    void newData(const QString data);
};

#endif // PROCESSINGTHREAD_H
