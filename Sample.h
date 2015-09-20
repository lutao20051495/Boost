#ifndef SAMPLE_H
#define SAMPLE_H
#include "FeatureChannel.h"
class Sample
{
public:
        //unsigned int width_;
        //unsigned int height_;
        int width_;
        int height_;
        FeatureChannel fc_;
        int label_;

        vector<float> aux_feature_vec_;
        static vector<Sample> train_sample_vec_;
public:
        Sample(){};
        Sample::Sample(Size& s, int label=0);
        Sample(unsigned int patch_width, unsigned int patch_height, int label);
        Sample(Mat& img, int label=0);
        Sample(Size& size, FeatureChannel& fc, int label=0);

        bool isPositive();

        void getPatch(Rect& roi, Sample& sample_patch);

        Size getSize();

        static void load(string& sample_dir, string& type, int label, vector<Sample>& sample_vec,
                                                                            int max_sample_num=1000000);

        static void genSample(vector<Mat>& img_vec, int label, vector<Sample>& sample_vec);

        static void genRandomNegSample(const vector<Mat>& img_vec, vector<Sample>& sample_vec, 
                                                Size& sample_size, int label, unsigned max_sample_num);

        static void genRandomSample(const vector<Mat>& img_vec, vector<Sample>& sample_vec, 
                                                Size& sample_size, int label, unsigned max_sample_num);

        static void genRandomSample(string& img_path, vector<Sample>& sample_vec, Size& sample_size,
                                                int label, unsigned max_sample_num);

        static void genRandomNegSample(string& img_path, vector<Sample>& sample_vec, Size& sample_size,
                                                int label, unsigned max_sample_num);

        static double calcPurity(vector<Sample>& sample_vec, vector<size_t>& index_vec);

        static double calcEntropy(vector<Sample>& sample_vec, vector<size_t>& index_vec);

        static int calcPosNum(vector<Sample>& sample_vec, vector<size_t>& index_vec);

        void loadMultiDir(string& sample_dir, string& sub_dir_prefix, int start_index, int end_index,
                          string& type, int label, vector<Sample>& sample_vec, int max_sample_num_per_dir);
};
#endif