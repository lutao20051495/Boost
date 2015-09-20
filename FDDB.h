
#include <string>
using namespace std;


typedef struct ellipse
{
        double major_axis_radius_;
        double minor_axis_radius_;
        double angle_;
        Point2f center_;
} FDDB_Ellipse;

/* Face Detection Data Set and Benchmark */
class FDDB
{
public:
        string data_dir_;
        Size   save_sample_size_;
public:
        FDDB(string& data_dir);
        void genFacePatch(string& dst_dir);
        bool genFacePatch(string& anno_file_path, string& img_dir, vector<Mat>& img_vec);
        bool genFacePatch(Mat& img, FDDB_Ellipse& ellipse, vector<Mat>& img_vec);
        void genInnerOuterRect(FDDB_Ellipse& ellipse, Rect& inner_rect, Rect& outer_rect, Size& range);
        void genImagePatch(Mat& img, Rect& inner_rect, Rect& outer_rect, vector<Mat>& patch_vec);
        void loadFacePatch(string& face_patch_dir, int start_index, int end_index, vector<Mat>& img_patch_vec);
};

void genFacePatchFDDB(string& data_dir, string& save_dir);