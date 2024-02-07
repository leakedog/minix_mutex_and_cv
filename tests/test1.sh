#!/bin/sh

. test_utils

make || {
	echo Make failed.
	exit 2
}

rm -f out/*

echo Testy mutex\'ow bez sygnalow i exitow.

echo Test 1: lock na 1, unlock na 2, unlock na 1, unlock na 1

run 1 lock_unlock u1t1r1.out u1t1.out
run 2 lock_unlock u1t1r1.out u1t1.out

echo Test 2: wiele lockow i unlockow, 1 process, rozna kolejnosc

run 1 simple_many u1t2r1.out u1t2.out
run 2 simple_many u1t2r1.out u1t2.out
run 3 simple_many u1t2r1.out u1t2.out


echo Test 3: wiele procesow, jeden mutex \(trwa ok 30s\)

echo -n run 1 ""
$BIN/many_procs 5 5 | sort > $OUT/u1t3r1.out
mdiff u1t3r1.out u1t3.out

echo Test 4: test kolejnosci, wiele procesow jeden mutex \(ok 16s\)

echo -n run 1 ""
$BIN/check_fifo 5 > $OUT/u1t4r1.out
mdiff u1t4r1.out u1t4r1.out

echo -n run 2 ""
$BIN/check_fifo 11 > $OUT/u1t4r2.out
mdiff u1t4r2.out u1t4r2.out

echo Test 5: trzy grupy procesow kazda jeden mutex \(ok 60s\)

run5(){
	echo -n run $1 ""
	$BIN/many_procs 5 5 | sort > $OUT/u1t5r$1.g1.out &
	$BIN/many_procs 5 5 | sort > $OUT/u1t5r$1.g2.out &
	$BIN/many_procs 5 5 | sort > $OUT/u1t5r$1.g3.out &
	wait
	mdiff u1t5r$1.g1.out u1t5.out
	mdiff u1t5r$1.g2.out u1t5.out
	mdiff u1t5r$1.g3.out u1t5.out
}

run5 1
run5 2

echo Test 6: wiele procesow, wiele mutexow, jedna grupa
echo run 1
$BIN/many_pm 4 3 mmtest
for n in 0 1 2 3 4 
do
	sort mmtest.$n.out > $OUT/u1t6r1.f$n.out
	rm -f mmtest.$n.out
	mdiff u1t6r1.f$n.out u1t6.out
done

echo Test 7: wiele procesow, wiele mutexow, wiele grup
echo run 1
$BIN/many_pm 4 3 mmtest1 &
$BIN/many_pm 4 3 mmtest2 &
$BIN/many_pm 4 3 mmtest3 &
wait
for m in 1 2 3 
do
	for n in 0 1 2 3 4 
	do
		sort mmtest$m.$n.out > $OUT/u1t7r1.f$n.g$m.out
		rm -f mmtest$m.$n.out 
		mdiff u1t7r1.f$n.g$m.out u1t7.out
	done
done
