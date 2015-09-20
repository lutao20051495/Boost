#ifndef FRAME_ZONE_H
#define FRAME_ZONE_H

#include <opencv2/opencv.hpp>
using namespace cv;

struct FrameZone
{
public:
	Rect up_boundary_rect_;
	Rect down_boundary_rect_;
public:
	FrameZone();
	FrameZone(Size main_win_size, int up_boundary_height,int down_boundary_height);

};


#endif