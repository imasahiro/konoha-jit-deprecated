class A {
	A() {
	}
}

test "create constructor #18664" {
	assert (new A()).class == A;
}
