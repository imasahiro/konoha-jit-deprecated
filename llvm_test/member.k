class A{
    int x;
    float y;
    String z;
    int w;
    A(int x, double y, String z){
        _x = x;
        _y = y;
        _z = z;
    }

    int func(){
        return x + 1;
    }
}

void main(){
    A a = new A(12, 3.45, "hoge");
    a.w = a.x;
    print a;
    //print a.w;
    //print a.func();
    return;
}
