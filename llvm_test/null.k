
int func(String s){
	if(s != null){
		return 1;
	}else{
		return 2;
	}
}

void main(){
	print func(null);
	print func("hello");
}

