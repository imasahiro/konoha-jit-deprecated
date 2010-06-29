-- Lua microbench by kimio kuramitsu

function testSimpleLoop()
	for i = 0, 100000000 do
	end
	return i
end

function testLocalVariable()
	y = 0
	for i = 0, 100000000 do
		y = y + 1
	end
	return y
end

global_x = 0
global_y = 0
global_z = 0

function testGlobalVariable()
	for i = 0, 100000000 do
		global_y = global_y + 1
	end
	return global_y
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

function testFloatOperation()
	f = 0.0
	for i = 0, 100000000 do
		f = f + 0.1
	end
	return f
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

--

function testFieldVariable()
	d = Dim.new()
	for i = 0, 100000000 do
		d.y = d.y + 1
	end
	return d
end


function run()
	s = os.clock() * 1000
	testSimpleLoop()
	period = os.clock() * 1000 - s
	score = period
	print("SimpleLoop[ms]: ", "period", string.format("%d", period), "score", string.format("%d", score))
	delta = period
	--
	s = os.clock() * 1000
	testLocalVariable()
	period = os.clock() * 1000 - s
	score = period - delta
	print("LocalVariable[ms]:", "period", string.format("%d", period), "score", string.format("%d", score))
	--
	s = os.clock() * 1000
	testGlobalVariable()
	period = os.clock() * 1000 - s
	score = period - delta
	print("GlobalVariable[ms]:", "period", string.format("%d", period), "score", string.format("%d", score))
	--
	s = os.clock() * 1000
	testFunctionCall()
	period = os.clock() * 1000 - s
	score = period - delta
	print("FunctionCall[ms]:", "period", string.format("%d", period), "score", string.format("%d", score))
	--
	s = os.clock() * 1000
	testFunctionReturn()
	period = os.clock() * 1000 - s
	score = period - delta
	print("FunctionReturn[ms]:", "period", string.format("%d", period), "score", string.format("%d", score))
	--
	s = os.clock() * 1000
	testFloatOperation()
	period = os.clock() * 1000 - s
	score = period - delta
	print("FloatOperation[ms]:", "period", string.format("%d", period), "score", string.format("%d", score))
	--
	print("Object-Oriented Programming")
	s = os.clock() * 1000
	testFieldVariable()
	period = os.clock() * 1000 - s
	score = period - delta
	print("FieldVariable[ms]:", "period", string.format("%d", period), "score", string.format("%d", score))
end

run()
