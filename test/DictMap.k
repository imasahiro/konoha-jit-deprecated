DictMap d = new DictMap();
d["0"] = "a";
d["1"] = "b";
d["2"] = "c";


d.remove("1");
test "delete second element #19926" {
	assert d["1"] == null;
}
