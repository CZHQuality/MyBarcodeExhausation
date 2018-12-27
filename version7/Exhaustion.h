#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <dmtx.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#define QR_MAXI(_a,_b)      ((_a)-((_a)-(_b)&-((_b)>(_a))))
#define QR_MINI(_a,_b)      ((_a)+((_b)-(_a)&-((_b)<(_a))))


typedef enum{
    FilterForbiden,
    FilterSmooth,
    FilterErode,
    FilterDilate,
    FliterClose,
    FilterOpen,
    FilterMedian,
    FilterSharp,
    FilterGIF
}FilterType;

typedef struct locateRect_struct{
    int Xcoordi;
    int Ycoordi;
    int Xlength;
    int Ylength;
}locateRect;

namespace {
  const int BLOCK_SIZE_POWER = 3;
  const int BLOCK_SIZE = 1 << BLOCK_SIZE_POWER; // ...0100...00
  const int BLOCK_SIZE_MASK = BLOCK_SIZE - 1;   // ...0011...11
  const int MINIMUM_DIMENSION = BLOCK_SIZE * 5; // 40
}//ZXingBW

namespace 
{
  inline int getBlackPointFromNeighbors(int* blackPoints, int subWidth, int x, int y) {
    return (blackPoints[(y-1)*subWidth+x] +
            2*blackPoints[y*subWidth+x-1] +
            blackPoints[(y-1)*subWidth+x-1]) >> 2;
  }
}//ZXingBW

namespace 
{
  inline int cap(int value, int min, int max) 
  {
    return value < min ? min : value > max ? max : value;
  }
}//ZXingBW   

using namespace std;
using namespace cv;

extern int Recog(cv::InputArray input, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Adj (cv::InputArray input, int FilterKnown, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Smooth(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Erode(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Dilate(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Open(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Close(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_Median(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern int Filter_GIF (cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer);
extern unsigned char *qr_binarize(const unsigned char *_img, int _width, int _height, locateRect* BWpointer);

extern void getBlackMatrix(InputArray _input, OutputArray _dst);
extern int* calculateBlackPoints(unsigned char* luminances, int subWidth, int subHeight, int width, int height);
extern void calculateThresholdForBlock(uchar* luminances, int subWidth, int subHeight, int width, int height, int* blackPoints, uchar* matrix_p);
extern void thresholdBlock(uchar* luminances, int xoffset, int yoffset, int threshold, int stride, uchar* matrix_p, int width);
extern void ColorInversion(InputArray _src, OutputArray _dst);
extern void PadarraySym(InputArray _src, OutputArray _dst, int scale);
extern void CroppingResult(InputArray _src, OutputArray _dst, int scale);
extern int ZXingBW(InputArray _src, OutputArray _dst, locateRect* BWpointer);
extern void EdgeCleanBW(InputArray _src, OutputArray _dst, locateRect* BWpointer);

extern void PadarrayZeroBoth(InputArray _src, OutputArray _dst, int scale);
extern int ImageMean(InputArray _src);
extern void YangBW(InputArray _src, OutputArray _dst, locateRect* BWpointer);