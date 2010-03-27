class Chardev;
class Task;
class File;

Chardev! Chardev.new(String! devname);
void Chardev.open(File! file);
@Virtual void Chardev.read(File! file, OutputStream ous);
@Virtual void Chardev.write(File! file, InputStream ins);
Boolean! Chardev.register();
Boolean! Chardev.unregister();
Int! File.getMode();
Int! Task.getPid();

class MyDev extends Chardev {};
void MyDev.read(File! file, OutputStream ous)
{
	ous << "hello world" << EOL;
}

void MyDev.write(File! file, InputStream ins)
{
	OUT << ins.readLine() << EOL;
}
