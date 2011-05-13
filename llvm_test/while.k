int f0(boolean b){
	int i=0;
	while(i < 10){
		i++;
	}
	return i;
}

int f1(){
	int i=0;
	while(true){
		i++;
	}
	return i;
}

int f2(int i, int j){
	while(i < j){
		i++;
	}
	return i;
}

int f3(boolean x, boolean y){
	while(x && y){
		x = !x;
	}
	return 1;
}

float f4(float x, float y){
	while(x < y){
		x += y;
	}
	return x;
}


