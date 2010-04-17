
test "Print Tuple data #21444" {
    p = ("1","2","3","4");
    %k(p);
    assert (p.size == 4);
}
