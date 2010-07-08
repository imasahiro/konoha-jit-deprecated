
int fibo(int n) {
  if(n < 3) return 1;
  return fibo(n-1) + fibo(n-2);
}

int fibo2(int n) {
  if(n > 2) return fibo2(n-1) + fibo2(n-2);
  return 1;
}

{
	n = 36;
	s = System.getTime();
	print "fibo(40):", fibo(n);
	e = System.getTime();
	print "\ttime=", e - s;
	s = System.getTime();
	print "fibo2(40):", fibo2(n);
	e = System.getTime();
	print "\ttime=", e - s;
}

