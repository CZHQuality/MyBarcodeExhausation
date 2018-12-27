#include "Exhaustion.h"

using namespace cv;
using namespace std;


extern int ZXingBW(InputArray _src, OutputArray _dst, locateRect* BWpointer){
    Mat img_src = _src.getMat();
    //_dst.create(img_src.size(), img_src.type());
    //Mat img_dst = _dst.getMat();
    
    Mat img_gray;
    cvtColor(img_src, img_gray, CV_RGB2GRAY);//?????????RGB or GRAY????????????

    //_dst.create(img_gray.size(), img_gray.type());
    //Mat img_dst = _dst.getMat();

    int scale = 50;
    int width = img_gray.cols;
    int height = img_gray.rows;
    int minumum = 0;
    if(width>height){
        minumum = height;
    }else{
        minumum = width;
    }
    if(minumum <= scale)
    {
        cout<<"QZXing binary Error : The scale of the barcode is too small to use this binarization method !!!!"<<endl;
        return 0;
    }
    Mat img_gray2, img_gray3;
    PadarraySym(img_gray, img_gray2, scale);//padded
    //imshow("source image", img_gray);
    ColorInversion(img_gray2, img_gray3);//this method is suitable for the barcode with black code and white background
    //imshow("inversed image", img_gray);
    Mat img_dst, img_dst2;
    getBlackMatrix(img_gray3, img_dst);
    CroppingResult(img_dst, img_dst2, scale);
   
    //imshow("mainfunction", img_dst2);imwrite("/home/chezhaohui/桌面/DataBase/BWresults/ZXing_bwneibu.bmp", img_dst2);waitKey();
    //cvtColor(img_dst2, img_dst2, CV_GRAY2RGB);
    _dst.create(img_dst2.size(), img_dst2.type());
    //cout<<"The image info :"<<img_dst2.type()<<"The channel is :"<<img_dst2.channels()<<"width"<<img_dst2.cols<<endl;
    Mat img_dst3 = _dst.getMat();

    Mat img_dst4;
    EdgeCleanBW(img_dst2, img_dst4, BWpointer);

    for(int y=0; y<img_dst3.rows; y++){
        uchar* head_row_3 = img_dst3.ptr<uchar> (y);
        uchar* head_row_4 = img_dst4.ptr<uchar> (y);
        for(int x=0; x<img_dst3.cols; x++){
            head_row_3[x] = head_row_4[x];

        }
    }
    
    
    


    //imwrite ("/home/chezhaohui/桌面/DataBase/patch0725/QZxingBWresult/temp.bmp",img_dst);
    
    //Mat temp_dst = img_dst.clone();
    //EdgeCleanBW(temp_dst, img_dst, BWpointer);
    return 0;
    
}



extern void getBlackMatrix(InputArray _input, OutputArray _dst) 
{
    Mat img = _input.getMat();
    int width = img.cols;
    int height = img.rows;
    //cout<<"width is :"<<width<<endl;
    unsigned char* luminances = img.data; 
    //cout<<"The channels :"<<img.channels()<<endl;
    //cout<<"The img step is :"<<img.step<<endl;
    //cout<<"The source pixels is :"<<endl;//step = img.cols * img.channels()
    
    _dst.create(img.size(), img.type());
    Mat img_dst = _dst.getMat();

    int subWidth = width >> BLOCK_SIZE_POWER;// 1/8
    if ((width & BLOCK_SIZE_MASK) != 0) {
      subWidth++;
    }
    //cout<<"subWidth is :"<<subWidth<<endl;//18
    int subHeight = height >> BLOCK_SIZE_POWER;
    if ((height & BLOCK_SIZE_MASK) != 0) {
      subHeight++;
    }
    //cout<<"subHeight is :"<<subHeight<<endl;//18

    int* blackPoints = calculateBlackPoints(luminances, subWidth, subHeight, width, height);          
    
    calculateThresholdForBlock(luminances, subWidth, subHeight, width, height, blackPoints, img_dst.data);
    
  
}


