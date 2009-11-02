String s = """
Triple Quote
""";

test "Triple Quote #19425"{
	assert s == "Triple Quote\n";
}

test "String.sort #18821" {
	String[] str = ['a','hoge','0','bho'];
	str.sort();
	assert str[0] == "0" && str[1] == "a" && str[2] == "bho" && str[3] == "hoge";
}
