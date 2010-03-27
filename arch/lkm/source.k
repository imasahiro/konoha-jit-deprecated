#!/usr/local/bin/konoha
// generate class definition form ./src/knh_Chardev.c
void main (String[] args)
{
	ifile = "./src/knh_Chardev.c";
	ofile = "./script/Chardev.k";
	ins = new InputStream( ifile, "r");
	ous = new OutputStream(ofile, "w");
	ous << "class Chardev;" << EOL;
	ous << "class Task;" << EOL;
	ous << "class File;" << EOL << EOL;

	foreach (String l in ins) {
		if ((l.startsWith("//##"))) {
			s = l.split("method",1).get(1);
			ous << s << EOL;
			print s;
			//ous << %("EXPORT_SYMBOL(%s{0});", s) << EOL;
		}
	}
	//ous << "#endif" << EOL;
	ous.close();
	ins.close();
}
