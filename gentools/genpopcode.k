//    PCODEDATA
//data = """
//#define PCODE_HALT    (OPCODE_MAX + 0)
//""";
void main (String[] args) {
    ins = new InputStream("./src/lang/opcode.h");
    ous = new OutputStream("pcode_opcode.txt");
    String[] a = [];
    int i = 0;
    foreach (String s in ins) {
        if (i == 0) {
            if (s.indexOf("PCODEDATA") >= 0) {
                i = 1;
            }
            continue;
        }
        if (i == 1 && s.indexOf("};") >= 0) {
            break;
        }
        if (s.indexOf("{") > 0) {
            v = s.split(",").get(0).split("{").get(1);
            a.add(v);
        }

    }
    for (i=0; i < a.size; i++) {
        String s = a[i];
        ous.println("#define " + s + " (OPCODE_MAX + " + i + ")");
    }
}
