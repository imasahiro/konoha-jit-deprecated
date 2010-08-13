
def testSimpleLoop
  i = 0
  100000000.times{ |i|
  }
  return i
end


def testLocalVariable
  y = 0
  100000000.times { |i|
    y = i
  }
  return y
end

$x = 0
$y = 0
$z = 0

def testGlobalVariable
  100000000.times { $y = 1
  }
  return $y
end

def testStringAssignment
  y = ""
  100000000.times { |i|
    y = "a"
  }
  return y
end

def testIntegerOperation
  y = 0
  100000000.times { |i|
    y += 1
  }
  return y
end

def testFloatOperation
  y = 0.0
  100000000.times { |i|
    y += 0.1
  }
  return y
end

def func0
end

def testFunctionCall
  i = 0
  100000000.times { |i|
    func0()
  }
  return i
end

def func1
  return 1
end

def testFunctionReturn
  res = 0
  100000000.times {
    res = func1()
  }
  return res
end

def testMathFabs
  100000000.times {
    (-0.1).abs()
  }
  return 0
end


def testCallFunctionObject
  f = func1
  res = 0
  100000000.times {
    res = f()
  }
  return res
end

def fibo(n)
  if n < 3
    1
  else
    fibo(n-1) + fibo(n-2)
  end
end

class Dim
  def initialize
    @x = 0
    @y = 0
    @z = 0
  end
  attr_accessor :x, :y, :z

  def f
  end
end

def testObjectCreation
  i = 0
  100000000.times { |i|
    d = Dim.new
  }
  return i
end

def testFieldVariable
  i = 1
  d = Dim.new
  100000000.times{ |i|
    d.y += 1
  }
  return i
end 


def testMethodCall
  i = 1
  d = Dim.new
  100000000.times { |i|
    d.f()
  }
  return i

end

def mops(score)
	if(score < 1)
		return 0
	end
	return (10000000 / score) / 100.0
end




def run
	s = Time.now.to_f
	testSimpleLoop()
	t = (Time.now.to_f - s) * 1000
	score = t
	print("SimpleLoop[mops,ms]: \tt=", mops(score), ", ", t , "\n")
	delta = t

 	s = Time.now.to_f
 	testLocalVariable()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("LocalVariable[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testGlobalVariable()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("GlobalVariable[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testStringAssignment()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("StringAssignment[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testIntegerOperation()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("IntegerOperation[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testFloatOperation()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("FloatOperation[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testFunctionCall()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("FunctionCall[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testFunctionReturn()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("FunctionReturn[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	testMathFabs()
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("NativeFunction[mops,ms]: \tt=", mops(score), ", ", t , "\n")

 	s = Time.now.to_f
 	t = (Time.now.to_f - s) * 1000
 	score = t - delta
 	print("CallFunctionObject[mops,ms]: \tt=", mops(score), ", ", t , "\n")

    print("Object-Oriented Programming")
	s = Time.now.to_f
    testObjectCreation()
	t = (Time.now.to_f - s) * 1000
	score = t - delta
	print("ObjectCreation[mops,ms]: \tt=", mops(score), ", ", t , "\n")

	s = Time.now.to_f
	testFieldVariable()
	t = (Time.now.to_f - s) * 1000
	score = t - delta
	print("FieldVariable[mops,ms]: \tt=", mops(score), ", ", t , "\n")

	s = Time.now.to_f
	testMethodCall()
	t = (Time.now.to_f - s) * 1000
	score = t - delta
	print("MethodCall[mops,ms]: \tt=", mops(score), ", ", t , "\n")

    print("MISC (this is not microbench")
	s = Time.now.to_f
	fibo(36);
	t = (Time.now.to_f - s) * 1000
	score = t - delta
	print("fibonacci(36)[mops,ms]: \tt=", mops(score), ", ", t , "\n")


end
run()

