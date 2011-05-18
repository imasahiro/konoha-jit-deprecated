int fib(int n){
	if(n < 3){
		return 1;
	}else{
		return fib(n-1) + fib(n-2);
	}
}

int f(int n){
	OUT.println("f hello!!");
	return n+1;
}

void main(String[] args){
	fib(10);
	if(f(10) == 11){
		OUT.println("call ok");
	}
	return;
}


