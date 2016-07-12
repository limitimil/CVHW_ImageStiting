#include"util.h"
#include<iostream>
#include<cstdio>
#include<string>
void util::showBoard_sw(unsigned* board){
	static int count = 1;
	if(count == C_showBoard){
		count = 1; 
		//flag control above//
		/*working*/
		using namespace std;
		cout<<"showBoard:\n";
		for(int i=0;i<2;i++){
			for(int j=0;j<3;j++){
				cout<<"\t"<<board[3*i+j];
			}
			cout<< endl;
		}
		/*work done*/
	}else{
		count++;
	}
}
void util::MSG_sw(const char* str){
	printf("%s\n",str);
}
void util::MSG_sw(string str){
	cout << str <<endl;
}
void util::nameValue_sw(string name, double value){
	cout <<name<<":"<<value<<endl;
}
void util::nameValue_sw(string name, int value){
	cout <<name<<":"<<value<<endl;
}
void util::nameValue_sw(char* name, int value){
	cout <<name<<":"<<value<<endl;
}

void util::unsignedHex_sw(unsigned value){
	cout <<hex<<value<<dec<<endl;
}

void util::condPause_sw(unsigned a,unsigned b){
	if(a == b){
		this->pause();
	}
}
void util::hit_sw(string msg){
	hitCount ++;
	cout << msg <<" hit"<< ": "<< hitCount<<endl;
}

