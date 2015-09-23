#include "FrameZone.h"


FrameZone::FrameZone()
{
	;
}
FrameZone::FrameZone(Size main_win_size, int up_boundary_height, int down_boundary_height)
{
	up_boundary_rect_    = Rect(0, 0, main_win_size.width, up_boundary_height);
	down_boundary_rect_  = Rect(0, main_win_size.height- down_boundary_height,main_win_size.width, down_boundary_height);
	//mid_line_ = Rect(0, up_boundary_height, main_win_size.width,  main_win_size.height-up_boundary_height-down_boundary_height);
}