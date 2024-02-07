#!/bin/sh

. test_utils

make || {
	echo Make failed.
	exit 2
}

rm -f out/*

echo Testy sygnalow i exitow.

echo Test 1: lock exit

run 1 lock_exit u3t1r1.out u3t1.out
run 2 lock_exit u3t1r2.out u3t1.out

echo Test 2: many lock exit

run 1 mlocks_exit u3t2r1.out u3t2.out
run 2 mlocks_exit u3t2r2.out u3t2.out
run 3 mlocks_exit u3t2r3.out u3t2.out

echo Test 3: lock kill

run 1 lock_kill u3t3r1.out u3t3.out
run 2 lock_kill u3t3r2.out u3t3.out

echo Test 4: many lock kill

run 1 mlocks_kill u3t4r1.out u3t4.out
run 2 mlocks_kill u3t4r2.out u3t4.out
run 3 mlocks_kill u3t4r3.out u3t4.out

echo Test 5: cond kill

run 1 cond_kill u3t5r1.out u3t5.out
run 2 cond_kill u3t5r2.out u3t5.out

echo Test 6: many conds kill

run 1 mconds_kill u3t6r1.out u3t6.out
run 2 mconds_kill u3t6r2.out u3t6.out
run 3 mconds_kill u3t6r3.out u3t6.out
