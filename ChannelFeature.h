
#include <opencv2/opencv.hpp>
using namespace cv;

#include "Feature.h"
class ChannelFeature:public Feature
{
public:
        int index_;
        Rect roi_;
        double thresh_;
public:
        ChannelFeature(void);
        ChannelFeature(int index, Rect& roi, double thresh);
        double sumValue(const Sample& sample);
        int featureValue(const Sample& sample);
        void setThresh(double thresh);
        bool save(const string& save_dir);
        bool load(string& load_dir);
        static void genChannelFeaturePool(Size& range, vector<Feature*>& feature_vec, int num=128);
};