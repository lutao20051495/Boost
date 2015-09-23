#include "draw.h"

void drawRect(Mat&img, Rect_<double>& rect, Scalar& s)
{
        rectangle(img, rect, s,2);
        return;
}
void drawRect(Mat& img, Rect& rect, Scalar& s)
{
        rectangle(img, rect, s,2);
        return;
}

void drawRect(Mat& img, vector<Rect>& rect_vec, Scalar& s)
{
        for(int i=0; i<rect_vec.size(); i++)
        {
                drawRect(img, rect_vec[i], s);
        }
}