import java.util.Date;
import java.lang.Math;


class Dim {
	int x;
	int y;
	int z;
	void f() {
	}
}

class microbench3 {
	static int global_x = 0;
	static int global_y = 0;
	static int global_z = 0;

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

	static int testGlobalVariable() {
		int i;
		for (i = 0; i < 100000000; i++) {
			global_y = i;
		}
		return global_y;
	}

	static String testStringAssignment() {
		String s = "";
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

	static void func0() {}

	static int testFunctionCall() {
		int i = 0;
		for (i = 0; i < 100000000; i++) {
			func0();
		}
		return i;
	}

	static int func1() { return 1; }

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
			Math.abs(-1.0);
		}
		return i;
	}

	/*
	static int testCallFunctionObject() {
		int i;
		for(i = 0; i< 100000000; i++) {
			func0();
		}
		return i;
	}
	*/

	static int fibo(int n) {
		if (n < 3) return 1;
		return fibo(n - 1) + fibo(n - 2);
	}
	
	static int testObjectCreation() {
		int i;
		for (i = 0; i < 100000000; i++) {
			Dim d = new Dim();
		}
		return i;
	}

	static Dim testFieldVariable() {
		Dim d = new Dim();
		int i;
		for (i = 0; i < 100000000; i++) {
			d.y = 1;
		}
		return d;
	}

	static int testMethodCall() {
		int i;
		Dim d = new Dim();
		for (i = 0; i < 100000000; i++) {
			d.f();
		}
		return i;
	}

	static long mops(long score) {
		if (score < 1) return 0;
		return (10000000 / score) / 100;
	}
	
	public static void main(String args[]) {
		long s = System.currentTimeMillis();
		testSimpleLoop();
		long t = System.currentTimeMillis() - s;
		long score = t;
		System.out.println("SimpleLoop[mops, ms] : " + mops(score) + ", t = " + t);
		long delta = t;
		//
		s = System.currentTimeMillis();
		testLocalVariable();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("LocalVariable[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testGlobalVariable();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("GlobalVariable[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testStringAssignment();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("StringAssignment[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testIntegerOperation();
		t = System.currentTimeMillis() -s ;
		score = t - delta;
		System.out.println("IntegerOperation[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testFloatOperation();
		t = System.currentTimeMillis() -s ;
		score = t - delta;
		System.out.println("FloatOperation[mops, ms] : " + mops(score) + ", t = "  + t);
		// 
		s = System.currentTimeMillis();
		testFunctionCall();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("FunctionCall[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testFunctionReturn();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("FunctionReturn[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testMathFabs();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("NativeFunction[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		/*
		s = System.currentTimeMillis();
		testCallFunctionObject();
		t = System.currentTimeMillis() - s;
		score = t - delta;
		System.out.println("CallFunctionObject[mops, ms] : " + mops(score) + ", t = "  + t);
		*/
		//
		System.out.println("Object-Oriented Programming");
		s = System.currentTimeMillis();
		testObjectCreation();
		t = System.currentTimeMillis() -s ;
		score = t - delta;
		System.out.println("ObjectCreation[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testFieldVariable();
		t = System.currentTimeMillis() -s ;
		score = t - delta;
		System.out.println("FieldVariable[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		s = System.currentTimeMillis();
		testMethodCall();
		t = System.currentTimeMillis() -s ;
		score = t - delta;
		System.out.println("MethodCall[mops, ms] : " + mops(score) + ", t = "  + t);
		//
		System.out.println("MISC (this is not microbench)");
		s = System.currentTimeMillis();
		fibo(36);
		t = System.currentTimeMillis() - s;
		score = t;
		System.out.println("fibonacci(36)[mops, ms] : " + mops(score) + ", t = "  + t);
	}
}
