#include "warp.h"
#include "../DevTool/utilCV.h"
#include <cassert>
Mat make_constraints(const KeyPoint& k1,const KeyPoint& k2,const KeyPoint& k3){
	Mat result(6,6,CV_32FC1);
	Mat_<float> result_ = result;
	result.setTo(0.0);
	result_(0,0) = result_(1,3) = k1.pt.x;
	result_(0,1) = result_(1,4) = k1.pt.y;
	result_(2,0) = result_(3,3) = k2.pt.x;
	result_(2,1) = result_(3,4) = k2.pt.y;
	result_(4,0) = result_(5,3) = k3.pt.x;
	result_(4,1) = result_(5,4) = k3.pt.y;
	result_(0,2) = result_(1,5) =
	result_(2,2) = result_(3,5) =
	result_(4,2) = result_(5,5) = 1;
	return result;
}
Mat make_offset(const KeyPoint& k1,const KeyPoint& k2,const KeyPoint& k3){
	
	Mat result(6,1,CV_32FC1);
	Mat_<float> result_ = result;
	result_(0) = k1.pt.x;
	result_(1) = k1.pt.y;
	result_(2) = k2.pt.x;
	result_(3) = k2.pt.y;
	result_(4) = k3.pt.x;
	result_(5) = k3.pt.y;
	return result;
}
void RecoveryShow_forward(const Mat& img1,const Mat& img2,Mat& f,Mat& imgResult){ //the third parameter is map function.
//stage5.0.1 use forward mapping
	img2.copyTo(imgResult);
	Point2f coordinate2; //target, no use now
	Point2f coordinate1; //object
	for(int r=0;r< img1.rows;r++)
	for(int c=0;c< img1.cols;c++){
		if(! notWhite(img1.at<Vec3b>(r,c))) continue;
		coordinate2 = do_mul(f,c,r);
		if(	coordinate2.x < img2.cols-1 &&
			coordinate2.x >= 0 &&
			coordinate2.y < img2.rows-1 &&
			coordinate2.y >=  0 
		)
			imgResult.at<Vec3b>(coordinate2) = img1.at<Vec3b>(r,c);	
	}
	assert(imgResult.data != NULL);
	//imshow("remap result",imgResult);
	//waitKey(0);	
	//destroyWindow("remap result");
}
void RecoveryShow_backward(const Mat& img1,const Mat& img2,Mat& f,Mat& imgResult){ //the third parameter is map function.
	img2.copyTo(imgResult);

	Mat model = model_inv(f);//
	Point2f coordinate2; //target, no use now
	Point2f coordinate1; //object
	for(int r=0;r< img2.rows;r++)
	for(int c=0;c< img2.cols;c++){
		coordinate1 = do_mul_backward(model,c,r);
		if(	coordinate1.x < img1.cols-1 &&
			coordinate1.x >= 0 &&
			coordinate1.y < img1.rows-1 &&
			coordinate1.y >=  0 &&
			notWhite(img1.at<Vec3b>(coordinate1))
		)
			imgResult.at<Vec3b>(r,c) = img1.at<Vec3b>(coordinate1);	
	}
	assert(imgResult.data != NULL);
	//imshow("remap result",imgResult);
	//waitKey(0);	
	//destroyWindow("remap result");
}
Point2f do_mul(Mat& f,float x,float y){
	assert(f.rows == 6);
	assert(f.cols == 1);
	Point2f result;
	result.x = f.at<float>(0) * x + f.at<float>(1) * y + f.at<float>(2);
	result.y = f.at<float>(3) * x + f.at<float>(4) * y + f.at<float>(5);
	return result;
	
}
bool notWhite(Vec3b pixel){
	if(pixel[0]==pixel[1]&& pixel[2] == 255 && pixel[0]==pixel[2]) return false;
	else 
		return true;
}
Mat model_inv(Mat& f){
	utilCV UTIL(false);
	UTIL.showCVobj(f,"f");
	assert(f.rows == 6);
	Mat tmp(3,3,CV_32FC1);

	tmp.setTo(0.0);
	Mat(f.rowRange(0,3).t()).copyTo(tmp.row(0));
	Mat(f.rowRange(3,6).t()).copyTo(tmp.row(1));
	tmp.at<float>(2,2) = 1;
	UTIL.showCVobj(tmp,"tmp");
	return tmp.inv();
}
Point2f do_mul_backward(Mat& m,float x,float y){
	Mat_<float> tmp(Mat(3,1,CV_32FC1));
	Point2f result;

	tmp(0) = x;tmp(1) = y;tmp(2) = 1;
	tmp = m* tmp;
	result.x = tmp(0);
	result.y = tmp(1);
	
	return result;
}
