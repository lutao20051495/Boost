#include <fstream>
using namespace std;

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
                CreateDirectory(save_dir.c_str(), NULL);
        char buf[200];
        for(size_t i=0; i<img_vec.size(); i++)
        {
                sprintf(buf, "%s/%d.jpg", save_dir.c_str(), start_index+i);
                imwrite(buf, img_vec[i]);
        }
        return;
}


void saveImages(vector<Mat>& img_vec, string& sv_dir, vector<size_t>& sv_index_vec)
{
        if(!FileExist(sv_dir))
                CreateDirectory(sv_dir.c_str(), NULL);

        char buf[50];
        for(size_t i=0; i<sv_index_vec.size(); i++)
        {
                sprintf(buf, "%s/%d.jpg", sv_dir.c_str(), i);
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
