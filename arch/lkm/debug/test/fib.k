#!/usr/local/bin/konoha
using debug.*;

int fib(int n) {
	if(n<3) return 1;
	return fib(n-1)+fib(n-2);
}

void main(String[] argv) {
	int n = 36;
	print argv;
	if(|argv| > 1) {
		n = (int)argv[1];
	}
	OUT << "fib(" << n << "): ";
	OUT << fib(n) << EOL;
}
(Script.main).setDebugMode();
//(Script.fib).setDebugMode();

