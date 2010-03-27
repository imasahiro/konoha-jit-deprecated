#!/usr/local/bin/konoha

void main(String[] args)
{

    InputStream ins;
    OutputStream out;
    String str, buf;
    int brace_count,num_of_brace = 0;
    while (1) {
        ins = new InputStream("/dev/konoha", "r");
        out = new OutputStream("/dev/konoha", "w");
        OUT << "(kshell) ";
        if (brace_count == 0) {
            OUT << ">>";
        }
        else {
            OUT << "..";
        }
        str = IN.readLine();
        num_of_brace  = (str.split("{",0).size - 1);
        num_of_brace -= (str.split("}",0).size - 1);
        brace_count += num_of_brace;
        if (brace_count == 0) {
            out << buf + str;
            buf = "";
            brace_count = 0;
            s = ins.readLine();
            OUT <<  s + EOL;
        }
        else {
            buf += str;
        }
        ins.close();
        out.close();
    }
    ins.close();
    out.close();
}

