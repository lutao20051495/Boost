
#include <fstream>
using namespace std;

#include "WeakClassifier.h"
#include "util/File.h"

float WeakClassifier::max_err_rate_ = 0.45;

WeakClassifier::WeakClassifier()
{
	pfeature_ = NULL;
	polar_ = 0;
	err_rate_ = -1.0;
}

bool WeakClassifier::Train(vector<size_t>& sample_index_vec, vector<float>& weight_vec)
{
        vector<Sample>& train_sample_vec = Sample::train_sample_vec_;

        //gen feature pool
        vector<FEATURE_TYPE> feature_type_vec;
        feature_type_vec.push_back(CHANNEL);
        vector<Feature*> pfeature_vec;
        Size sample_size = train_sample_vec[sample_index_vec[0]].getSize();
        Feature::genFeaturePool(sample_size, feature_type_vec, pfeature_vec, 128);
        Feature::genFeatureThreshold(train_sample_vec, sample_index_vec, pfeature_vec);

        //select optimal feature
        float err_rate = -1.0;
        int optimal_index = SelectOptimalFeature(pfeature_vec, sample_index_vec, weight_vec, polar_, err_rate_);
	
        if (optimal_index>=0)
        {
		if (AccurayEnough())
		{
			pfeature_ = pfeature_vec[optimal_index];
		}
		else
		{
			delete pfeature_vec[optimal_index];
			pfeature_vec[optimal_index] = NULL;
		}
        }

        //free feature
        Feature::FreeFeaturePool(pfeature_vec, optimal_index);
	
	if (pfeature_vec[optimal_index])
		return true;
	else
		return false;
}


bool WeakClassifier::AccurayEnough()
{
	if(err_rate_ > WeakClassifier::max_err_rate_)
	{
		return false;
	}
	else
	{
		return true;
	}
}


size_t WeakClassifier::SelectOptimalFeature(vector<Feature*>& pfeature_vec,
                                                vector<size_t>& sample_index_vec,
                                                vector<float>& weight_vec,
                                                int& polar,
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
        vector<Sample>& sample_vec = Sample::train_sample_vec_;
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
        sum_pos_weight = 0.0f;
        sum_neg_weight = 0.0f;
        vector<Sample>& train_sample_vec = Sample::train_sample_vec_;
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


void WeakClassifier::Save(const string& save_dir)
{
        if(!FileExist(save_dir))
        {
                CreateDir(save_dir);
        }
        string weak_clf_path = save_dir + "/" + "weak_clf.txt";
        fstream weak_clf_file(weak_clf_path.c_str(), ios::out);
        if(!weak_clf_file)
        {
                cout << "Creating " << weak_clf_path << "failed!" << endl;
                return;
        }

        weak_clf_file << "polar: " << polar_ << endl;
        weak_clf_file << "err_rate: " << err_rate_ << endl;
        weak_clf_file << "feature_type: " << (int)(pfeature_->feat_type_) << endl;
        weak_clf_file.close();

        //save feature
        if(pfeature_)
        {
                pfeature_->save(save_dir);
        }

        return;
}


bool WeakClassifier::Trained()
{
        if(pfeature_)
        {
                return true;
        }
        else
        {
                return false;
        }
}

void WeakClassifier::FreeFeature()
{
        if(pfeature_)
        {
                delete pfeature_;
        }

        return;
}