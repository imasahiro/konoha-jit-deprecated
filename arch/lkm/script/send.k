#!/usr/local/bin/konoha

void main(String[] args)
{

    InputStream ins = new InputStream(args[1]);
    String file = "";
    foreach(String s in ins) {
        file += s + "\n";
    }
    OUT << file << EOL;
    OutputStream ous = new OutputStream("/dev/konoha", "w");
    ous <<  file + EOL;
    ous.close();
}

