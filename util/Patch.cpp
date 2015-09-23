#include "Patch.h"

void Patch::genRandomPatch(const vector<Mat>& img_vec, vector<Mat>& patch_vec, Size& patch_size,
                           int patch_num)
{
        int gen_patch_num = 0;
        cv::RNG rng(cv::getTickCount());
        while(gen_patch_num < patch_num)
        {
                int img_index = rng.uniform(0, img_vec.size());
                int x1 = rng.uniform(0, img_vec[img_index].cols);
                int x2 = rng.uniform(0, img_vec[img_index].cols);
                int y1 = rng.uniform(0, img_vec[img_index].rows);
                int y2 = rng.uniform(0, img_vec[img_index].rows);
                int width = abs(x1-x2)+1;
                int height = abs(y1-y2)+1;
                //any size allowed
                if((width<=0)||(height<=0))
                {
                        continue;
                }
                int x = x1<x2 ? x1 : x2;
                int y = y1<y2 ? y1 : y2;
                Rect r(x, y, width, height);

                Mat patch = (img_vec[img_index])(r).clone();
                resize(patch, patch, patch_size);
                patch_vec.push_back(patch);

                gen_patch_num++;
        }
}