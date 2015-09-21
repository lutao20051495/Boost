#include <string>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>
#include "File.h"
/*
¹¦ÄÜ:
 ¼ì²é¸ø¶¨µÄÎÄ¼þ(»òÕßÄ¿Â¼)ÊÇ·ñ´æÔÚ.
*/

bool FileExist(const string &name)
{
#ifdef WIN32
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
#else
	if (opendir(name.c_str()))
	{
		return true; 
	}
	else
	{
		return false;
	}
#endif
}

/* Get file name extension */
string getFileNameExt(const string& file_name)
{
	return file_name.substr(file_name.find_last_of('.')+1);
}

/* Returns a list of files in a directory (except the ones that begin with a dot) */
bool GetFileName(const string &directory, const string& type, std::vector<string> &out)
{
#ifdef WIN32
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
	{
		cout << "no file found!" << endl;
		return false; 
	}
	do {
		const string file_name = file_data.cFileName;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;
	
		if (getFileNameExt(file_name)==type)
		{
			out.push_back(file_name);
		}
	} while (FindNextFile(dir, &file_data));
	FindClose(dir);
    
#else
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    if(!dir)
    {
	    cout << "no files found in " << directory << endl;
	    return false;
    }
    while ((ent = readdir(dir)) != NULL) 
    {
	    const string file_name = ent->d_name;

	    if (file_name[0] == '.')
		continue;
		
	    const string full_file_name = directory + "/" + file_name;
	    if (stat(full_file_name.c_str(), &st) == -1)
		continue;

	    const bool is_directory = (st.st_mode & S_IFDIR) != 0;

	    if (is_directory)
		continue;
	
	    if (getFileNameExt(file_name)==type)
	    {
		    out.push_back(file_name);
	    }
    }
    closedir(dir);
#endif
    return true;
} // GetFilesInDirectory


//¶ÁÈ¡×ÓÎÄ¼þ¼ÐÏÂµÄÎÄ¼þÃû
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
                GetFileName(dir, type, img_name_vec);
                for(size_t i=0; i<img_name_vec.size(); i++)
                {
                        string file_name = dir + "//" + img_name_vec[i];
                        img_name_vec.push_back(file_name);
                }

        }
}


//µÝ¹é¶ÁÈ¡µ±Ç°ÎÄ¼þÏÂµÄËùÓÐÎÄ¼þ
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
                //ÕÒµ½Ò»¸öÎÄ¼þ£¬strFoundFileNameÎªÎÄ¼þÃû
                //ÔÚ´ËÌí¼Ó´¦Àí
            }   
        }
         
    }
     
    tempFind.Close();
}
*/

void readImage(const string& img_path, const string& type, vector<Mat>& img_vec, int num)
{
        vector<string> img_name_vec;
        GetFileName(img_path, type, img_name_vec);
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



void CreateDir(const string& dir_name)
{
#ifdef WIN32
        CreateDirectory(dir_name.c_str(), NULL);
#else
        mkdir(dir_name.c_str(), 777);
#endif
        return;
}






