#ifndef __KNN__H__
#define __KNN__H__
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <vector>
using namespace cv;
using std::vector;
class KD_TREE;
class KNN;
struct node_t{
	int value;
	struct node_t *left,*right,*parent;
};
typedef struct node_t node;
struct CV_info_t{
	//vector<float>    distance(1,1.0/0.0); //the vector size should be change by "K"
	vector<KeyPoint> keypoint;
	Mat descriptor;
};
typedef struct CV_info_t CV_info;
class KNN{
	struct KNN_info_t{
		int index;
		float distance;
	};

	typedef struct KNN_info_t KNN_info;	
	public:
		unsigned the_K = 2;
		const std::vector<KeyPoint> *objectKeyPoint;
		const std::vector<KeyPoint> *targetKeyPoint;
		const Mat *objectDescriptor, *targetDescriptor;
		KD_TREE* tree;
		//tree traversal global
		Mat      currentDescriptor;
		KNN_info* result;
	public:
		//KNN(vector<KeyPoint>&,vector<KeyPoint>&,Mat&,Mat&);
		KNN(vector<KeyPoint>*,Mat*);
		~KNN();
		CV_info findKNN(Mat&);
		void findKNN_rec(node*,int,Mat_<float>&);
		int  findKNN_naive(Mat&);
		void add_keypoint(int,float);
		void setK(float);
//		Mat getObjectDescriptor(vector<KeyPoint>&, Mat&);
//		Mat getTargetDescriptor(vector<KeyPoint>&, Mat&);
	
};
class KD_TREE{
	public:
	node* root =0;
		KD_TREE(Mat*);
		~KD_TREE();
		int getRightChild(int);
		int getLeftChild(int);
		int getParent(int);
		void recursively_delete(node*);
		
};
class KNN_set: public KNN{
	vector<int*>* sets;
	public:
	KNN_set(vector<KeyPoint>*,vector<KeyPoint>*,Mat*,Mat*);
	~KNN_set();
	void prepare();
	const KeyPoint& queryKeyPoint(int,int);
	KeyPoint operator()(int,int);
	
};
#endif
