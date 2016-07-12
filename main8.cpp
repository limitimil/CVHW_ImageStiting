#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <cassert>
#include <vector>
#include "src/KNN.h"
#include "src/RANSAC.h"
#include "src/warp.h"
#include "src/parameter.h"
#include "DevTool/util.h"
using namespace std;
using namespace cv;
int main(int argc,const char** argv)
{
	parameter p(argv[1]);
	util UTIL(true);
	p.showParameter();
	Mat img_1;
	Mat img_2;
	if(p.values[object_in]!=0){
		img_1 = imread(p.values[object_in]);
		UTIL.MSG("check parameter");
		UTIL.MSG(p.values[object_in]);
	}
	else
		img_1 = imread("testfield/object.jpg");
	if(p.values[target_in]!=0)
		img_2 = imread(p.values[target_in]);
	else
		img_2 = imread("testfield/target.jpg");

	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();

	////-- Step 1: Detect the keypoints:
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)    
	Mat descriptors_1, descriptors_2;
	f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);

	Mat feat1, feat2;
	drawKeypoints(img_1, keypoints_1, feat1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img_2, keypoints_2, feat2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	if(p.values[object_out]!=0){
		imshow(p.values[object_out], feat1);
		imwrite(p.values[object_out], feat1);
		waitKey(0);
		destroyWindow(p.values[object_out]);
	}else{
		imshow("testfield/Object_KeyPoint.jpg", feat1);
		imwrite("testfield/Object_KeyPoint.jpg", feat1);
		waitKey(0);
		destroyWindow("testfield/Object_KeyPoint.jpg");
	}
	if(p.values[target_out]!=0){
		imshow(p.values[target_out], feat2);
		imwrite(p.values[target_out], feat2);
		waitKey(0);
		destroyWindow(p.values[target_out]);
	}else{
		imshow("testfield/Target_KeyPoint.jpg", feat2);
		imwrite("testfield/Target_KeyPoint.jpg", feat2);
		waitKey(0);
		destroyWindow("testfield/Target_KeyPoint.jpg");
	}
	int key1 = keypoints_1.size();
	int key2 = keypoints_2.size();
	printf("Keypoint1=%d \nKeypoint2=%d \n", key1, key2);
	printf("Descriptor1=(%d,%d) \nDescriptor2=(%d,%d)", descriptors_1.size().height, descriptors_1.size().width, descriptors_2.size().height, descriptors_2.size().width);
	KNN_set knn(&keypoints_1,&keypoints_2,&descriptors_1,&descriptors_2);
	RANSAC r;
	Mat model,imgResult;

	knn.prepare();
	model = r.best_model(knn,img_1,img_2);//
	if(p.values[recoveryType]!=0 && strcmp(p.values[recoveryType],"backward")==0)
		RecoveryShow_backward(img_1,img_2,model,imgResult);
	else
		RecoveryShow_forward (img_1,img_2,model,imgResult);
	assert(imgResult.data != NULL);
	imshow("result", imgResult);
	if(p.values[result_out]!=0) imwrite(p.values[result_out],imgResult);
	waitKey(0);
	destroyWindow("result");
	return 0;

}
