#include"Exhaustion.h"
#include"guidedfilter.h"
using namespace std;
using namespace cv;

extern int Filter_Adj (cv::InputArray input, int FilterKnown, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int Type_Filter, Type_Mask, Scale_Mask, Times_Filter, PASS_flag = 0, temp;
    Mat I_filtered, Temp_Image;
    I_filtered = input.getMat();
    Temp_Image = I_filtered.clone();//avoid the filter operator destroy the original image

    /*
    if(Mode == 1){
        Temp_Image = Temp_Image (Rect(SRpointer->Xcoordi, SRpointer->Ycoordi, SRpointer->Xlength, SRpointer->Ylength));
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/result_cropping.bmp", Temp_Image);//need to be delete in release version

    }
    */
        

    if (FilterKnown == -1)
    {
        temp = FilterKnown+2;
    }
    else
    {
        temp = FilterKnown;
    }

            switch( temp )
            {
                case 0:
                {
                      cout<<"The filter is forbiden."<<endl;
                      if(FilterKnown==0)
                      return 0;
                }
                
                case 1:
                {
                    cout<<"Current Process is : Testing Median Filtering......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 1; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 3; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 4; Times_Filter++)
                            {
                                 PASS_flag = Filter_Median (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                            
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Median Filtering."<<endl;
                                    return 1;
                                } 
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 1)
                    {
                        cout<<"The Primary Set : Median Filtering does not work!!!"<<endl;
                                return 0;
                    }
                                                      
                }

                case 2:
                {
                    cout<<"Current Process is : Testing Smooth Filtering ......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 3; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 3; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 4; Times_Filter++)
                            {
                                 PASS_flag = Filter_Smooth (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                                 //cout<<"...";
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Smooth Filtering."<<endl;
                                    
                                    return 1;
                                }
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 2)
                    {
                        cout<<"The Primary Set : Smooth Filtering does not work!!! "<<endl;
                                return 0;
                    }
                                                      
                }

                case 3:
                {
                    cout<<"Current Process is : Testing Erode Filtering......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 3; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 5; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 4; Times_Filter++)
                            {
                                PASS_flag = Filter_Erode (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                                //cout<<"...";
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Erode Filtering."<<endl;
                                    return 1;
                                } 
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 3)
                    {
                        cout<<"The Primary Set : Erode Filtering does not work!!!"<<endl;
                                return 0;
                    }
                                                      
                }

                case 4:
                {
                    cout<<"Current Process is : Testing Dilate Filtering......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 3; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 5; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 4; Times_Filter++)
                            {
                                PASS_flag = Filter_Dilate (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                                //cout<<"...";
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Dilate Filtering."<<endl;
                                    return 1;
                                } 
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 4)
                    {
                        cout<<"The Primary Set : Dilate Filtering does not work!!! "<<endl;
                                return 0;
                    }
                                                      
                }

                case 5:
                {
                    cout<<"Current Process is : Testing Open Filtering......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 3; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 5; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 4; Times_Filter++)
                            {
                                PASS_flag = Filter_Open (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                                //cout<<"...";
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Open Filtering. \n"<<endl;
                                    return 1;
                                } 
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 5)
                    {
                        cout<<"The Primary Set : Open Filtering does not work!!!"<<endl;
                                return 0;
                    }
                                                      
                }

                case 6:
                {
                    cout<<"Current Process is : Testing Close Filtering......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 3; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 5; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 4; Times_Filter++)
                            {
                                 PASS_flag = Filter_Close (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                            
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Close Filtering."<<endl;
                                    return 1;
                                } 
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 6)
                    {
                        cout<<"The Primary Set : Close Filtering does not work!!!"<<endl;
                                return 0;
                    }
                                                      
                }        

                case 7:
                {
                    cout<<"Current Process is : Testing Guided Filtering......"<<endl;
                    I_filtered = Temp_Image.clone();
                    for (Type_Mask = 0; Type_Mask < 2; Type_Mask++)
                    {
                        for (Scale_Mask = 0; Scale_Mask < 1; Scale_Mask++)
                        {
                            for (Times_Filter = 1; Times_Filter < 2; Times_Filter++)
                            {
                                 PASS_flag = Filter_GIF (I_filtered, Type_Mask, Scale_Mask, Times_Filter, SRpointer, Mode, BWpointer);
                            
                                //PASS_flag = Recog(I_filtered);
                                if(PASS_flag == 1)
                                {
                                    cout<<"The effective filter is Guided Filtering."<<endl;
                                    return 1;
                                } 
                                
                            }
                                                       
                        }                      
                    }
                    
                    if(FilterKnown == 7)
                    {
                        cout<<"The Primary Set : Guided Filtering does not work!!!"<<endl;
                                return 0;
                    }
                                                      
                }                                          
            
                default:
                {
                    cout<<"Failed!!! All Filters have been tested."<<endl;
                    return 0;
                }               
            }
        

}

