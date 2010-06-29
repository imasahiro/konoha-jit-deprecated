// konoha microbench by kimio kuramitsu

int testSimpleLoop() {
	int i;
	for(i = 0; i < 100_000_000; i++) {
	}
	return i;
}

int testLocalVariable() {
	y = 0;
	for(i = 0; i < 100_000_000; i++) {
		y += 1;
	}
	return y;
}

global_x = 0;
global_y = 0;
global_z = 0;

int testGlobalVariable() {
	for(i = 0; i < 100_000_000; i++) {
		global_y += 1;
	}
	return global_y;
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

float testFloatOperation() {
	f = 0.0;
	for(i = 0; i < 100_000_000; i++) {
		f += 0.1;
	}
	return f;
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
		d.y += 1;
	}
	return d;
}

void run() {
	s = System.getTime();
	testSimpleLoop();
	period = System.getTime() - s;
	score = period;
	print "SimpleLoop[ms]: ", period, score;
	delta = period;
	//
	s = System.getTime();
	testLocalVariable();
	period = System.getTime() - s;
	score = period - delta;
	print "LocalVariable[ms]:", period, score;
	//
	s = System.getTime();
	testGlobalVariable();
	period = System.getTime() - s;
	score = period - delta;
	print "GlobalVariable[ms]:", period, score;
	// 
	s = System.getTime();
	testFunctionCall();
	period = System.getTime() - s;
	score = period - delta;
	print "FunctionCall[ms]:", period, score;
	//
	s = System.getTime();
	testFunctionReturn();
	period = System.getTime() - s;
	score = period - delta;
	print "FunctionReturn[ms]:", period, score;
	//
	s = System.getTime();
	testFloatOperation();
	period = System.getTime() -s ;
	score = period - delta;
	print "FloatOperation[ms]:", period, score;
	//
	print "Object-Oriented Programming";
	s = System.getTime();
	testFieldVariable();
	period = System.getTime() -s ;
	score = period - delta;
	print "FieldVariable[ms]:", period, score;

}

run();



