c = new Chardev("script");
c.register();
void Chardev.read(File file, OutputStream ous)
{
  ous << "hello world" << EOL;
}
