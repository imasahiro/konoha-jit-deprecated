import time, math

def testSimpleLoop():
	i = 0
	while i < 100000000:
		i += 1
	return i

def testLocalVariable():
	i = 0
	y = 0
	while i < 100000000:
		y += 1
		i += 1
	return y

global_x = 0
global_y = 0
global_z = 0

def testGlobalVariable():
	global global_y
	i = 0
	while i < 100000000:
		global_y += 1
		i += 1
	return global_y

def func0():
	pass

def testFunctionCall():
	i = 0
	while i < 100000000:
		func0()
		i += 1
	return i

def func1():
	return 1

def testFunctionReturn():
	i = 0
	y = 0
	while i < 100000000:
		y = func1()
		i += 1
	return y

def testFloatOperation():
	i = 0
	y = 0.0
	while i < 100000000:
		y += 0.2
		i += 1
	return y

class Dim:
	x = 0
	y = 0
	z = 0
	def __init__(self):
		pass

def testFieldVariable():
	d = Dim()
	i = 0
	while i < 100000000:
		d.y += 1
		i += 1
	return d

def testMathFabs():
	i = 0
	f = 1.0
	while i < 100000000:
		i += 1
		f = math.fabs(-1.0)
	return f

def run():
	s = time.time()
	testSimpleLoop()
	period = (time.time() - s) * 1000
	print "SimpleLoop[ms]: ", period, int(period)
	delta = period
	##
	s = time.time()
	testLocalVariable()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "LocalVariable[ms]:", period, int(score)
	##
	s = time.time()
	testGlobalVariable()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "GlobalVariable[ms]:", period, int(score)
	##
	s = time.time()
	testFunctionCall()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "FunctionCall[ms]:", period, int(score)
	##
	s = time.time()
	testFunctionReturn()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "FunctionReturn[ms]:", period, int(score)
	##
	s = time.time()
	testFloatOperation()
	period = ((time.time() - s) * 1000)
	score = period - delta
	print "FloatOperation[ms]:", period, int(score)
	s = time.time()
	testMathFabs()
	period = ((time.time() - s) * 1000)
	score = period - (delta)
	print "MathFabs[ms]:", period, int(score)
	##
	print "Object-Oriented Programming"
	s = time.time()
	testFieldVariable()
	period = (time.time() - s) * 1000
	score = period - delta
	print "FieldVariable[ms]:", period, int(score)

run()


