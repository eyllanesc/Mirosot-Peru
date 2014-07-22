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
    ProcessingSettings psettings;
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
    cv::Mat currentFrameCopy2;
    cv::Rect originalROI;
    cv::Rect currentROI;

    cv::Mat team;
    cv::Mat teambin;
    cv::Mat Robot1;
    cv::Mat Robot1bin;

    cv::Rect campROI;

    cv::Scalar meantmp;
    cv::Scalar stdtmp;

    QString data;
    QImage frame;
    QImage frame1;
    QImage hist;
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

    cv::Size temp;
    // Processing flags
    bool playOn;
    bool bsOn;
    bool campOn;
    bool teamOn;
    bool robot1On;
    bool robot2On;
    bool ballOn;
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
    void newFrame2(const QImage &frame);
    void newData(const QString data);
    void newProcessingSettings(struct ProcessingSettings p_settings);
    void newHistogram(const QImage &hist);
};

#endif // PROCESSINGTHREAD_H
