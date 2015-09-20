#ifndef ACANGLE_H
#define ACANGLE_H

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Feature.h"
#include "Sample.h"

class Acangle:public Feature
{
public:
        //�߶�
        int scale_;

        //����a
        Rect    rect_;

        //ָʾ
        int  up_degree_;

        int  low_degree_;

        //��ֵ
        double  theta_;

public:
        //���캯��
        Acangle(void);
        Acangle(enum FEATURE_TYPE feature_type);
        Acangle::Acangle(Acangle& acg);
        Acangle(int scale, Rect& rect, int low_degree, int up_degree,int theta);
        Acangle(int scale,Rect& rect, int low_degree, int up_degree,int theta, enum FEATURE_TYPE feature_type);
        //����ֵ
        //int featureValue(const Mat& gradient_mag_angle_img) const;
        int featureValue(const Sample& sample) const;

        //������
        //double AbsoluteValue(const Mat& gradient_mag_angle_img) const;
        double Acangle::AbsoluteValue(const Sample& sample) const;

        void Acangle::setThresh(double thresh);
        //����
        bool save( string& save_dir) const;

        //����
        bool load( string& src_dir);

        //�Ƿ񸲸���ǰ��
        //bool CoverForground(const TriparePatch &patch) const;

        Acangle* Acangle::cloneNewObject(void);

        void Acangle::free(void);
        void Acangle::drawFeatureRect(Mat& img);

public:
        //��������
        void Plot(Mat &rgb_img);

public:

        static void genAcanglePool(const Size &img_size, vector<Feature> &acangle_vec, unsigned max_num);
        static void genAcanglePool(const Size &img_size, vector<Feature*> &pacangle_vec, unsigned max_num);
        static double calcAbsoluteValue(const Mat& gradient_mag_angle_img, const Rect& rect, int low_degree, int high_degree);

};



#endif