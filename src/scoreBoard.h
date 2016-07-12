#ifndef __SCOREBOARD__H__
#define __SCOREBOARD__H__
class scoreBoard{
	int size_;
public:
	int* keys;
	int* values;
public:
	scoreBoard(int);
	~scoreBoard();
	int size();
	void update(int,int);
};
void printBoard(scoreBoard&);
#endif
