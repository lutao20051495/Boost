
#include <string>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#ifdef WINDOWS
    #include <windows.h>
#else
    #include <sys/types.h>
    #include <dirent.h>
    #include <sys/stat.h>
#endif 
/*
¹¦ÄÜ:
 ¼ì²é¸ø¶¨µÄÎÄ¼þ(»òÕßÄ¿Â¼)ÊÇ·ñ´æÔÚ.
*/
bool FileExist(const string &name);

void readFileName(const string& imgFilePath, string& type, vector<string> &imgNames);

void readImage(const string& img_path, const string& type, vector<Mat>& img_vec, int num = 10000000);

void readFileNameMultiDir(string& file_path, string& type, vector<string>& img_name_vec, int max_dir_num);