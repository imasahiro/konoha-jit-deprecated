mkdir foo
mount -t kfs / ./foo/
mkdir ./foo/aaa
mkdir ./foo/bbb
mkdir ./foo/cccdd
mkdir ./foo/dd
mkdir ./foo/aaa/111
mkdir ./foo/aaa/222
mkdir ./foo/aaa/333
touch ./foo/aaa/hoge
echo "hi" > foo/aaa/fuga
cat foo/aaa/fuga
ls foo/aaa
umount ./foo/
rmdir foo
