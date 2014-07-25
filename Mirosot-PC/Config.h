/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <opencv2/opencv.hpp>
// Image buffer size
#define DEFAULT_IMAGE_BUFFER_SIZE 1

// COLOR
#define DEFAULT_COLOR_TYPE 0 // Options: [RGB=0,HSV=1,YCrCb=2]
#define DEFAULT_COLOR_PARAM_1 0
#define DEFAULT_COLOR_PARAM_2 0
#define DEFAULT_COLOR_PARAM_3 0
#define DEFAULT_COLOR_PARAM_1_2 255
#define DEFAULT_COLOR_PARAM_2_2 255
#define DEFAULT_COLOR_PARAM_3_2 255

// BS
#define DEFAULT_BS_ITERATIONS 100
#define AREAMIN_BS  50
#define AREAMAX_BS  500

#define DEFAULT_SMOOTH_ITERATIONS 1

#define LAMBDA 3.5

#define NFLIP    2
#define XFLIP   0
#define YFLIP   1
#define BFLIP   -1

#endif // CONFIG_H
