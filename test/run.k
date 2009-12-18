void main (String[] args)
{
	try {
		if(|args| != 3) {
		}
		dir = args[2];
		String[] files = os.listDir(dir);
		foreach(String file in files) {
			if (file.endsWith(".k") && file.split("/",0).pop() != args[0].split("/",0).pop()) {
				file = dir + "/" + file;
				int ret = system(args[1] + " " + file);
				if(ret != 0) {
					ERR << "test fail" << EOL;
					break;
				}
			}
		}
	} catch(Exception e) {
			ERR << %("usage : %s{0} path/to/konoha path/to/test",args[0]) << EOL;
	}
}
