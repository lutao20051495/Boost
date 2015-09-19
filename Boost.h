
#include <vector>
using namespace std;

#include "StrongClassifier.h"

class Boost
{
public:
    vector<float> sample_weight_vec_;
    StrongClassifier sclf_;
public:
    void Train();
};