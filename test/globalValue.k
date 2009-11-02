// #18806
String s = "test";
class Test {
	String disp() {
		return __s;
	}
}

test "global value access #18806" {
	assert "test" == Test.disp();
}
