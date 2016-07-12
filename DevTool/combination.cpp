#include "combination.h"
#include <cassert>
#include <cstdlib>
combination::combination(Ctype type,int n,int m):n(n),m(m),type(type){
	value_block = new int[n];
}
combination::~combination(){
//	delete [] value_block;
	//if i delete here, will get double free or corruption bug
}
void combination::init(){
switch(type){
	case n_num_combination_of_m:
		assert(m > n);
		for(int i=0;i<n;i++) value_block[i] = i;
		break;
	case n_num_in_range_of_m:
		for(int i=0;i<n;i++) value_block[i] = 0;
		break;
	default:
		assert(1==0);
		exit(510);
}
}
void combination::next(){
	int i = n-1;

switch(type){
	case n_num_combination_of_m:
		while(value_block[i] == m-(n-i)) i--;
		if(i>=0){
			value_block[i]++;
			for(int j=i+1;j<n;j++)
				value_block[j] = value_block[j-1]+1;
		}	
		else {
			for(int j=0;j<n;j++)
				value_block[j] = -1;
		}
		break;
	case n_num_in_range_of_m:
		value_block[n-1] ++;
		for(i=n-1;i>=0;i--) {
			if(value_block[i] == m){
				value_block[i-1]++;
				value_block[i] = 0;
			}else{
				break;
			}
		}
		if(i<0)
			for(int j=0;j<n;j++)
				value_block[j] = -1;
		break;
	default:
		assert(1==0);
		exit(510);
}
}
bool combination::isLast(){
	int i = n-1;
	for(int j=0;j<n;j++){
		if(value_block[j] < 0)
			return true;
		else
			continue;
	}
	return false;
switch(type){
	case n_num_combination_of_m:
		while(value_block[i] == m-(n-i)) i--;
		if(i>=0)
			return false;
		else 
			return true;
	case n_num_in_range_of_m:
		for(i=n-1;i>=0;i--) {
			if(value_block[i] == m-1) 
				continue;
			else
				return false;
		}
		return true;
	default:
		assert(1==0);
		exit(510);
}

}
int combination::operator[](int i){
	assert(i<n);
	assert(0<=i);
	return value_block[i];
}
void combination::set(const int* s){
	for(int i=0;i<n;i++) value_block[i] = s[i];	
}
