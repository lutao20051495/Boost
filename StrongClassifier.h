
#include "WeakClassifier.h"

class StrongClassifier
{
public:
        vector<WeakClassifier> weak_clf_vec_;
        vector<float> weak_clf_weight_vec_;
        float max_err_rate_;
        float thresh_;
public:
        void Train(vector<size_t>& sample_index_vec, unsigned int weak_clf_num, const string& save_model_dir);
};