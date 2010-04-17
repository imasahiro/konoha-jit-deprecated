void main (String[] args)
{
	try {
		if(|args| != 3) {
		}
		dir = args[2];
		flag = false;
		String[] files = os.listDir(dir);
		foreach(String file in files) {
			if (file.endsWith(".k") && file.split("/",0).pop() != args[0].split("/",0).pop()) {
				file = dir + file;
				OUT << "file=" + file << EOL;
				int ret = system(args[1] + " -g " + file);
				if(ret != 0) {
					flag = true;
				}
			}
		}
		if (flag) {
			ERR << "test fail" << EOL;
		}
	} catch(Exception e) {
			ERR << %("usage : %s{0} path/to/konoha path/to/test",args[0]) << EOL;
	}
}
