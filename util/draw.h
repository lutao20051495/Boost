#include <opencv2/opencv.hpp>
using namespace cv;
#include <string>
using namespace std;

void drawRect(Mat& img, Rect& rect, Scalar& s);
void drawRect(Mat&img, Rect_<double>& rect, Scalar& s);

void drawRect(Mat& img, vector<Rect>& rect_vec, Scalar& s);