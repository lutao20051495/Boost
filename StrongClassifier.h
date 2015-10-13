
#include "WeakClassifier.h"
#include "Sample.h"
#include "Classifier.h"

class StrongClassifier:public Classifier
{
public:
        vector<WeakClassifier> weak_clf_vec_;
        vector<float> weak_clf_weight_vec_;
        float thresh_;
        static unsigned int max_weak_clf_num_;
public:
        StrongClassifier();
	StrongClassifier(int weak_clf_num);
        void Train(vector<size_t>& sample_index_vec, const string& save_model_dir);
	float TrainingError(vector<Sample>& sample_vec);
	float TrainingLoss(vector<Sample>& sample_vec);
	int   Predict(Sample& sample, int label_num=2);
        void Predict(Sample& sample, float& score);
        void Predict(Mat& img, float& score);
        int WeakClfNum();
        void Save(const string& model_dir);
        bool LoadModelParameter(const string& para_file_path);
        bool Load(const string& model_dir);
};
