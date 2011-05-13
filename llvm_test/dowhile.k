int f0(int i){
	do{
		i++;
	}while(i < 10);
	return i;
}

int f1(int i){
	do{
		i++;
	}while(true);
	return i;
}

int f2(int i, int j){
	do{
		i++;
	}while(i < j);
	return i;
}

int f3(boolean x, boolean y){
	do{
		x = !x;
	}while(x and y);
	return 1;
}

