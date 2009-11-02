int f() {
	return 1;
}
test "call Closure #18889" {
	Closure c = f;
	assert c.class == int(void) && c() == 1;
}
