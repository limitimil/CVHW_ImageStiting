#include "parameter.h"
#include <fstream>
#include <cstdio>
#include <cstring>
using std::ifstream;
const string parameter::names[]={ 
	"recoveryType",
	"object_in",
	"object_out",
	"target_in",
	"target_out",
	"result_out",
	"end"
}; 
parameter::parameter(const char* configFile){
	ifstream in;
	in.open(configFile);
	int i;
	for(int i=0;i<size;i++) values[i] = 0;
	while(!in.eof() && in.good()){
		char buffer[100];
		in.getline(buffer,100,':');
		if((i = pIndex(buffer)) >= 0){
			int len ;

			in.getline(buffer,100);
			len = strlen(buffer)+1;
			values[i] = new char[len];
			strcpy(values[i],buffer);
		}
		else continue;
	}
	in.close();	
}
parameter::~parameter(){
	for(int i=0;i<size;i++) delete [] values[i];
}
void parameter::showParameter(){
	for(int i=0;i<size;i++){
		printf("%s:%s\n",names[i].c_str(),values[i]);
	}
}
int parameter::pIndex(const char* n){
	int i;
	for(i=size-1;i>=0;i--)
		if(names[i].compare(n) ==0) return i;
	return i;
}	
