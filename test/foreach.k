test "foreach String[]" {
	String[] slist = ["a","b","c","d"];
	String str = "";
	foreach(s in slist) {
		str += s;
	}
	print str;
	assert str == "abcd";
}

test "foreach String[]" {
	Int[] ilist = new Int[]();
	ilist.add(1);
	ilist.add(2);
	ilist.add(3);
	Int v;
	foreach(i in ilist) {
		v += i;
	}
	print v;
	assert v == 6;
}