extern int Filter_Median(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag;
    Mat element;
    Mat temp = input.getMat();

for (i=1; i<=Times_Filter; i++)
{
    if(Type_Mask==0 && Scale_Mask==0)
        medianBlur(temp, temp, 3);
    if(Type_Mask==0 && Scale_Mask==1)
        medianBlur(temp, temp, 5);
    if(Type_Mask==0 && Scale_Mask==2)
        medianBlur(temp, temp, 7);

}
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version   
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //Mode = 1;
        //temp = temp(Rect(SmallReg.Xcoordi, SmallReg.Ycoordi, SmallReg.Xlength, SmallReg.Ylength));
        //cout<<"The Efficient Filter is Smooth. \n"<<endl;
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_median.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}

extern int Filter_Smooth(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag;
    Mat element;
    Mat temp = input.getMat();

for (i=1; i<=Times_Filter; i++)
{
    if(Type_Mask==0 && Scale_Mask==0)
        blur (temp, temp, Size(3, 3), Point(-1, -1));
    if(Type_Mask==0 && Scale_Mask==1)
        blur (temp, temp, Size(5, 5), Point(-1, -1));
    if(Type_Mask==0 && Scale_Mask==2)
        blur (temp, temp, Size(7, 7), Point(-1, -1));

    if(Type_Mask==1 && Scale_Mask==0)
        boxFilter (temp, temp, -1, Size(3, 3));
    if(Type_Mask==1 && Scale_Mask==1)
        boxFilter (temp, temp, -1, Size(5, 5));
    if(Type_Mask==1 && Scale_Mask==2)
        boxFilter (temp, temp, -1, Size(7, 7));
    

    if(Type_Mask==2 && Scale_Mask==0)
        GaussianBlur (temp, temp, Size(3, 3), 0, 0);
    if(Type_Mask==2 && Scale_Mask==1)
        GaussianBlur (temp, temp, Size(5, 5), 0, 0);
    if(Type_Mask==2 && Scale_Mask==2)
        GaussianBlur (temp, temp, Size(7, 7), 0, 0);

}
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //Mode = 1;
        //temp = temp(Rect(SmallReg.Xcoordi, SmallReg.Ycoordi, SmallReg.Xlength, SmallReg.Ylength));
        //cout<<"The Efficient Filter is Smooth. \n"<<endl;
        
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_smooth.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}

extern int Filter_Erode(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag;
    Mat element;
    Mat temp = input.getMat();

    if(Type_Mask==0 && Scale_Mask==0)
        element = getStructuringElement(MORPH_RECT, Size(1, 1));
    if(Type_Mask==0 && Scale_Mask==1)
        element = getStructuringElement(MORPH_RECT, Size(2, 2));
    if(Type_Mask==0 && Scale_Mask==2)
        element = getStructuringElement(MORPH_RECT, Size(3, 3));
    if(Type_Mask==0 && Scale_Mask==3)
        element = getStructuringElement(MORPH_RECT, Size(5, 5));
    if(Type_Mask==0 && Scale_Mask==4)
        element = getStructuringElement(MORPH_RECT, Size(7, 7));

    if(Type_Mask==1 && Scale_Mask==0)
        element = getStructuringElement(MORPH_CROSS, Size(1, 1));
    if(Type_Mask==1 && Scale_Mask==1)
        element = getStructuringElement(MORPH_CROSS, Size(2, 2));
    if(Type_Mask==1 && Scale_Mask==2)
        element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    if(Type_Mask==1 && Scale_Mask==3)
        element = getStructuringElement(MORPH_CROSS, Size(5, 5));
    if(Type_Mask==1 && Scale_Mask==4)
        element = getStructuringElement(MORPH_CROSS, Size(7, 7));

    if(Type_Mask==2 && Scale_Mask==0)
        element = getStructuringElement(MORPH_ELLIPSE, Size(1, 1));
    if(Type_Mask==2 && Scale_Mask==1)
        element = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
    if(Type_Mask==2 && Scale_Mask==2)
        element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    if(Type_Mask==2 && Scale_Mask==3)
        element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    if(Type_Mask==2 && Scale_Mask==4)
        element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    
    for (i=1; i<=Times_Filter; i++)
    {
        erode (temp, temp, element);
    }
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //cout<<"The filter is erode. \n"<<endl;
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_erode.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}

