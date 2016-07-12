#ifndef __PARAMETER__H__
#define __PARAMETER__H__
#include <string>
using std::string;
enum keys{
	recoveryType,
	object_in,
	object_out,
	target_in,
	target_out,
	result_out,
	end
	};
class parameter{
	const int size = 7;
public:
	char* values[7];
	static const string names[];
public:
	parameter(const char*);
	~parameter();
	void showParameter();
	int pIndex(const char* );
};

#endif
