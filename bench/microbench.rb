# Ruby microbench 

def testSimpleLoop
	i = 0
	100000000.times { |i|
	}
	return i
end

def testLocalVariable
	y = 0
	100000000.times { |i|
		y += i
	}
	return y
end

$x = 0
$y = 0
$z = 0

def testGlobalVariable
	100000000.times{ 
		$y += 1
	}
	return $y
end

def func0
end

def testFunctionCall
	i = 0
	100000000.times{ |i|
		func0()
	}
	return i
end

def func1
	return 1
end

def testFunctionReturn
	res = 0
	100000000.times{
		res = func1()
	}
	return res
end

def testFloatOperation
	f = 0
	100000000.times{
		f += 0.1
	}
	return f
end

class Dim
	def initialize
		@x = 0
		@y = 0
		@z = 0
	end
	attr_accessor :x, :y, :z
end 

def testFieldVariable
	d = Dim.new
	100000000.times{
		d.y += 1
	}
	return d
end 

def run
	s = Time.now.to_f
	testSimpleLoop()
	period = (Time.now.to_f - s) * 1000
	score = period
	print("SimpleLoop[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")
	delta = period

	s = Time.now.to_f
	testLocalVariable()
	period = (Time.now.to_f - s) * 1000
	score = period - delta
	print("LocalVariable[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")

	s = Time.now.to_f
	testGlobalVariable()
	period = (Time.now.to_f - s) * 1000
	score = period - delta
	print("GlobalVariable[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")

	s = Time.now.to_f
	testFunctionCall()
	period = (Time.now.to_f - s) * 1000
	score = period - delta
	print("FunctionCall[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")

	s = Time.now.to_f
	testFunctionReturn()
	period = (Time.now.to_f - s) * 1000
	score = period - delta
	print("FunctionReturn[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")

	s = Time.now.to_f
	testFloatOperation()
	period = (Time.now.to_f - s) * 1000
	score = period - delta
	print("FloatOperation[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")

	s = Time.now.to_f
	testFieldVariable()
	period = (Time.now.to_f - s) * 1000
	score = period - delta
	print("FieldVariable[ms]: \tperiod=", period.to_i, ", score=", score.to_i , "\n")

end
run()

