//
String disp (String? msg) {
	if (msg == null) {
		return "nullable";
	} else {
		return msg;
	}
}
test "function #19393" {
	assert disp("hoge") == "hoge";
}
test "nullable function args #19393" {
	assert disp(null) == "nullable";
}

test "null compare #18781" {
	assert os.getProperty("NOT_USED_PROP") == null;
}
