
#include <string>
using namespace std;

#include "Classifier.h"

class ModelEvaluation
{
public:
	Classifier&	clf_;
	float thresh_;
	int display_;
	int max_test_num_;
	bool load_all_neg_img_;
	int sample_patch_num_;
	Size sample_size_normalized_;
	Size crop_size_;
	Size patch_size_;
public:
	ModelEvaluation(Classifier& clf, 
			unsigned int max_test_num,
			float thresh);
	float calcFpr(const string& neg_img_dir);
	float calcMr(const string& pos_patch_dir, unsigned int& sum_num, unsigned int& err_num, bool show=true);
	float calcMrMultiDir(const string& pos_patch_dir, const string& prefix);
	bool Positive(float score);
};

void Evaluate( int option,
		const string& dir, 
		const string& sub_dir,
		float thresh,
		const string& para_file_path);

