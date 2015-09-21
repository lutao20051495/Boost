#include <stdio.h>
#include "FeatureChannel.h"

int FeatureChannel::feature_channel_num_ = 8;

FeatureChannel::FeatureChannel(void)
{
}


FeatureChannel::FeatureChannel(Mat& gray_img)
{
        /*
        gray_img_ = gray_img.clone();
        calGradientImage();
        //integral(gray_img_, integral_gray_img_, CV_32SC1);
        integral(gradient_mag_img_, integral_mag_img_, CV_32SC1);
        //calGradientImageOnMag();
        //calCtImage();
        */
        feature_channel_vec_.resize(feature_channel_num_);
        calIntegralImage(gray_img, feature_channel_vec_[0]);
        Mat mag_img, angle_img;
        calGradientImage(gray_img, mag_img, angle_img);
        //imshow("mag", mag_img);
        calIntegralImage(mag_img, feature_channel_vec_[1]);

        vector<Mat> angle_img_vec;
        splitAngleImage(angle_img, mag_img, angle_img_vec);
        for(size_t i=0; i<angle_img_vec.size(); i++)
        {
                char name[100];
                sprintf(name, "anlge image %d", i);
                //imshow(string(name), angle_img_vec[i]);
                calIntegralImage(angle_img_vec[i], feature_channel_vec_[2+i]);
        }
        //waitKey();
}


/*
void FeatureChannel::calFeatureChannel(Mat& gray_img)
{
        gray_img.copyTo(gray_img_);
        //cv::integral(gray_img_, integral_gray_img_);
        calGradientImage();
        integral(gradient_mag_img_, integral_mag_img_, CV_32SC1);
        //calGradientImageOnMag();
        //calCtImage();
}
*/

void FeatureChannel::calIntegralImage(Mat& gray_img, Mat& integral_gray_img)
{
        if(!(gray_img.data))
        {
                cout << "no image data" << endl;
                return;
        }
        cv::integral(gray_img, integral_gray_img);
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

	magnitude = cvRound( sqrt(  ( 1.0f*dy*dy*dy*dy+1.0f*dx*dx*dx*dx )/(1.0f*dy*dy+1.0f*dx*dx ) ) );//×¢ÒâÒç³öÎÊÌâ
	
	

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
¹¦ÄÜ: ¼ÆËãÒ»ÕÅ»Ò¶ÈÍ¼µÄÌÝ¶ÈÍ¼	
*/
void FeatureChannel::calGradientImage(Mat& gray_img, Mat& mag_img, Mat& angle_img)
{
	
	mag_img = Mat(gray_img.size(),CV_8UC1, Scalar(0,0,0));
        angle_img = Mat(gray_img.size(),CV_8SC1,Scalar(100,100,100));
	for (int r=1;	r<gray_img.rows-1;	r++)
	{
		for (int c=1;	c<gray_img.cols-1;	c++)
		{

			int dx = int( gray_img.at<uchar>(r,c+1) ) - int( gray_img.at<uchar>(r,c-1) ); 
			int dy = int( gray_img.at<uchar>(r+1,c) ) - int( gray_img.at<uchar>(r-1,c) );
			int magnitude;
			int angle;
			calGradient(dy,dx,magnitude,angle);
			mag_img.at<uchar>(r,c) = uchar( magnitude );
                        if (magnitude>0)
                        {
                                angle_img.at<char>(r,c) = char( angle );
                        }else
                        {
                                angle_img.at<char>(r,c) = char( 100 );
                        }
                        
		}//for c
	}//for r	
}

/*
 ÔÚÌÝ¶ÈÍ¼ÉÏ¼ÆËãÌÝ¶È·½ÏòÍ¼
*/
/*
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
*/

/*
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


                        //if (cntr < gradient_mag_img_.at<uchar>(r,  c+1   ) )  ct_value += 0x80;
                        //if (cntr < gradient_mag_img_.at<uchar>(r+1,c+1   ) )  ct_value += 0x40;
                        //if (cntr < gradient_mag_img_.at<uchar>(r+1,c     ) )  ct_value += 0x20;
                        //if (cntr < gradient_mag_img_.at<uchar>(r+1,c-1   ) )  ct_value += 0x10;
                        //if (cntr < gradient_mag_img_.at<uchar>(r,  c-1   ) )  ct_value += 0x08;
                        //if (cntr < gradient_mag_img_.at<uchar>(r-1,c-1   ) )  ct_value += 0x04;
                        //if (cntr < gradient_mag_img_.at<uchar>(r-1,c     ) )  ct_value += 0x02;
                        //if (cntr < gradient_mag_img_.at<uchar>(r-1,c+1   ) )  ct_value += 0x01;

                        //ct_img_.at<uchar>(r,c) = ct_value;
                        
                }
        }
}
*/

 /*
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
*/


void FeatureChannel::splitAngleImage(Mat& angle_img, Mat& mag_img, vector<Mat>& angle_img_vec)
{
        //·ÖÎª6¸öbin
        int bin = 6;
        angle_img_vec.resize(6);
        for(size_t i=0; i<bin; i++)
        {
                angle_img_vec[i] = Mat(angle_img.size(), CV_8UC1, Scalar(0));
        }
        int per_bin_angle = 180/bin;
        
        for(int r=0; r<angle_img.rows; r++)
        {
                for(int c=0; c<angle_img.cols; c++)
                {
                        int index = (angle_img.at<char>(r,c)+90)/per_bin_angle;
                        if(index >= bin)
                        {
                                continue;
                        }
                        angle_img_vec[index].at<uchar>(r,c) = mag_img.at<uchar>(r,c);
                }
        }
        
}


void FeatureChannel::getPatch(Rect& roi, FeatureChannel& patch_fc)
{
        Rect expand_roi(roi.x, roi.y, roi.width+1, roi.height+1);
        //Ç³¿½±´
        //patch_fc.gray_img_ = gray_img_(roi);
        //patch_fc.integral_gray_img_ = integral_gray_img_(expand_roi);
        //patch_fc.integral_mag_img_ =  integral_mag_img_(expand_roi);
        //patch_fc.gradient_mag_img_ =  gradient_mag_img_(roi);
        //patch_fc.gradient_angle_img_ =  gradient_angle_img_(roi);

        //patch_fc.gradient_mag_mag_img_ = gradient_mag_mag_img_(roi);
        //patch_fc.gradient_mag_angle_img_ = gradient_mag_angle_img_(roi);

        patch_fc.feature_channel_vec_.resize(feature_channel_vec_.size());
        for(size_t i=0; i<patch_fc.feature_channel_vec_.size(); i++)
        {
                patch_fc.feature_channel_vec_[i] = feature_channel_vec_[i](expand_roi);
        }

}


Size FeatureChannel::getSize()
{
        return Size(feature_channel_vec_[0].cols-1, feature_channel_vec_[0].rows-1);
}


