#include "Classifier.h"

void Classifier::Train(vector<size_t>& sample_index_vec, const string& save_model_dir)
{
        return;
}
void Classifier::Predict(const Mat& img, float& score)
{
	return;
}

void Classifier::Predict(const Sample& img, float& score)
{
	return;
}

bool Classifier::LoadModelParameter(const string& para_file_path)
{
        return true;
}

void Classifier::Save(const string& model_dir)
{
        return;
}

bool Classifier::Load(const string& model_dir)
{
        return true;
}

bool Classifier::InputRaw()
{
        return true;
}