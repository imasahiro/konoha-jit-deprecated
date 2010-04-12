String f() {
	return "hello";
}
test "call Closure #18889" {
	Closure c = f;
	assert c.class == String(void) && c() == "hello";
}
