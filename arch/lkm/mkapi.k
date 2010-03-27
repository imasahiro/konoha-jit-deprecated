#!/usr/local/bin/konoha
// generate EXPORT_SYMBOLs form include/konoha/konoha_api.h
void main (String[] args)
{
	ifile = "../../include/konoha/konoha_api.h";
	ofile = "./konoha/konoha_lkm_api.h";
	ins = new InputStream( ifile, "r");
	ous = new OutputStream(ofile, "w");
	ous << "#ifdef KONOHA_ON_LKM" << EOL;
	ous << "#define __tobytes(s)      knh_String_tobytes(s)" << EOL;
	foreach (String l in ins) {
		if ((l.startsWith("KNHAPI")) ||
				(l.startsWith("METHOD")) ||
				(l.startsWith("FASTAPI"))) {
			s = l.split(" ").get(1).split("(").get(0);
			ous << %("EXPORT_SYMBOL(%s{0});", s) << EOL;
		}
	}
	ous << %("EXPORT_SYMBOL(%s{0});", "setjmp") << EOL;
	ous << "#endif" << EOL;
	ous.close();
	ins.close();
}
