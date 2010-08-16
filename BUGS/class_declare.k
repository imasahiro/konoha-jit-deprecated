class A {
	int x;
}

class B extends A {

};

class C extends A;

void f (A a) {
	OUT << a.x << EOL;
}

f(new B());
f(new C());
