
#include "WeakClassifier.h"

void WeakClassifier::Train(vector<size_t>& sample_index_vec, vector<float>& weight_vec)
{
        vector<Sample>& train_sample_vec = Sample::train_sample_vec;

        //gen feature pool
        vector<FEATURE_TYPE> feature_type_vec;
        feature_type_vec.push_back(ACANGLE);
        vector<Feature*> pfeature_vec;
	Size sample_size = train_sample_vec[sample_index_vec[0]].getSize();
        Feature::genFeaturePool(sample_size, feature_type_vec, pfeature_vec, 128);
        Feature::genFeatureThreshold(train_sample_vec, sample_index_vec, pfeature_vec);

        //select optimal feature
        float err_rate = -1.0;
        int optimal_index = SelectOptimalFeature(pfeature_vec, sample_index_vec, weight_vec, polar_, err_rate_);
        pfeature_ = NULL;
        if(optimal_index>=0)
        {
                pfeature_ = pfeature_vec[optimal_index];
        }

        //free feature
        Feature::FreeFeaturePool(pfeature_vec, optimal_index);
        return;
}


size_t WeakClassifier::SelectOptimalFeature(vector<Feature*>& pfeature_vec,
                                                vector<size_t>& sample_index_vec,
                                                vector<float>& weight_vec,
                                                int polar,
                                                float& min_err_rate)
{
        min_err_rate = 1.0;
        polar = 0;
        int optimal_index = -1;
        for (unsigned int i=0; i<pfeature_vec.size(); i++)
        {
                vector<size_t> left_index_vec(0);
                vector<size_t> right_index_vec(0);
                SplitSample(pfeature_vec[i], sample_index_vec, left_index_vec, right_index_vec);
                float left_pos_weight, left_neg_weight, right_pos_weight, right_neg_weight;
                SumSampleWeight(left_index_vec, weight_vec, left_pos_weight, left_neg_weight);
                SumSampleWeight(right_index_vec, weight_vec, right_pos_weight, right_neg_weight);
                if ((left_neg_weight+right_pos_weight)<(left_pos_weight+right_neg_weight))
                {
                        if (min_err_rate>(left_neg_weight+right_pos_weight))
                        {
                                min_err_rate = left_neg_weight+right_pos_weight;
                                polar = -1;
                                optimal_index = i;
                        }
                }
                else
                {
                        if (min_err_rate>(left_pos_weight+right_neg_weight))
                        {
                                min_err_rate = left_pos_weight+right_neg_weight;
                                polar = 1;
                                optimal_index = i;
                        }
                }
        }

        return optimal_index;
}


void WeakClassifier::SplitSample(Feature* pfeature,
                                 vector<size_t>& sample_index_vec, 
                                 vector<size_t>& left_index_vec,
                                 vector<size_t>& right_index_vec)
{
        vector<Sample>& sample_vec = Sample::train_sample_vec;
        for (unsigned int i=0; i<sample_index_vec.size(); i++)
        {
                size_t index = sample_index_vec[i];
                if(pfeature->featureValue(sample_vec[index]))
                {
                        right_index_vec.push_back(index);
                }
                else
                {
                        left_index_vec.push_back(index);
                }
        }
}



void WeakClassifier::SumSampleWeight(vector<size_t>& sample_index_vec,
                     vector<float>& weight_vec, 
                     float& sum_pos_weight,
                     float& sum_neg_weight)
{
        vector<Sample>& train_sample_vec = Sample::train_sample_vec;
        for(unsigned int i=0; i<sample_index_vec.size(); i++)
        {
                size_t index = sample_index_vec[i];
                if(train_sample_vec[index].isPositive())
                {
                        sum_pos_weight += weight_vec[index];
                }
                else
                {
                        sum_neg_weight += weight_vec[index];
                }
        }

        return;
}


int WeakClassifier::Predict(Sample& sample)
{
        if(pfeature_->featureValue(sample))
        {
                return polar_;
        }
        else
        {
                return -polar_;
        }
}