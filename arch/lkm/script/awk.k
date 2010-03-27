void main (String[] args)
{
    i = new InputStream("/proc/devices");
    String id = "";
    while(!(s = i.readLine()).split("..",1).get(0).split(" ",1).opHas("script"));
    OUT << s.split("..",1).get(0).split(" ",1).get(0);
}
