#include "KNN.h"
#include "../DevTool/utilCV.h"
#include <cassert>
#include <limits>
using namespace cv;
using std::vector;
KNN::~KNN(){
	delete tree;
	delete [] result;
}
KNN::KNN(vector<KeyPoint>* keypoint, Mat* descriptors): targetKeyPoint(keypoint), targetDescriptor(descriptors){
	assert(keypoint->size() == descriptors->size().height);
	assert(descriptors->size().width == 128);
	tree = new KD_TREE(descriptors);
	result = new KNN_info[the_K];	
}
int KNN::findKNN_naive(Mat& descriptor){
	float min = std::numeric_limits<float>::max();
	int   result = -1;
	for(int i=0;i<targetDescriptor->rows;i++){
		
		Mat_<float> tmp(descriptor.size()); 
		float dist;
		absdiff(targetDescriptor->row(i),descriptor,tmp);
		dist = norm(tmp,NORM_L2);
		if(dist < min){
			min = dist;// how to calculate dst
			result = i;
		}
	}
	return result;
}
CV_info KNN::findKNN(Mat& descriptor){
	utilCV UTIL(true);
	assert(descriptor.cols == 128);
	Mat_<float> prune_info(descriptor.size()); //1* 128 ,all init with 0
	for(int i=0;i<128;i++) prune_info(i) = 0.0;
	//clean the private result block;
	for(int i=0;i<the_K;i++){
		result[i].index    = -1;
		result[i].distance = std::numeric_limits<float>::max();
	}
	CV_info r;
	
	currentDescriptor= descriptor;
	findKNN_rec(tree->root,0,prune_info);
	for(int i=0;i< the_K;i++){
		assert(result[i].index >= 0);
		r.keypoint.push_back( (*targetKeyPoint)[result[i].index] );
		r.descriptor = targetDescriptor->row(result[i].index);
	}
	return r;
		
}
void KNN::findKNN_rec(node* ptr,int axis,Mat_<float>& prune_info){
	utilCV UTIL(false);
	UTIL.nameValue("value",ptr->value);
	UTIL.nameValue("axis",axis);
	node *ptr1, *ptr2;
	float value_current = targetDescriptor->row(ptr->value).at<float>(axis);
	float value_object = currentDescriptor.at<float>(axis);
	float diff = value_object-value_current;
	Mat_<float> tmp(currentDescriptor.size()); 
	float dist;
	absdiff(targetDescriptor->row(ptr->value),currentDescriptor,tmp);
	dist = norm(tmp,NORM_L2);
	if( diff > 0){	
		ptr1 = ptr-> right;
		ptr2 = ptr-> left;
	}else{
		ptr1 = ptr-> left;
		ptr2 = ptr-> right;
	}
	if(ptr1 != 0){
		findKNN_rec(
			ptr1,
			(axis+1)%128,
			prune_info
		);
	}
	if(ptr2 !=0){
		float old_value = prune_info(axis);
		prune_info(axis) = diff;
		if(norm(prune_info,NORM_L2) < result[the_K-1].distance){
			findKNN_rec(
				ptr2,
				(axis+1)%128,
				prune_info
			); 
		} 
		prune_info(axis) = old_value;
	}
	if(dist < result[the_K-1].distance){
		add_keypoint(ptr->value,dist);

	}
	return;
}
void KNN::add_keypoint(int i,float d){
	int p = the_K;
	KNN_info tmp1;
	while(d < result[p-1].distance && p != 0) p--;
	for(int j=the_K;j> p;j--) result[j] = result[j--];
	if(p != the_K){
		result[p].index = i;
		result[p].distance = d;
	}
	return;
}
KD_TREE::KD_TREE(Mat* descriptors){
	assert(descriptors-> cols == 128);
	root = new node();
	root-> value = 0;
	root-> left = root-> right = 0;
	for(int i=1;i!=descriptors->rows;i++){
		node* ptr = root;
		int pivot= 0;
		Mat target_desc = descriptors-> row(i);
		while(true){
			Mat current_desc = descriptors->row(ptr->value);
			if(target_desc.at<float>(pivot) > current_desc.at<float>(pivot)){
				if(ptr-> right == 0){
					ptr -> right = new node();
					ptr = ptr-> right;
					ptr -> value = i;
					ptr -> left = ptr -> right = 0;	
					break;	
				}else{
					ptr = ptr-> right;
				}

					
			}else{
				if(ptr-> left == 0){
					ptr -> left = new node();
					ptr = ptr-> left;
					ptr -> value = i;
					ptr -> left = ptr -> right = 0;	
					break;	
				}else{
					ptr = ptr-> left;
				}
			}
			pivot = (pivot + 1)% 128 ;
		}	
		
	}
	return;
}
KD_TREE::~KD_TREE(){
	recursively_delete(root);
}
void KD_TREE::recursively_delete(node* ptr){
	if(ptr == 0){
		return;
	}else{
		recursively_delete(ptr->right);
		recursively_delete(ptr->left);
		delete ptr;
		return;
	}
}
KNN_set::KNN_set(vector<KeyPoint>* k_object,vector<KeyPoint>* k_target,Mat* d_object,Mat* d_target): 
	KNN(k_target,d_target) 
{
	
	assert(k_target->size() == d_target->size().height);
	assert(k_object->size() == d_object->size().height);
	assert(d_object->size().width == d_target->size().width);
	assert(d_target->size().width == 128);
	objectKeyPoint = k_object;
	objectDescriptor = d_object;
	sets = new vector<int*>(k_object->size(),0);
	for(int j=0;j< objectKeyPoint->size();j++)
		(*sets)[j] = new int[the_K];
}
KNN_set::~KNN_set(){
	for(int i=0;i<sets->size();i++) delete [] (*sets)[i];
	delete sets;
}
void KNN_set::prepare(){
	utilCV UTIL(true);
	Mat_<float> prune_info(Mat(1,128,CV_32FC1)); //1* 128 ,all init with 0
	for(int j=0;j< objectKeyPoint->size();j++){
		for(int i=0;i<128;i++) prune_info(i) = 0.0;
		//clean the private result block;
		for(int i=0;i<the_K;i++){
			result[i].index    = -1;
			result[i].distance = std::numeric_limits<float>::max();
		}
		CV_info r;
		
		currentDescriptor= objectDescriptor->row(j);
		findKNN_rec(tree->root,0,prune_info);
		for(int i=0;i< the_K;i++){
			assert(result[i].index >= 0);
			(*sets)[j][i] = result[i].index;
		}

	}
}
KeyPoint KNN_set::operator()(int o,int t){
	assert(t< the_K);
	assert(o< objectKeyPoint->size());
	return (*targetKeyPoint)[(*sets)[o][t]];
} 
const KeyPoint& KNN_set::queryKeyPoint(int o,int t) //o for object, t for target
{
	utilCV UTIL(false);
	UTIL.nameValue("t",t);
	assert(t< the_K);
	assert(o< objectKeyPoint->size());
	return (*targetKeyPoint)[(*sets)[o][t]];
}
void KNN::setK(float k){
	the_K = k;
}
