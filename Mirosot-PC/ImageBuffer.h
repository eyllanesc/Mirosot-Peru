/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

// Qt header files
#include <QMutex>
#include <QQueue>
#include <QSemaphore>
#include <QDebug>

// OpenCV header files
#include <opencv2/highgui/highgui.hpp>

class ImageBuffer
{

public:
    ImageBuffer(int size);
    void addFrame(const cv::Mat& frame);
    cv::Mat getFrame();
    void clearBuffer();
    int getSizeOfImageBuffer();
private:
    QMutex imageQueueProtect;
    QQueue<cv::Mat> imageQueue;
    QSemaphore *freeSlots;
    QSemaphore *usedSlots;
    QSemaphore *clearBuffer1;
    QSemaphore *clearBuffer2;
    int bufferSize;
};

#endif // IMAGEBUFFER_H
