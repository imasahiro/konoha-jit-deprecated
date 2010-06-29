// konoha microbench by kimio kuramitsu

using konoha.math.Math;

int testSimpleLoop() {
	int i;
	for(i = 0; i < 100_000_000; i++) {
	}
	return i;
}

int testLocalVariable() {
	y = 0;
	for(i = 0; i < 100_000_000; i++) {
		y = i;
	}
	return y;
}

global_x = 0;
global_y = 0;
global_z = 0;

int testGlobalVariable() {
	for(i = 0; i < 100_000_000; i++) {
		global_y = i;
	}
	return global_y;
}

String testStringAssignment() {
	s = "";
	for(i = 0; i < 100_000_000; i++) {
		s = "A";
	}
	return s;
}

int testIntegerOperation() {
	y = 0;
	for(i = 0; i < 100_000_000; i++) {
		y += 1;
	}
	return y;
}

float testFloatOperation() {
	f = 0.0;
	for(i = 0; i < 100_000_000; i++) {
		f += 0.1;
	}
	return f;
}

void func0() {}

int testFunctionCall() {
	i=0;
	for(i = 0; i < 100_000_000; i++) {
		func0();
	}
	return i;
}

int func1() { return 1; }

int testFunctionReturn() {
	res = 0;
	for(i = 0; i< 100_000_000; i++) {
		res = func1();
	}
	return res;
}

int testMathFabs() {
	int i;
	for(i = 0; i< 100_000_000; i++) {
		Math.fabs(-1.0);
	}
	return i;
}

int testCallFunctionObject() {
	int i;
	f = func0;
	for(i = 0; i< 100_000_000; i++) {
		f();
	}
	return i;
}

int fibo(int n) {
    if(n < 3) return 1;
    return fibo(n-1) + fibo(n-2);
}

//

class Dim {
	int x;
	int y;
	int z;
}

Dim testFieldVariable() {
	d = new Dim();
	for(i = 0; i < 100_000_000; i++) {
		d.y = 1;
	}
	return d;
}

float mops(int score) {
	if(score < 1) return 0;
	return (10000000 / score) / 100.0;
}

void run() {
	s = System.getTime();
	testSimpleLoop();
	t = System.getTime() - s;
	score = t;
	print "SimpleLoop[mops,ms]: ", mops(score), t;
	delta = t;
	//
	s = System.getTime();
	testLocalVariable();
	t = System.getTime() - s;
	score = t - delta;
	print "LocalVariable[mops,ms]:", mops(score), t;
	//
	s = System.getTime();
	testGlobalVariable();
	t = System.getTime() - s;
	score = t - delta;
	print "GlobalVariable[mops,ms]:", mops(score), t;
	//
	s = System.getTime();
	testStringAssignment();
	t = System.getTime() - s;
	score = t - delta;
	print "StringAssignment[mops,ms]:", mops(score), t;
	//
	s = System.getTime();
	testIntegerOperation();
	t = System.getTime() -s ;
	score = t - delta;
	print "IntegerOperation[mops,ms]:", mops(score), t;	
	//
	s = System.getTime();
	testFloatOperation();
	t = System.getTime() -s ;
	score = t - delta;
	print "FloatOperation[mops,ms]:", mops(score), t;
	// 
	s = System.getTime();
	testFunctionCall();
	t = System.getTime() - s;
	score = t - delta;
	print "FunctionCall[mops,ms]:", mops(score), t;
	//
	s = System.getTime();
	testFunctionReturn();
	t = System.getTime() - s;
	score = t - delta;
	print "FunctionReturn[mops,ms]:", mops(score), t;
	//
	s = System.getTime();
	testMathFabs();
	t = System.getTime() - s;
	score = t - delta;
	print "NativeFunction[mops,ms]:", mops(score), t;
	//
	s = System.getTime();
	testCallFunctionObject();
	t = System.getTime() - s;
	score = t - delta;
	print "CallFunctionObject[mops,ms]:", mops(score), t;
	//
	print "Object-Oriented Programming";
	s = System.getTime();
	testFieldVariable();
	t = System.getTime() -s ;
	score = t - delta;
	print "FieldVariable[mops,ms]:", mops(score), t;
	//
	print "MISC (this is not microbench)";
	s = System.getTime();
	fibo(36);
	t = System.getTime() - s;
	score = t;
	print "fibonacci(36)[mops,ms]:", mops(score), t;
}

run();



