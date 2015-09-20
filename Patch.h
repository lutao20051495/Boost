#ifndef PATCH_H
#define PATCH_H
#include <opencv2/opencv.hpp>
using namespace cv;

class Patch
{
public:
        static void genRandomPatch(const vector<Mat>& img_vec, vector<Mat>& patch_vec, Size& patch_size, 
                                int patch_num);
};
#endif
