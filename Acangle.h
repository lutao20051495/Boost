#ifndef ACANGLE_H
#define ACANGLE_H

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Feature.h"
#include "Sample.h"

class Acangle:public Feature
{
public:
        //³ß¶È
        int scale_;

        //ÇøÓòa
        Rect    rect_;

        //Ö¸Ê¾
        int  up_degree_;

        int  low_degree_;

        //·§Öµ
        double  theta_;

public:
        //¹¹Ôìº¯Êý
        Acangle(void);
        Acangle(enum FEATURE_TYPE feature_type);
        Acangle(Acangle& acg);
        Acangle(int scale, Rect& rect, int low_degree, int up_degree,int theta);
        Acangle(int scale,Rect& rect, int low_degree, int up_degree,int theta, enum FEATURE_TYPE feature_type);
        //ÌØÕ÷Öµ
        //int featureValue(const Mat& gradient_mag_angle_img) const;
        int featureValue(const Sample& sample) const;

        //¾ø¶ÔÁ¿
        //double AbsoluteValue(const Mat& gradient_mag_angle_img) const;
        double AbsoluteValue(const Sample& sample) const;

        void setThresh(double thresh);
        //±£´æ
        bool save( string& save_dir) const;

        //ÔØÈë
        bool load( string& src_dir);

        //ÊÇ·ñ¸²¸ÇÁËÇ°¾°
        //bool CoverForground(const TriparePatch &patch) const;

        Acangle* cloneNewObject(void);

        void free(void);
        void drawFeatureRect(Mat& img);

public:
        //»­³öÌØÕ÷
        void Plot(Mat &rgb_img);

public:

        static void genAcanglePool(const Size &img_size, vector<Feature> &acangle_vec, unsigned max_num);
        static void genAcanglePool(const Size &img_size, vector<Feature*> &pacangle_vec, unsigned max_num);
        static double calcAbsoluteValue(const Mat& gradient_mag_angle_img, const Rect& rect, int low_degree, int high_degree);

};



#endif