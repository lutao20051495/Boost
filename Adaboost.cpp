#include "Adaboost.h"
#include "FDDB.h"
#include "File.h"


Adaboost::Adaboost(unsigned int weak_clf_num)
{
        weak_clf_num_ = weak_clf_num;
}

void Adaboost::Train(vector<size_t>& sample_index_vec, string& save_model_dir)
{
        sclf_.Train(sample_index_vec, weak_clf_num_, save_model_dir);
        return;
}


void TrainAdaboost(string& pos_sample_dir, string& neg_img_dir, string& save_model_dir)
{
        //read pos sample
        vector<Sample> pos_sample_vec;
        FDDB fddb(pos_sample_dir);
        vector<Mat> pos_img_vec;
        fddb.loadFacePatch(string(""), 1, 1, pos_img_vec);
        Sample::genSample(pos_img_vec, 1, pos_sample_vec);
        if (pos_sample_vec.size()<=0)
        {
                cout << "pos sample num: " << pos_sample_vec.size() << endl;
                return;
        }

        Size sample_size = pos_sample_vec[0].getSize();
        //gene neg image
        vector<Mat> neg_img_vec;
        readImage(neg_img_dir, string("jpg"), neg_img_vec, 100);
        vector<Sample> neg_sample_vec;
        Sample::genRandomNegSample(neg_img_vec, neg_sample_vec, sample_size, 0, pos_sample_vec.size()*2);

        vector<Sample>& train_data_vec = Sample::train_sample_vec_;
        train_data_vec = pos_sample_vec;
        train_data_vec.insert(train_data_vec.end(), neg_sample_vec.begin(), neg_sample_vec.end());

        //gene sample index
        vector<size_t> sample_index_vec(0);
        for (size_t i=0; i<train_data_vec.size(); i++)
        {
                sample_index_vec.push_back(i);
        }

        Adaboost adaboost(10);
        adaboost.Train(sample_index_vec, save_model_dir);
}