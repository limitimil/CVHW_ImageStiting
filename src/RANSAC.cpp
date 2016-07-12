#include <cmath>
#include <cstdlib>
#include <limits>
#include <cassert>
#include <opencv2/imgproc.hpp>
#include "RANSAC.h"
#include "../DevTool/combination.h"
#include "../DevTool/utilCV.h"
#include "warp.h"
#include "scoreBoard.h"
using namespace cv;
using namespace std;
RANSAC::RANSAC() {}
RANSAC::RANSAC(vector<KeyPoint>* points): points(points){}
void RANSAC::myline(Mat& img){
	utilCV UTIL(false);
	combination c(n_num_combination_of_m,2,points->size());
	UTIL.nameValue("szie",(int)points->size());
	int best_c[2] = {0,0};
	Mat best_model;
	int max_inlier_count = 0;
	Mat img_clone(img.clone());
	for(c.init();!c.isLast();c.next()){
		UTIL.MSG("loop start");
		UTIL.nameValue("c[0]",c[0]);
		UTIL.nameValue("c[1]",c[1]);
		Mat model = make_model((*points)[c[0]],(*points)[c[1]]);
		UTIL.showCVobj(model,"model");
		int inlier_count=0;
		for(int i=0;i<points->size();i++){
			float dist = calc_distance(model, (*points)[i]);
			assert (dist >= 0.0);
			UTIL.nameValue("dist",(float)dist);
			if(dist < thredshold) inlier_count ++;
		}
		UTIL.nameValue("inlier_count",inlier_count);
		if(inlier_count > max_inlier_count){
			max_inlier_count = inlier_count;
			if(!best_model.empty())best_model.release();
			best_model= model.clone();
			memcpy(best_c,c.value_block,sizeof(int)*2);
		}
	}
	line (img_clone,(*points)[best_c[0]].pt,(*points)[best_c[1]].pt,Scalar(0,0,0),1,1);
	imshow("img with line", img_clone);
}
Mat RANSAC::make_model(const KeyPoint& p1, const KeyPoint& p2){
	Mat result(2,1,CV_32FC1);
	Mat Y(2,1,CV_32FC1);
	Mat X_s(2,2,CV_32FC1);
	Mat_<float> Y_ = Y,X_s_ = X_s;
	Y_(0) = p1.pt.y;
	Y_(1) = p2.pt.y;
	X_s_(0,0) = p1.pt.x;
	X_s_(1,0) = p2.pt.x;
	X_s_(0,1) = X_s_(1,1) = 1.0;
	result = X_s.inv() * Y;
	return result;
}
float RANSAC::calc_distance(Mat& m,const KeyPoint& p){
	assert(m.rows == 2);
	utilCV UTIL(false);
	float tmp = abs(p.pt.y - m.at<float>(0) * p.pt.x - m.at<float>(1));
	UTIL.nameValue("tmp is",tmp);
	UTIL.showCVobj(m,"obj");
	return tmp / sqrt(1+m.at<float>(0)* m.at<float>(0));
}
float calc_min_dist(Point2f& p,KNN_set& knn,int kp){ // only for RANSAC::best_model
	float min_dist = numeric_limits<float>::max();
	for(int i=0;i<knn.the_K;i++){
		float dist = norm(Mat(p),Mat(knn(kp,i).pt));
		if(dist < min_dist){
			min_dist = dist ;
		}
	}
	return min_dist;
}
inline static int random_select(scoreBoard& sb,float r){
	int pick;
	if(r > 1.0) pick = rand() % sb.size();
	else pick = rand() % int(sb.size() * r);
	return sb.keys[pick];
}
inline static void updateScore(int key1,int key2,int key3,scoreBoard& sb,int inlier_count){
	if(sb.values[key1] < inlier_count) sb.update(key1, inlier_count);
	if(sb.values[key2] < inlier_count) sb.update(key2, inlier_count);
	if(sb.values[key3] < inlier_count) sb.update(key3, inlier_count);
	return;
}
inline static void updateAnealing(float& a){
	const float ratio = 1.005; //equals to 100^(1/100)
	a = a*ratio;
	return;
}
Mat RANSAC::best_model(KNN_set& knn,const Mat& img1,const Mat& img2){
	
	utilCV UTIL(true);
	thredshold = 10;
	//
	combination  c2(n_num_in_range_of_m,3,knn.the_K);
	scoreBoard   sb(knn.objectKeyPoint->size());
	Mat imgResult;

	Mat best_model;
	int max_inlier_count = 0;
	int inlier_count;
	float anealing = 1;
	
	const vector<KeyPoint>& oKP = *knn.objectKeyPoint;
	do{
		int key1,key2,key3;
		float range = (101-anealing)/100;
		key1 = random_select(sb,range);//
		while(key2 = random_select(sb,range),key2 == key1)UTIL.MSG("duplicate");//
		while(key3 = random_select(sb,range),key3 == key2 || key3 == key1)UTIL.MSG("duplicate");//
		Mat C = make_constraints(oKP[key1],oKP[key2 ],oKP[key3 ]);
	c2.init();
	do{
		Mat T = make_offset(
			knn(key1 ,c2[0]),
			knn(key2 ,c2[1]),
			knn(key3 ,c2[2])
		);
		Mat R = C.inv() * T;
		inlier_count = 0;
		for(int i=0;i< knn.objectKeyPoint->size();i++){
			Point2f point = do_mul(R,oKP[i].pt.x,oKP[i].pt.y);
			float dist = calc_min_dist(point,knn,i);
			if(dist < thredshold) inlier_count ++ ;
		}
		if(inlier_count > max_inlier_count){
			max_inlier_count = inlier_count;
			R.copyTo(best_model);
		}
		updateScore(key1,key2,key3,sb,inlier_count);//
		c2.next();
		if(inlier_count > 400 && !img1.empty() && !img2.empty()) {
			//RecoveryShow_forward(img1,img2,R,imgResult);		
		}
	}while(!c2.isLast());
	updateAnealing(anealing);
	UTIL.nameValue("anealing",anealing);
	}while(anealing <= 100);
	return best_model;
}
