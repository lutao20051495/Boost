#include <opencv2/opencv.hpp>
using namespace cv;

class Classifier
{
public:
    int clf_type_;
    int input_img_type_;
    Size input_img_size_;
    Size crop_size_;
public:
    virtual void Predict(Mat& img, float& score);
};