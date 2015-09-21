
#include <fstream>
using namespace std;

#include "ChannelFeature.h"
#include "Image.h"


ChannelFeature::ChannelFeature(void):Feature(ACANGLE)
{
}

ChannelFeature::ChannelFeature(int index, Rect& roi, double thresh):Feature(CHANNEL)
{
        index_ = index;
        roi_ = roi;
        thresh_ = thresh;
}


double ChannelFeature::sumValue(const Sample& sample) 
{
        const Mat& fc = sample.fc_.feature_channel_vec_[index_];
        return  sumValueByIntegral(fc, roi_); 
}


int ChannelFeature::featureValue(const Sample& sample) 
{
        if(sumValue(sample) < thresh_)
        {
                return 0;
        }
        else
        {
                return 1;
        }
}


void ChannelFeature::setThresh(double thresh)
{
        thresh_ = thresh;
}


bool ChannelFeature::save(const string& save_dir) 
{
        string save_path = save_dir + "\\" + "channel.txt";
        fstream save_file(save_path.c_str(), ios::out);
        if(!save_file)
        {
                return false;
        }
        save_file<<"index:         "<<index_<<endl;
        save_file<<"rect_x,y,w,h:  "<<roi_.x<<"  "<<roi_.y<<"  "<<roi_.width<<"  "<<roi_.height<<endl;
        save_file<<"thresh:         "<<thresh_<<endl;
        save_file<<endl;

        save_file.close();

        return true;
}


bool ChannelFeature::load(string& dir)
{
        string path = dir + "\\" + "channel.txt";
        fstream src_file(path.c_str(), ios::in);
        if(!src_file)
        {
                return false;
        }
        //×°ÔØÊý¾Ý
        string title;

        src_file  >> title;
        src_file  >> index_;


        src_file  >>  title;
        src_file  >>  roi_.x >> roi_.y >> roi_.width >> roi_.height;

        src_file  >>  title;
        src_file  >>  thresh_;

        src_file.close();
        return true;
}




void ChannelFeature::genChannelFeaturePool(Size& range, vector<Feature*>& feature_vec, int max_num)
{
        int cur_feature_num = 0;
        RNG rng( getTickCount() );
        while( cur_feature_num < max_num*3 )
        {
                //feature channel index
                int index = rng.uniform(0, FeatureChannel::feature_channel_num_);

                //²úÉúÇøÓò(±Ü¿ª±ßÔµµã)
                int min_w = range.width/8 ;
                int min_h = range.height/8 ;
                int w  = rng.uniform( min_w, range.width/4);
                int h  = rng.uniform( min_h, range.height/4);
                int x  = rng.uniform( 1, range.width  - w - 1 );
                int y  = rng.uniform( 1, range.height - h - 1 );
                Rect zone(x,y, w,h);

                //²úÉú·§Öµ
                double thresh = 0.0;

                ChannelFeature* pcf = new ChannelFeature(index, zone, thresh);

                feature_vec.push_back( pcf );
               
                cur_feature_num++;

        }//for k
}