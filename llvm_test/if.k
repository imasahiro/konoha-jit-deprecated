int f0(int x, int y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

int f1(int n){
    int x = n;
    if (n > 0){
        x = x;
    }
    else {
        x = -x;
    }
    return x + 1;
}

void main (String[] args)
{
    print f0(10, 20);
    print f1(10);
}
