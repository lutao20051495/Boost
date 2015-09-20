
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