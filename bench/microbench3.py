import time, math

def testSimpleLoop():
	i = 0
	while i < 10000000:
		i += 1
	return i

def testLocalVariable():
	i = 0
	y = 0
	while i < 10000000:
		y = i
		i += 1
	return y

global_x = 0
global_y = 0
global_z = 0

def testGlobalVariable():
	global global_y
	i = 0
	while i < 10000000:
		global_y = 1
		i += 1
	return global_y

def testStringAssignment():
	i = 0
	y = ""
	while i < 10000000:
		y = "2"
		i += 1
	return y

class Dim:
	x = 0
	y = 0
	z = 0
	def __init__(self):
		pass

	def f(self):
		pass

def testObjectCreation():
	i = 0
	while i < 10000000:
		d = Dim()
		i += 1
	return i

def testFieldVariable():
	d = Dim()
	i = 0
	while i < 10000000:
		d.y = i
		i += 1
	return d

def testMethodCall():
	d = Dim()
	i = 0
	while i < 10000000:
		d.f()
		i += 1
	return d

def testIntegerOperation():
	i = 0
	y = 0
	while i < 10000000:
		y += 1
		i += 1
	return y

def testFloatOperation():
	i = 0
	y = 0.0
	while i < 10000000:
		y += 0.2
		i += 1
	return y

def func0():
	pass

def testFunctionCall():
	i = 0
	while i < 10000000:
		func0()
		i += 1
	return i

def func1():
	return 1

def testFunctionReturn():
	i = 0
	y = 0
	while i < 10000000:
		y = func1()
		i += 1
	return y

def testMathFabs():
	i = 0
	f = 1.0
	while i < 10000000:
		i += 1
		f = math.fabs(-1.0)
	return f

def testCallFunctionObject():
	i = 0
	f = func0
	while i < 10000000:
		i += 1
		f()
	return i

def testFibo(n):
	if n < 3 : return 1
	return testFibo(n-1) + testFibo(n-2)

def mops(score):
	return 10 * 1000 / score

def run():
	s = time.time()
	testSimpleLoop()
	period = (time.time() - s) * 1000
	print "SimpleLoop[mpos,ms]: ", mops(period), period
	delta = period
	##
	s = time.time()
	testLocalVariable()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "LocalVariable[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testGlobalVariable()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "GlobalVariable[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testStringAssignment()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "StringAssignment[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testIntegerOperation()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "IntegerOperation[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testFloatOperation()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "FloatOperation[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testFunctionCall()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "FunctionCall[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testFunctionReturn()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "FunctionReturn[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testMathFabs()
	period = ((time.time() - s) * 1000)
	score = period - (delta)
	print "NaitiveFunction[mpos,ms]:", mops(score), period
	##
	s = time.time()
	testCallFunctionObject()
	period = ((time.time() - s) * 1000)
	score = period - (delta)
	print "CallFunctionObject[mpos,ms]:", mops(score), period
	##
	print "Object-Oriented Programming"
	s = time.time()
	testObjectCreation()
	period = (time.time() - s) * 1000
	score = period - delta
	print "ObjectCreation[mpos,ms]:", mops(score), period
	s = time.time()
	testFieldVariable()
	period = (time.time() - s) * 1000
	score = period - delta
	print "FieldVariable[mpos,ms]:", mops(score), period
	s = time.time()
	testMethodCall()
	period = (time.time() - s) * 1000
	score = period - delta
	print "MethodCall[mpos,ms]:", mops(score), period
	##
	print "misc (it might take a lot of time)"
	s = time.time()
	testFibo(36)
	period = ((time.time() - s) * 1000)
	score = period
	print "fibonacci(36)[mpos,ms]:", mops(score/10), period

run()


