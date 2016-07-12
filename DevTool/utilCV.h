#ifndef __UTILCV_H__
#define __UTILCV_H__
#include "util.h"
#include <iostream>
class utilCV: public util
{
	public:
		template <typename T>
		void showCVobj(T obj,const char* str){
			if(switchon) showCVobj_sw(obj,str);
		}
	private:
		using util::util;
		template <typename T>
		void showCVobj_sw(T obj,const char* str){
			std::cout<< "show object"<<str <<":\n" << obj << std::endl;
		}
};
#endif
