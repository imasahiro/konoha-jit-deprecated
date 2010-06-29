#!/usr/local/bin/konoha

int fib(int n) {
    if(n<3) return 1;
    return fib(n-1)+fib(n-2);
}

void main(String[] argv) {
    int n = 36;
    if(|argv| > 0) {
	n = (int)argv[0];
    }
    OUT << "fib(" << n << "): ";
    OUT << fib(n) << EOL;
}

