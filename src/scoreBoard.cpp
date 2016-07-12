#include "scoreBoard.h"
#include "../DevTool/util.h"
#include <cstring>
#include <cassert>
#include <cstdio>
scoreBoard::~scoreBoard() {
	delete [] keys;
	delete [] values;
}
scoreBoard::scoreBoard(int s): size_(s) {
	keys = new int[s];	
	values = new int[s];
	for(int i=0;i<s;i++) keys[i] = i;
	memset(values,0,sizeof(int)*s);
}
int scoreBoard::size(){
	return size_;
}
void scoreBoard::update(int i,int v){
	util UTIL(false);
	assert(i< size_);
	int src=0,dst;
	while(keys[src]!=i) {
		assert(src< size_);
		src++;
	}
	dst = src;
	while(values[keys[dst]] <= v && dst!=0) dst--;
	UTIL.nameValue("src",src);
	UTIL.nameValue("dst",dst);
	for(int j=src;j>dst;j--) keys[j] = keys[j-1];
	keys[dst] = i;
	values[i] = v;
}
void printBoard(scoreBoard& sb){
	for(int i=0;i<sb.size();i++){
		printf("[%d]:%d,",sb.keys[i],sb.values[sb.keys[i]]);
	}
	printf("\n");
}
