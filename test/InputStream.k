class Foo {
	Int x;
}
test "InputStream.readData #19792" {
	try {
		ins = new InputStream("/dev/null");
		Foo f = ins.readData();
		f.x;
	} catch (Exception e) {
	}
}
