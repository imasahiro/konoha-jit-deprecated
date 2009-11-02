// #19424
String s = "konoha";
test "%k(s)" {
	assert %k(s) == "\"konoha\"";
}
test "%s(int)" {
	assert %s(1) == "1";
}
test "%c(int)" {
	assert %c(65) == "A";
}
