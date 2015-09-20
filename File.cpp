#include <string>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;
#include "File.h"
/*
功能:
 检查给定的文件(或者目录)是否存在.
*/

bool FileExist(const string &name)
{

	WIN32_FIND_DATAA   FindFileData; 
	HANDLE   hFind   =   INVALID_HANDLE_VALUE;

	hFind = FindFirstFileA(name.c_str(),&FindFileData);        
	if(hFind  !=INVALID_HANDLE_VALUE )
	{
		return true;
	}else
	{
		return false;
	}
}

// 内存不足，图像逐一读入
void readFileName(const string& imgFilePath,string& type,vector<string> &imgNames)
{	
	imgNames.clear();

	char tmpDirSpec[MAX_PATH+1];
        sprintf (tmpDirSpec, "%s/*.%s", imgFilePath.c_str(), type.c_str());

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile(tmpDirSpec , &f);

	if(h != INVALID_HANDLE_VALUE)
	{
                if(type == "*")
                {
		        FindNextFile(h, &f);	//read ..
		        FindNextFile(h, &f);	//read .
                }
		do
		{
			imgNames.push_back(f.cFileName);
		} while(FindNextFile(h, &f));

	}
	FindClose(h);	
}


//读取子文件夹下的文件名
void readFileNameMultiDir(string& file_path, string& type, vector<string>& img_name_vec, int max_dir_num)
{
        for(int i=0; i<max_dir_num; i++)
        {
                char buf[50];
                sprintf(buf, "sample_%d", i);
                string dir = file_path + "//" + string(buf);
                if( !FileExist(dir))
                        continue;
                vector<string> img_name_vec;
                readFileName(dir, type, img_name_vec);
                for(size_t i=0; i<img_name_vec.size(); i++)
                {
                        string file_name = dir + "//" + img_name_vec[i];
                        img_name_vec.push_back(file_name);
                }

        }
}


//递归读取当前文件下的所有文件
/*
void readAllFileName(string& file_path, string& type, vector<string>& file_name_vec)
{
    CFileFind tempFind;
    char strTempFileFind[MAX_PATH];
     
    sprintf(strTempFileFind,"%s\\*.*", strDirName);
     
    BOOL IsFinded = tempFind.FindFile(strTempFileFind);
    while (IsFinded)
    {
        IsFinded = tempFind.FindNextFile();
         
        if (!tempFind.IsDots()) 
        {
            char strFoundFileName[MAX_PATH];
            strcpy(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));
             
            if (tempFind.IsDirectory())
            {
                char strTempDir[MAX_PATH];
                sprintf(strTempDir,"%s\\%s", strDirName, strFoundFileName);
                ParserDirectory(strTempDir);
            }
            else   
            {
                //找到一个文件，strFoundFileName为文件名
                //在此添加处理
            }   
        }
         
    }
     
    tempFind.Close();
}
*/

void readImage(string& img_path, string& type, vector<Mat>& img_vec, int num)
{
        vector<string> img_name_vec;
        readFileName(img_path, type, img_name_vec);
        for(unsigned i=0; i<img_name_vec.size(); i++)
        {
                string img_name = img_path + "\\" + img_name_vec[i];
                Mat img = imread(img_name, 0);
                if(img.data)
                        img_vec.push_back(img);

                if(img_vec.size() >= num)
                        break;
        }
}






