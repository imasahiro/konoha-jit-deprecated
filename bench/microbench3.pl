#!/usr/bin/perl
use strict;
use warnings;

package Dim;
sub new {
	my $class = shift;
	return bless {x => shift, y => shift, z => shift}, $class;
}
sub f {
}

package main;

sub testSimpleLoop () {
	my $i = 0;
	for($i=0;$i < 10000000; $i++) {
	}
	return $i;
}


sub testLocalVariable() {
	my $y = 0;
	for (my $i=0; $i < 10000000; $i++) {
		$y = $i;
	}
}

my $global_x;
my $global_y;
my $global_z;

sub testGlobalVariable {
	for (my $i=0; $i < 10000000; $i++) {
		$global_y = $i;
	}
	return $global_y;
}

sub testStringAssignment {
	my $s = "";
	for (my $i = 0; $i < 10000000; $i++) {
		$s = "A";
	}
	return $s;
}

sub testIntegerOperation() {
	my $y = 0;
	for (my $i = 0; $i < 10000000; $i++) {
		$y += $i;
	}
	return $y;
}

sub testFloatOperation {
	my $f = 0.0;
	for (my $i = 0; $i < 10000000; $i++) {
		$f += 0.1;
	}
	return $f;
}


sub func0 {
}

sub testFunctionCall {
	my $i;
	for ($i = 0; $i < 10000000; $i++) {
		func0();
	}
	return $i;
}

sub testFunctionReturn {
	my $res = 0;
	for (my $i = 0; $i < 10000000; $i++) {
		$res = func1();
	}
	return $res;
}

sub func1 {
	return 1;
}

sub testMathFabs() {
	my $res = 0.0;
	my $i = 0;
	for ($i = 0; $i < 10000000; $i++) {
		$res = abs(-1.0);
	}
	return $i;
}

sub testCallFunctionObject{

}

sub testObjectCreation {
	my $i = 0;
	for ($i=0; $i < 10000000; $i++) {
		my $d = new Dim();
	}
	return $i;
}

sub testFieldVariable {
	my $d = new Dim();
	my $i = 0;
	for ($i=0; $i < 10000000; $i++) {
		($d)->{y} = 1;
	}
	return $d;
}


sub testMethodCall {
	my $d = new Dim();
	my $i = 0;
	for ($i=0; $i < 10000000; $i++) {
		($d)->f();
	}
	return $d;
}

sub fibo {
	my ($n) = @_;
	if ($n < 3) {
		return 1;
	}
	return fibo($n - 1) + fibo($n - 2);
}

sub mops {
	my ($score) = @_;
	if ($score < 1) {
		return 0;
	}
	#return (10000000 / ($score)) / 100.0;
	return (10000 / ($score)) / 1000;
}

my ($score, $period, $delta);
my ($user1, $system, $cursor, $csystem);
my ($user2, $system2, $cursor2, $csystem2);
($user1, $system, $cursor, $csystem) = times;
testLocalVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$period = $user2 - $user1;
$score = mops($period);
print "SimpleLoop:$score ", $user2-$user1, "[sec]\n";
$delta = $period;

($user1, $system, $cursor, $csystem) = times;
testLocalVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "LocalVariable:$score ", $user2-$user1, "[sec]\n";


($user1, $system, $cursor, $csystem) = times;
testGlobalVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "GlobalVariable:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testStringAssignment();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "StringAssignment:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testIntegerOperation();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "IntegerOperation:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testFloatOperation();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "FloatOperation:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testFunctionCall();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "FunctionCall:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testFunctionReturn();
($user2, $system2, $cursor2, $csystem2) = times;

print "FunctionReturn:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testMathFabs();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "NativeFunction: (none)\n";

($user1, $system, $cursor, $csystem) = times;
testObjectCreation();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "ObjectCreation:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testFieldVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "FieldVariable:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
testMethodCall();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user1 - $delta);
print "MethodCall:$score ", $user2-$user1, "[sec]\n";

($user1, $system, $cursor, $csystem) = times;
fibo(36);
($user2, $system2, $cursor2, $csystem2) = times;
$score = ($user2-$user1);
print "fib(36):$score ", $user2-$user1, "[sec]\n";

