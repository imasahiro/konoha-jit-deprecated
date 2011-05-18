int fact(int n){
    if(n == 0){
        return 1;
    }
    return n * fact(n-1);
}

void main(void){
    int v = fact(3);
    OUT.println(v);
}

