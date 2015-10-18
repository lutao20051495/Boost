#include <fstream>
using namespace std;

#include <sys/stat.h> 
#include <stdio.h>

#include "Image.h"
#include "File.h"

void readImages(string& img_path, string& type, vector<Mat>& img_vec)
{
        vector<string> img_name_vec;
        GetFileName(img_path, type, img_name_vec);
        for(unsigned i=0; i<img_name_vec.size(); i++)
        {
                string img_name = img_path + "\\" + img_name_vec[i];
                Mat img = imread(img_name, 0);
                if(img.data)
                        img_vec.push_back(img);
        }
}


void readImagesMultiDir(string& img_path, string& type, vector<Mat>& img_vec, int max_dir_num)
{
        vector<string> img_name_vec;
        readFileNameMultiDir(img_path, type, img_name_vec, max_dir_num);
        for(unsigned i=0; i<img_name_vec.size(); i++)
        {
                string img_name = img_path + "\\" + img_name_vec[i];
                Mat img = imread(img_name, 0);
                if(img.data)
                        img_vec.push_back(img);
        }
}



void genRandomPatch(string& img_path, vector<Mat>& patch_vec, 
                                Size& patch_size, unsigned max_patch_num)
{
        RNG rng( getTickCount() );
        //vector<Mat> img_vec;
        //readImages(img_path, string("jpg"), img_vec);
        vector<string> img_name_vec;
        GetFileName(img_path, string("jpg"), img_name_vec);
        while(patch_vec.size() < max_patch_num)
        {
                
                int img_index = rng.uniform(0, img_name_vec.size());
                string img_name = img_path + "\\" + img_name_vec[img_index];
                Mat img = imread(img_name, 0);
                //int w = rng.uniform(patch_size.width, patch_size.width*10);
                //int h = rng.uniform(patch_size.height, patch_size.height * 10);
                int w = img.cols;
                int h = img.rows;

                Mat resized_img;
                resize(img, resized_img, Size(w, h));

                int x = rng.uniform(0, w-patch_size.width);
                int y = rng.uniform(0, h-patch_size.height);

                Rect ori(x, y, patch_size.width, patch_size.height);
                
                Mat patch;
                resized_img(ori).copyTo(patch);

                patch_vec.push_back(patch);
        }
}


void resizeImages(vector<Mat>& img_vec, vector<Mat>& resized_img_vec, Size& dsize)
{
        for(size_t i=0; i<img_vec.size(); i++)
        {
                Mat resized_img;
                resize(img_vec[i], resized_img, dsize);
                resized_img_vec.push_back(resized_img);
        }
}

void saveImages(vector<Mat>& img_vec, string& save_dir, int start_index)
{
        if(!FileExist(save_dir))
                CreateDir(save_dir);
        char buf[200];
        for(unsigned i=0; i<img_vec.size(); i++)
        {
                sprintf(buf, "%s/%u.jpg", save_dir.c_str(), start_index+i);
                imwrite(buf, img_vec[i]);
        }
        return;
}


void saveImages(vector<Mat>& img_vec, string& sv_dir, vector<size_t>& sv_index_vec)
{
        if(!FileExist(sv_dir))
                CreateDir(sv_dir);

        char buf[50];
        for(unsigned i=0; i<sv_index_vec.size(); i++)
        {
                sprintf(buf, "%s/%u.jpg", sv_dir.c_str(), i);
                imwrite(buf, img_vec[ sv_index_vec[i] ]);
        }

        return;
}

void saveFloatMat(Mat& mat, string& sv_path)
{
        fstream out_file(sv_path.c_str(), ios::out);
        for(int i=0; i<mat.rows; i++)
        {
                for(int j=0; j<mat.cols; j++)
                {
                        out_file << mat.at<float>(i,j) << "\t";
                }
                out_file << endl;
        }
        out_file.close();

        return;
}


int sumValueByIntegral(const Mat& integral_img, Rect& roi)
{
        return integral_img.at<int>(roi.br())
                - integral_img.at<int>(roi.br().y, roi.tl().x)
                - integral_img.at<int>(roi.tl().y, roi.br().x)
                + integral_img.at<int>(roi.tl().y, roi.tl().x);
}



void loadImage(const string& dir, vector<Mat>& img_vec, const string& type)
{
	vector<string> file_name_vec;
	GetFileName(dir, type, file_name_vec);
	
	for(unsigned int i=0; i<file_name_vec.size(); i++)
	{
		string img_name = dir + "/" + file_name_vec[i];
		Mat img = imread(img_name, -1);
		img_vec.push_back(img);
	}
}


void genRandPatch(Mat& img, Mat& patch)
{
	cv::RNG rng(cv::getTickCount());
	int x1 = rng.uniform(0, img.cols);
	int y1 = rng.uniform(0, img.rows);
	int x2 = rng.uniform(0, img.cols);
	int y2 = rng.uniform(0, img.rows);
	
	Rect roi;
	if (x1<x2)
	{
		roi.x = x1;
		roi.width = x2-x1+1;
	}
	else
	{
		roi.x = x2;
		roi.width = x1-x2+1;
	}
	
	if(y1<y2)
	{
		roi.y = y1;
		roi.height = y2-y1+1;
	}
	else
	{
		roi.y = y2;
		roi.height = y1-y2+1;
	}
	patch = img(roi).clone();
	
	return;
}


void genRandPatch(Mat& img, vector<Mat>& patch_vec, int patch_num)
{
	for(int i=0; i<patch_num; i++)
	{
		Mat patch;
		genRandPatch(img, patch);
		patch_vec.push_back(patch);
	}
	
	return;
}


