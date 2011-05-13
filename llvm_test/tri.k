float f1(float x){
	float y = (x > (float)0 ) ? x : -x;
	return y;
}

int f0(int x){
	return (x > 0) ? x : -x ;
}

boolean f2(boolean x){
	return (x == true) ? x : !x ;
}
