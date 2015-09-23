
#include <opencv2/opencv.hpp>
using namespace cv;

void resizeImages(vector<Mat>& img_vec, vector<Mat>& resized_img_vec, Size& dsize);

void readImages(string& img_path, string& type, vector<Mat>& img_vec);
void readImagesMultiDir(string& img_path, string& type, vector<Mat>& img_vec, int max_dir_num);

void genRandomPatch(string& img_path, vector<Mat>& patch_vec, 
                                Size& patch_size, unsigned max_patch_num);


void saveImages(vector<Mat>& img_vec, string& save_dir, int start_index = 0);

void saveImages(vector<Mat>& img_vec, string& sv_dir, vector<size_t>& sv_index_vec);

void saveFloatMat(Mat& mat, string& sv_path);

int sumValueByIntegral(const Mat& integral_img, Rect& roi);


void loadImage(const string& dir, vector<Mat>& img_vec, const string& type);

void genRandPatch(Mat& img, Mat& patch);

void genRandPatch(Mat& img, vector<Mat>& patch_vec, int patch_num);

void ResizeImages(vector<Mat>& img_vec, vector<Mat>& img_vec_resized, Size& patch_size);

void SaveImages(const vector<Mat>& img_vec, const string& prefix, int start_index, const string& type, const string& save_dir);

void GenRandPatch(const string& src_img_dir, const string& type, const string& dst_patch_dir,
						int patch_in_dir, int patch_num, unsigned int display=10000);

bool CropImagePatch(Mat& src_img, Size& crop_size, vector<Mat>& patch_vec);

void ConvertColorToGray(const string& src_img_dir, const string& dst_img_dir, const string& type);