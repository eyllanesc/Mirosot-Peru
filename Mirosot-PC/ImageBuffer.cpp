/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "ImageBuffer.h"

ImageBuffer::ImageBuffer(int bufferSize) : bufferSize(bufferSize)
{
    // Semaphore initializations
    freeSlots = new QSemaphore(bufferSize);
    usedSlots = new QSemaphore(0);
    clearBuffer1 = new QSemaphore(1);
    clearBuffer2 = new QSemaphore(1);
} // ImageBuffer constructor

void ImageBuffer::addFrame(const cv::Mat& frame)
{
    // Acquire semaphores
    clearBuffer1->acquire();
    freeSlots->acquire();
    // Add frame to queue
    imageQueueProtect.lock();
        imageQueue.enqueue(frame);
    imageQueueProtect.unlock();
    // Release semaphores
    clearBuffer1->release();
    usedSlots->release();
} // addFrame()

cv::Mat ImageBuffer::getFrame()
{
    // Acquire semaphores
    clearBuffer2->acquire();
    usedSlots->acquire();
    // Temporary data
    cv::Mat tempFrame;
    // Take frame from queue
    imageQueueProtect.lock();
        tempFrame=imageQueue.dequeue();
    imageQueueProtect.unlock();
    // Release semaphores
    freeSlots->release();
    clearBuffer2->release();
    // Return frame to caller
    return tempFrame;
} // getFrame()

void ImageBuffer::clearBuffer()
{
    // Check if buffer is not empty
    if(imageQueue.size()!=0)
    {
        // Stop adding frames to buffer
        clearBuffer1->acquire();
        // Stop taking frames from buffer
        clearBuffer2->acquire();
        // Release all remaining slots in queue
        freeSlots->release(imageQueue.size());
        // Acquire all queue slots
        freeSlots->acquire(bufferSize);
        // Reset usedSlots to zero
        usedSlots->acquire(imageQueue.size());
        // Clear buffer by dequeuing items
        while(imageQueue.size()!=0)
        {
            // Temporary data
            cv::Mat temp;
            // Dequeue frame
            temp=imageQueue.dequeue();
            // Release frame
            temp.release();
        }
        // Release all slots
        freeSlots->release(bufferSize);
        // Allow getFrame() to resume
        clearBuffer2->release();
        // Allow addFrame() to resume
        clearBuffer1->release();
        qDebug() << "Image buffer successfully cleared.";
    }
    else
        qDebug() << "WARNING: Could not clear image buffer: already empty.";
} // clearBuffer()

int ImageBuffer::getSizeOfImageBuffer()
{
    return imageQueue.size();
} // getSizeOfImageBuffer()
