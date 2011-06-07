class A{
	int x;
	int y;
	A(){
		_x = 123;
		_y = 456;
	}
	
	int y(void){
		return _y;
	}
}
void main(void){
	A a = new A();
	int i = 789;
	OUT  <<< a + " " + i*2 + " " + a.y() <<< EOL;
		OUT.flush();
}

