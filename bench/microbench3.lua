-- Lua microbench by kimio kuramitsu

function testSimpleLoop()
	for i = 0, 100000000 do
	end
	return i
end

function testLocalVariable()
	y = 0
	for i = 0, 100000000 do
		y = i
	end
	return y
end

global_x = 0
global_y = 0
global_z = 0

function testGlobalVariable()
	for i = 0, 100000000 do
		global_y = 1
	end
	return global_y
end

function testStringAssignment()
	str = "";
	for i = 0, 100000000 do
		str = "A";
	end
	return str;
end

function testIntegerOperation()
	y = 0;
	for i = 0, 100000000 do
		y = y + 1;
	end
	return y;
end

function testFloatOperation()
	f = 0.0;
	for i = 0, 100000000 do
		f = f + 0.1;
	end
	return f;
end


function func0()
end

function testFunctionCall()
	i = 0
	for i = 0, 100000000 do
		func0()
	end
	return i
end

function func1()
	return 1
end

function testFunctionReturn()
	res = 0
	for i = 0, 100000000 do
		res = func1()
	end
	return res
end

function testMathFabs()
	for i = 0, 100000000 do
		math.abs(-0.1);
	end
	return i
end

function testCallFunctionObject()
	f = func1;
	for i = 0, 100000000 do
		f();
	end
	return i;
end

Dim = {}
function Dim.new()
	local instance = {
		x = 0,
		y = 0,
		z = 0
	}
	return instance
end

function Dim:f()
end

--

function testObjectCreation()
	for i = 0, 100000000 do
		d = Dim.new()
	end
	return d
end

function testFieldVariable()
	d = Dim.new()
	for i = 0, 100000000 do
		d.y = d.y + 1
	end
	return d
end


function testMethodCall()
	d = Dim.new()
	for i = 0, 100000000 do
		d.f();
	end
	return d
end


function fibo(n)
	if n < 3 then
		return 1
	else
		return fibo(n-1) + fibo(n-2)
	end
end

function mops(score) 
	--if n < 1.0 then 
	--	return 0.0;
	--end
	return (10000000 / score) / 100.0;
end

function run()
	s = os.clock() * 1000
	testSimpleLoop()
	t = os.clock() * 1000 - s
	score = t
	print("SimpleLoop[mops,ms]: ", string.format("%f", mops(score)), string.format("%d", t))
	delta = t
	--
	s = os.clock() * 1000
	testLocalVariable()
	t = os.clock() * 1000 - s
	score = t - delta
	print("LocalVariable[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testGlobalVariable()
	t = os.clock() * 1000 - s
	score = t - delta
	print("GlobalVariable[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testStringAssignment()
	t = os.clock() * 1000 - s
	score = t - delta
	print("StringAssignment[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testIntegerOperation()
	t = os.clock() * 1000 - s
	score = t - delta
	print("IntegerOperation[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testFloatOperation()
	t = os.clock() * 1000 - s
	score = t - delta
	print("FloatOperation[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testFunctionCall()
	t = os.clock() * 1000 - s
	score = t - delta
	print("FunctionCall[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testFunctionReturn()
	t = os.clock() * 1000 - s
	score = t - delta
	print("FunctionReturn[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testCallFunctionObject()
	t = os.clock() * 1000 - s
	score = t - delta
	print("CallFunctionObject[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	s = os.clock() * 1000
	testMathFabs()
	t = os.clock() * 1000 - s
	score = t - delta
	print("NativeFunction[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	--
	print("Object-Oriented Programming")
	s = os.clock() * 1000
	testObjectCreation()
	t = os.clock() * 1000 - s
	score = t - delta
	print("ObjectCreation[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
	s = os.clock() * 1000
	testFieldVariable()
	t = os.clock() * 1000 - s
	score = t - delta
	print("FieldVariable[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))

	s = os.clock() * 1000
	testMethodCall()
	t = os.clock() * 1000 - s
	score = t - delta
	print("MethodCall[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))

	--
	print "MISC (this is not microbench)";
	s = os.clock() * 1000
	fibo(36)
	t = os.clock() * 1000 - s
	score = t - delta
	print("fibonacci[mops,ms]:", string.format("%f", mops(score)), string.format("%d", t))
end

run()