extern int* calculateBlackPoints(unsigned char* luminances, int subWidth, int subHeight, int width, int height)
{
    //int* blackPoints = new int[subWidth*subHeight];//如果用new还得用delete删除内存 比较费时
    int _Scale = subWidth * subHeight;
    int Patch[_Scale];
    for(int i=0; i<_Scale; i++)
    {
      Patch[i]=0;
    }
    int* blackPoints = Patch;
    const int minDynamicRange = 24;
    //blackPoints = (int *)malloc(subWidth*subHeight*sizeof(*blackPoints));

    for (int y = 0; y < subHeight; y++) 
    {
    int yoffset = y << BLOCK_SIZE_POWER;// 8
    int maxYOffset = height - BLOCK_SIZE;
    if (yoffset > maxYOffset) 
    {
      yoffset = maxYOffset;
    }
    for (int x = 0; x < subWidth; x++) {
      int xoffset = x << BLOCK_SIZE_POWER;
      int maxXOffset = width - BLOCK_SIZE;
      if (xoffset > maxXOffset) {
        xoffset = maxXOffset;
      }
      int sum = 0;
      int min = 0xFF;
      int max = 0;
      for (int yy = 0, offset = yoffset * width + xoffset;
           yy < BLOCK_SIZE;
           yy++, offset += width) {
        for (int xx = 0; xx < BLOCK_SIZE; xx++) {
          int pixel = luminances[offset + xx] & 0xFF;
          sum += pixel;
          // still looking for good contrast
          if (pixel < min) {
            min = pixel;
          }
          if (pixel > max) {
            max = pixel;
          }
        }

        // short-circuit min/max tests once dynamic range is met
        if (max - min > minDynamicRange) {
          // finish the rest of the rows quickly
          for (yy++, offset += width; yy < BLOCK_SIZE; yy++, offset += width) {
            for (int xx = 0; xx < BLOCK_SIZE; xx += 2) {
              sum += luminances[offset + xx] & 0xFF;
              sum += luminances[offset + xx + 1] & 0xFF;
            }
          }
        }
      }
      // See
      // http://groups.google.com/group/zxing/browse_thread/thread/d06efa2c35a7ddc0
      int average = sum >> (BLOCK_SIZE_POWER * 2);
      if (max - min <= minDynamicRange) {
        average = min >> 1;
        if (y > 0 && x > 0) {
          int bp = getBlackPointFromNeighbors(blackPoints, subWidth, x, y);
          if (min < bp) {
            average = bp;
          }
        }
      }
      blackPoints[y * subWidth + x] = average;
    }
  }
  return blackPoints;
}


extern void calculateThresholdForBlock(uchar* luminances, int subWidth, int subHeight, int width, int height, int* blackPoints, uchar* matrix_p)
{                                            
  for (int y = 0; y < subHeight; y++) {
    int yoffset = y << BLOCK_SIZE_POWER;
    int maxYOffset = height - BLOCK_SIZE;
    if (yoffset > maxYOffset) {
      yoffset = maxYOffset;
    }
    for (int x = 0; x < subWidth; x++) {
      int xoffset = x << BLOCK_SIZE_POWER;
      int maxXOffset = width - BLOCK_SIZE;
      if (xoffset > maxXOffset) {
        xoffset = maxXOffset;
      }
      int left = cap(x, 2, subWidth - 3);
      int top = cap(y, 2, subHeight - 3);
      int sum = 0;
      for (int z = -2; z <= 2; z++) {
        int *blackRow = &blackPoints[(top + z) * subWidth];
        sum += blackRow[left - 2];
        sum += blackRow[left - 1];
        sum += blackRow[left];
        sum += blackRow[left + 1];
        sum += blackRow[left + 2];
      }
      int average = sum / 25;
      thresholdBlock(luminances, xoffset, yoffset, average, width, matrix_p, width);
    }
  }
}

extern void thresholdBlock(uchar* luminances, int xoffset, int yoffset, int threshold, int stride, uchar* matrix_p, int width)
{                                    
  for (int y = 0, offset = yoffset * stride + xoffset;
       y < BLOCK_SIZE;
       y++,  offset += stride) {
    for (int x = 0; x < BLOCK_SIZE; x++) {
      int pixel = luminances[offset + x] & 0xff;
      if (pixel <= threshold) {
        //matrix->set(xoffset + x, yoffset + y);
        matrix_p[(yoffset+y)*width+(xoffset+x)] = 255;
      }
    }
  }
}

