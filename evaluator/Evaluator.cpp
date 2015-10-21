
#include <vector>
#include <string>
using namespace std;
#include <stdio.h>


#include "Evaluator.h"
#include "../util/File.h"
#include "../util/Image.h"
#include "../util/draw.h"
#include "../util/shape.h"
#include "../Sample.h"
#include "../StrongClassifier.h"


Evaluator::Evaluator()
{
        pclf_ = NULL;
        max_test_num_ = 100000;
        load_all_neg_img_ = true;
        sample_patch_num_ = 10;
        display_ = 10000;
        thresh_ = 0.5;
}

bool Evaluator::LoadParameter(const string& para_file_path)
{
        cv::FileStorage para_file(para_file_path, FileStorage::READ);
        if( !(para_file.isOpened()) )
        {
                cout << para_file_path << " can not be open!" << endl;
                return false;
        }
        FileNode fn = para_file["Evaluator"];
        thresh_ = fn["thresh"];
        max_test_num_ = fn["max_test_num"];
        load_all_neg_img_ = (int)fn["load_all_neg_image"];
        sample_patch_num_ = fn["patch_per_image"];
        display_ = fn["display"];
	
	
	//detection parameter
	min_object_size_.width = fn["min_object_width"];
	min_object_size_.height = fn["min_object_height"];
	max_object_size_.width = fn["max_object_width"];
	max_object_size_.height = fn["max_object_height"];
	
	shift_step_ = fn["shift_step"];
	scale_step_ = fn["scale_step"];
        return true;
}


void Evaluator::TrainAdaboost(const string& pos_sample_dir, const string& neg_img_dir, const string& save_model_dir,
                              const string& para_file_path)
{
        //StrongClassifier sclf;
        pclf_ = new StrongClassifier();
        if (!pclf_->LoadModelParameter(para_file_path))
        {
                cout << "sclf: load model parameter failed!" << endl;
                return;
        }

        //read pos sample
        vector<Sample> pos_sample_vec;
        Sample::LoadFDDBSample(pos_sample_dir, string("face_patch"), 1, 5, pclf_->input_sample_size_, pos_sample_vec);
        cout << "train positive sample num: " << pos_sample_vec.size() << endl;

        //gene neg image
        vector<Sample> neg_sample_vec;
        Sample::GenRandPatchSample(neg_img_dir, string("jpg"), -1, pclf_->input_sample_size_, pos_sample_vec.size(), neg_sample_vec);
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

        pclf_->Train(sample_index_vec, save_model_dir);
        pclf_->Save(save_model_dir);
        delete pclf_;

        return;
}

//*
bool Evaluator::Positive(float score)
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


float Evaluator::MissRateMultiDir(const string& pos_patch_dir, const string& prefix, const string& img_type, 
                                  unsigned int& sum_num, unsigned int& err_num)
{
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
		    MissRate(pos_patch_dir+"/"+string(sub_dir), img_type, sum_num, err_num,false);
		    cout << "patch_num: " << sum_num << "\tmr:\t" << (float)err_num/sum_num << endl;
	}
	
	return (float)err_num/sum_num;
}

