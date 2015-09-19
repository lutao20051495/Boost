
#include <vector>
using namespace std;

#include "WeakClassifier.h"

class Boost
{
public:
    vector<float> sample_weight_vec_;
    vector<WeakClassifier> weak_clf_vec_;
    unsigned int weak_clf_num_;
    float thresh_;
public:
    void Train();
};