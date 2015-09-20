#include "FeatureChannel.h"

FeatureChannel::FeatureChannel(void)
{
}


FeatureChannel::FeatureChannel(Mat& gray_img)
{
        gray_img_ = gray_img.clone();
        calGradientImage();
        //integral(gray_img_, integral_gray_img_, CV_32SC1);
        integral(gradient_mag_img_, integral_mag_img_, CV_32SC1);
        //calGradientImageOnMag();
        //calCtImage();
}

void FeatureChannel::calFeatureChannel(Mat& gray_img)
{
        gray_img.copyTo(gray_img_);
        //cv::integral(gray_img_, integral_gray_img_);
        calGradientImage();
        integral(gradient_mag_img_, integral_mag_img_, CV_32SC1);
        //calGradientImageOnMag();
        //calCtImage();
}


void FeatureChannel::calIntegralImage(void)
{
        if(!(gray_img_.data))
        {
                cout << "no image data" << endl;
                return;
        }
        cv::integral(gray_img_, integral_gray_img_);
        return;
}



void FeatureChannel::calGradient(int dy, int dx, int &magnitude, int &angle)
{
	if(dx==0 && dy==0)
	{
		angle = 100;
		magnitude	= 0;
		return;
	}

	magnitude = cvRound( sqrt(  ( 1.0f*dy*dy*dy*dy+1.0f*dx*dx*dx*dx )/(1.0f*dy*dy+1.0f*dx*dx ) ) );//注意溢出问题
	
	

	if (dx!=0 && dy==0)
	{
		angle = -90;

	}else
	{
		angle = cvRound( atan(1.0f*dx/dy) / CV_PI * 180  ) ;
	}
	
	return;
}


/*
功能: 计算一张灰度图的梯度图	
*/
void FeatureChannel::calGradientImage(void)
{
	
	gradient_mag_img_ = Mat(gray_img_.size(),CV_8UC1, Scalar(0,0,0));
        gradient_angle_img_ = Mat(gray_img_.size(),CV_8SC1,Scalar(100,100,100));
	for (int r=1;	r<gray_img_.rows-1;	r++)
	{
		for (int c=1;	c<gray_img_.cols-1;	c++)
		{

			int dx = int( gray_img_.at<uchar>(r,c+1) ) - int( gray_img_.at<uchar>(r,c-1) ); 
			int dy = int( gray_img_.at<uchar>(r+1,c) ) - int( gray_img_.at<uchar>(r-1,c) );
			int magnitude;
			int angle;
			calGradient(dy,dx,magnitude,angle);
			gradient_mag_img_.at<uchar>(r,c) = uchar( magnitude );
                        if (magnitude>0)
                        {
                                gradient_angle_img_.at<char>(r,c) = char( angle );
                        }else
                        {
                                gradient_angle_img_.at<char>(r,c) = char( 100 );
                        }
                        
		}//for c
	}//for r	
}

/*
 在梯度图上计算梯度方向图
*/
void FeatureChannel::calGradientImageOnMag(void)
{
	
	gradient_mag_mag_img_ = Mat(gray_img_.size(),CV_8UC1, Scalar(0,0,0));
        gradient_mag_angle_img_ = Mat(gray_img_.size(),CV_8SC1,Scalar(100,100,100));
	for (int r=1;	r<gray_img_.rows-1;	r++)
	{
		for (int c=1;	c<gray_img_.cols-1;	c++)
		{

			int dx = int( gray_img_.at<uchar>(r,c+1) ) - int( gray_img_.at<uchar>(r,c-1) ); 
			int dy = int( gray_img_.at<uchar>(r+1,c) ) - int( gray_img_.at<uchar>(r-1,c) );
			int magnitude;
			int angle;
			calGradient(dy,dx,magnitude,angle);
			gradient_mag_mag_img_.at<uchar>(r,c) = uchar( magnitude );
                        if (magnitude>0)
                        {
                                gradient_mag_angle_img_.at<char>(r,c) = char( angle );
                        }else
                        {
                                gradient_mag_angle_img_.at<char>(r,c) = char( 100 );
                        }
                        
		}//for c
	}//for r	
}


void FeatureChannel::calCtImage(void)
{
	ct_img_ = Mat(gradient_mag_img_.size(), CV_8UC1,Scalar(0,0,0));

        for (int r=1; r<gradient_mag_img_.rows-1; r++ )
        {
                for (int c=1; c<gradient_mag_img_.cols-1; c++)
                {
                        int ct_value = 0;
                        int cntr = gradient_mag_img_.at<uchar>(r,c);

                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r,  c+1)) << 0;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r+1,c+1)) << 1;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r+1,c  )) << 2;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r+1,c-1)) << 3;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r,  c-1)) << 4;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r-1,c-1)) << 5;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r-1,c  )) << 6;
                        ct_value |= (cntr < gradient_mag_img_.at<uchar>(r-1,c+1)) << 7;

/*
                        if (cntr < gradient_mag_img_.at<uchar>(r,  c+1   ) )  ct_value += 0x80;
                        if (cntr < gradient_mag_img_.at<uchar>(r+1,c+1   ) )  ct_value += 0x40;
                        if (cntr < gradient_mag_img_.at<uchar>(r+1,c     ) )  ct_value += 0x20;
                        if (cntr < gradient_mag_img_.at<uchar>(r+1,c-1   ) )  ct_value += 0x10;
                        if (cntr < gradient_mag_img_.at<uchar>(r,  c-1   ) )  ct_value += 0x08;
                        if (cntr < gradient_mag_img_.at<uchar>(r-1,c-1   ) )  ct_value += 0x04;
                        if (cntr < gradient_mag_img_.at<uchar>(r-1,c     ) )  ct_value += 0x02;
                        if (cntr < gradient_mag_img_.at<uchar>(r-1,c+1   ) )  ct_value += 0x01;
*/
                        ct_img_.at<uchar>(r,c) = ct_value;
                        
                }
        }
}


void FeatureChannel::cropTo(FeatureChannel& patch_fc, Rect& roi)
{
        (this->gray_img_)(roi).copyTo(patch_fc.gray_img_);

        Rect expand_roi(roi.x, roi.y, roi.width+1, roi.height+1);
        //(this->integral_gray_img_)(expand_roi).copyTo(patch_fc.integral_gray_img_);

        (this->gradient_mag_img_)(roi).copyTo(patch_fc.gradient_mag_img_);
        (this->gradient_angle_img_)(roi).copyTo(patch_fc.gradient_angle_img_);
        //(this->gradient_mag_mag_img_)(roi).copyTo(patch_fc.gradient_mag_mag_img_);
        //(this->gradient_mag_angle_img_)(roi).copyTo(patch_fc.gradient_mag_angle_img_);
}


void FeatureChannel::getPatch(Rect& roi, FeatureChannel& patch_fc)
{
        Rect expand_roi(roi.x, roi.y, roi.width+1, roi.height+1);
        //浅拷贝
        patch_fc.gray_img_ = gray_img_(roi);
        //patch_fc.integral_gray_img_ = integral_gray_img_(expand_roi);
        patch_fc.integral_mag_img_ =  integral_mag_img_(expand_roi);
        patch_fc.gradient_mag_img_ =  gradient_mag_img_(roi);
        patch_fc.gradient_angle_img_ =  gradient_angle_img_(roi);

        //patch_fc.gradient_mag_mag_img_ = gradient_mag_mag_img_(roi);
        //patch_fc.gradient_mag_angle_img_ = gradient_mag_angle_img_(roi);


}