#ifndef __UTIL_H__ 
#define __UTIL_H__   

#include<cstdio>
#include<iostream>
#include<string>
using namespace std;
class util{
	friend class utilCV;
	private:
		bool switchon=true;
		unsigned hitCount;
	public:
		unsigned C_showBoard= 1;
		unsigned C_pause= 1;
	public:
		util(): hitCount(0) {}
		util(bool flag): hitCount(0) {switchon=flag;}
		~util(){}
		void pause(){
			if(switchon)pause_sw();
		}
		void showBoard(unsigned* board){
			if(switchon)showBoard_sw(board);
		}
		void MSG(const char* str){
			if(switchon)MSG_sw(str);
		}
		void MSG(string str){
			if(switchon)MSG_sw(str);
		}
		void nameValue(string name, double value){
			if(switchon)nameValue_sw(name,value);
		}
		void nameValue(string name, int value){
			if(switchon)nameValue_sw(name,value);
		}
		/*void nameValue(char* name, int value){
			if(switchon)nameValue_sw(name,value);
		}*/
		void unsignedHex(unsigned value){
			if(switchon)unsignedHex_sw(value);
		}
		void condPause(unsigned a,unsigned b){
			if(switchon) condPause_sw(a,b);
		}
		void hit(string msg){
			if(switchon) hit_sw(msg);
		}
	private:
		void pause_sw(){
			static unsigned count=1;
			if(C_pause == count){
				count = 1;
				getchar();
			}else{
				count++;
			}
		}
		void showBoard_sw(unsigned* board);
		void MSG_sw(const char* str);
		void MSG_sw(string str);
		void nameValue_sw(string name,double value);
		void nameValue_sw(string name,int value);
		void nameValue_sw(char* name,int value);
		void unsignedHex_sw(unsigned value);
		void condPause_sw(unsigned a,unsigned b);
		void hit_sw(string msg);
} ;

#endif