float Evaluator::MissRate(const string& pos_patch_dir, const string& img_type, unsigned int& sum_num, unsigned int& err_num, bool show)
{
	vector<string> file_name_vec;
	if (!GetFileName(pos_patch_dir, img_type, file_name_vec)
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
		Mat img = imread(img_name);
		if (!img.data)
		{
			continue;
		}
		sum_num++;

                //imshow(file_name_vec[i], img);
		//waitKey();

		float score = 0.0;
		pclf_->Predict(img, score);
                cout << "score: " << score << endl;

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

float Evaluator::FalsePositiveRate(const string& img_dir, const string& img_type, unsigned int& sum, unsigned int& err)
{
	cv::RNG rng(cv::getTickCount());
	int test_num = 0;
	int fp_num = 0;
	if (load_all_neg_img_)
	{
		vector<Mat> img_vec;
		loadImage(img_dir, img_vec, img_type);
                cout << "load image: " << img_vec.size() << endl;
		while (test_num<max_test_num_)
		{
			int img_index = rng.uniform(0, img_vec.size());
			Mat patch;
			genRandPatch(img_vec[img_index], patch);
			float score = 0.0;
			pclf_->Predict(patch, score);
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
		if (!GetFileName(img_dir, img_type, file_name_vec)
			|| file_name_vec.size()<=0)
		{
			return 0.0;
		}
		
		while (test_num<max_test_num_)
		{
			int img_index = rng.uniform(0, file_name_vec.size());
			string img_name = img_dir + "/" + file_name_vec[img_index];
			Mat img = imread(img_name);
			if (!img.data)
			{
				continue;
			}
			vector<Mat> patch_vec;
			genRandPatch(img, patch_vec, sample_patch_num_);
			for (int i=0; i<sample_patch_num_; i++)
			{
				float score = 0.0;
				pclf_->Predict(patch_vec[i], score);
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
        sum += test_num;
        err += fp_num;
	float fpr = (float)fp_num/test_num;
	return fpr;
}


float Evaluator::FalsePositiveRateMultiDir(const string& dir, const string& sub_dir, const string& img_type, unsigned int& sum, unsigned& err)
{
        return 0.0;
}

void Evaluator::Run(int argc,char *argv[])
{
        if(argc<=0)
        {
                cout << "no function option selected!" << endl;
                return;
        }
        if (string(argv[0])=="Train")
        {
                Train(argc-1, argv+1);
        }
        else if (string(argv[0])=="TestMr")
        {
                TestMr(argc-1, argv+1);
        }
        else if(string(argv[0])=="TestFpr")
        {
                TestFpr(argc-1, argv+1);
        }
        else if(string(argv[0])=="DetectOnImage")
        {
                DetectOnImage(argc-1, argv+1);
        }
        else
        {
                cout << "option of function error!" << endl;
        }
        return;
}

void Evaluator::Train(int argc, char* argv[])
{
        if ((string(argv[0])=="Adaboost")&&(argc==5))
        {
                TrainAdaboost(string(argv[1]), string(argv[2]), string(argv[3]), string(argv[4]));
        }
        else
        {
                cout << "option for train is error!" << endl;
        }
        return;
}

bool Evaluator::CreateClassifier(const string& clf_type, const string& model_dir)
{
        if (clf_type=="Adaboost")
        {
                pclf_ = new StrongClassifier();
        }
        else
        {
                cout << "Create Classifier error!" << endl;
                return false;
        }

        return pclf_->Load(model_dir);
}

void Evaluator::TestMr(int argc, char* argv[])
{
        cout << argc << endl;
        assert((argc==6));
        //assert(CreateClassifier(string(argv[0]), string(argv[1]))==true);

        if (!CreateClassifier(string(argv[0]), string(argv[1])))
        {
                cout << "create classifier error: " << string(argv[0])
                     << "\t" << string(argv[1]) << endl;
                return;
        }
        unsigned int sum=0;
        unsigned int err=0;
        float mr = -1.0;
        if (string(argv[3]).length()<=0)
        {
                mr = MissRate(string(argv[2]), string(argv[4]), sum, err);
        }
        else
        {
                mr = MissRateMultiDir(string(argv[2]), string(argv[3]), string(argv[4]), sum, err);
        }
        cout << "thresh: " << thresh_ << endl;
        cout << "sum: " << sum << " err: " << err << "\tmr:\t" << mr << endl;
        return;
}


void Evaluator::TestFpr(int argc, char* argv[])
{
        assert(argc==6);
        if (!CreateClassifier(string(argv[0]), string(argv[1])))
        {
                cout << "create classifier error: " << string(argv[0])
                     << "\t" << string(argv[1]) << endl;
                return;
        }
        unsigned int sum = 0;
        unsigned int err = 0;
        float fpr = -1.0;
        if (string(argv[3]).length()<=0)
        {
                fpr = FalsePositiveRate(string(argv[2]), string(argv[4]), sum, err);
        }
        else
        {
                fpr = FalsePositiveRateMultiDir(string(argv[2]), string(argv[3]), string(argv[4]), sum, err);
        }
        cout << "thresh: " << thresh_ << endl;
        cout << "fpr:\t" << fpr << endl;
}


void Evaluator::DetectOnImage(int argc, char* argv[])
{
        assert(argc==6);
        if (!CreateClassifier(string(argv[0]), string(argv[1])))
        {
                cout << "create classifier error: " << string(argv[0])
                     << "\t" << string(argv[1]) << endl;
                return;
        }

        DetectOnImageMultiScale(string(argv[2]), string(argv[3]), string(argv[4]), atoi(argv[5]));
}


void Evaluator::DetectInVideo(int argc, char* argv[])
{
        assert(argc==6);
        if (!CreateClassifier(string(argv[0]), string(argv[1])))
        {
                cout << "create classifier error: " << string(argv[0])
                     << "\t" << string(argv[1]) << endl;
                return;
        }

        DetectInVideo(string(argv[2]), string(argv[3]), string(argv[4]), atoi(argv[5]));
}


void Evaluator::DetectInVideo(const string& src_dir, const string& type, const string& save_dir, const bool show)
{
	vector<string> file_name_vec;
	if (!GetFileName(src_dir, type, file_name_vec)
		|| file_name_vec.size()<=0)
	{
		return;
	}
	if (!FileExist(save_dir))
	{
	    CreateDir(save_dir);
	}

        for (size_t i=0; i<file_name_vec.size(); i++)
        {
                string video_name = src_dir + "/" + file_name_vec[i];
                string save_video_name = save_dir + "/" + file_name_vec[i];
                /*
                cv::VideoCapture video(video_name);
                if (!video.isOpened())
                {
                        cout << "failed to open video: " << video_name << endl;
                        continue;
                }
                */
                DetectInVideo(video_name, save_video_name, show);
        }
}

void Evaluator::DetectInVideo(const string& video_name, const string& save_video_name, bool show)
{
        cv::VideoCapture video(video_name);
        if (!video.isOpened())
        {
                cout << "failed to open video: " << video_name << endl;
                return;
        }
        Mat frame;
        if (!video.read(frame))
        {
                return;
        }
        cv::VideoWriter save_video(save_video_name, CV_FOURCC('M', 'J', 'P', 'G'), 25.0, frame.size());
        
        while (1)
        {
                vector<Rect> rect_vec(0);
                DetectOnImageMultiScale(frame, rect_vec);
                DrawRect(frame, rect_vec, Scalar(255,0,0));
                if (show)
                {
                        imshow("DetectInVideo", frame);
                        waitKey();
                }

                save_video << frame;

                if (!video.read(frame))
                {
                        break;
                }
        }

        return;
}


void Evaluator::DetectOnImageMultiScale(const string& src_dir, const string& type, const string& save_dir, const bool show)
{
	vector<string> file_name_vec;
	if (!GetFileName(src_dir, type, file_name_vec)
		|| file_name_vec.size()<=0)
	{
		return;
	}
	if (!FileExist(save_dir))
	{
	    CreateDir(save_dir);
	}
	for (size_t i=0; i<file_name_vec.size(); i++)
	{
		string img_name = src_dir + "/" + file_name_vec[i];
		Mat img = imread(img_name);
		vector<Rect> rect_vec;
		DetectOnImageMultiScale(img, rect_vec);
		for (size_t j=0; j<rect_vec.size(); j++)
		{
			cv::rectangle(img, rect_vec[j], Scalar(255,0,0),2);
		}
		if (show)
		{
			imshow("detection", img);
			waitKey();
		}
		
		string save_img_name = save_dir + "/" + file_name_vec[i];
		imwrite(save_img_name, img);
	}
}


void Evaluator::DetectOnImageMultiScale(const Mat& img, vector<Rect>& rect_vec)
{
	float max_scale_x = (float)pclf_->crop_size_.width/min_object_size_.width;
	float min_scale_x = (float)pclf_->crop_size_.width/max_object_size_.width;
	float max_scale_y = (float)pclf_->crop_size_.height/min_object_size_.height;
	float min_scale_y = (float)pclf_->crop_size_.height/max_object_size_.height;
	
	float scale_x = min_scale_x;
	float scale_y = min_scale_y;
	Mat cvt_img;
	CvtImageType(img, cvt_img, pclf_->input_sample_type_);
	int shift_step_x = pclf_->crop_size_.width*shift_step_;
	int shift_step_y = pclf_->crop_size_.height*shift_step_;
	while ((scale_x<=max_scale_x)&&(scale_y<=max_scale_y))
	{
		Mat resized_img;
		cv::resize(cvt_img, resized_img, Size(0,0), scale_x, scale_y);
		vector<Rect> scale_rect_vec;
		if (pclf_->InputRaw())
		{
		    DetectOnImageSingleScale(resized_img, scale_rect_vec, shift_step_x, shift_step_y);
		}
		else
		{
		    Sample sample(resized_img);
		    DetectOnImageSingleScale(sample, scale_rect_vec, shift_step_x, shift_step_y);
		}
		ResizeRect(scale_rect_vec, scale_rect_vec, 1.0f/scale_x, 1.0f/scale_y);
		rect_vec.insert(rect_vec.end(), scale_rect_vec.begin(), scale_rect_vec.end());

                scale_x *= scale_step_;
                scale_y *= scale_step_;
	}
}

template <typename Dtype>
void Evaluator::DetectOnImageSingleScale(const Dtype& input, vector<Rect>& rect_vec, int shift_step_x, int shift_step_y)
{
	Size& slide_size = pclf_->crop_size_;
	for (int r=0; r<=input.size().height-slide_size.height; r+=shift_step_y)
	{
		for (int c=0; c<=input.size().width-slide_size.width; c+=shift_step_x)
		{
			Rect roi(c, r, slide_size.width, slide_size.height);
                        float score = 0;
                        const Dtype sample = input(roi);
			pclf_->Predict(sample, score);
                        if (Positive(score))
                        {
                                rect_vec.push_back(roi);
                        }
		}
	}
}




