#include <opencv2/opencv.hpp>
using namespace cv;

void calCenterLocation(Rect& r, Point2d& center_location);

void ResizeRect(Rect& src, Rect& dst, float dx, float dy);

void ResizeRect(vector<Rect>& src_vec, vector<Rect>& dst_vec, float dx, float dy);

void shiftRect(int shift_x, int shift_y, Rect& src_rect, Rect& dst_rect);

void rectIntersect(Rect_<double>& mean_obj_rect, Rect& r, Rect_<double>& inter_rect);

double interUnion(Rect_<double>& mean_obj_rect, Rect& r);

double interUnion(Rect& r1, Rect& r2);

bool genRandomRect(Rect& range, Size& size, int num, vector<Rect>& rect_vec);

double maxOverlap(vector<Rect>& rect_vec);

 bool cover(Rect& r1, Rect& r2);