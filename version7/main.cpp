#include"Exhaustion.h"
using namespace std;
using namespace cv;
                                                                                                                                                                                                                                                                  


int main()
{
    //int Recog(InputArray input); 

    Mat I_src, I_coarse;
    double QAscore[8][5];
    int PASS_flag = 0, FilterKnown = -1, Mode = 1;//Mode = 0:detect the location of the barcode; Mode = 1: Decode the extracted region; This Should be optimized since two times are time wasting
    //locateRect SmallReg;
    locateRect* SRpointer;
    locateRect* BWpointer;
    int width;
    int height;
    //I_src = imread("/home/chezhaohui/桌面/helloworld/timg.jpeg");
    //I_src = imread("/home/chezhaohui/桌面/Exhaustion/version5/testimage/19_0_ref_0.28.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/nonpro19.bmp");  
    //I_src = imread("/home/chezhaohui/桌面/DataBase/unsolved/9_small.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/DataMatrixSample/GS1dmt.bmp");
    //I_src = imread("/home/chezhaohui/下载/libdmtx/libdmtx-master/test/rotate_test/images/test_image18.png");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/未解决的/dotpeen/9.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/未解决的/Patch/15_pad.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    //I_src = imread("/home/chezhaohui/桌面/DataBase/Patch3/7_gray.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    //I_src = imread("/home/chezhaohui/桌面/DataBase/newpatch/patch4/16_pad.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/newpatch/patch6/test.bmp");
    //I_src = imread("/home/chezhaohui/桌面/ResizePadTest/1.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/CreateImage3.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/newpatch/patch3/2_resize.bmp");
    I_src = imread("/home/chezhaohui/2DTG/DM_EP_Linux_64_so_v.16.09_Trial/images/test22.bmp");
    //I_src = imread("/home/chezhaohui/桌面/DataBase/temp3.bmp");
    
    if(!I_src.data){
        cout<<"Load image failed!\n"<<endl;
        return 0;
    } 
    
    width = I_src.cols;
    height = I_src.rows;

    SRpointer = (locateRect *)calloc(1, sizeof(locateRect));
    SRpointer->Xcoordi = 0;//initialize the SmallReg
    SRpointer->Ycoordi = 0;
    SRpointer->Xlength = I_src.cols - 1;
    SRpointer->Ylength = I_src.rows - 1;
    
    BWpointer = (locateRect *)calloc(1, sizeof(locateRect));//人为给定 后续将matlab改成c后可自动
	BWpointer->Xcoordi = 0;
	BWpointer->Ycoordi = 0;
	BWpointer->Xlength = I_src.cols - 1;
	BWpointer->Ylength = I_src.rows - 1;
    /*
    BWpointer = (locateRect *)calloc(1, sizeof(locateRect));//人为给定 后续将matlab改成c后可自动
	BWpointer->Xcoordi = 113;
	BWpointer->Ycoordi = 61;
	BWpointer->Xlength = 410;
	BWpointer->Ylength = 416;
    */
    
        
    if (BWpointer->Xcoordi < 0 || BWpointer->Ycoordi < 0)
    {
        cout << "The BWpointer is wrong " << endl;
        return 0;
    }
		
	if (BWpointer->Xcoordi + BWpointer->Xlength > width || BWpointer->Ycoordi + BWpointer->Ylength > height)
    {
        cout << "The BWpointer is wrong2" << endl;
        return 0;
    }
		
    //I_src = I_src(Rect(830, 480, 120, 130));
    //imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/cropping.bmp", I_src);
    


    I_coarse = I_src.clone();//try to protect the original image version as I_src

    //I_coarse = coarsedetect(Mat I_coarse, int DataMatrix);
    
    PASS_flag = Recog(I_coarse, SRpointer, 1, BWpointer);
    if (PASS_flag == 1)
    {
        cout<<"The image can be recognized directly. \n"<<endl;
        
    }
    else
    {
        
        cout<<"The original image cannot be recognized directly. Please wait for filter selection step.\n"<<endl;
        if (Mode == 0)
            PASS_flag = Filter_Adj (I_coarse, FilterKnown, SRpointer, Mode, BWpointer);//detect the location of barcode 

        //if (PASS_flag == 1)//yuanlai
        if(Mode==1)//shan
        {
            Mode = 1;
            PASS_flag = Filter_Adj (I_coarse, FilterKnown, SRpointer, Mode, BWpointer);//decode
        }
            
    }

    
    

    return 0;

}


