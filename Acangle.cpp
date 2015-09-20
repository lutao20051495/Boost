#include <fstream>
using namespace std;

#include "Acangle.h"
#include "draw.h"


/*
¹¹Ôìº¯Êý.
*/
Acangle::Acangle(void):Feature(ACANGLE)
{
}


Acangle::Acangle(enum FEATURE_TYPE feature_type):Feature(feature_type)
{
}


Acangle::Acangle(Acangle& acg):Feature(ACANGLE)
{
        scale_       = acg.scale_;
        rect_        = acg.rect_;
        up_degree_   = acg.up_degree_;
        low_degree_  = acg.low_degree_;
        theta_       = acg.theta_;
}

Acangle::Acangle(int scale,Rect& rect, int low_degree, int up_degree,int theta):Feature(ACANGLE)
{
        scale_       = scale;
        rect_        = rect;
        up_degree_   = up_degree;
        low_degree_  = low_degree;
        theta_       = theta;
}

Acangle::Acangle(int scale,Rect& rect, int low_degree, int up_degree,int theta, enum FEATURE_TYPE feature_type)
                                                                                        :Feature(feature_type)
{
        scale_       = scale;
        rect_        = rect;
        up_degree_   = up_degree;
        low_degree_  = low_degree;
        theta_       = theta;
}



/*
¹¦ÄÜ:
¼ÆËãÌØÕ÷Öµ.
×¢Òâ:
ÎÒÃÇÔÚÕâÀïÃ»ÓÐ¼ì²érectÊÇ·ñÔÚpatchµÄÍ¼Æ¬·¶Î§ÄÚ. ÒòÎªÔÚ½ÓÏÂÀ´µÄÔËËãÖÐ,Èç¹û³öÏÖÕâÖÖÇé¿ö,»á±£´æ.
*/
//int Acangle::featureValue(const Mat &gradient_mag_angle_img) const
int Acangle::featureValue(const Sample& s) const
{
        //int abso_val = AbsoluteValue( gradient_mag_angle_img );
        double abso_val = AbsoluteValue( s);
        //cout << "abso_val: " << abso_val << "   theta: " << theta_ <<endl;
        if ( abso_val < theta_ )
        {
                return 0;
        }else
        {
                return 1;
        }

}	

/*
 double Acangle::AbsoluteValue(const TriparePatch &patch) const
 {
         const Mat &angle_img = patch.angle_img_;
         const Mat &mag_img   = patch.mag_img_;
 
         int aim_mag = 0;
         int all_mag = 0;
         for (int r=rect_.y; r<rect_.y+rect_.height; r++ )
         {
                 for (int c=rect_.x; c<rect_.x+ rect_.width; c++)
                {
 
                         int mag   =  mag_img.at<uchar>(r,c);
                         all_mag += mag;
 
                         int angle =  angle_img.at<char>(r,c);
                         if ( low_degree_<=angle && angle<=up_degree_)
                         {
                                 aim_mag += mag;
                         }
 
                         
                 }
         }
 
         double ratio = 0;
         if(all_mag!=0)
         {
                 ratio =  1.0*aim_mag/all_mag;
         }
 
         
         
         return ratio;
 
 }
 */


double Acangle::AbsoluteValue(const Sample& sample) const
{
        //¸ù¾Ý³ß¶ÈÑ¡ÔñÏàÓ¦½Ç¶ÈÍ¼
        Mat angle_img;
        Mat mag_img;
        //switch(scale_)
        //{
              //case 1:
                      {
                              angle_img = sample.fc_.gradient_angle_img_;
                              mag_img = sample.fc_.gradient_mag_img_;
                              //break; 
                      }
              //default: cout<<"error"<<endl;
        //}

        int sum_index = 0;        
        for (int r=rect_.y; r<rect_.y+rect_.height; r++ )
        {
                for (int c=rect_.x; c<rect_.x+ rect_.width; c++)
                {

                        int angle =  angle_img.at<char>(r,c);
                        if ( low_degree_<=angle && angle<=up_degree_)
                        {
                                if(scale_)
                                        sum_index += mag_img.at<uchar>(r,c);
                                else
                                        sum_index++;
                                //sum_index++;
                        }

                }
        }
//*
        int up_left_x = rect_.x; int up_left_y = rect_.y;
        int down_right_x = rect_.br().x; int down_right_y = rect_.br().y;
        int sum_mag = sample.fc_.integral_mag_img_.at<int>(down_right_y, down_right_x)
                            - sample.fc_.integral_mag_img_.at<int>(down_right_y, up_left_x)
                            - sample.fc_.integral_mag_img_.at<int>(up_left_y, down_right_x)
                            + sample.fc_.integral_mag_img_.at<int>(up_left_y, up_left_x);
      //cout << ((double)sum_index)/sum_mag << endl;
        if(sum_mag <= 0)
                return 0.0;
        else
                return ((double)sum_index)/sum_mag;
                //*/

        return sum_index;

}	

/*
double Acangle::AbsoluteValue(const Mat &gradient_mag_angle_img) const
{
        //¸ù¾Ý³ß¶ÈÑ¡ÔñÏàÓ¦½Ç¶ÈÍ¼
        Mat angle_img;
        switch(scale_)
        {
              case 1: {angle_img = gradient_mag_angle_img;  break; }
              default: cout<<"error"<<endl;
        }

        int sum_index = 0;        
        for (int r=rect_.y; r<rect_.y+rect_.height; r++ )
        {
                for (int c=rect_.x; c<rect_.x+ rect_.width; c++)
                {

                        int angle =  angle_img.at<char>(r,c);
                        if ( low_degree_<=angle && angle<=up_degree_)
                        {
                               sum_index++;
                        }

                }
        }

      
        return sum_index;

}

*/

