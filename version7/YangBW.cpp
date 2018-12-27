#include "Exhaustion.h"
#include <fstream>//delete
using namespace std;
using namespace cv;


extern void YangBW(InputArray _src, OutputArray _dst, locateRect* BWpointer)
{
    Mat img_srcc = _src.getMat();
    Mat img_src;
    img_srcc.convertTo(img_src, CV_8U);
    //imshow("imgSrc",img_src);imwrite("/home/chezhaohui/桌面/DataBase/BWresults/SrcTest.bmp",img_src);//delete
    /*
    ofstream file("lenaB.txt", ios::out);//delete
    for(int i=0; i<img_src.rows; i++){
        uchar* head_row = img_src.ptr<uchar>(i);
        for(int j=0; j<img_src.step; j=j+img_src.channels()){ //img.step !!!!!!!!!!   +3 !!!!!!!!!!!
            file<<(int)head_row[j]<<'\t';
        }
        file<<endl;
    }
    file.close();//delete
    */


    Mat img_ROI = img_src(Rect(BWpointer->Xcoordi, BWpointer->Ycoordi, BWpointer->Xlength, BWpointer->Ylength));
    
    Mat img_gray_ori;
    cvtColor(img_ROI, img_gray_ori, CV_RGB2GRAY);//把图片转化为灰度图
    
    Mat img_gray;
    cvtColor(img_ROI, img_gray, CV_RGB2GRAY);//把图片转化为灰度图
    Mat img_edge;
    /*
    imshow("imgGray",img_gray);imwrite("/home/chezhaohui/桌面/DataBase/BWresults/CannyTest.bmp",img_gray);//delete
    ofstream file2("lenaC.txt", ios::out);//delete
    for(int ii=0; ii<img_gray.rows; ii++){
        uchar* head_row2 = img_gray.ptr<uchar>(ii);
        for(int jj=0; jj<img_gray.step; jj=jj+img_gray.channels()){ //img.step !!!!!!!!!!   +3 !!!!!!!!!!!
            file2<<(int)head_row2[jj]<<'\t';
        }
        file2<<endl;
    }
    file2.close();
    waitKey(0);
    */
    Canny(img_gray, img_edge, 150, 100, 3);
    //imshow("CannyResult", img_edge);
    
    int barcode_size = 14 - 1;//should be optimized as an auto value
    int S_m = ceil (1.5 * (img_gray.rows / barcode_size));// maximum search window size

    PadarrayZeroBoth(img_gray, img_gray, S_m*6);//pad size is big enough to make sure the memory right
    PadarrayZeroBoth(img_edge, img_edge, S_m*6);
    //cout<<"The original value of D_m is : "<<img_gray.rows<<endl;
    Mat BW_index;
    BW_index.create(img_gray.size(), img_gray.type());
    BW_index.setTo(cv::Scalar::all(150));//150 means the pixels are not binarized yet

    //imshow("Padded Gray",img_gray);imshow("Padded Edge",img_edge);
    //int mean = ImageMean(img_gray);cout<<"The mean is:"<<mean<<endl;

    for(int i=S_m; i<img_gray.rows-S_m; i++)
    //for(int i=6*S_m; i<6*S_m+15; i++)
    {
        uchar* head_row_1 = img_gray.ptr<uchar> (i);
        uchar* head_row_2 = img_edge.ptr<uchar> (i);
        uchar* head_row_3 = BW_index.ptr<uchar> (i);
        for(int j=S_m; j<img_gray.cols-S_m; j++)
        //for(int j=6*S_m; j<6*S_m+15; j++)
        {
            int gray_pixel = (int)head_row_1[j];
            int edge_pixel = (int)head_row_2[j];
            int bw_pixel = (int)head_row_3[j];

            if(bw_pixel == 150)
            {
            Mat patch_edge;
            int D_m = img_gray.rows;//minumum pixel-to-edge distance
            int D_pe = 0;//pixel-to-edge distance
            int center_row_offset = 0;
            int center_col_offset = 0;
            int center_row = 0;
            int center_col = 0;
            int W_b;//binarize window radius
            patch_edge = img_edge(Rect(j-S_m, i-S_m, 2*S_m, 2*S_m));

            for(int ii=0; ii<patch_edge.rows; ii++)//find out the nearest edge pixel of the pending pixel
            {
                uchar* head_row_patch = patch_edge.ptr<uchar>(ii);
                for(int jj=0; jj<patch_edge.cols; jj++)
                {
                    int patch_pixel = (int)head_row_patch[jj];
                    if(patch_pixel == 255)
                    {
                        D_pe = (int)(sqrt( (ii - S_m)*(ii-S_m) + (jj-S_m)*(jj-S_m) )); 
                        if(D_pe < D_m)
                        {
                            //cout<<"The ii is :"<<ii<<'\t'<<"The jj is :"<<jj<<'\t'<<"The S_m is :"<<S_m<<endl;
                            //cout<<"The D_pe is :"<<D_pe<<endl;
                            D_m = D_pe;
                            center_row_offset = ii-S_m;
                            center_col_offset = jj-S_m;
                        }  
                    }
                }
            }
            //cout<<"The mimumum pixel-to-edge distance is :"<<D_m<<endl;
            //cout<<"offset row is :"<<center_row_offset<<'\t'<<"offset col is :"<<center_col_offset<<endl;
            center_row = i + center_row_offset;
            center_col = j + center_col_offset;
            if(D_m == 0 || D_m == img_gray.rows)//0 means the pixel is edge, img_gray.rows means there is no edge pixels in the biggest search windows
              W_b = 5;//I choose a common window size
            else
              W_b = (int)1.5 * D_m;

            Mat patch_gray = img_gray(Rect(center_col-W_b, center_row-W_b, 2*W_b, 2*W_b));
            int thresh = ImageMean(patch_gray);//binarize threshold 
            //cout<<"The thresh is:"<<thresh<<endl;
            Mat patch_bw = BW_index(Rect(center_col-W_b, center_row-W_b, 2*W_b, 2*W_b));
            for(int iii=0; iii<patch_gray.rows; iii++)
            {
               
                uchar* head_row_gray = patch_gray.ptr<uchar>(iii);
                uchar* head_row_bw = patch_bw.ptr<uchar>(iii);
                for(int jjj=0; jjj<patch_gray.cols; jjj++)
                {
                    //cout<<"Thresh is :"<<thresh<<endl;
                    //cout<<"gray value is :"<<head_row_gray[jjj]<<endl;
                    if(head_row_gray[jjj] < thresh)
                        head_row_bw[jjj] = 0;
                    else
                        head_row_bw[jjj] = 255;
                    
                }
            }
            patch_bw.copyTo(BW_index(Rect(center_col-W_b, center_row-W_b, 2*W_b, 2*W_b)));
            }


            //cout<<edge_pixel<<'\t';
        }
        //cout<<endl;
    }
    
    //Mat img_bw;
    //img_bw.create(img_gray_ori.size(), img_gray_ori.type());
    //img_bw.setTo(cv::Scalar::all(150));
    Mat img_bw = BW_index(Rect(S_m*6, S_m*6, img_gray_ori.cols, img_gray_ori.rows));
    for(int i_4=0; i_4<img_bw.rows; i_4++)
    {
        uchar* head_row_final = img_bw.ptr<uchar>(i_4);
        for(int j_4=0; j_4<img_bw.cols; j_4++)
        {
            if(head_row_final[j_4]==150){
                head_row_final[j_4] = 0;
            }
        }
    }
    
    PadarrayZeroBoth(img_bw, img_bw, S_m*2);

    _dst.create(img_bw.size(), img_bw.type());
    Mat img_dst3 = _dst.getMat();

     for(int y=0; y<img_dst3.rows; y++){
        uchar* head_row_3 = img_dst3.ptr<uchar> (y);
        uchar* head_row_4 = img_bw.ptr<uchar> (y);
        for(int x=0; x<img_dst3.cols; x++){
            head_row_3[x] = head_row_4[x];
        }
    }
  
    //imshow("Binarized Result",img_bw);  
    //imshow("Binarized Result2",img_dst3);  
    //waitKey();  
}


extern void PadarrayZeroBoth(InputArray _src, OutputArray _dst, int scale)//模仿matlab中的padarray函数 用0进行填充,且方式为both
{
    Mat img_src = _src.getMat();
    _dst.create(img_src.rows+2*scale, img_src.cols+2*scale, img_src.type());
    Mat img_dst = _dst.getMat();
    img_dst.setTo(cv::Scalar::all(0));
    img_src.copyTo(img_dst(Rect(scale, scale, img_src.cols, img_src.rows)));

}

extern int ImageMean(InputArray _src)
{
    Mat img_src = _src.getMat();
    int width = img_src.cols;
    int height = img_src.rows;
    int dims = img_src.channels();
    int stride = img_src.step;
    int mean = 0;
    int sum = 0;
    if(dims>1)
    {
        cvtColor(img_src, img_src, CV_RGB2GRAY);
    }
    for(int i=0; i<height; i++)
    {
        uchar* head_row = img_src.ptr<uchar>(i);
        for(int j=0; j<width; j++)
        {
            sum = sum + (int)head_row[j];
        }
    }
    mean = (int)(sum/(width*height));

    return mean;
}

//compile command : 
//g++ `pkg-config --cflags opencv` -o test main.o  `pkg-config --libs opencv`