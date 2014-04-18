//#include "logging.h"
class B 
{
	int b;
};
class C
{
	int c;
};
class A
{
public:
	explicit A(B* b){;}
	~A(){;}
};

template <bool>
struct CompileAssert
{
};

#define COMPILE_ASSERT(expr,msg)\
	typedef CompileAssert<bool(expr)> msg[bool(expr)? 1:-1]

//#define A(x) COMPILE_ASSERT(0,avilable_missing)

int main(int argc,char *argv[])
{
	A a(C());
	//long num_cookies(999);
	//LOG(INFO); /*<< "Found " << num_cookies << " cookies";*/
	return 0;
}