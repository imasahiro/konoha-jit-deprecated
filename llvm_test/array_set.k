void f0_set(String[] a, int n, String v){
    a[n] = v;
}

void main (String[] args)
{
    a = [1, 2];
    a[0] = 10;
    print a;
    f0_set(args, 0, "hello");
    print args;
}
