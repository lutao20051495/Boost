#include <stdio.h>
#include "StrongClassifier.h"
#include "File.h"


StrongClassifier::StrongClassifier()
{
        max_err_rate_= 0.45f;
}

void StrongClassifier::Train(vector<size_t>& sample_index_vec, unsigned int weak_clf_num, const string& save_model_dir)
{
        //create saving dir
        if(!FileExist(save_model_dir))
        {
                CreateDir(save_model_dir);
        }

        vector<Sample>& train_sample_vec = Sample::train_sample_vec_;
        //init weight
        vector<float> sample_weight_vec(sample_index_vec.size(), 1.0f/sample_index_vec.size());

        unsigned int train_weak_clf_num = 0;
        while(weak_clf_vec_.size()<weak_clf_num &&
                        train_weak_clf_num<weak_clf_num*5)
        {
                WeakClassifier weak_clf;
                weak_clf.Train(sample_index_vec, sample_weight_vec);
                train_weak_clf_num++;
                cout << "train the " << weak_clf_vec_.size() << "th weak clf" << endl;
                cout << "training weak clf err_rate: " << weak_clf.err_rate_ << endl << endl;
                if (weak_clf.pfeature_&&weak_clf.err_rate_<max_err_rate_)
                {
                        weak_clf_vec_.push_back(weak_clf);
                        char sv_dir[128];
                        sprintf(sv_dir, "%s/weak_clf_%d", save_model_dir.c_str(), weak_clf_vec_.size());
                        weak_clf.Save(string(sv_dir));
                }
                else
                {
                        weak_clf.FreeFeature();
                        continue;
                }

                //classifier weight
                float clf_weight = 0.0f;
                if (weak_clf.err_rate_>0)
                {
                        clf_weight = 0.5f*log((1-weak_clf.err_rate_)/weak_clf.err_rate_);
                        weak_clf_weight_vec_.push_back(clf_weight);
                }
                else
                {
                        break;
                }

                //update sample weight
                for(unsigned int i=0; i<sample_index_vec.size(); i++)
                {
                        size_t index = sample_index_vec[i];
                        Sample& sample = train_sample_vec[index];
                        sample_weight_vec[index] = sample_weight_vec[index]*
                                                        exp(-clf_weight*sample.label_*weak_clf.Predict(sample));
                }

                //normalize sample weight
                float sum_weight = 0.0;
                for(unsigned int i=0; i<sample_weight_vec.size(); i++)
                {
                        sum_weight += sample_weight_vec[i];
                }
                for(unsigned int i=0; i<sample_weight_vec.size(); i++)
                {
                        sample_weight_vec[i] /= sum_weight;
                }
        }
}
