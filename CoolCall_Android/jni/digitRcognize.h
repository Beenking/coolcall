#ifndef DIGITRCOGNIZE_H
#define DIGITRCOGNIZE_H

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
        if(pointPolygonTest(vp,cv::Point(xi,midheight),0)!=-1) //0 -1 1鐨勬剰涔?
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


// return 0: recognize 11 phone number
// return 100000+n~=11 (n>0) : n contours
// return -1 : recognize 11 numbers ,but not like phone.
int digitRecognize(const cv::Mat image,int phoneNum[]){
	// 灰度图像
    cv::Mat image_gray;
    cv::cvtColor(image, image_gray, CV_BGR2GRAY);

    //二值图像
    cv::Mat image_binary;
    cv::threshold(image_gray, image_binary, 0, 1, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
    // cv::threshold(image_gray, image_binary, 128, 255,CV_THRESH_BINARY_INV);

    //连通域向量
    std::vector<std::vector<cv::Point> > contours;

    cv::Mat image_binary_contours=image_binary.clone();
    findContours(image_binary_contours, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    //删除不在区域中间的连通域
    vector< vector<Point> >::iterator it = contours.begin();
    while (it!=contours.end()) {
        if(verifyContours(*it,image)){
            ++it; // A valid rectangle found
        } else {
            it= contours.erase(it);
        }
    }

    //娴嬭瘯鍖呭洿鐩?
    vector<cv::Rect> vr;
    for(unsigned int n=0;n<contours.size();n++) {
        cv::Rect r0 = cv::boundingRect(cv::Mat(contours[n]));
        vr.push_back(r0);
    }//end for


     //纭繚涓?11浣嶆墜鏈哄彿鐮?
    if(vr.size()!=11){
        return vr.size()+100000;
    	//return -2;
    }

    //浠庡乏鍒板彸鎺掑簭11涓暟瀛楀彿鐮?
    std::sort(vr.begin(),vr.end(),CompLess);

    //鍒嗗壊寰呮娴嬬殑鏁板瓧骞剁缉鏀惧ぇ灏?
    Mat image_resized[11];
    Size size(20,40);
    for(int i=0;i<=10;i++){
        Mat ROIbinary(image_binary,vr[i]);
        cv::resize(ROIbinary,image_resized[i],size,0,0,INTER_NEAREST);
        //output<<image_resized[i]<<endl<<endl<<endl;//resize鍚庣殑鍥惧儚鐭╅樀
    }


    //鍒嗗埆缁?11涓暟瀛楁壘鍑烘渶鍖归厤鐨勬暟瀛楁ā鏉?
    for(int i=0;i<=10;i++){
        cv::Mat dst;
        int maxSum=0;int digit=-1;
        for(int n=0;n<10;n++){
            cv::compare(image_resized[i],DigitsTemplate[n],dst,CMP_EQ);
            int sum_ = ElementSum(dst)/255;//sum_涓哄緟妫?鏁板瓧鍜屾暟瀛楁ā鏉跨殑鐩稿悓鍍忕礌鐨勪釜鏁?
            //cout<<sum_<<endl;

            if(sum_>maxSum){
                maxSum = sum_;
                digit = n;
            }
        }

        if(maxSum<500)//
            return -1;
        phoneNum[i] = digit;
    }
    return 0;
}



#endif // DIGITRCOGNIZE_H
