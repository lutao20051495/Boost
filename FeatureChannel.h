#ifndef FeatureChannel_H
#define FeatureChannel_H

#include <opencv2/opencv.hpp>
using namespace cv;
#include <iostream>
using namespace std;


class FeatureChannel
{
public:
	//�Ҷ�ͼ
	Mat    gray_img_;

        //���ֻҶ�ͼ
	Mat    integral_gray_img_;
        Mat    integral_mag_img_;

        //�ݶ�ͼ
        Mat     gradient_mag_img_;
        Mat     gradient_angle_img_;

        //�ݶ��ݶ�ͼ
        Mat     gradient_mag_mag_img_;
        Mat     gradient_mag_angle_img_;

        //ct ͼ
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