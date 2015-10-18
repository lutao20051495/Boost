#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Sample.h"

class Classifier
{
public:
    int clf_type_;
    string input_sample_type_;
    Size input_sample_size_;
    Size crop_size_;
public:
        virtual void Train(vector<size_t>& sample_index_vec, const string& save_model_dir);
        virtual void Predict(const Mat& img, float& score);
        virtual void Predict(const Sample& sample, float& score);
        virtual bool LoadModelParameter(const string& para_file_path);
        virtual void Save(const string& model_dir);
        virtual bool Load(const string& model_dir);
        virtual bool InputRaw();
};

#endif