double Acangle::calcAbsoluteValue(const Mat& gradient_mag_angle_img, const Rect& rect, int low_degree, int up_degree)
{
        int sum_index = 0;        
        for (int r=rect.y; r<rect.y+rect.height; r++ )
        {
                for (int c=rect.x; c<rect.x+ rect.width; c++)
                {

                        int angle =  gradient_mag_angle_img.at<char>(r,c);
                        if ( low_degree<=angle && angle<=up_degree)
                        {
                               sum_index++;
                        }

                }
        }

      
        return sum_index;
}


void Acangle::setThresh(double thresh)
{
        theta_ = thresh;
}
/*
¹¦ÄÜ:
±£´æÌØÕ÷.
*/

bool Acangle::save(string &save_dir) const
{
        string save_path = save_dir + "\\" + "acangle.txt";
        fstream save_file(save_path.c_str(), ios::out);
        if(!save_file)
        {
                return false;
        }
        save_file<<"scale:         "<<scale_<<endl;
        save_file<<"rect_x,y,w,h:  "<<rect_.x<<"  "<<rect_.y<<"  "<<rect_.width<<"  "<<rect_.height<<endl;
        save_file<<"up_degree:         "<<up_degree_<<endl;
        save_file<<"low_degree:        "<<low_degree_<<endl;
        save_file<<"theta:         "<<theta_<<endl;
        save_file<<endl;

        save_file.close();

        return true;
}


/*
¹¦ÄÜ:
ÔØÈëÌØÕ÷.
*/
bool Acangle::load(string& dir)
{
        string path = dir + "\\" + "acangle.txt";
        fstream src_file(path.c_str(), ios::in);
        if(!src_file)
        {
                return false;
        }
        //×°ÔØÊý¾Ý
        string title;

        src_file  >> title;
        src_file  >> scale_;


        src_file  >>  title;
        src_file  >>  rect_.x >> rect_.y >> rect_.width >> rect_.height;


        src_file  >>  title;
        src_file  >>  up_degree_;


        src_file  >>  title;
        src_file  >>  low_degree_;



        src_file  >>  title;
        src_file  >>  theta_;

        src_file.close();
        return true;
}



void  Acangle::genAcanglePool(const Size &img_size, vector<Feature> &acangle_vec, unsigned max_num)
{
        //acangle_vec.clear();
        int cur_feature_num = 0;
        RNG rng( getTickCount() );
        while( cur_feature_num < max_num )
        {
                //²úÉú³ß¶È(Ö»²úÉú1¸ö³ß¶È)
                //int scale = rng.uniform(0,2);          
                int scale = 1;
                //cout << "scale: " << scale << endl;
                //²úÉúÇøÓò(±Ü¿ª±ßÔµµã)
                int min_w = img_size.width/6 ;
                int min_h = img_size.height/6 ;
                int w  = rng.uniform( min_w, img_size.width-4  +1 );
                int h  = rng.uniform( min_h, img_size.height-4 +1 );
                int x  = rng.uniform( 2, img_size.width  - w - 2 );
                int y  = rng.uniform( 2, img_size.height - h - 2 );

                Rect zone(x,y, w,h);

                //²úÉúÉÏÏÂ½ç
                int s = rng.uniform(5, 45);
                
                int up_degree  = rng.uniform(-90+s, 91 );
                int low_degree = up_degree - s;

                //²úÉú·§Öµ
                int ave_hit = cvRound(s/180.0 * w*h);
                int theta =  rng.uniform( 1, 2*ave_hit + w + h );

                Acangle acangle(scale,zone,low_degree,up_degree ,theta);

                acangle_vec.push_back( acangle );
               
                cur_feature_num++;

        }//for k
}



void  Acangle::genAcanglePool(const Size &img_size, vector<Feature*> &pacangle_vec, unsigned max_num)
{
        //acangle_vec.clear();
        int cur_feature_num = 0;
        RNG rng( getTickCount() );
        while( cur_feature_num < max_num*3 )
        {
                //²úÉú³ß¶È(Ö»²úÉú1¸ö³ß¶È)
                //int scale = rng.uniform(0,2);          
                int scale = 1;
                //cout << "scale: " << scale << endl;
                //²úÉúÇøÓò(±Ü¿ª±ßÔµµã)
                int min_w = img_size.width/6 ;
                int min_h = img_size.height/6 ;
                int w  = rng.uniform( min_w, img_size.width-4  +1 );
                int h  = rng.uniform( min_h, img_size.height-4 +1 );
                int x  = rng.uniform( 2, img_size.width  - w - 2 );
                int y  = rng.uniform( 2, img_size.height - h - 2 );

                Rect zone(x,y, w,h);

                //²úÉúÉÏÏÂ½ç
                int s = rng.uniform(5, 45);
                
                int up_degree  = rng.uniform(-90+s, 91 );
                int low_degree = up_degree - s;

                //²úÉú·§Öµ
                int ave_hit = cvRound(s/180.0 * w*h);
                int theta =  rng.uniform( 1, 2*ave_hit + w + h );

                Acangle* pacangle = new Acangle(scale,zone,low_degree,up_degree ,theta);

                pacangle_vec.push_back( pacangle );
               
                cur_feature_num++;

        }//for k
}



Acangle* Acangle::cloneNewObject(void)
{
        Acangle *pfeat = new Acangle(*this);
        return pfeat;
}

void Acangle::free(void)
{
        delete this;
}


void Acangle::drawFeatureRect(Mat& img)
{
        Scalar rgb(0,255,0);
        drawRect(img, rect_, rgb);
        return;
}
