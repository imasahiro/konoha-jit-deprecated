test "Triple Quote #19425" {
	String s = """
Triple Quote
""";
	assert s == "Triple Quote\n";
}

test "String.sort #18821" {
	String[] str = ['a','hoge','0','bho'];
	str.sort();
	assert (str[0] == "0" && str[1] == "a" && str[2] == "bho" && str[3] == "hoge");
}

test "String.opCase" {
	assert ("" != "test");
}

test "String.replace" {
	s = "1".replace("123", "321");
	assert (s == "1");
}