/*
  void set(int x, int y) {
    int offset = y * rowSize + (x >> logBits);
    bits[offset] |= 1 << (x & bitsMask);
  }
  */
extern void ColorInversion(InputArray _src, OutputArray _dst)//just for gray images
{
    Mat img_src = _src.getMat();
    _dst.create(img_src.size(), img_src.type());
    Mat img_dst = _dst.getMat();
    uchar* a = img_src.data;
    uchar* b = img_dst.data;
    int width = img_src.cols;
    int height = img_src.rows;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            b[i*width+j] = 255 - a[i*width+j];

        }
    }



}

extern void PadarraySym(InputArray _src, OutputArray _dst, int scale)//模仿matlab中的padarray函数 用symmetic的方式进行填充,且方式为post  just for grayscale images
{
    Mat img_src = _src.getMat();
    _dst.create(img_src.rows+scale, img_src.cols+scale, img_src.type());
    Mat img_dst = _dst.getMat();
    img_src.copyTo(img_dst(Rect(0,0,img_src.cols,img_src.rows)));

    Mat img_ROI_1 = img_src(Rect(img_src.cols-scale,0,scale,img_src.rows));
    img_ROI_1.copyTo(img_dst(Rect(img_src.cols,0,scale,img_src.rows)));

    Mat img_ROI_2 = img_src(Rect(0,img_src.rows-scale,img_src.cols,scale));
    img_ROI_2.copyTo(img_dst(Rect(0,img_src.rows,img_src.cols,scale)));

    Mat img_ROI_3 = img_src(Rect(img_src.cols-scale,img_src.rows-scale,scale,scale));
    img_ROI_3.copyTo(img_dst(Rect(img_src.cols,img_src.rows,scale,scale)));

}

extern void CroppingResult(InputArray _src, OutputArray _dst, int scale)
{
    Mat img_src = _src.getMat();
    _dst.create(img_src.rows-scale, img_src.cols-scale, img_src.type());
    Mat img_dst = _dst.getMat();
    Mat img_ROI = img_src(Rect(0,0,img_src.cols-scale,img_src.rows-scale));
    img_ROI.copyTo(img_dst);

}                                        

extern void EdgeCleanBW(InputArray _src, OutputArray _dst, locateRect* BWpointer)
{
    Mat img_src = _src.getMat();
    _dst.create(img_src.size(), img_src.type());
    Mat img_dst = _dst.getMat();
    
    int width = img_dst.cols;
    int height = img_dst.rows;
    
    for(int y=0; y<BWpointer->Ycoordi; y++){
        uchar* head_row = img_src.ptr<uchar>(y);
        for(int x=0; x<width; x++){
            head_row[x] = 0;
        }
    }
    for(int y2=BWpointer->Ycoordi + BWpointer->Ylength; y2<height; y2++){
        uchar* head_row2 = img_src.ptr<uchar>(y2);
        for(int x2=0; x2<width; x2++){
            head_row2[x2] = 0;
        }
    }
    for(int y3=BWpointer->Ycoordi; y3<BWpointer->Ycoordi+BWpointer->Ylength; y3++){
        uchar* head_row3 = img_src.ptr<uchar>(y3);
        for(int x3=0; x3<BWpointer->Xcoordi; x3++){
            head_row3[x3] = 0;
        }
    }
    for(int y4=BWpointer->Ycoordi; y4<BWpointer->Ycoordi+BWpointer->Ylength; y4++){
        uchar* head_row4 = img_src.ptr<uchar>(y4);
        for(int x4=BWpointer->Xcoordi+BWpointer->Xlength; x4<img_src.cols; x4++){
            head_row4[x4] = 0;
        }
    }


        for(int y5=0; y5<img_src.rows; y5++){
        uchar* head_row_src = img_src.ptr<uchar> (y5);
        uchar* head_row_dst = img_dst.ptr<uchar> (y5);
        for(int x5=0; x5<img_src.cols; x5++){
            head_row_dst[x5] = head_row_src[x5];

        }
    }

    
}
