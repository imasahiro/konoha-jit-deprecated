int fib(int n){
	if(n < 3){
		return 1;
	}else{
		return fib(n-1) + fib(n-2);
	}
}

void main(void){
    OUT.println("finish");
    if(fib(40) == 102334155){
        OUT.println("call ok");
    } else {
        OUT.println("call fail");
    }
	return;
}

