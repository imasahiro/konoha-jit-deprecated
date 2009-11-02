// #19048
String f(int n)
{
	String r = "";
	for (i=0; i < n; i++) {
		r += (String)i;
	}
	return r;
}

String g(int n)
{
	String r = "";
	for (int i=0; i < n; i++) {
		r += (String)i;
	}
	return r;
}


test "for stmt" {
	assert f(5) == "01234" && g(5) == "01234";
}
