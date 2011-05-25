
String f0(String[] a, int n){
	return a[n];
}

String f1(String[] a){
	return a[0];
}

Int f2(Int[] a, int n){
	return a[n];
}

void main(String[] args){
    print args;
    print args[0];
    print f0(args, 0)
    print f1(args)
    return;
}

