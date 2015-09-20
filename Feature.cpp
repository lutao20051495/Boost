#include "Feature.h"
#include "Acangle.h"

Feature::Feature(FEATURE_TYPE feat_type)
{
        feat_type_ = feat_type;
}

bool Feature::save(string& save_dir) const
{
        return false;
}

 int Feature::featureValue(const Sample& sample) const
{
        return -1;
}

double Feature::AbsoluteValue(const Sample& sample) const
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

//覆盖基类虚函数时，派生类函数和基类中的要完全一致，
//（返回值，函数名，参数个数，参数类型，参数次序都不能有差异）
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
                        Acangle::genAcanglePool(img_size, feature_vec, num);
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
                        Acangle::genAcanglePool(img_size, pfeature_vec, num);
                        break;
                default:
                        cout << "error!" << endl;
                        return false;
                }
        }

        return true;
}


void Feature::FreeFeaturePool(vector<Feature*> &pfeature_vec, size_t exclude_index)
{
        for (unsigned int i=0; i<pfeature_vec.size(); i++)
        {
                if(i != exclude_index)
                {
                        delete pfeature_vec[i];
                }
        }
}

void Feature::genFeatureThreshold(vector<Sample>& sample_vec, vector<size_t>& sample_index_vec, vector<Feature>& feature_vec)
{
        //随机数生成器
        RNG rng( getTickCount() );
        double min_thresh = 0.0;
        double max_thresh = 0.0;
        for(unsigned i=0; i<feature_vec.size(); i++)
        {
                int rand_index = rng.uniform(0, sample_vec.size());
                Sample &sample = sample_vec[rand_index];
                min_thresh = feature_vec[i].AbsoluteValue(sample);
                max_thresh = min_thresh;
                int num = 0;
                while(num < 100)
                {
                        rand_index = rng.uniform(0, sample_vec.size());
                        double value = feature_vec[i].AbsoluteValue(sample_vec[rand_index]);
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
        //随机数生成器
        RNG rng( getTickCount() );
        double min_thresh = 0.0;
        double max_thresh = 0.0;
        for(unsigned i=0; i<pfeature_vec.size(); i++)
        {
                int rand_index = rng.uniform(0, sample_index.size());
                rand_index = sample_index[rand_index];

                Sample& sample = sample_vec[rand_index];
                min_thresh = pfeature_vec[i]->AbsoluteValue(sample);
                max_thresh = min_thresh;
                int num = 0;
                while(num < 100)
                {
                        rand_index = rng.uniform(0, sample_index.size());
                        rand_index = sample_index[rand_index];

                        double value = pfeature_vec[i]->AbsoluteValue( sample_vec[rand_index] );
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



void Feature::genFeatureThreshold(vector<Sample>& sample_vec, vector<Feature*>& pfeature_vec)
{
        //随机数生成器
        RNG rng( getTickCount() );
        double min_thresh = 0.0;
        double max_thresh = 0.0;
        for(unsigned i=0; i<pfeature_vec.size(); i++)
        {
                int rand_index = rng.uniform(0, sample_vec.size());
                Sample& sample = sample_vec[rand_index];

                min_thresh = pfeature_vec[i]->AbsoluteValue(sample);
                max_thresh = min_thresh;
                int num = 0;
                while(num < sample_vec.size()*0.25)
                {
                        rand_index = rng.uniform(0, sample_vec.size());

                        double value = pfeature_vec[i]->AbsoluteValue( sample_vec[rand_index] );
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


