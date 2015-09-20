
#include <opencv2/opencv.hpp>
using namespace cv;

#include <vector>
#include <fstream>
using namespace std;

#include <windows.h>
#include "FDDB.h"
#include "File.h"
#include "shape.h"
#include "Image.h"


FDDB::FDDB(string& data_dir)
{
        data_dir_ = data_dir;
        save_sample_size_ = Size(32,32);
}


void FDDB::genFacePatch(string& save_dir)
{
        if(!FileExist(save_dir))
        {
                CreateDirectory(save_dir.c_str(), NULL);
        }

        string img_dir = data_dir_ + "\\" + "originalPics";
        string anno_dir = data_dir_ + "\\" + "FDDB-folds";
        int anno_file_index = 1;
        int save_img_index = 0;
        while(1)
        {
                char anno_file_name[100];
                sprintf(anno_file_name, "FDDB-fold-%d-ellipseList.txt", anno_file_index);
                string anno_file_path = anno_dir + "\\" + anno_file_name;

                vector<Mat> img_vec;
                if( !genFacePatch(anno_file_path, img_dir, img_vec) )
                {
                        break;
                }

                char sub_dir[50];
                sprintf(sub_dir, "FDDB-fold-%d", anno_file_index);
                string save_path = save_dir + "\\" + sub_dir;

                vector<Mat> resized_img_vec;
                resizeImages(img_vec, resized_img_vec, save_sample_size_);
                saveImages(resized_img_vec, save_path, save_img_index);

                //save_img_index += img_vec.size();

                anno_file_index++;
        }

}

bool FDDB::genFacePatch(string& anno_file_path, string& img_dir, vector<Mat>& img_vec)
{
        fstream anno_file(anno_file_path.c_str(), ios::in);
        if(!anno_file)
        {
                return false;
        }
        int img_num = 0;
        while( !(anno_file.eof()) )
        {
                string img_name;
                anno_file >> img_name;
                string img_path = img_dir + "\\" + img_name + ".jpg";
                Mat img = imread(img_path);
                
                int face_num = 0;
                anno_file >> face_num;
                for(int fi=0; fi<face_num; fi++)
                {
                        FDDB_Ellipse ellipse;
                        anno_file >> ellipse.major_axis_radius_;
                        anno_file >> ellipse.minor_axis_radius_;
                        anno_file >> ellipse.angle_;
                        anno_file >> ellipse.center_.x;
                        anno_file >> ellipse.center_.y;

                        //不知道这儿为什么会有一个1
                        string one;
                        anno_file >> one;

                        if(ellipse.minor_axis_radius_ < 20)
                        {
                                continue;
                        }

                        genFacePatch(img, ellipse, img_vec);
                }
                /*
                if(img_num == 233)
                {
                        system("pause");
                }
                //*/
                cout <<img_num<< "\t" << img_name << "\t\t\t" << img_vec.size() << endl;
                img_num++;
        }

        return true;
}


bool FDDB::genFacePatch(Mat& img, FDDB_Ellipse& ellipse, vector<Mat>& face_patch_vec)
{
        if( !(img.data) )
                return false;

        Rect inner_rect;
        Rect outer_rect;
        genInnerOuterRect(ellipse, inner_rect, outer_rect, img.size());

        genImagePatch(img, inner_rect, outer_rect, face_patch_vec);

        return true;
}


