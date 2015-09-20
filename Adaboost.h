
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
    void Train(vector<size_t>& sample_index_vec);
};