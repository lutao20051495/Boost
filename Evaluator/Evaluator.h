
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
};

