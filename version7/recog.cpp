#include"Exhaustion.h"
using namespace std;
using namespace cv;

extern int Recog(cv::InputArray input, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    DmtxImage      *img;
    DmtxMessage    *msg;
    DmtxRegion     *reg;
    DmtxDecode     *dec;
    DmtxVector2    topLeft, topRight, bottomLeft, bottomRight;
    DmtxPixelLoc   pxTopLeft, pxTopRight, pxBottomLeft, pxBottomRight;
    DmtxTime       timeout;
    

    Mat temp = input.getMat(); // can we use the input directly?
    Mat img_bw;

    for(int BW_index = 1; BW_index <= 3; BW_index++)
    {
        switch(BW_index){
            case 1:
            {
                cout<<"Use the Filtered Results for Decoding ..."<<endl;
                img_bw = temp.clone();
                imwrite("/home/chezhaohui/桌面/DataBase/BWresults/OriImg.bmp", img_bw);
                break;
            }
            case 2:
            {
                cout<<"Use the Filtered Results with Clean QuietZone ..."<<endl;
                img_bw = temp.clone();
                Mat img_bw_gray;
                cvtColor(img_bw, img_bw_gray, CV_RGB2GRAY);
                Mat img_bw4;
                EdgeCleanBW(img_bw_gray, img_bw4, BWpointer);
                cvtColor(img_bw_gray, img_bw, CV_GRAY2RGB);
                //img_bw = img_bw4.clone();
                imwrite("/home/chezhaohui/桌面/DataBase/BWresults/OriImg_CQZ.bmp", img_bw);
                break;
            }
            case 3:
            {
                cout<<"Use the Zbar Binarization Method for Decoding ..."<<endl;
                
                img_bw = temp.clone();
                //cout<<"ZBar Channel:"<<img_bw.channels()<<endl;
                
                cvtColor(img_bw, img_bw, CV_RGB2GRAY);
                //cout<<"ZBar Channel:"<<img_bw.channels()<<endl;
                unsigned char *img_data;
                unsigned char *mask_sp;
                int width;
	            int height;
                img_data = img_bw.data;
                width = img_bw.cols;
                height = img_bw.rows;
                mask_sp = qr_binarize(img_data, width, height, BWpointer);
                img_bw.data = mask_sp;
                //cout<<"ZBar Channel:"<<img_bw.channels()<<endl;
                cvtColor(img_bw, img_bw, CV_GRAY2RGB);
                //cout<<"ZBar Channel:"<<img_bw.channels()<<endl;
                
                imwrite("/home/chezhaohui/桌面/DataBase/BWresults/Zbar_bw.bmp", img_bw);
                break;
            }
            case 4:
            {
                cout<<"Use the ZXing Binarization Method for Decoding ..."<<endl;

                Mat img_bw2;
                //img_bw2 = temp.clone();
                ZXingBW(temp, img_bw2, BWpointer);
                cvtColor(img_bw2, img_bw2, CV_GRAY2RGB);
                img_bw = img_bw2.clone();
                imwrite("/home/chezhaohui/桌面/DataBase/BWresults/ZXing_bw.bmp", img_bw);
                
                break;

            }
            case 5:
            {
                cout<<"Use the Yang Binarization Method for Decoding ..."<<endl;
                Mat img_bw3;
                YangBW(temp, img_bw3, BWpointer);
                cvtColor(img_bw3, img_bw3, CV_GRAY2RGB);
                img_bw = img_bw3.clone();
                imwrite("/home/chezhaohui/桌面/DataBase/BWresults/Yang_bw.bmp", img_bw);
                break;
            }
            default:
            {
                cout<<"Error!!! The Binarization Method has not been Appointed! Please Check Again !"<<endl;
                return 0;
            }

        }
    

    
    img = dmtxImageCreate (img_bw.data, img_bw.cols, img_bw.rows, DmtxPack24bppRGB);
    
    dec = dmtxDecodeCreate (img, 1);

    timeout = dmtxTimeAdd (dmtxTimeNow(), 1000);//milliseconds min:500 max:NULL

    reg = dmtxRegionFindNext (dec, &timeout);

    if(reg != NULL)
    {
        if(Mode == 0)
        {
        topLeft.X = bottomLeft.X = topLeft.Y = topRight.Y = -0.1;
        topRight.X = bottomRight.X = bottomLeft.Y = bottomRight.Y = 1.1;

        dmtxMatrix3VMultiplyBy(&topLeft, reg->fit2raw);
        dmtxMatrix3VMultiplyBy(&topRight, reg->fit2raw);
        dmtxMatrix3VMultiplyBy(&bottomLeft, reg->fit2raw);
        dmtxMatrix3VMultiplyBy(&bottomRight, reg->fit2raw);

        pxTopLeft.X = (int)(0.5 + topLeft.X);
        pxTopLeft.Y = (int)(0.5 + topLeft.Y);
        pxBottomLeft.X = (int)(0.5 + bottomLeft.X);
        pxBottomLeft.Y = (int)(0.5 + bottomLeft.Y);
        pxTopRight.X = (int)(0.5 + topRight.X);
        pxTopRight.Y = (int)(0.5 + topRight.Y);
        pxBottomRight.X = (int)(0.5 + bottomRight.X);
        pxBottomRight.Y = (int)(0.5 + bottomRight.Y);

        SRpointer->Xcoordi = pxBottomLeft.X - 6;//6 is a free parameter which is not robust
        SRpointer->Ycoordi = SRpointer->Ylength + 1 - pxBottomLeft.Y - 6;
        SRpointer->Xlength = abs(pxBottomRight.X - pxBottomLeft.X + 1) + 12;
        SRpointer->Ylength = abs(pxTopLeft.Y - pxBottomLeft.Y + 1) + 12;

        dmtxImageDestroy(&img);
        dmtxRegionDestroy(&reg);
        dmtxDecodeDestroy(&dec);
        return 1;
        }

        if(Mode == 1)
        {
            msg = dmtxDecodeMatrixRegion (dec, reg, DmtxUndefined);
            if (msg != NULL)
            {
                //imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/Processed/result.bmp",temp);
                if(*(msg->output) == NULL)
                {
                    cout<<"The decode result is wrong, the program is still running"<<endl;
                    return 0;
                }
                else
                    cout<<"The output is :\n"<<msg->output<<endl;
            
            
                dmtxImageDestroy(&img);
                //dmtxMessageDestroy(&msg);
                dmtxRegionDestroy(&reg);
                dmtxDecodeDestroy(&dec);

                return 1;
            }
        
        }

        
    }
    }
    
    return 0;   
}
