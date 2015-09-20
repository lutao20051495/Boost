#ifndef FEATURE_H
#define FEATURE_H

#include "FeatureChannel.h"
#include "Sample.h"
enum FEATURE_TYPE{ACANGLE = 0, CENTRIST, CENTRISTLETS, ACANGLELETS};

class Feature
{
public:
        FEATURE_TYPE feat_type_;

public:
        Feature(){};

        Feature(FEATURE_TYPE feat_type);

        virtual int featureValue(const Sample& sample) const;

        virtual double AbsoluteValue(const Sample& sample)const;

        virtual void setThresh(double thresh);

        virtual bool save(string& save_dir) const;

        virtual bool load(string& src_dir);

        virtual void free(void);

        virtual void drawFeatureRect(Mat& img);

        virtual Feature* cloneNewObject(void);

        static bool genFeaturePool(Size& img_size, vector<FEATURE_TYPE>& feature_type_vec, vector<Feature>& feature_vec, int num);
        static bool genFeaturePool(Size& img_size, vector<FEATURE_TYPE>& feature_type_vec, vector<Feature*>& pfeature_vec, int num);
        static void FreeFeaturePool(vector<Feature*>& pfeature_vec, size_t exlude_index);
        static void genFeatureThreshold(vector<Sample>& sample_vec, vector<Feature*>& pfeature_vec);
        static void genFeatureThreshold(vector<Sample>& sample_vec, vector<size_t>& sample_index_vec, vector<Feature>& feature_vec);
        static void genFeatureThreshold(vector<Sample>& sample_vec, vector<size_t>& sample_index, vector<Feature*>& pfeature_vec);
        ~Feature(){};
};

#endif