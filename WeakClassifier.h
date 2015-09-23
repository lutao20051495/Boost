
#include "Feature.h"
class WeakClassifier
{
public:
    Feature* pfeature_;
    int polar_;
    float err_rate_;
public:
    static float max_err_rate_;
public:
	WeakClassifier();
        bool Train(vector<size_t>& sample_index_vec, vector<float>& weight_vec);
        size_t SelectOptimalFeature(vector<Feature*>& pfeature_vec,
                                                vector<size_t>& sample_index_vec,
                                                vector<float>& weight_vec,
                                                int& polar,
                                                float& min_err_rate);
        void SplitSample(Feature* pfeature,
                                 vector<size_t>& sample_index_vec, 
                                 vector<size_t>& left_index_vec,
                                 vector<size_t>& right_index_vec);


        void SumSampleWeight(vector<size_t>& sample_index_vec,
                     vector<float>& weight_vec, 
                     float& sum_pos_weight,
                     float& sum_neg_weight);

        int Predict(Sample& sample);

        void Save(const string& save_dir);

        void FreeFeature();
	
	bool AccurayEnough();
};         