extern int Filter_Dilate(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag;
    Mat element;
    Mat temp = input.getMat();

    if(Type_Mask==0 && Scale_Mask==0)
        element = getStructuringElement(MORPH_RECT, Size(1, 1));
    if(Type_Mask==0 && Scale_Mask==1)
        element = getStructuringElement(MORPH_RECT, Size(2, 2));
    if(Type_Mask==0 && Scale_Mask==2)
        element = getStructuringElement(MORPH_RECT, Size(3, 3));
    if(Type_Mask==0 && Scale_Mask==3)
        element = getStructuringElement(MORPH_RECT, Size(5, 5));
    if(Type_Mask==0 && Scale_Mask==4)
        element = getStructuringElement(MORPH_RECT, Size(7, 7));

    if(Type_Mask==1 && Scale_Mask==0)
        element = getStructuringElement(MORPH_CROSS, Size(1, 1));
    if(Type_Mask==1 && Scale_Mask==1)
        element = getStructuringElement(MORPH_CROSS, Size(2, 2));
    if(Type_Mask==1 && Scale_Mask==2)
        element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    if(Type_Mask==1 && Scale_Mask==3)
        element = getStructuringElement(MORPH_CROSS, Size(5, 5));
    if(Type_Mask==1 && Scale_Mask==4)
        element = getStructuringElement(MORPH_CROSS, Size(7, 7));

    if(Type_Mask==2 && Scale_Mask==0)
        element = getStructuringElement(MORPH_ELLIPSE, Size(1, 1));
    if(Type_Mask==2 && Scale_Mask==1)
        element = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
    if(Type_Mask==2 && Scale_Mask==2)
        element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    if(Type_Mask==2 && Scale_Mask==3)
        element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    if(Type_Mask==2 && Scale_Mask==4)
        element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    
    for (i=1; i<=Times_Filter; i++)
    {
        dilate (temp, temp, element);
    }
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //cout<<"The filter is erode. \n"<<endl;
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_dilate.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}

extern int Filter_Open(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag;
    Mat element;
    Mat temp = input.getMat();

    if(Type_Mask==0 && Scale_Mask==0)
        element = getStructuringElement(MORPH_RECT, Size(1, 1));
    if(Type_Mask==0 && Scale_Mask==1)
        element = getStructuringElement(MORPH_RECT, Size(2, 2));
    if(Type_Mask==0 && Scale_Mask==2)
        element = getStructuringElement(MORPH_RECT, Size(3, 3));
    if(Type_Mask==0 && Scale_Mask==3)
        element = getStructuringElement(MORPH_RECT, Size(5, 5));
    if(Type_Mask==0 && Scale_Mask==4)
        element = getStructuringElement(MORPH_RECT, Size(7, 7));

    if(Type_Mask==1 && Scale_Mask==0)
        element = getStructuringElement(MORPH_CROSS, Size(1, 1));
    if(Type_Mask==1 && Scale_Mask==1)
        element = getStructuringElement(MORPH_CROSS, Size(2, 2));
    if(Type_Mask==1 && Scale_Mask==2)
        element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    if(Type_Mask==1 && Scale_Mask==3)
        element = getStructuringElement(MORPH_CROSS, Size(5, 5));
    if(Type_Mask==1 && Scale_Mask==4)
        element = getStructuringElement(MORPH_CROSS, Size(7, 7));

    if(Type_Mask==2 && Scale_Mask==0)
        element = getStructuringElement(MORPH_ELLIPSE, Size(1, 1));
    if(Type_Mask==2 && Scale_Mask==1)
        element = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
    if(Type_Mask==2 && Scale_Mask==2)
        element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    if(Type_Mask==2 && Scale_Mask==3)
        element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    if(Type_Mask==2 && Scale_Mask==4)
        element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    
    for (i=1; i<=Times_Filter; i++)
    {
        morphologyEx (temp, temp, MORPH_OPEN, element);
    }
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //cout<<"The filter is erode. \n"<<endl;
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_open.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}

