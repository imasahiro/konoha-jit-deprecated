int testSimpleLoop() {
    int i;
    for(i = 0; i < 100_000_000; i++) {
    }
    return i;
}
int testLocalVariable() {
    y = 0;
    for(i = 0; i < 100_000_000; i++) {
        y += 1;
    }
    return y;
}

float testFloatOperation() {
    f = 0.0;
    for(i = 0; i < 100_000_000; i++) {
        f += 0.1;
    }
    return f;
}

class Dim {
	int x;
	int y;
	int z;
}

Dim testFieldVariable() {
	d = new Dim();
	for(i = 0; i < 100_000_000; i++) {
		d.y = 1;
	}
	return d;
}



void main (String[] args)
{
    s = System.getTime();
    print testSimpleLoop();
    period = System.getTime() - s;
    score = period;
    print "SimpleLoop[ms]: ", period, score;
    delta = period;

    s = System.getTime();
    print testLocalVariable();
    period = System.getTime() - s;
    score = period - delta;
    print "LocalVariable[ms]:", period, score;
    //

    s = System.getTime();
    print testFloatOperation();
    period = System.getTime() -s;
    score = period - delta;
    print "FloatOperation[ms]:", period, score;

    //s = System.getTime();
    //testFieldVariable();
    //period = System.getTime() -s;
    //score = period - delta;
    //print "FieldVariable[mops,ms]:", period, score;
    //

}
