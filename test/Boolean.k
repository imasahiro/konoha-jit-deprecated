
test "Boolean op #19080" {
	assert ((true && false) == false) && ((true || false) == true);
}
int a = 1;
int b = 3;
test "Boolean.opOR #18782" {
	assert (!(a == 1) || b == 3) == true && (!(a == 1) or b == 3) == true;
}

