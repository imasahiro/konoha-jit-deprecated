#!/usr/local/bin/konoha-0.5

String Array.last()
{
    return (String) this[|this| - 1];
}

String[] removeEmpty(String[] a)
{
    r = new String[]();
    foreach (s in a) {
        if (s.size == 0) 
            continue;
        r.add(s);
    }
    return r;
}

void main (String[] args)
{
    ins = new InputStream("./gentools/gen_vm.py");
    String buf = "";
    insts = [[],[],[],[]];
    foreach (String s in ins) {
        if ("INSTRUCTIONS" in s)
            break;
    }
    foreach (String s in ins) {
        if ("\"\"\"" in s)
            break;
        if (s.size == 0)
            continue;
        if (s.startsWith("#")) 
            continue;
        a = s.split(" ", 1);
        a = removeEmpty(a);
        if (a.last() == "@0") {
            a.pop();
            insts[0].add(a);
        }
        else if (a.last() == "@2") {
            a.pop();
            insts[2].add(a);
        }
        else if (a.last() == "@3") {
            a.pop();
            insts[3].add(a);
        }
        else {
            insts[1].add(a);
        }
    }
    for (int i=0; i < |insts|; i++) {
        inst = insts[i];
        OUT << "== Level " + i + " instruction" << EOL;
        foreach (code in inst) {
            OUT << "=== " + code[0] << EOL;
            for (int j=0; j < |code|; j++) {
                String v = code[j];
                op = v;
                if (":" in v) {
                    t = v.split(":");
                    o = t[1];
                    op = %("(%s{1}) %s{0}", t[0], o);
                }
                OUT << "||" + op;
            }
            OUT << "||" << EOL << EOL;
        }
    }
}

