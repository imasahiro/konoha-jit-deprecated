class A{
	int x;
	float y;
	String z;
	int w;
	
	A(int x, double y, String z){
		_x = x;
		_y = y;
		_z = z;
		_w = _x * 2;
	}

	int func(){
		return x + 1;
	}
}

void main(){
	A a = new A(12, 3.45, "hoge");
	print a.x;
	print a.y;
	print a.z;
	print a.w;
	print a.func();

	return;
}
