int a = 5;
String str = "hello";
test "Int Class Name" {
	assert a.class == Int;
}

test "String Class Name #19275" {
	assert str.class == String;
}
test "Int instanceof Int #19275" {
	assert a instanceof Int == true;
}
test "Int instanceof String #19275" {
	assert a instanceof String == false;
}
