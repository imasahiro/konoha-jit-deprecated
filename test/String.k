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

test "String.format" {
    String str = "konoha";
    assert (str.format("%s") == str);
}

test "String.format (Back Quote)" {
    String str = "konoha";
    assert (`%s{str}` == str);
}

test "String.opCase" {
    String str = "tes";
	assert (str != "test");
}

test "String.opHas" {
    String str = "no";
    assert (str in "konoha");
}

test "String.split" {
    String[] strs = "konoha".split("o",0);
    assert(strs[0] == "k" && strs[1] == "n" && strs[2] == "ha");
}

test "String.indexOf" {
    String str = "konoha";
    assert (str.indexOf("n") == 2);
}

test "String.startsWith" {
    String str = "konoha";
    assert (str.startsWith("kon"));
}

test "String.endsWith" {
    String str = "konoha";
    assert (str.endsWith("oha"));
}

test "String.times" {
    String str = "konoha";
    assert (str.times(3) == "konohakonohakonoha");
}

test "String.substring" {
    String str = "__konoha__";
    assert (str.substring(2,6) == "konoha");
}

test "String.getUCS4" {
    String ch = "A";
    assert (ch.getUCS4() == 65);
}

test "String.replace" {
    String str = "k0n0ha";
    assert (str.replace("0","o") == "konoha");
}

String jaiu = "あいうえお";
String jaaa = "あああああ";
String ja = "あ";
String jp = "あaいaaあacいcc";
String jaiu2 = "あいうあいう";
String jk = "よろしくお願いします！";

test "String []" {
    assert(jk[5] == "願");
}

test "String +" {
    assert( jaiu+ja == "あいうえおあ");
}

test "String - String" {
    assert(jaiu2 - "いう" == "ああ");
}

test "String * int" {
    assert(jaiu.times(2)=="あいうえおあいうえお");
}

test "String + + *3" {
    assert(ja + jaiu2 + "を".times(3) == "ああいうあいうををを");
}

test "String.endsWith" {
    assert(jk.endsWith("！") && jp.endsWith("c"));
}

test "String.equals" {
    assert(jp.equals("あaいaaあacいc"+"c"));
}

test "String.get" {
    assert(jk.get(8)=="ま");
}

test "String.get size over" {
    try {
        jk.get(13);
    } catch(OutOfIndex!! e) {
        assert(true);
    }
}

test "String.getSize" {
    assert(jp.getSize()==11);
}

test "String.indexOf" {
    assert(jp.indexOf("いc")==8); //14??
}

test "String.replace" {
    assert(jp.replace("あ","a")=="aaいaaaacいcc");
}

test "String.replace(2)" {
    assert(jaaa.replace(ja,"い") == "いいいいい");
}

test "String.startsWith" {
    assert(ja.startsWith("あ") && jp.startsWith("あaい"));
}

test "String.startsWith (long arg)" {
    assert(!ja.startsWith("sあえせ"));
}

test "String.toLower" {
    assert(jp.toLower() == jp);
}

test "String.toUpper" {
    assert(jp.toUpper() == "あAいAAあACいCC");
}

test "String.trim" {
    assert("　あ　".trim() == "　あ　");
}

test "String.trim(2)" {
    assert("　あ い う ".trim() == "　あ い う");
}

test "String.toLower" {
    String str = "SMALL";
    assert (str.toLower() == "small");
}

test "String.toUpper" {
    String str = "big";
    assert (str.toUpper() == "BIG");
}

test "String.trim" {
    String str = "  konoha  ";
    assert (str.trim() == "konoha");
}

test "String.twofold" {
    String str = "hello,konoha";
    Pair<String,String> p = str.twofold(",");
    assert (p.op0() == "hello" && p.op1() == "konoha");
}

/*-----String.indexOf .lastIndexOf-----*/

str1 = "this is konoha";
str2 = "ｔｈｉｓ　ｉｓ　ｋｏｎｏｈａ";
str3 = "ｔｈis ｉｓ ｋｏｎｏｈａ";

// Indexof

test ("String.indexOf @ascii") {
    assert (str1.indexOf("is") == 2);
}

test ("String.indexOf @mbstr") {
    assert (str2.indexOf("ｉｓ") == 2);
}

test ("String.indexOf @mixed") {
    assert (str3.indexOf("ｉｓ") == 5);
}

// indexOf:IgnoreCase

test ("String.indexOf:IgnoreCase @ascii") {
    assert (str1.indexOf:IgnoreCase("IS") == 2);
}

test ("String.indexOf:IgnoreCase @mbstr") {
    assert (str2.indexOf:IgnoreCase("ＩＳ") == 2);
}

test ("String.indexOf:IgnoreCase @mixed") {
    assert (str3.indexOf:IgnoreCase("ＩＳ") == 5);
}

// lastIndexOf
      
test ("String.lastIndexOf @ascii") {
    assert (str1.lastIndexOf("is") == 5);
}

test ("String.lastIndexOf @mbstr") {
    assert (str2.lastIndexOf("ｉｓ") == 5);
}

test ("String.lastIndexOf @mixed") {
    assert (str3.lastIndexOf("ｉｓ") == 5);
}

// lastIndexOf:IgnoreCase

test ("String.lastIndexOf:IgnoreCase @ascii") {
    assert (str1.lastIndexOf:IgnoreCase("IS") == 5);
}

test ("String.lastIndexOf:IgnoreCase @mbstr") {
    assert (str2.lastIndexOf:IgnoreCase("ＩＳ") == 4);
}

test ("String.lastIndexOf:IgnoreCase @mixed") {
    assert (str3.lastIndexOf:IgnoreCase("ＩＳ") == 5);
}

