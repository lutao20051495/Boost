#ifndef FeatureChannel_H
#define FeatureChannel_H

#include <opencv2/opencv.hpp>
using namespace cv;
#include <iostream>
using namespace std;


class FeatureChannel
{
public:
	//»Ò¶ÈÍ¼
	Mat    gray_img_;

        //»ý·Ö»Ò¶ÈÍ¼
	Mat    integral_gray_img_;
        Mat    integral_mag_img_;

        //ÌÝ¶ÈÍ¼
        Mat     gradient_mag_img_;
        Mat     gradient_angle_img_;

        //ÌÝ¶ÈÌÝ¶ÈÍ¼
        Mat     gradient_mag_mag_img_;
        Mat     gradient_mag_angle_img_;

        //ct Í¼
        Mat ct_img_;
public:
        FeatureChannel(void);
        FeatureChannel(Mat& gray_img);

        void calIntegralImage(void);

        void calGradient(int dy, int dx, int &magnitude, int &angle);

        void calGradientImage(void);

        void calGradientImageOnMag(void);

        void calCtImage(void);

        void calFeatureChannel(Mat& gray_img);

        void cropTo(FeatureChannel& patch_fc, Rect& roi);

        void getPatch(Rect& roi, FeatureChannel& patch_fc);
};
#endif