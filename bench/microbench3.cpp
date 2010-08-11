#include <iostream>
#include <string>
#include <math.h>
#include <sys/time.h>
#include <memory>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

static int testSimpleLoop() {
  int i;
  for (i = 0; i < 100000000; i++) {
  }
  return i;
}

static int testLocalVariable() {
  int y = 0;
  int i;
  for (i = 0; i < 100000000; i++) {
	y = i;
  }
  return y;
}

static int global_x = 0;
static int global_y = 0;
static int global_z = 0;

static int testGlobalVariable() {
  int i;
  for (i = 0; i < 100000000; i++) {
	global_y = i;
  }
  return global_y;
}

static string testStringAssignment() {
  string s = "";
  int i;
  for (i = 0; i < 100000000; i++) {
	s = "A";
  }
  return s;
}

static int testIntegerOperation() {
  int y = 0;
  int i;
  for (i = 0; i < 100000000; i++) {
	y += 1;
  }
  return y;
}

static double testFloatOperation() {
  double f = 0.0;
  int i;
  for (i = 0; i < 100000000; i++) {
	f += 0.1;
  }
  return f;
}

static void func0(void) {}
typedef void(*Func)(void);

static int testFunctionCall() {
  int i = 0;
  for (i = 0; i < 100000000; i++) {
	func0();
  }
  return i;
}

static int func1(void) { return 1; }

static int testFunctionReturn() {
  int res = 0;
  int i;
  for (i = 0; i < 100000000; i++) {
	res = func1();
  }
  return res;
}

static int testMathFabs() {
  int i;
  for (i = 0; i < 100000000; i++) {
	fabs(-1.0);
  }
  return i;
}

static int testCallFunctionObject() {
  int i;
  Func f = func0;
  for (i = 0; i < 100000000; i++) {
	f();
  }
  return i;
}

static int fibo(int n) {
  if (n < 3) return 1;
  return fibo(n - 1) + fibo(n - 2);
}

//

class Dim 
{
public:
  int x;
  int y;
  int z;
  void f() {
  }
};

typedef shared_ptr<Dim> DimPtr;

static int testObjectCreation() {
  int i;
  for (i = 0; i < 100000000; i++) {
	DimPtr d(new Dim());
  }
  return i;
}

static DimPtr testFieldVariable() {
  DimPtr d(new Dim());
  int i;
  for (i = 0; i < 100000000; i++) {
	d->y = 1;
  }
  return d;
}

static int testMethodCall() {
  int i;
  DimPtr d(new Dim());
  for (i = 0; i < 100000000; i++) {
	d->f();
  }
  return i;
}


static double mops(double score) {
  if (score < 1) return 0;
  return (10000000 / score) / 100.0;
}

static double getTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main(int argc, char **argv)
{
  double s = getTime();
  testSimpleLoop();
  double t = getTime() - s;
  double score = t;
  cout << "SimpleLoop[mops, ms] : " << mops(score) << ", t = " << t << endl;
  double delta = t;
  //
  s = getTime();
  testLocalVariable();
  t = getTime() - s;
  score = t - delta;
  cout << "LocalVariable[mops, ms] : " << mops(score) << ", t = " <<  t << endl;
  //
  s = getTime();
  testGlobalVariable();
  t = getTime() - s;
  score = t - delta;
  cout << "GlobalVariable[mops, ms] : " << mops(score) << ", t = " <<  t << endl;
  //
  s = getTime();
  testStringAssignment();
  t = getTime() - s;
  score = t - delta;
  cout << "StringAssignment[mops, ms] : " << mops(score) << ", t = " <<  t << endl;
  //
  s = getTime();
  testIntegerOperation();
  t = getTime() -s ;
  score = t - delta;
  cout << "IntegerOperation[mops, ms] : " << mops(score) << ", t = " << t << endl;	
  //
  s = getTime();
  testFloatOperation();
  t = getTime() -s ;
  score = t - delta;
  cout << "FloatOperation[mops, ms] : " << mops(score) << ", t = " << t << endl;
  // 
  s = getTime();
  testFunctionCall();
  t = getTime() - s;
  score = t - delta;
  cout << "FunctionCall[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  s = getTime();
  testFunctionReturn();
  t = getTime() - s;
  score = t - delta;
  cout << "FunctionReturn[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  s = getTime();
  testMathFabs();
  t = getTime() - s;
  score = t - delta;
  cout << "NativeFunction[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  s = getTime();
  testCallFunctionObject();
  t = getTime() - s;
  score = t - delta;
  cout << "CallFunctionObject[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  cout << "Object-Oriented Programming" << endl;
  s = getTime();
  testObjectCreation();
  t = getTime() -s ;
  score = t - delta;
  cout << "ObjectCreation[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  s = getTime();
  testFieldVariable();
  t = getTime() -s ;
  score = t - delta;
  cout << "FieldVariable[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  s = getTime();
  testMethodCall();
  t = getTime() -s ;
  score = t - delta;
  cout << "MethodCall[mops, ms] : " << mops(score) << ", t = " << t << endl;
  //
  cout << "MISC (this is not microbench)" << endl;
  s = getTime();
  fibo(36);
  t = getTime() - s;
  score = t;
  cout << "fibonacci(36)[mops, ms] : " << mops(score) << ", t = " << t << endl;
  return 0;
}
