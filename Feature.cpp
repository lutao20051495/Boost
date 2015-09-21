#include "Feature.h"
#include "ChannelFeature.h"

Feature::Feature(FEATURE_TYPE feat_type)
{
        feat_type_ = feat_type;
}

bool Feature::save(const string& save_dir) 
{
        return false;
}

 int Feature::featureValue(const Sample& sample) 
{
        return -1;
}

double Feature::sumValue(const Sample& sample) 
{
        return 0.0;
}

void Feature::setThresh(double thresh)
{
        return;
}

bool Feature::load(string& src_dir)
{
        return false;
}


void Feature::free(void)
{
        return;
}

void Feature::drawFeatureRect(Mat& img)
{
        return;
}

//¸²¸Ç»ùÀàÐéº¯ÊýÊ±£¬ÅÉÉúÀàº¯ÊýºÍ»ùÀàÖÐµÄÒªÍêÈ«Ò»ÖÂ£¬
//£¨·µ»ØÖµ£¬º¯ÊýÃû£¬²ÎÊý¸öÊý£¬²ÎÊýÀàÐÍ£¬²ÎÊý´ÎÐò¶¼²»ÄÜÓÐ²îÒì£©
Feature* Feature::cloneNewObject(void)
{
        return NULL;
}


bool Feature::genFeaturePool(Size& img_size, vector<FEATURE_TYPE>& feature_type_vec, vector<Feature>& feature_vec, int num)
{
        feature_vec.clear();
        for(size_t i=0; i<feature_type_vec.size(); i++)
        {
                FEATURE_TYPE feature_type = feature_type_vec[i];
                switch(feature_type)
                {
                case ACANGLE:
                        //Acangle::genAcanglePool(img_size, feature_vec, num);
                        break;
                default:
                        cout << "error!" << endl;
                        return false;
                }
        }
        
        return true;
}


bool Feature::genFeaturePool(Size& img_size, vector<FEATURE_TYPE>& feature_type_vec, vector<Feature*>& pfeature_vec, int num)
{
        pfeature_vec.clear();
        for(size_t i=0; i<feature_type_vec.size(); i++)
        {
                FEATURE_TYPE feature_type = feature_type_vec[i];
                switch(feature_type)
                {
                case ACANGLE:
                        //Acangle::genAcanglePool(img_size, pfeature_vec, num);
                        break;
                case CHANNEL:
                        ChannelFeature::genChannelFeaturePool(img_size, pfeature_vec, num);
                        break;
                default:
                        cout << "error!" << endl;
                        return false;
                }
        }

        return true;
}


void Feature::FreeFeaturePool(vector<Feature*>& pfeature_vec, int exclude_index)
{
        for(unsigned int i=0; i<pfeature_vec.size(); i++)
        {
                if (i!=exclude_index)
                {
                        delete pfeature_vec[i];
                }
        }
}


void Feature::genFeatureThreshold(vector<Sample>& sample_vec, vector<size_t>& sample_index_vec, vector<Feature>& feature_vec)
{
        //Ëæ»úÊýÉú³ÉÆ÷
        RNG rng( getTickCount() );
        double min_thresh = 0.0;
        double max_thresh = 0.0;
        for(unsigned i=0; i<feature_vec.size(); i++)
        {
                int rand_index = rng.uniform(0, sample_vec.size());
                Sample &sample = sample_vec[rand_index];
                min_thresh = feature_vec[i].sumValue(sample);
                max_thresh = min_thresh;
                int num = 0;
                while(num < 100)
                {
                        rand_index = rng.uniform(0, sample_vec.size());
                        double value = feature_vec[i].sumValue(sample_vec[rand_index]);
                        if(min_thresh > value)
                                min_thresh = value;
                        if(max_thresh < value)
                                max_thresh = value;
                        num++;
                }
                double thresh = rng.uniform(min_thresh, max_thresh);
                feature_vec[i].setThresh(thresh);
        }
}


void Feature::genFeatureThreshold(vector<Sample>& sample_vec, vector<size_t>& sample_index, vector<Feature*>& pfeature_vec)
{
        //Ëæ»úÊýÉú³ÉÆ÷
        RNG rng( getTickCount() );
        double min_thresh = 0.0;
        double max_thresh = 0.0;
        for(unsigned i=0; i<pfeature_vec.size(); i++)
        {
                int rand_index = rng.uniform(0, sample_index.size());
                rand_index = sample_index[rand_index];

                Sample& sample = sample_vec[rand_index];
                min_thresh = pfeature_vec[i]->sumValue(sample);
                max_thresh = min_thresh;
                int num = 0;
                while(num < 100)
                {
                        rand_index = rng.uniform(0, sample_index.size());
                        rand_index = sample_index[rand_index];

                        double value = pfeature_vec[i]->sumValue( sample_vec[rand_index] );
                        if(min_thresh > value)
                                min_thresh = value;
                        if(max_thresh < value)
                                max_thresh = value;
                        num++;
                }
                double thresh = rng.uniform(min_thresh, max_thresh);
                //cout << "thresh: " << thresh << endl;
                pfeature_vec[i]->setThresh(thresh);
        }
}


