void main (String[] args)
{
	ins = new InputStream("./src/kc/konoha_code_.c");
	tab = "	";
	t = tab;
	ous = new OutputStream("./include/konoha/konoha_ctt.h","w");
	ous << "#ifndef KONOH_CTT_H" << EOL;
	ous << "#define KONOH_CTT_H" << EOL << EOL;

	foreach(String l in ins) {
		if(l.opHas("switch(KNH_OPCODE(pc))") == false) {
			continue;
		}
		break;
	}

	foreach(String l in ins) {
		if (l.opHas("KLR_")) {
			op  = l.trim().split("(",0).get(0);
			cop = op.replace("KLR","JIT_ASM");
			ous << "#undef " + op << EOL;
			ous << %("#define %s{0} %s{1}",op,cop) << EOL;
		}
	}
	ous << EOL << "#endif" << EOL;
	ous.close();
	ins.close();
}
