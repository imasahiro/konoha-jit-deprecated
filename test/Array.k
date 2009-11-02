Array a = [1,2];
Int[] ia = [1,2,3];
int i = 3;
a[1] = i;

test "Array access #18971" {
	assert a[1] == 3;
}
test "Array Increment #18784" {
	ia[0]++;
	ia[1]--;
	assert ia[0] == 2 && ia[1] == 1;
}
