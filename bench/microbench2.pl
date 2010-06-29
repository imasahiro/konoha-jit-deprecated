#!/usr/bin/perl

package Dim;
sub new {
	my $class = shift;
	return bless {x => shift}, $class;
}	

package main;

sub testSimpleLoop () {
	for($i=0;$i < 10000000; $i++) {
	}
	return $i;
}


sub testLocalVariable() {
	my $y = 0;
	for ($i=0; $i < 10000000; $i++) {
		$y = $i;
	}
}

$global_x;
$global_y;
$global_z;

sub testGlobalVariable {
	for ($i=0; $i < 10000000; $i++) {
		$global_y = $i;
	}
	return $global_y;
}

sub testStringAssignment {
	my $s = "";
	for ($i=0; $i < 10000000; $i++) {
		$s = "A";
	}
	return $s;
}

sub testIntegerOperation() {
	my $y = 0;
	for ($i = 0; $i < 10000000; $i++) {
		$y += $i;
	}
	return $y;
}

sub testFloatOperation {
	my $f = 0.0;
	for ($i = 0; $i < 10000000; $i++) {
		$f += 0.1;
	}
	return $f;
}


sub func0 {
}

sub testFunctionCall {
	for ($i = 0; $i < 10000000; $i++) {
		func0();
	}
	return $i;
}

sub testFunctionReturn {
	$res = 0;
	for ($i = 0; $i < 10000000; $i++) {
		$res = func1();
	}
	return $res;
}

sub func1 {
	return 1;
}

sub testMathFabs() {
    $res = 0.0;
	for ($i=0; $i < 10000000; $i++) {
		$res = abs(-1.0);
	}
	return $i;
}

sub testCallFunctionObject{

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
	return (10000 / ($score)) / 1000;
}

sub testFieldVariable {
	my $d = new Dim();
	for ($i=0; $i < 10000000; $i++) {
		($d)->{y} = 1;
	}
	return $d;
}

sub mops {
	my ($score) = @_;
	return 10 / $score;
}

#use Time::HiRes qw(gettimeofday, tv_interval);


#$t0 = Benchmark->new;
#testSimpleLoop();
#$t1 = Benchmark->new;
#$td = timediff($t1, $t0);
#$score = mops($td);
#$print "testSimpleLoop:$score", timestr($td), "\n";

($user, $system, $cursor, $csystem) = times;
testLocalVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$period = $user2 - $user;
$score = mops($period);
print "testSimpleLoop:$score ", $user2-$user1, "[sec]\n";
$delta = $period;

($user, $system, $cursor, $csystem) = times;
testLocalVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testLocalVariable:$score ", $user2-$user1, "[sec]\n";


($user, $system, $cursor, $csystem) = times;
testGlobalVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testGlobalVariable:$score ", $user2-$user1, "[sec]\n";

($user, $system, $cursor, $csystem) = times;
testStringAssignment();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testStringAssignment:$score ", $user2-$user1, "[sec]\n";

($user, $system, $cursor, $csystem) = times;
testIntegerOperation();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testIntegerOperation:$score ", $user2-$user1, "[sec]\n";

($user, $system, $cursor, $csystem) = times;
testFloatOperation();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testFloatOperation:$score ", $user2-$user1, "[sec]\n";

($user, $system, $cursor, $csystem) = times;
testFunctionCall();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testFunctionCall:$score ", $user2-$user1, "[sec]\n";

($user, $system, $cursor, $csystem) = times;
testFunctionReturn();
($user2, $system2, $cursor2, $csystem2) = times;

print "testFunctionReturn:$score ", $user2-$user1, "[sec]\n";

($user, $system, $cursor, $csystem) = times;
testMathFabs();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testMathFabs: (none)\n";

($user, $system, $cursor, $csystem) = times;
fibo(36);
($user2, $system2, $cursor2, $csystem2) = times;
$score = ($user2-$user);
print "fib(36):$score ", $user2-$user1, "[sec]\n";


($user, $system, $cursor, $csystem) = times;
testFieldVariable();
($user2, $system2, $cursor2, $csystem2) = times;
$score = mops($user2-$user - $delta);
print "testFieldVariable:$score ", $user2-$user1, "[sec]\n";


