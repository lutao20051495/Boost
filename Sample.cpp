
#include <stdio.h>

#include "Sample.h"       
#include "Patch.h"
#include "File.h"

vector<Sample> Sample::train_sample_vec = vector<Sample>(0);

Sample::Sample(Size& s, int label)
{
        width_ = s.width;
        height_ = s.height;
        label_ = label;

        aux_feature_vec_.clear();
}
Sample::Sample(unsigned int patch_width, unsigned int patch_height, int label)
{
        width_ = patch_width;
        height_ = patch_height;
        label_ = label;

        aux_feature_vec_.clear();
}

Sample::Sample(Mat& gray_img, int label)
{
        width_ = gray_img.cols;
        height_ = gray_img.rows;
        label_ = label;
        fc_ = FeatureChannel(gray_img);
        //fc_.calFeatureChannel(gray_img);

        aux_feature_vec_.clear();
}


Sample::Sample(Size& size, FeatureChannel& fc, int label)
{
        width_ = size.width;
        height_ = size.height;
        fc_ = fc;

        label_ = label;
        aux_feature_vec_.clear();
}



bool Sample::isPositive()
{
        if(label_ == 1)
        {
                return true;
        }
        else
        {
                return false;
        }
}


void Sample::load(string& sample_dir, string& type, int label, vector<Sample>& sample_vec,
                        int max_sample_num)
{
        vector<Mat> img_vec;
        readImage(sample_dir, type, img_vec, max_sample_num);

        Sample::genSample(img_vec, label, sample_vec);
        return;
}


 void Sample::genSample(vector<Mat>& img_vec, int label, vector<Sample>& sample_vec)
 {
        for(unsigned i=0; i<img_vec.size(); i++)
        {
                Sample sample(img_vec[i], label);

                sample_vec.push_back(sample);
        }
 }

void Sample::loadMultiDir(string& sample_dir, string& sub_dir_prefix, int start_index, int end_index,
                          string& type, int label, vector<Sample>& sample_vec, int max_sample_num_per_dir)
{
        for(int i=start_index; i<=end_index; i++)
        {
                char i_str[10];
                sprintf(i_str, "_%d", i);
                string sub_dir = sub_dir_prefix + "\\" + i_str;
                string full_sub_dir = sample_dir + "\\" + sub_dir;
                load(full_sub_dir, type, label, sample_vec, max_sample_num_per_dir);
        }

        return;
}


void Sample::genRandomSample(const string& img_path, vector<Sample>& sample_vec, 
                                Size& sample_size, int label, unsigned max_sample_num)
{
        vector<Mat> img_vec;
        readImage(img_path, string("jpg"), img_vec);

        genRandomSample(img_vec, sample_vec, sample_size, label, max_sample_num);
}


void Sample::genRandomSample(const vector<Mat>& img_vec, vector<Sample>& sample_vec, 
                                Size& sample_size, int label, unsigned max_sample_num)
{
        vector<Mat> patch_vec;
        Patch::genRandomPatch(img_vec, patch_vec, sample_size, max_sample_num);

        for(size_t i=0; i<patch_vec.size(); i++)
        {
                Sample sample(patch_vec[i], label);

                sample_vec.push_back(sample);
        }
}

void Sample::genRandomNegSample(string& img_path, vector<Sample>& sample_vec, 
                                Size& sample_size, int label, unsigned max_sample_num)
{
        genRandomSample(img_path, sample_vec, sample_size, label, max_sample_num);
}


void Sample::genRandomNegSample(const vector<Mat>& img_vec, vector<Sample>& sample_vec, 
                                Size& sample_size, int label, unsigned max_sample_num)
{
        genRandomSample(img_vec, sample_vec, sample_size, label, max_sample_num);
}


void Sample::getPatch(Rect& roi, Sample& sample_patch)
{
        FeatureChannel fc_patch;
        fc_.getPatch(roi, fc_patch);
	Size patch_size = roi.size();
        sample_patch = Sample(patch_size, fc_patch);
        return;
}


Size Sample::getSize()
{
        return Size(width_, height_);
}

double Sample::calcPurity(vector<Sample>& sample_vec, vector<size_t>& index_vec)
{
        if(index_vec.size() <= 0)
        {
                return 0.0;
        }
        int pos_num = Sample::calcPosNum(sample_vec, index_vec);
        return (double)(pos_num)/index_vec.size();
}


double Sample::calcEntropy(vector<Sample>& sample_vec, vector<size_t>& index_vec)
{
        double purity = Sample::calcPurity(sample_vec, index_vec);
        double entropy = 0.0;

        if(purity <= 0.0||purity >= 1.0)
        {
                entropy = 0.0;
        }
        else
        {
                entropy = -purity*log(purity) - (1-purity)*log(1-purity);
        }

        return entropy;
}


int Sample::calcPosNum(vector<Sample>& sample_vec, vector<size_t>& index_vec)
{
        int pos_num = 0;
        for(size_t i=0; i<index_vec.size(); i++)
        {
                int index = index_vec[i];
                if(sample_vec[index].isPositive())
                {
                        pos_num++;
                }
        }

        return pos_num;
}

