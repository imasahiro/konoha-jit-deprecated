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
	Any a1;
	try {
		a.hi();
	} catch(Exception e) {
		b = true;
	} finally {
		assert b;
	}
}
