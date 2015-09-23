
#include "WeakClassifier.h"
#include "Sample.h"

class StrongClassifier
{
public:
        vector<WeakClassifier> weak_clf_vec_;
        vector<float> weak_clf_weight_vec_;
        float thresh_;
public:
	StrongClassifier(int weak_clf_num);
        void Train(vector<size_t>& sample_index_vec, const string& save_model_dir);
	float TrainingError(vector<Sample>& sample_vec);
	float TrainingLoss(vector<Sample>& sample_vec);
	int   Predict(Sample& sample, int label_num=2);
};

void TrainAdaboost(const string& pos_sample_dir, const string& neg_img_dir, const string& save_model_dir);