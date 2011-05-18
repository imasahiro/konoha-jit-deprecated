int fib(int n){
	if(n < 3){
		return 1;
	}else{
		return fib(n-1) + fib(n-2);
	}
}

void main(void){
	int v = fib(36);
	OUT.println("finish");
	if(fib(10) == 55){
		OUT.println("call ok");
	}
	return;
}

