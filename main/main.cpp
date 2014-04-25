#include "utilities.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "commandlineflags.h"
#include "logging.h"
#include "raw_logging.h"


struct data
{
 int a;
 long b;
 data()
 {
	a=0;
	b=0;
 }
 data& operator<<(int i)
 {
	a+=i;
	return *this;
 }
private:
	void operator&(data&){}
};
class A
{
public:
	A(){}

	void operator&(data&){}
};
class B
{
public:
	B()
	{
		int a=0;
	}
	data& func()
	{
		return d; 
	}
	data d;
private:
	void operator&(data&){}
};
#define ADD\
	(true) ? (void)0: A()& B().func()
int main(int argc,char *argv[])
{
	ADD<<1;
	return 0;
}