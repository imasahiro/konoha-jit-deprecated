using debug.*;
String ns = __ns__;
void f() {
	int i = 10;
	OUT << ns << "hi" << EOL;
}
void g() {
	f();
}



void main (String[] args)
{
	f();
}
(Script.f).setDebugMode();
