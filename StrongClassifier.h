
#include "WeakClassifier.h"

class StrongClassifier
{
public:
    vector<WeakClassifier> weak_classifier_vec_;
    float thresh_;
};