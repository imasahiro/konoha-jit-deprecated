
void main(){
	int[] a = [];
	a.add(10);
	a.add(20);
	a.add(30);
	a.add(40);

	print a.size;
	print a;

	a.remove(1);

	int i;
	for(i=0; i<a.size; i++){
		print a[i];
	}
}