void GenRandPatch(const string& src_img_dir, const string& type, const string& dst_patch_dir, 
		    int patch_in_dir, int patch_num, unsigned int display)
{
	vector<string> img_name_vec;
	GetFileName(src_img_dir, type, img_name_vec);
	if(img_name_vec.size() <=0)
	{
		return;
	}

	//mkdir(dst_patch_dir.c_str(),0777);
        if(!FileExist(dst_patch_dir))
        {
                CreateDir(dst_patch_dir);
        }
	int sub_index = 0;

	string save_dir = dst_patch_dir + "/patch_0";

        if(!FileExist(save_dir))
        {
                CreateDir(save_dir);
        }
	
	int patch_per_img = cvRound((double)patch_num/img_name_vec.size());
	int sum_patch_num = 0;
	unsigned int g_sum_patch_num = 0;
	unsigned int mul = 1;
	for(unsigned int i=0; i<img_name_vec.size(); i++)
	{
		string img_name = src_img_dir + "/" + img_name_vec[i];
		Mat img = imread(img_name,-1);
		//imshow("test", img);
		//waitKey();
		vector<Mat> patch_vec;
		genRandPatch(img, patch_vec, patch_per_img);
		SaveImages(patch_vec, img_name_vec[i], 0, type, save_dir);
		sum_patch_num = sum_patch_num+patch_vec.size();
		g_sum_patch_num += patch_vec.size();
		if(g_sum_patch_num >= mul*display)
		{
				cout << "patch num: " << g_sum_patch_num << endl;
				mul++;
		}
		if (sum_patch_num>patch_in_dir && i<(img_name_vec.size()-1))
		{
			sub_index++;
			char sub_dir_name[100];
			sprintf(sub_dir_name, "patch_%d", sub_index);
			save_dir = dst_patch_dir + "/" + sub_dir_name;
			if(!FileExist(save_dir))
                        {
                                CreateDir(save_dir);
                        }
			sum_patch_num = 0;
		}
	}
}


void ResizeImages(vector<Mat>& img_vec, vector<Mat>& img_vec_resized, Size& patch_size)
{
	for(unsigned int i=0; i<img_vec.size(); i++)
	{
		if(!img_vec[i].data)
		{
		    continue;
		}
		Mat img_resized;
		resize(img_vec[i], img_resized, patch_size);
		img_vec_resized.push_back(img_resized);
	}
	
	return;
}


void SaveImages(const vector<Mat>& img_vec,const string& prefix, int start_index, const string& type,  const string& save_dir)
{
	for (unsigned int i=0; i<img_vec.size(); i++)
	{
		if(!img_vec[i].data)
		{
		    continue;
		}
		char save_name[100];
		sprintf(save_name, "%s_%d.%s", prefix.c_str(), i, type.c_str());
		string save_path = save_dir + "/" + save_name;
		imwrite(save_path, img_vec[start_index++]);
	}
	
	return;
}


bool CropImagePatch(Mat& src_img, Size& crop_size, vector<Mat>& patch_vec)
{
	if (src_img.cols<=crop_size.width
			||src_img.rows<=crop_size.height)
	{
		return false;
	}
	/*left top*/
	Rect crop_roi(0,0,crop_size.width,crop_size.height);
	Mat patch = src_img(crop_roi).clone();
	patch_vec.push_back(patch);
	/*right top*/
	crop_roi = Rect(src_img.cols-crop_size.width,
					0,
					crop_size.width,
					crop_size.height);
	patch = src_img(crop_roi).clone();
	patch_vec.push_back(patch);
	/*left bottom*/
	crop_roi = Rect(0,
					src_img.rows-crop_size.height,
					crop_size.width,
					crop_size.height);
	patch = src_img(crop_roi).clone();
	patch_vec.push_back(patch);
	/*right bottom*/
	crop_roi = Rect(src_img.cols-crop_size.width,
					src_img.rows-crop_size.height,
					crop_size.width,
					crop_size.height);
	patch = src_img(crop_roi).clone();
	patch_vec.push_back(patch);
	
	/*center*/
	crop_roi = Rect(src_img.cols/2-crop_size.width/2,
					src_img.rows/2-crop_size.height/2,
					crop_size.width,
					crop_size.height);
	patch = src_img(crop_roi).clone();
	patch_vec.push_back(patch);	
	return true;
}


void ConvertColorToGray(const string& src_img_dir, const string& dst_img_dir, const string& type)
{
	vector<string> img_name_vec;
	if (!GetFileName(src_img_dir, type, img_name_vec))
	{
		return;
	}
	
        if (!FileExist(dst_img_dir))
        {
                CreateDir(dst_img_dir);
        }
	for (unsigned int i=0; i<img_name_vec.size(); i++)
	{
		cout << "i: " << i << "\t" << img_name_vec[i] << endl;
		string img_name = src_img_dir + "/" + img_name_vec[i];
		string img_name_sv = dst_img_dir + "/" + img_name_vec[i];
		Mat img = imread(img_name);
		if (!img.data)
		{
			continue;
		}
		Mat gray_img;
		cv::cvtColor(img, gray_img, CV_BGR2GRAY);
		imwrite(img_name_sv, gray_img);
	}	
}


void CvtImageType(const Mat& src, Mat& dst, const string& img_type)
{
        if ((img_type=="COLOR")&&(src.channels()==1))
        {
                cvtColor(src, dst, CV_GRAY2BGR);
        }
        else if ((img_type=="GRAY")&&(src.channels()==3))
        {
                cvtColor(src, dst, CV_BGR2GRAY);
        }
        else
        {
                dst = src.clone();
        }
        
        return;
}
