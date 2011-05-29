
int f0(int[] a, int n){
	a[n] = 123;
	return a[n];
}

double f1(double[] a, int n){
	a[n] = 1.23;
	return a[n];
}

String f2(String[] a, int n){
	a[n] = "abc";
	return a[n];
}

void main(){
	int[] a = new int[8];
	print f0(a, 1);

	float[] b = new float[8];
	print f1(b, 1);

	String[] c = new String[8];
	print f2(c, 1);

	return;
}




