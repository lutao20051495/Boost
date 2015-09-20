#ifndef FeatureChannel_H
#define FeatureChannel_H

#include <opencv2/opencv.hpp>
using namespace cv;
#include <iostream>
using namespace std;


class FeatureChannel
{
public:
	//灰度图
	Mat    gray_img_;

        //积分灰度图
	Mat    integral_gray_img_;
        Mat    integral_mag_img_;

        //梯度图
        Mat     gradient_mag_img_;
        Mat     gradient_angle_img_;

        //梯度梯度图
        Mat     gradient_mag_mag_img_;
        Mat     gradient_mag_angle_img_;

        //ct 图
        Mat ct_img_;
public:
        FeatureChannel(void);
        FeatureChannel(Mat& gray_img);

        void calIntegralImage(void);

        void FeatureChannel::calGradient(int dy, int dx, int &magnitude, int &angle);

        void FeatureChannel::calGradientImage(void);

        void FeatureChannel::calGradientImageOnMag(void);

        void FeatureChannel::calCtImage(void);

        void FeatureChannel::calFeatureChannel(Mat& gray_img);

        void FeatureChannel::cropTo(FeatureChannel& patch_fc, Rect& roi);

        void FeatureChannel::getPatch(Rect& roi, FeatureChannel& patch_fc);
};
#endif