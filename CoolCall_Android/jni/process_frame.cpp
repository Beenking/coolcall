#include <com_example_coolcall_CallActivity.h>
#include <opencv2/opencv.hpp>

JNIEXPORT jint JNICALL Java_com_example_coolcall_CallActivity_processFrame
  (JNIEnv *, jobject, jlong addrInRGBA, jlong addrOut) {
    cv::Mat* pMatInRGBA = (cv::Mat*)addrInRGBA;
    cv::Mat* pMatOut = (cv::Mat*)addrOut;

    cv::Mat mbgri,mbgro;
    cv::cvtColor(*pMatInRGBA, mbgri, CV_RGBA2BGR);

    //cv::Canny(mbgri, mbgro, 30, 90);
    mbgro = mbgri;

    cv::cvtColor(mbgro,*pMatOut,CV_BGR2RGBA);

    //cv::Mat imageGray;
    //cv::cvtColor(*pMatInRGBA, imageGray, CV_RGBA2GRAY);
    //cv::Canny(imageGray, *pMatOut, 30, 90);
    return 1;
}
