int i = 3;
Any a = i;

test "Int to Any #18812" {
	assert a == 3;
}

class Foo {
	Any bar;
	void f() {
		i = _bar.hi();
	}
}


test "Any Method #19581" {
	boolean b = false;
	try {
		Foo.f();
	} catch(Exception e) {
		b = true;
	} finally {
		assert b;
	}
}
test "No Such Method #19581" {
	boolean b = false;
	Any a;
	try {
		a.hi();
	} catch(Exception e) {
		b = true;
	} finally {
		assert b;
	}
}

class Box {
	Any m;
	Box (int n) {
		m = n;
	}
}

class Box2 {
	Any m;
	Box2 (String n) {
		m = n;
	}
}

test "UNBOX, BOX" {
	int i = 5;
	String str = "5";
	b1 = new Box(i);
	b2 = new Box2(str);
	assert b1.m != b2.m;
}

test "null as String #21400" {
    String s = null;
    |s|
}
