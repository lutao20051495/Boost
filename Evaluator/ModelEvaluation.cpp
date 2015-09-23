
#include <vector>
#include <string>
using namespace std;
#include <stdio.h>

#include "ModelEvaluation.h"
#include "File.h"
#include "Image.h"

ModelEvaluation::ModelEvaluation(Classifier& clf,
				unsigned int max_test_num, float thresh)
:clf_(clf), 
max_test_num_(max_test_num),
thresh_(thresh)
{
	load_all_neg_img_ = true;
	sample_patch_num_ = 10;
	display_ = 10000;
}


bool ModelEvaluation::Positive(float score)
{
	if ( score >thresh_ )
	{
		return true;
	}
	else
	{
		return false;
	}
}


float ModelEvaluation::calcMrMultiDir(const string& pos_patch_dir, const string& prefix)
{
	unsigned int sum_num = 0;
	unsigned int err_num = 0;
	float mr = -1.0;
	for (unsigned int i=0; i<1000; i++)
	{
		    if (sum_num>max_test_num_)
		    {
			    break;
		    }
		    cout << "i: " << i << endl;
		    char sub_dir[128];
		    sprintf(sub_dir, "%s_%d", prefix.c_str(), i);
		    calcMr(pos_patch_dir+"/"+string(sub_dir), sum_num, err_num,false);
		    cout << "patch_num: " << sum_num << "\tmr:\t" << (float)err_num/sum_num << endl;
	}
	
	return (float)err_num/sum_num;
}

float ModelEvaluation::calcMr(const string& pos_patch_dir, unsigned int& sum_num, unsigned int& err_num, bool show)
{
	vector<string> file_name_vec;
	if (!GetFileName(pos_patch_dir, "bmp", file_name_vec)
		|| file_name_vec.size()<=0)
	{
		return 0.0f;
	}
	
	for (unsigned int i=0; i<file_name_vec.size(); i++)
	{
		if (sum_num>max_test_num_)
		{
			break;
		}
		//cout << "i: " << i << endl;
		string img_name = pos_patch_dir + "/" + file_name_vec[i];
		Mat img = imread(img_name, clf_.input_img_type_);
		if (!img.data)
		{
			continue;
		}
		sum_num++;
		/*
		imshow(file_name_vec[i], img);
		waitKey();
		*/
		float score = 0.0;
		clf_.Predict(img, score);

		if(!Positive(score))
		{
			err_num++;
		}
		
		if (show&&(sum_num%display_==0))
		{
			cout << "test patch " << sum_num << "\t\t" << (float)err_num/sum_num << endl; 
		}
	}
	return (float)err_num/sum_num;
}


float ModelEvaluation::calcFpr(const string& img_dir)
{
	cv::RNG rng(cv::getTickCount());
	int test_num = 0;
	int fp_num = 0;
	if (load_all_neg_img_)
	{
		vector<Mat> img_vec;
		loadImage(img_dir, img_vec, "jpg");

		while (test_num<max_test_num_)
		{
			int img_index = rng.uniform(0, img_vec.size());
			Mat patch;
			genRandPatch(img_vec[img_index], patch);
			float score = 0.0;
			clf_.Predict(patch, score);
			if (Positive(score))
			{
				fp_num++;
			}
			test_num++;
			if ((test_num+1)%display_ == 0)
			{
				cout << "test patch " << test_num << "\t\t" << (float)fp_num/test_num << endl; 
			}
		}
	}
	else
	{
	    	vector<string> file_name_vec;
		if (!GetFileName(img_dir, "jpg", file_name_vec)
			|| file_name_vec.size()<=0)
		{
			return 0.0;
		}
		
		while (test_num<max_test_num_)
		{
			int img_index = rng.uniform(0, file_name_vec.size());
			string img_name = img_dir + "/" + file_name_vec[img_index];
			Mat img = imread(img_name, clf_.input_img_type_);
			if (!img.data)
			{
				continue;
			}
			vector<Mat> patch_vec;
			genRandPatch(img, patch_vec, sample_patch_num_);
			for (int i=0; i<sample_patch_num_; i++)
			{
				float score = 0.0;
				clf_.Predict(patch_vec[i], score);
				if (Positive(score))
				{
					//cout << "fp score: " << score_vec[1] << endl;  
					fp_num++;
				}
			}
			test_num += sample_patch_num_;
			if (test_num%display_==0)
			{
				cout << "test patch " << test_num << "\t\t" << (float)fp_num/test_num << endl; 
			}
		}
	}
	float fpr = (float)fp_num/test_num;
	return fpr;
}


void Evaluate( int option,
		const string& dir, 
		const string& sub_dir,
		float thresh,
		const string& para_file_path)
{
	cv::FileStorage para_file(para_file_path, FileStorage::READ);
	if( !(para_file.isOpened()) )
	{
		cout << para_file_path << " can not be open!" << endl;
		return;
	}
	string model_type = para_file["ModelType"];
	Classifier* clf = NULL;
	string model_file_path;
	if(model_type == "Caffe")
	{
		FileNode model_para = para_file["CaffeModelParameter"];
		string proto_file_path = model_para["proto_file_path"];
		model_file_path = (string)(model_para["model_file_path"]);
		string mean_file_path = model_para["mean_file_path"];
		string label_file_path = model_para["label_file_path"];
		int sample_width = model_para["sample_width"];
		int sample_height = model_para["sample_height"];
		int crop_width = model_para["crop_width"];
		int crop_height = model_para["crop_height"];
		string img_type = model_para["image_type"];
		/*
		clf = new Caffe(proto_file_path, model_file_path, mean_file_path, label_file_path,
				    Size(sample_width, sample_height), Size(crop_width, crop_height));
				     */
	}
	else if(model_type == "Adaboost")
	{
		
	}
	else
	{
		cout << "model type error!" << endl;
		return;
	}

	FileNode eval_para = para_file["EvaluationParameter"];
	/*unsigned type is not supported*/
	//unsigned int max_test_num = (unsigned int)(eval_para["max_test_num"]);
	int max_test_num = eval_para["max_test_num"];
	para_file.release();
	
	ModelEvaluation model_evl(*clf,  max_test_num, thresh);
	cout << model_type <<" model: " << model_file_path << endl;
	cout << "src_dir: " << dir << endl;
	cout << "thresh: " << thresh << endl;
	cout << "max_test_num: " << max_test_num << endl;
	if(option == 1)
	{
		float mr = -1.0;
		if (sub_dir.length()>0)
		{
				mr = model_evl.calcMrMultiDir(dir, sub_dir);
		}
		else
		{
				unsigned int sum_num = 0;
				unsigned int err_num = 0;
				mr = model_evl.calcMr(dir, sum_num, err_num);
				cout << "sum_num: " << sum_num << endl;
		}
		cout << "mr:\t" << mr << endl;
	}
	else if(option == 0)
	{
		float fpr = model_evl.calcFpr(dir);
		cout << "fpr:\t" << fpr << endl;
	}
	else 
	{
		cout << "err option in evalute function" << endl;
	}
	
	delete clf;
	return;
}



