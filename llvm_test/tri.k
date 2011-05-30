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

MINDEPTH = 4;
int g() {
    int n = 10;
    int maxDepth = (MINDEPTH + 2 > n) ? MINDEPTH + 2 : n;
    int stretchDepth = maxDepth + 1;
    return stretchDepth;
}

void main (String[] args)
{
    print f0( 10);
    print f0(-10);
    print f1( 10.0);
    print f1(-10.0);
    print f2(true);
    print f2(false);
    print g();
    print g();
}
