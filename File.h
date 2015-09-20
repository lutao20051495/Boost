
#include <string>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <Windows.h>
/*
功能:
 检查给定的文件(或者目录)是否存在.
*/
bool FileExist(const string &name);

void readFileName(const string& imgFilePath, string& type, vector<string> &imgNames);

void readImage(string& img_path, string& type, vector<Mat>& img_vec, int num = 10000000);

void readFileNameMultiDir(string& file_path, string& type, vector<string>& img_name_vec, int max_dir_num);