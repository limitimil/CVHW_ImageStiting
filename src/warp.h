#ifndef __WARP__H__
#define __WARP__H__
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
using namespace cv;
using std::vector;
Mat make_constraints(const KeyPoint&,const KeyPoint&,const KeyPoint&);
Mat make_offset(const KeyPoint&,const KeyPoint&,const KeyPoint&);
void RecoveryShow_backward(const Mat&,const Mat&,Mat&,Mat&);
void RecoveryShow_forward(const Mat&,const Mat&,Mat&,Mat&);
bool notWhite(Vec3b pixel);
Point2f do_mul(Mat&,float,float); //design for making Mat* point be done
Mat model_inv(Mat&);
Point2f do_mul_backward(Mat&,float,float);
#endif
