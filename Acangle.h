#ifndef ACANGLE_H
#define ACANGLE_H

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Feature.h"
#include "Sample.h"

class Acangle:public Feature
{
public:
        //尺度
        int scale_;

        //区域a
        Rect    rect_;

        //指示
        int  up_degree_;

        int  low_degree_;

        //阀值
        double  theta_;

public:
        //构造函数
        Acangle(void);
        Acangle(enum FEATURE_TYPE feature_type);
        Acangle::Acangle(Acangle& acg);
        Acangle(int scale, Rect& rect, int low_degree, int up_degree,int theta);
        Acangle(int scale,Rect& rect, int low_degree, int up_degree,int theta, enum FEATURE_TYPE feature_type);
        //特征值
        //int featureValue(const Mat& gradient_mag_angle_img) const;
        int featureValue(const Sample& sample) const;

        //绝对量
        //double AbsoluteValue(const Mat& gradient_mag_angle_img) const;
        double Acangle::AbsoluteValue(const Sample& sample) const;

        void Acangle::setThresh(double thresh);
        //保存
        bool save( string& save_dir) const;

        //载入
        bool load( string& src_dir);

        //是否覆盖了前景
        //bool CoverForground(const TriparePatch &patch) const;

        Acangle* Acangle::cloneNewObject(void);

        void Acangle::free(void);
        void Acangle::drawFeatureRect(Mat& img);

public:
        //画出特征
        void Plot(Mat &rgb_img);

public:

        static void genAcanglePool(const Size &img_size, vector<Feature> &acangle_vec, unsigned max_num);
        static void genAcanglePool(const Size &img_size, vector<Feature*> &pacangle_vec, unsigned max_num);
        static double calcAbsoluteValue(const Mat& gradient_mag_angle_img, const Rect& rect, int low_degree, int high_degree);

};



#endif