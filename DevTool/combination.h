#ifndef __COMBINATION__H__
#define __COMBINATION__H__
enum Ctype{n_num_combination_of_m,n_num_in_range_of_m};//combination type
class combination
{
	Ctype type;
public:
	int n,m;
	int* value_block;
	combination(Ctype,int,int);
	~combination();
	void set(const int*);
	void init();
	void next();
	bool isLast();
	int  operator[](int);
};
#endif
