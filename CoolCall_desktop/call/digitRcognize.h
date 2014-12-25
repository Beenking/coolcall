#ifndef DIGITRCOGNIZE_H
#define DIGITRCOGNIZE_H


#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "digitTemplate.h"

using namespace std;
using namespace cv;

//#define SHOWIMAGE_

const cv::Mat Mdigit0(40, 20, CV_8UC1, digit0);
const cv::Mat Mdigit1(40, 20, CV_8UC1, digit1);
const cv::Mat Mdigit2(40, 20, CV_8UC1, digit2);
const cv::Mat Mdigit3(40, 20, CV_8UC1, digit3);
const cv::Mat Mdigit4(40, 20, CV_8UC1, digit4);
const cv::Mat Mdigit5(40, 20, CV_8UC1, digit5);
const cv::Mat Mdigit6(40, 20, CV_8UC1, digit6);
const cv::Mat Mdigit7(40, 20, CV_8UC1, digit7);
const cv::Mat Mdigit8(40, 20, CV_8UC1, digit8);
const cv::Mat Mdigit9(40, 20, CV_8UC1, digit9);

const cv::Mat DigitsTemplate[10]={Mdigit0,Mdigit1,Mdigit2,Mdigit3,Mdigit4,Mdigit5,Mdigit6,Mdigit7,Mdigit8,Mdigit9};

//ofstream output("D:output.txt");

const char strDigit[10]={'0','1','2','3','4','5','6','7','8','9'};

bool verifyContours(vector<Point> vp,cv::Mat image)
{
    int midheight = image.rows/2;
    for(int xi=0;xi<image.cols;xi++){
        if(pointPolygonTest(vp,cv::Point(xi,midheight),0)!=-1) //0 -1 1鐨勬剰涔�
            return true;
    }
    return false;
}


bool CompLess(const Rect rect1, const Rect rect2)
{
    return rect1.x < rect2.x;
}


int ElementSum(const Mat& image)
{
    int sum=0;
    int nr=image.rows;
    int nc=image.cols;
    if(image.isContinuous())
    {
        nr=1;       
        nc=nc*image.rows*image.channels();
    }
    for(int i=0;i<nr;i++)
    {
        const uchar* inData=image.ptr<uchar>(i);
        for(int j=0;j<nc;j++){
            //int data = *inData;
            sum+=*inData;
            inData++;
        }
    }
    return sum;
}







int digitRecognize(cv::Mat image,int phoneNum[]){

#ifdef SHOWIMAGE_
    namedWindow("src",0);
    cv::imshow("src",image);
#endif


    //鐏板害鍖�
    cv::Mat image_gray;
    cv::cvtColor(image, image_gray, CV_BGR2GRAY);
#ifdef SHOWIMAGE_
    namedWindow("gray",0);
    cv::imshow("gray",image_gray);
#endif

    //浜屽�煎寲
    cv::Mat image_binary;
    cv::threshold(image_gray, image_binary, 0, 1, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
    //cv::threshold(image_gray, image_binary, 128, 255,CV_THRESH_BINARY_INV);
#ifdef SHOWIMAGE_
    namedWindow("binary",0);
    cv::imshow("binary",image_binary);
#endif


    //褰㈡�佸澶勭悊
   /* cv::Mat morphImage;
    cv::Mat elements(5,5,CV_8U,cv::Scalar());
    cv::morphologyEx(image_binary,morphImage,MORPH_OPEN,elements);*/


    //鎵惧嚭杩為�氬煙骞剁敾鍑�
    vector< vector< Point> > contours;
    //鍊煎緱娉ㄦ剰鐨勬槸findContours()鏀瑰彉image_binary_contours鐨勫��
    cv::Mat image_binary_contours=image_binary.clone();
    findContours(image_binary_contours, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
#ifdef SHOWIMAGE_//绛涢�夊墠鐨勮繛閫氬煙
    cv::Mat contours_img1(image.size(),CV_8U,cv::Scalar(255));
    cv::drawContours(contours_img1,contours,-1,cv::Scalar(0),1);
    namedWindow("Contours1",0);
    imshow("Contours1",contours_img1);
#endif
    //绛涢�夐渶瑕佺殑杩為�氬煙
    vector<vector<Point> >::iterator it = contours.begin();
    while (it!=contours.end()) {
        if(verifyContours(*it,image)){
            ++it; // A valid rectangle found
        } else {
            it= contours.erase(it);
        }
    }
#ifdef SHOWIMAGE_
    cv::Mat contours_img(image.size(),CV_8U,cv::Scalar(255));
    cv::drawContours(contours_img,contours,-1,cv::Scalar(0),2);
    namedWindow("Contours2",0);
    imshow("Contours2",contours_img);
#endif


    //娴嬭瘯鍖呭洿鐩�
    vector<cv::Rect> vr;
    for(unsigned int n=0;n<contours.size();n++) {
        cv::Rect r0 = cv::boundingRect(cv::Mat(contours[n]));
        vr.push_back(r0);

#ifdef SHOWIMAGE_
        cv::rectangle(contours_img,r0,cv::Scalar(0),2);
        cout<<r0<<endl;
#endif

    }//end for


     //纭繚涓�11浣嶆墜鏈哄彿鐮�
    if(vr.size()!=11){
#ifdef SHOWIMAGE_
        waitKey();
        cout<<"娌℃湁鎵惧埌11涓繛閫氬尯鍩�";
#endif
        return -1;
    }


    //浠庡乏鍒板彸鎺掑簭11涓暟瀛楀彿鐮�
    std::sort(vr.begin(),vr.end(),CompLess);
#ifdef SHOWIMAGE_
    namedWindow("retangle",0);
    imshow("retangle",contours_img);
#endif


    //鍒嗗壊寰呮娴嬬殑鏁板瓧骞剁缉鏀惧ぇ灏�
    Mat image_resized[11];
    Size size(20,40);
    for(int i=0;i<=10;i++){
        Mat ROIbinary(image_binary,vr[i]);
        cv::resize(ROIbinary,image_resized[i],size,0,0,INTER_NEAREST);
        //output<<image_resized[i]<<endl<<endl<<endl;//resize鍚庣殑鍥惧儚鐭╅樀
    }


    //鍒嗗埆缁�11涓暟瀛楁壘鍑烘渶鍖归厤鐨勬暟瀛楁ā鏉�
    for(int i=0;i<=10;i++){
        cv::Mat dst;
        int maxSum=0;int digit=-1;
        for(int n=0;n<10;n++){
            cv::compare(image_resized[i],DigitsTemplate[n],dst,CMP_EQ);
            int sum_ = ElementSum(dst)/255;//sum_涓哄緟妫�鏁板瓧鍜屾暟瀛楁ā鏉跨殑鐩稿悓鍍忕礌鐨勪釜鏁�
            //cout<<sum_<<endl;

            if(sum_>maxSum){
                maxSum = sum_;
                digit = n;
            }
        }

        if(maxSum<500)//濡傛灉鍜屾瘡涓ā鏉跨浉浼煎儚绱犵殑涓暟浣庝簬500涓紝妫�娴嬪彿鐮佸け璐�
            return -2;
        //cout<<endl;

        phoneNum[i] = digit;
    }
#ifdef SHOWIMAGE_
    cv::waitKey();
#endif
    return 1;
}



#endif // DIGITRCOGNIZE_H