void FDDB::genInnerOuterRect(FDDB_Ellipse& ellipse, Rect& inner_rect, Rect& outer_rect, Size& range)
{
        //inner rect
        inner_rect.x = ellipse.center_.x - ellipse.minor_axis_radius_;
        if(inner_rect.x < 0)
        {
                inner_rect.x = 0;
        }

        inner_rect.y = ellipse.center_.y - ellipse.minor_axis_radius_;
        if(inner_rect.y < 0)
        {
                inner_rect.y = 0;
        }
        inner_rect.width = inner_rect.height = ellipse.minor_axis_radius_*2;
        if(inner_rect.br().x > range.width)
        {
                inner_rect.width -= (inner_rect.br().x - range.width);
        }
        if(inner_rect.br().y > range.height)
        {
                inner_rect.height -= (inner_rect.br().y - range.height);
        }

        //outer rect
        outer_rect.x = ellipse.center_.x - ellipse.major_axis_radius_;
        if(outer_rect.x < 0)
        {
                outer_rect.x = 0;
        }

        outer_rect.y = ellipse.center_.y - ellipse.major_axis_radius_;
        if(outer_rect.y < 0)
        {
                outer_rect.y = 0;
        }

        outer_rect.width = outer_rect.height = ellipse.major_axis_radius_*2;
        if(outer_rect.br().x > range.width)
        {
                outer_rect.width -= (outer_rect.br().x - range.width);
        }
        if(outer_rect.br().y > range.height)
        {
                outer_rect.height -= (outer_rect.br().y - range.height);
        }


}

void FDDB::genImagePatch(Mat& img, Rect& inner_rect, Rect& outer_rect, vector<Mat>& patch_vec)
{
        Rect slide_window;
        double width = inner_rect.width;
        int height = inner_rect.height;
        double scale_step = 1.1;
        double x_step = 0.125;
        double y_step = 0.125;
        /*
        Mat img_show = img.clone();
        rectangle(img_show, inner_rect, Scalar(0,255,0),2);
        rectangle(img_show, outer_rect, Scalar(0,0,255),2);
        //imshow("inner_outer_rect", img_show);
        //waitKey();
        */
        while(1)
        {
                //平移当width<8||height<8时，就停止了，假设width，height>=4
                for(int x=outer_rect.x; (x+width)<=outer_rect.br().x; x+=cvRound(width*x_step))
                {
                        for(int y=outer_rect.y; (y+height)<=outer_rect.br().y; y+=cvRound(height*y_step))
                        {
                                slide_window = Rect(x, y, width, height);
                                /*
                                Mat img_show2 = img_show.clone();
                                rectangle(img_show2, slide_window, Scalar(0,255,255),2);
                                imshow("slide window", img_show2);
                                waitKey();
                                //*/
                                if((interUnion(slide_window, inner_rect)>0.7)/*&&cover(inner_rect, slide_window)*/)
                                {
                                        Mat patch = img(slide_window).clone();
                                        patch_vec.push_back(patch);
                                }
                        }
                }
                //如果不向上取整数，width就一直增加不上去，这里假设初始width>=5
                width = cvRound(width*scale_step);
                height = cvRound(height*scale_step);
                if( (width>outer_rect.width)||(height>outer_rect.height) )
                {
                        break;
                }
        }
}


void FDDB::loadFacePatch(string& face_patch_dir, int start_index, int end_index, vector<Mat>& img_patch_vec)
{
        for(int i=start_index; i<=end_index; i++)
        {
                char sub_dir[50];
                sprintf(sub_dir, "FDDB-fold-%d", i);

                if(face_patch_dir.empty())
                {
                        //默认目录
                        face_patch_dir =  data_dir_ + "\\" + "face_patch";
                }

                string face_patch_path =  face_patch_dir + "\\" + sub_dir;

                vector<string> img_name_vec;
                GetFileName(face_patch_path, string("jpg"), img_name_vec);

                for(size_t img_i=0; img_i<img_name_vec.size(); img_i++)
                {
                        string img_path = face_patch_path + "\\" + img_name_vec[img_i];
                        Mat img = imread(img_path);
                        if(!img.data)
                        {
                                continue;
                        }
                        img_patch_vec.push_back(img);
                }
        }
}


void genFacePatchFDDB(string& data_dir, string& save_dir)
{
        FDDB fddb(data_dir);
        fddb.genFacePatch(save_dir);
        return;
}