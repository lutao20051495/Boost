#include "shape.h"
#include "draw.h"


void calCenterLocation(Rect& r, Point2d& center_location)
{
        center_location.x = r.x + (r.width - 1.0)/2.0;
        center_location.y = r.y + (r.height - 1.0)/2.0;
        return;
}

void resizeRect(Rect& r, double scale)
{
        r.x = r.x * scale;
        r.y = r.y * scale;
        r.width = r.width * scale;
        r.height = r.height * scale;
}

void resizeRect(vector<Rect>& rect_vec, double scale)
{
        for(unsigned int i=0; i<rect_vec.size(); i++)
        {
                resizeRect(rect_vec[i], scale);
        }
}

void shiftRect(int shift_x, int shift_y, Rect& src_rect, Rect& dst_rect)
{
        dst_rect.x = src_rect.x + shift_x;
        
        dst_rect.y = src_rect.y + shift_y;

        dst_rect.width = src_rect.width;
        dst_rect.height = src_rect.height;

        return;
}


 /*whether r1 is covered by r2*/
 bool cover(Rect& r1, Rect& r2)
 {
        if( (r1.x<r2.x)||(r1.y<r2.y) )
        {
                return false;
        }

        if( (r1.br().x>r2.br().x)||(r1.br().y>r2.br().y) )
        {
                return false;
        }

        return true;
 }

void rectIntersect(Rect& r1, Rect& r2, Rect& inter_rect)
{
        inter_rect.x = r1.x > r2.x ? r1.x : r2.x;
        inter_rect.y = r1.y > r2.y ? r1.y : r2.y;

        int brx = r1.br().x < r2.br().x ? r1.br().x : r2.br().x;
        int bry = r1.br().y < r2.br().y ? r1.br().y : r2.br().y;


        if(brx <= inter_rect.x)
                inter_rect.width = 0;
        else
                inter_rect.width = brx - inter_rect.x;
        
        if(bry <= inter_rect.y)
                inter_rect.height = 0;
        else
                inter_rect.height = bry - inter_rect.y;
}


void rectIntersect(Rect_<double>& mean_obj_rect, Rect& r, Rect_<double>& inter_rect)
{
        inter_rect.x = mean_obj_rect.x > r.x ? mean_obj_rect.x : r.x;
        inter_rect.y = mean_obj_rect.y > r.y ? mean_obj_rect.y : r.y;

        double brx = mean_obj_rect.br().x < r.br().x ? mean_obj_rect.br().x : r.br().x;
        double bry = mean_obj_rect.br().y < r.br().y ? mean_obj_rect.br().y : r.br().y;

        if(brx <= inter_rect.x)
                inter_rect.width = 0;
        else
                inter_rect.width = brx - inter_rect.x;
        
        if(bry <= inter_rect.y)
                inter_rect.height = 0;
        else
                inter_rect.height = bry - inter_rect.y;
        
}


double interUnion(Rect& r1, Rect& r2)
{
        Rect inter_rect;
        rectIntersect(r1, r2, inter_rect);

        double iu = (double)inter_rect.area()/(r1.area() + r2.area() - inter_rect.area());

        return iu;
}


double interUnion(Rect_<double>& mean_obj_rect, Rect& r)
{
        Rect_<double> inter_rect;
        rectIntersect(mean_obj_rect, r, inter_rect);
        
        //test
        //cout << "intersect union: " << iu << endl;
        //Mat test_img(48,48,CV_8UC3,Scalar(255,255,255));
        //drawRect(test_img, mean_obj_rect, Scalar(255,0,0));

        //drawRect(test_img, r, Scalar(0,255,0));
        //imshow("test", test_img);
        //waitKey();
        
        double iu = inter_rect.area()/(mean_obj_rect.area() + r.area() - inter_rect.area());
        
        return iu;

}



double maxOverlap(vector<Rect>& rect_vec, Rect& r)
{
        double max_overlap = 0.0;
        for(size_t i=0; i<rect_vec.size(); i++)
        {
                double overlap = interUnion( rect_vec[i], r);
                if(overlap > max_overlap)
                {
                        max_overlap = overlap;
                }
        }

        return max_overlap;
}


double maxOverlap(vector<Rect>& rect_vec)
{
        if(rect_vec.size() == 0)
        {
                return -1.0;
        }
        else if(rect_vec.size() == 1)
        {
                return 0.0;
        }
        else
        {
                double max_overlap = -1.0;
                for(size_t i=0; i<rect_vec.size(); i++)
                {
                        for(size_t j=i+1; j<rect_vec.size(); j++)
                        {
                                double overlap = interUnion( rect_vec[i] ,rect_vec[j]);
                                if(overlap > max_overlap)
                                                 {
                                        max_overlap = overlap;
                                }
                        }
                }
                return max_overlap;
        }
       
}



bool genRandomRect(Rect& range, Size& size, int num, vector<Rect>& rect_vec)
{
        RNG rng( getTickCount() );
        //cout << "size: " << size.width << "   " << size.height << endl;
        //cout << "range: " << range.width <<"   " << range.height << endl;
        //cout << "num: " << num << endl;
        bool find = false;
        int i = 0;
        while(!find && i<100)
        {
                vector<Rect> tmp_rect_vec;
                for(int k=0; k<num; k++)
                {
                        Rect r;
                        r.x = rng.uniform(range.x, range.br().x-size.width);
                        r.y = rng.uniform(range.y, range.br().y-size.height);
                        r.width = size.width;
                        r.height = size.height;
                        tmp_rect_vec.push_back(r);
                }

                if(maxOverlap(tmp_rect_vec) <= 0.1)
                {
                        rect_vec = tmp_rect_vec;
                        find = true;
                }
                i++;
        }
        
        if(i >= 100)
                return false;
        else
                return true;
}