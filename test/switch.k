// #19398

String sw(String name)
{
	switch (name) {
		case "String":
			return "String";
		break;
		case "Int":
			return "int";
		break;
		default :
			return "Any";
			break;
	}
}

test "switch #19398" {
	assert "String" == sw("String") && ("int" == sw("Int")) && "Any" == sw("foo");
}

String null_sw()
{
	String str = "";
	switch (str) {
		case "case1":
			return "case1";
		break;
		default:
			return "default";
	}
	return "";
}

test "switch #20168" {
	assert "default" == null_sw();
}

