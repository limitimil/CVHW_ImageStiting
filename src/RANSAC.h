#ifndef __RANSAC__H__
#define __RANSAC__H__
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include "KNN.h"
using namespace cv;
using std::vector;
class RANSAC{
	const vector<KeyPoint>* points;
	float thredshold = 100.0;
public:
	RANSAC(vector<KeyPoint>*);
	RANSAC();
	void myline(Mat& );
	Mat make_model(const KeyPoint&,const KeyPoint&);
	Mat best_model(KNN_set& knn,const Mat&,const Mat&);
	float calc_distance(Mat&,const KeyPoint&);
};

#endif
