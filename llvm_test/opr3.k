/*work*/
int f5(int x, int y, int z){
	return x + y * z;
}

/*work*/
int f4(int x, int y, int z){
	int tmp = x * y + z;
	return tmp;
}

/*not work*/
int f3(int x, int y, int z){
	return  x * y + z;
}

/*work*/
int f2(int x, int y, int z){
	int tmp = x + y;
	return tmp + z;
}

///*not work*/
//int f1(int x, int y, int z){
//	return x + y + z;
//}
//
///*not work*/
//int f0(int x, int y, int z){
//	int ret = x + y + z;
//	return ret;
//}
