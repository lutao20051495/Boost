
#include <vector>
using namespace std;

#include "StrongClassifier.h"
#include "Sample.h"

class Adaboost
{
public:
        vector<float> sample_weight_vec_;
        StrongClassifier sclf_;
        unsigned int weak_clf_num_;
public:
        Adaboost(unsigned int weak_clf_num);
        void Train(vector<size_t>& sample_index_vec, const string& model_dir);
};

void TrainAdaboost(const string& pos_sample_dir, const string& neg_img_dir, const string& save_model_dir);