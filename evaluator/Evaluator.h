
#include <string>
using namespace std;

#include "../Classifier.h"

class Evaluator
{
public:
	Classifier*     pclf_;
	float thresh_;
	int display_;
	int max_test_num_;
	bool load_all_neg_img_;
	int sample_patch_num_;
	Size sample_size_normalized_;
	Size crop_size_;
	Size patch_size_;

        //detection
        Size min_object_size_;
        Size max_object_size_;
        float scale_step_;
	float shift_step_;
public:
        Evaluator();
        bool LoadParameter(const string& para_file_path);
        void Run(int argc,char *argv[]);
        void Train(int argc, char* argv[]);
        void TrainAdaboost(const string& pos_sample_dir, const string& neg_img_dir, const string& save_model_dir,
                const string& para_file_path);
        bool CreateClassifier(const string& clf_type, const string& model_dir);
        void TestMr(int argc, char* argv[]);
        float MissRateMultiDir(const string& pos_patch_dir, const string& prefix, const string& img_type, unsigned int& sum_num, unsigned int& err_num);
        float MissRate(const string& pos_patch_dir, const string& img_type, unsigned int& sum_num, unsigned int& err_num, bool show=true);
        bool Positive(float score);
        void TestFpr(int argc, char* argv[]);
	float FalsePositiveRate(const string& neg_img_dir, const string& img_type, unsigned int& sum, unsigned int& err);
        float FalsePositiveRateMultiDir(const string& neg_img_dir, const string& sub_dir, const string& img_type, 
                unsigned int& sum, unsigned int& err);
	
        void DetectOnImage(int argc, char* argv[]);
	void DetectOnImageMultiScale(const string& src_dir, const string& type, const string& save_dir, const bool show=false);
	void DetectOnImageMultiScale(const Mat& img, vector<Rect>& rect_vec);
	template <typename Dtype>
	void DetectOnImageSingleScale(const Dtype& input, vector<Rect>& rect_vec, int shift_step_x, int shift_step_y);

        void DetectInVideo(int argc, char* argv[]);
        void DetectInVideo(const string& src_dir, const string& type, const string& save_dir, const bool show=false);
        void DetectInVideo(const string& video_name, const string& save_video_name, bool show=false);
};

