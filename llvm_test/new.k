class A{
	int x;
	int y;
	
	A(int x, int y){
		_x = x;
		_y = y;
	}
}

void main(){
	A a = new A(12, 30);
	print a;
	print a.x;
	//print a.y;
}
