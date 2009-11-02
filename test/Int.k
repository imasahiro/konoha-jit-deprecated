int a = 1;
int b = 3;
test "Boolean.opOR" {
	assert (!(a == 1) || b == 3) == true && (!(a == 1) or b == 3) == true;
}

