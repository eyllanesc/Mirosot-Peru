/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef MATTOQIMAGE_H
#define MATTOQIMAGE_H

// Qt header files
#include <QtGui>
#include <QDebug>

// OpenCV header files
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QImage MatToQImage(const cv::Mat&);

#endif // MATTOQIMAGE_H
