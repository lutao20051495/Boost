#include "Adaboost.h"

void Adaboost::Train(vector<size_t>& sample_index_vec)
{
        sclf_.Train(sample_index_vec, weak_clf_num_);
        
        return;
}