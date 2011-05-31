String s = "abcd";
void f() {
    s = "1234";
}

//class A {
//    A() {}
//    void func() {
//        __s = "xxxx";
//    }
//}

void main (String[] args)
{
    f();
    print s;
    A a = new A();
    a.func();
    print s;
}
