#include "draw.h"

void DrawRect(Mat&img, Rect_<double>& rect, Scalar& s)
{
        rectangle(img, rect, s,2);
        return;
}
void DrawRect(Mat& img, Rect& rect, Scalar& s)
{
        rectangle(img, rect, s,2);
        return;
}

void DrawRect(Mat& img, vector<Rect>& rect_vec, Scalar& s)
{
        for(int i=0; i<rect_vec.size(); i++)
        {
                DrawRect(img, rect_vec[i], s);
        }
}