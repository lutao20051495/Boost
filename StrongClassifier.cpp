#include <stdio.h>
#include "StrongClassifier.h"
#include "util/File.h"
#include "FDDB/FDDB.h"


StrongClassifier::StrongClassifier(int weak_clf_num)
{
	weak_clf_vec_.resize(weak_clf_num);
        weak_clf_weight_vec_.resize(weak_clf_num, 0.0);
	thresh_ = 0.0;
}

void StrongClassifier::Train(vector<size_t>& sample_index_vec, const string& save_model_dir)
{
        //create saving dir
        if(!FileExist(save_model_dir))
        {
                CreateDir(save_model_dir);
        }

        vector<Sample>& train_sample_vec = Sample::train_sample_vec_;
        //init weight
        vector<float> sample_weight_vec(sample_index_vec.size(), 1.0f/sample_index_vec.size());

        unsigned int train_weak_clf_num = 0;
	unsigned int cur_sclf_size = 0;
        while (cur_sclf_size<weak_clf_vec_.size() &&
                        train_weak_clf_num<weak_clf_vec_.size()*5)
        {
                WeakClassifier weak_clf;
                if (!weak_clf.Train(sample_index_vec, sample_weight_vec))
		{
			train_weak_clf_num++;
			continue;
		}
                
                cout << "train the " << cur_sclf_size << "th weak clf" << endl;
                cout << "training weak clf err_rate: " << weak_clf.err_rate_ << endl;

		//save clf
		weak_clf_vec_[cur_sclf_size] = weak_clf;
		char sv_dir[128];
		sprintf(sv_dir, "%s/weak_clf_%d", save_model_dir.c_str(), cur_sclf_size);
		weak_clf.Save(string(sv_dir));


                //classifier weight
                if (weak_clf.err_rate_>0)
                {
                        weak_clf_weight_vec_[cur_sclf_size] = 0.5f*log((1-weak_clf.err_rate_)/weak_clf.err_rate_);
                }
                else
                {
                        weak_clf_weight_vec_[cur_sclf_size] = 0.5*log(10000.0f);
                        break;
                }

                float clf_weight = weak_clf_weight_vec_[cur_sclf_size];
                //update sample weight
                for(unsigned int i=0; i<sample_index_vec.size(); i++)
                {
                        size_t index = sample_index_vec[i];
                        Sample& sample = train_sample_vec[index];
                        sample_weight_vec[index] = sample_weight_vec[index]*
                                                        exp(-clf_weight*sample.label_*weak_clf.Predict(sample));
                }

                //normalize sample weight
                float sum_weight = 0.0;
                for(unsigned int i=0; i<sample_weight_vec.size(); i++)
                {
                        sum_weight += sample_weight_vec[i];
                }
                for(unsigned int i=0; i<sample_weight_vec.size(); i++)
                {
                        sample_weight_vec[i] /= sum_weight;
                }
		
		//test accuracy&loss
		cout << "training error: " << TrainingError(train_sample_vec) << endl;
                cout << "training loss: " << TrainingLoss(train_sample_vec) << endl << endl;
                
                train_weak_clf_num++;
                cur_sclf_size++;
        }
}


float StrongClassifier::TrainingError(vector<Sample>& sample_vec)
{
	if (sample_vec.size()<=0)
	{
		return -1.0;
	}
	unsigned int err_num = 0;
	for (unsigned int i=0; i<sample_vec.size(); i++)
	{
                //cout << "test sample i: " << i << endl;
		if (Predict(sample_vec[i])!=sample_vec[i].label_)
		{
			err_num++;
		}
	}
	
	return (float)err_num/sample_vec.size();
}


int StrongClassifier::Predict(Sample& sample, int label_num)
{
	//two class
	if(label_num<=2)
	{
		float score = 0.0;
		Predict(sample, score);
		if (score>thresh_)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		cout << "label_num does not equals 2" << endl;
		return -1;
	}
}


void StrongClassifier::Predict(Sample& sample, float& score)
{
        score = 0.0;
        for (unsigned int i=0; i<weak_clf_vec_.size(); i++)
        {
                if (weak_clf_vec_[i].Trained())
                {
                        score += weak_clf_weight_vec_[i]*weak_clf_vec_[i].Predict(sample);
                }
        }
        return;
}


float StrongClassifier::TrainingLoss(vector<Sample>& sample_vec)
{
	float loss = 0.0f;
	for (unsigned int i=0; i<sample_vec.size(); i++)
	{
		Sample& sample = sample_vec[i];
		float score = 0.0f;
		Predict(sample, score);
		//exp loss
		loss += 0.5f*exp(-sample.label_*score);
	}
	
	loss /= sample_vec.size();
	
	return loss;
}


int StrongClassifier::WeakClfNum()
{
        return weak_clf_vec_.size();
}


void TrainAdaboost(const string& pos_sample_dir, const string& neg_img_dir, const string& save_model_dir)
{
        //read pos sample
        vector<Sample> pos_sample_vec;
        FDDB fddb(pos_sample_dir);
        vector<Mat> pos_img_vec;
        fddb.loadFacePatch(string("face_patch"), 1, 1, pos_img_vec);
        //test
        pos_img_vec.resize(100);
        Sample::genSample(pos_img_vec, 1, pos_sample_vec);
        cout << "train positive sample num: " << pos_sample_vec.size() << endl;


        Size sample_size = pos_sample_vec[0].getSize();
        //gene neg image
        vector<Mat> neg_img_vec;
        readImage(neg_img_dir, string("jpg"), neg_img_vec, 100);
        vector<Sample> neg_sample_vec;
        Sample::genRandomNegSample(neg_img_vec, neg_sample_vec, sample_size, -1, pos_sample_vec.size());
        cout << "train negative sample num: " << neg_sample_vec.size() << endl;

        vector<Sample>& train_data_vec = Sample::train_sample_vec_;
        train_data_vec = pos_sample_vec;
        train_data_vec.insert(train_data_vec.end(), neg_sample_vec.begin(), neg_sample_vec.end());
        cout << "all training sample num: " << train_data_vec.size() << endl;

        //gene sample index
        vector<size_t> sample_index_vec(0);
        for (size_t i=0; i<train_data_vec.size(); i++)
        {
                sample_index_vec.push_back(i);
        }

        StrongClassifier sclf(50);
        sclf.Train(sample_index_vec, save_model_dir);
        cout << "strong classifier size: " << sclf.WeakClfNum() << endl;
}