extern int Filter_Close(cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag;
    Mat element;
    Mat temp = input.getMat();

    if(Type_Mask==0 && Scale_Mask==0)
        element = getStructuringElement(MORPH_RECT, Size(1, 1));
    if(Type_Mask==0 && Scale_Mask==1)
        element = getStructuringElement(MORPH_RECT, Size(2, 2));
    if(Type_Mask==0 && Scale_Mask==2)
        element = getStructuringElement(MORPH_RECT, Size(3, 3));
    if(Type_Mask==0 && Scale_Mask==3)
        element = getStructuringElement(MORPH_RECT, Size(5, 5));
    if(Type_Mask==0 && Scale_Mask==4)
        element = getStructuringElement(MORPH_RECT, Size(7, 7));

    if(Type_Mask==1 && Scale_Mask==0)
        element = getStructuringElement(MORPH_CROSS, Size(1, 1));
    if(Type_Mask==1 && Scale_Mask==1)
        element = getStructuringElement(MORPH_CROSS, Size(2, 2));
    if(Type_Mask==1 && Scale_Mask==2)
        element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    if(Type_Mask==1 && Scale_Mask==3)
        element = getStructuringElement(MORPH_CROSS, Size(5, 5));
    if(Type_Mask==1 && Scale_Mask==4)
        element = getStructuringElement(MORPH_CROSS, Size(7, 7));

    if(Type_Mask==2 && Scale_Mask==0)
        element = getStructuringElement(MORPH_ELLIPSE, Size(1, 1));
    if(Type_Mask==2 && Scale_Mask==1)
        element = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
    if(Type_Mask==2 && Scale_Mask==2)
        element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    if(Type_Mask==2 && Scale_Mask==3)
        element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    if(Type_Mask==2 && Scale_Mask==4)
        element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    
    for (i=1; i<=Times_Filter; i++)
    {
        morphologyEx (temp, temp, MORPH_CLOSE, element);
    }
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //cout<<"The filter is erode. \n"<<endl;
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_close.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}

extern int Filter_GIF (cv::InputArray input, int Type_Mask, int Scale_Mask, int Times_Filter, locateRect* SRpointer, int Mode, locateRect* BWpointer)
{
    int i, type, SZ, PASS_flag, r_radius;
    double eps;
    Mat element, p, q;
    Mat temp = input.getMat();
    

for (i=1; i<=Times_Filter; i++)
{
    if(Type_Mask==0 && Scale_Mask==0)//Enhancement
    {
        temp.convertTo(temp, CV_32F, 1.0 / 255.0);
        p = temp;
        r_radius = 5;
        eps = 1 * 1;
        q = guidedFilter(temp, p, r_radius, eps);
        temp = (temp - q) * 20 + q;
    }           
    if(Type_Mask==0 && Scale_Mask==1)
    {
        temp.convertTo(temp, CV_32F, 1.0 / 255.0);
        p = temp;
        r_radius = 10;
        eps = 5 * 5;
        q = guidedFilter(temp, p, r_radius, eps);
        temp = (temp - q) * 20 + q;
    }
    if(Type_Mask==0 && Scale_Mask==2)
    {
        temp.convertTo(temp, CV_32F, 1.0 / 255.0);
        p = temp;
        r_radius = 20;
        eps = 10 * 10;
        q = guidedFilter(temp, p, r_radius, eps);
        temp = (temp - q) * 20 + q;
    }

    if(Type_Mask==1 && Scale_Mask==0)//Smoothing
    {
        //temp.convertTo(temp, CV_32F, 1.0 / 255.0);
        p = temp;
        r_radius = 2;
        eps = 0.02 * 0.02;
        eps *= 255 * 255;
        temp = guidedFilter(temp, p, r_radius, eps);
    }
    if(Type_Mask==1 && Scale_Mask==1)
    {
        //temp.convertTo(temp, CV_32F, 1.0 / 255.0);
        p = temp;
        r_radius = 4;
        eps = 0.02 * 0.02;
        eps *= 255 * 255;
        temp = guidedFilter(temp, p, r_radius, eps);
    }
    if(Type_Mask==1 && Scale_Mask==2)
    {
        //temp.convertTo(temp, CV_32F, 1.0 / 255.0);
        p = temp;
        r_radius = 8;
        eps = 0.02 * 0.02;
        eps *= 255 * 255;
        temp = guidedFilter(temp, p, r_radius, eps);
    }

}
    cout<<"Type_mask is : "<<Type_Mask<<'\t'<<"Scale_Mask is : "<<Scale_Mask<<'\t'<<"Times_Filter is : "<<Times_Filter<<endl;//delete in release version
    PASS_flag = Recog(temp, SRpointer, Mode, BWpointer);
    if(PASS_flag==1)
    {
        //Mode = 1;
        //temp = temp(Rect(SmallReg.Xcoordi, SmallReg.Ycoordi, SmallReg.Xlength, SmallReg.Ylength));
        //cout<<"The Efficient Filter is Smooth. \n"<<endl;
        imwrite ("/home/chezhaohui/桌面/DataBase/DataMatrixSample/19/filterresult_gif.bmp", temp);//need to be delete in release version
        return 1;
    }
    else
    {
        return 0;
    } 
}
     



