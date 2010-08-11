// javascript microbench

function testSimpleLoop() {
    for(var i = 0; i < 100000000; i++) { }
    return i;
}

function testLocalVariable() {
    var y = 0;
    for(var i = 0; i < 100000000; i++) {
        y = i;
    }
    return y;
}

global_x = 0;
global_y = 0;
global_z = 0;

function testGlobalVariable() {
    for(var i = 0; i < 100000000; i++) {
        global_y = i;
    }
    return global_y;
}

function testStringAssignment() {
    var s = "";
    for(var i = 0; i < 100000000; i++) {
        s = "A";
    }
    return s;
}

function testIntegerOperation() {
    var y = 0;
    for(var i = 0; i < 100000000; i++) {
        y += 1;
    }
    return y;
}

function testFloatOperation() {
    var f = 0.0;
    for(var i = 0; i < 100000000; i++) {
        f += 0.1;
    }
    return f;
}

function func0() {}

function testFunctionCall() {
    for(var i = 0; i < 100000000; i++) {
        func0();
    }
    return i;
}

function func1() { return 1; }

function testFunctionReturn() {
    var res = 0;
    for (var i = 0; i< 100000000; i++) {
        res = func1();
    }
    return res;
}

function testMathFabs() {
    for(var i = 0; i< 100000000; i++) {
        Math.abs(-1.0);
    }
    return i;
}

function testCallFunctionObject() {
    f = func0;
    for(var i = 0; i< 100000000; i++) {
        f();
    }
    return i;
}

function fibo(n) {
    if (n < 3) return 1;
    return fibo(n-1) + fibo(n-2);
}

//

function Dim() {
    this.x = 0;
    this.y = 0;
    this.z = 0;
}

function testFieldVariable() {
    d = new Dim();
    for(var i = 0; i < 100000000; i++) {
        d.y = 1;
    }
    return d;
}

function mops(score) {
    if (score < 1) return 0;
    return (10000000 / score) / 100.0;
}

function run() {
    s = new Date().getTime();
    testSimpleLoop();
    t = new Date().getTime() - s;
    score = t;
    print("SimpleLoop[mops,ms]: ", mops(score), t);
    delta = t;
    //
    s = new Date().getTime();
    testLocalVariable();
    t = new Date().getTime() - s;
    score = t - delta;
    print("LocalVariable[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testGlobalVariable();
    t = new Date().getTime() - s;
    score = t - delta;
    print("GlobalVariable[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testStringAssignment();
    t = new Date().getTime() - s;
    score = t - delta;
    print("StringAssignment[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testIntegerOperation();
    t = new Date().getTime() -s ;
    score = t - delta;
    print("IntegerOperation[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testFloatOperation();
    t = new Date().getTime() -s ;
    score = t - delta;
    print("FloatOperation[mops,ms]:", mops(score), t);
    // 
    s = new Date().getTime();
    testFunctionCall();
    t = new Date().getTime() - s;
    score = t - delta;
    print("FunctionCall[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testFunctionReturn();
    t = new Date().getTime() - s;
    score = t - delta;
    print("FunctionReturn[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testMathFabs();
    t = new Date().getTime() - s;
    score = t - delta;
    print("NativeFunction[mops,ms]:", mops(score), t);
    //
    s = new Date().getTime();
    testCallFunctionObject();
    t = new Date().getTime() - s;
    score = t - delta;
    print("CallFunctionObject[mops,ms]:", mops(score), t);
    //
    print("Object-Oriented Programming");
    s = new Date().getTime();
    testFieldVariable();
    t = new Date().getTime() -s ;
    score = t - delta;
    print("FieldVariable[mops,ms]:", mops(score), t);
    //
    print("MISC (this is not microbench)");
    s = new Date().getTime();
    fibo(36);
    t = new Date().getTime() - s;
    score = t;
    print("fibonacci(36)[mops,ms]:", mops(score), t);
}

run();

