#include <opencv2/opencv.hpp>
using namespace cv;
#include <string>
using namespace std;

void DrawRect(Mat& img, Rect& rect, Scalar& s);
void DrawRect(Mat&img, Rect_<double>& rect, Scalar& s);

void DrawRect(Mat& img, vector<Rect>& rect_vec, const Scalar& s);