#include <digitRcognize.h>
#include "com_example_opencvandroidboilerplate_MainActivity.h"
#include <opencv2/opencv.hpp>


using namespace cv;

JNIEXPORT jint JNICALL Java_com_example_opencvandroidboilerplate_MainActivity_processFrame
  (JNIEnv *env, jobject, jlong addrInRGBA, jlong addrOut, jintArray jphoneNum) {
    cv::Mat& pMatInRGBA = *(cv::Mat*)addrInRGBA;
    cv::Mat& pMatOut = *(cv::Mat*)addrOut;
    int *phoneNumber = env->GetIntArrayElements(jphoneNum, 0);

    cv::Mat mbgr;
    cv::cvtColor(pMatInRGBA, mbgr, CV_BGRA2BGR);

    //draw the rectangle which restrict the phone number.
    int width = mbgr.cols;
    int height = mbgr.rows;
    int x_ = 0.25*width;
    int y_ = 0.5*height-0.0625*width;
    cv::Rect rect_=cv::Rect(x_,y_,int(0.5*width),int(0.125*width));
    int thickness = 2;
    cv::rectangle(mbgr,rect_,cv::Scalar(255,0,0),thickness);

    //native process function
    cv::Mat roiFrame = mbgr(cv::Rect(rect_.x+thickness,rect_.y+thickness,
    		rect_.width-2*thickness,rect_.height-2*thickness)); //must minus the thickness. it waster me 2 days !!!!

    int result = digitRecognize(roiFrame,phoneNumber);
    //phoneNumber[0] = width;
    //phoneNumber[1]=height;
    /*///////////////////////////////////////////////////////
    Mat processMat = roiFrame.clone();
    Mat image_gray;//(0.125*width, 0.5*width, CV_8UC1);
    cv::cvtColor(processMat, image_gray, CV_BGR2GRAY);
    Mat image_binary;
    cv::threshold(image_gray, image_binary, 128, 255,CV_THRESH_BINARY_INV);
    std::vector<std::vector<cv::Point> > contours;
    findContours(image_binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    ////////////////////////////////////////////////////////*/
    //cv::cvtColor(image_binary, pMatOut, CV_GRAY2BGRA);
    cv::cvtColor(mbgr,pMatOut,CV_BGR2BGRA);

    env->ReleaseIntArrayElements(jphoneNum,phoneNumber,0);
    return result;
}


