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

test "switch" {
	assert "String" == sw("String") && ("int" == sw("Int")) && "Any" == sw("foo");
}
