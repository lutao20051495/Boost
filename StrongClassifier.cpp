#include <stdio.h>
#include <fstream>
using namespace std;

#include "StrongClassifier.h"
#include "util/File.h"
#include "FDDB/FDDB.h"

unsigned int StrongClassifier::max_weak_clf_num_ = 1000;

StrongClassifier::StrongClassifier()
{
        weak_clf_vec_.resize(0);
        weak_clf_weight_vec_.resize(0);
        thresh_ = 0.0;
}

StrongClassifier::StrongClassifier(int weak_clf_num)
{
        weak_clf_vec_.resize(weak_clf_num);
        weak_clf_weight_vec_.resize(weak_clf_num, 0.0);
        thresh_ = 0.0;
}


bool StrongClassifier::LoadModelParameter(const string& para_file_path)
{
        cv::FileStorage para_file(para_file_path, FileStorage::READ);
        if( !(para_file.isOpened()) )
        {
                cout << para_file_path << " can not be open!" << endl;
                return false;
        }
        
        FileNode model_para = para_file["Adaboost"];
        int sample_width = model_para["sample_width"];
        int sample_height = model_para["sample_height"];
        input_sample_size_ = Size(sample_width, sample_height);
        int weak_clf_num = model_para["weak_clf_num"];
        weak_clf_vec_.resize(weak_clf_num);
        weak_clf_weight_vec_.resize(weak_clf_num, 0.0);

        
        return true;
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

void StrongClassifier::Predict(Mat& img, float& score)
{
        Mat resized_img;
        if(img.cols!=input_sample_size_.width
                ||img.rows!=input_sample_size_.height)
        {
                resize(img, resized_img, input_sample_size_);
        }
        else
        {
                resized_img = img;
        }
        Sample sample(resized_img);
        Predict(sample, score);

        return;
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


void StrongClassifier::Save(const string& model_dir)
{
        string save_file_name = model_dir+"/"+"strong_classifier.txt";
        fstream save_file(save_file_name.c_str(),ios::out);
        assert(save_file);
        save_file << "sample_size: " << input_sample_size_.width 
                << "\t" << input_sample_size_.height << endl;
        save_file << "weak_clf_num: " << WeakClfNum() << endl;
        save_file << "weak_clf_weight:" << endl;
        for (size_t i=0; i<weak_clf_vec_.size(); i++)
        {
                save_file << weak_clf_weight_vec_[i] << endl;
        }
        save_file.close();
}


bool StrongClassifier::Load(const string& model_dir)
{
        if(!FileExist(model_dir))
        {
                cout << "load strong clf error: " << model_dir << endl;
                return false;
        }

        string sclf_file_name = model_dir + "/" + "strong_classifier.txt";
        fstream sclf_file(sclf_file_name.c_str(), ios::in);
        if(!sclf_file)
        {
                cout << "strong classifier error: " << sclf_file_name << endl;
                return false;
        }
        string txt;
        //sample size
        sclf_file >> txt;
        sclf_file >> input_sample_size_.width;
        sclf_file >> input_sample_size_.height;
        //weak clf num
        sclf_file >> txt;
        int weak_clf_num = 0;
        sclf_file >> weak_clf_num;
        weak_clf_vec_.resize(weak_clf_num);
        weak_clf_weight_vec_.resize(weak_clf_num);
        //weak clf weight
        sclf_file >> txt;
        string prefix = "weak_clf";
        for (int i=0; i<weak_clf_num; i++)
        {
                char sub_dir[64];
                sprintf(sub_dir, "%s_%d", prefix.c_str(), i);
                string weak_clf_dir = model_dir + "/" + string(sub_dir);
                if(!weak_clf_vec_[i].Load(weak_clf_dir))
                {
                        break;
                }
                else
                {
                        cout << "load " << weak_clf_dir << endl;
                }
                sclf_file >> weak_clf_weight_vec_[i];
        }
        sclf_file.close();

        return true;
}