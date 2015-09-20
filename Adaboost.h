
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
        void Train(vector<size_t>& sample_index_vec, string& model_dir);
};

void TrainAdaboost(string& pos_sample_dir, string& neg_img_dir, string& save_model_dir);