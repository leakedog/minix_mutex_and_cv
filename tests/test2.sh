#!/bin/sh

. test_utils

make || {
	echo Make failed.
	exit 2
}

rm -f out/*

echo Testy condition variables bez sygnalow i exitow.

echo Test 1: jedno zdarzenie dwa procesy 

run 1 simple_cv u2t1r1.out u2t1.out
run 2 simple_cv u2t1r2.out u2t1.out
run 3 simple_cv u2t1r3.out u2t1.out
echo -n run 4 ""
$BIN/simple_cv 1 > $OUT/u2t1r4.out
mdiff u2t1r4.out u2t1.out
echo -n run 5 ""
$BIN/simple_cv 1 > $OUT/u2t1r5.out
mdiff u2t1r5.out u2t1.out

echo Test 2: jedno zdarzenie dwa procesy \(tic tac\)

echo -n run 1 ""
$BIN/tictac 3 > $OUT/u2t2r1.out
mdiff u2t2r1.out u2t2r1.out
echo -n run 2 ""
$BIN/tictac 7 > $OUT/u2t2r2.out
mdiff u2t2r2.out u2t2r2.out

echo Test 3: dwa zdarzenia trzy procesy \(tic tac tac\)
 
echo -n run 1 ""
$BIN/tictactac 3 > $OUT/u2t3r1.out
mdiff u2t3r1.out u2t3r1.out
echo -n run 2 ""
$BIN/tictactac 7 > $OUT/u2t3r2.out
mdiff u2t3r2.out u2t3r2.out

echo Test 4: grupa testow jak test 3 \(3 x \(tic tac tac\)\)
 
echo -n run 1 ""
$BIN/tictactac 3 > $OUT/u2t4r1.g1.out &
$BIN/tictactac 3 > $OUT/u2t4r1.g2.out &
$BIN/tictactac 3 > $OUT/u2t4r1.g3.out &
wait
mdiff u2t4r1.g1.out u2t4r1.out
mdiff u2t4r1.g2.out u2t4r1.out
mdiff u2t4r1.g2.out u2t4r1.out
echo -n run 1 ""
$BIN/tictactac 7 > $OUT/u2t4r2.g1.out &
$BIN/tictactac 7 > $OUT/u2t4r2.g2.out &
$BIN/tictactac 7 > $OUT/u2t4r2.g3.out &
wait
mdiff u2t4r2.g1.out u2t4r2.out
mdiff u2t4r2.g2.out u2t4r2.out
mdiff u2t4r2.g2.out u2t4r2.out

echo Test 4.3: jeden mutex i wiele zmiennych

echo -n run 1 ""
$BIN/many_cv2 10 | sort > $OUT/u2t43r1.out
mdiff u2t43r1.out u2t43.out

echo -n run 2 ""
$BIN/many_cv2 10 | sort > $OUT/u2t43r2.out
mdiff u2t43r2.out u2t43.out

echo -n run 3 ""
$BIN/many_cv2 10 | sort > $OUT/u2t43r3.out
mdiff u2t43r3.out u2t43.out

echo Test 4.5: wiele mutexow i zmiennych

echo -n run 1 ""
$BIN/many_cv 10 > $OUT/u2t45r1.out
mdiff u2t45r1.out u2t45.out

echo -n run 2 ""
$BIN/many_cv 10 > $OUT/u2t45r2.out
mdiff u2t45r2.out u2t45.out

echo -n run 3 ""
$BIN/many_cv 10 > $OUT/u2t45r3.out
mdiff u2t45r3.out u2t45.out

echo Test 5: producenci i konsumenci, rozne konfiguracje jedna grupa 
 
echo -n run 1 ""
$BIN/prod_cons 4 4 50 template_mode |sort > $TEMPLATE/u2t5r1.out 
$BIN/prod_cons 4 4 50 |sort > $OUT/u2t5r1.out 
mdiff u2t5r1.out u2t5r1.out

echo -n run 2 ""
$BIN/prod_cons 3 4 50 template_mode |sort > $TEMPLATE/u2t5r2.out 
$BIN/prod_cons 3 4 50 |sort > $OUT/u2t5r2.out 
mdiff u2t5r2.out u2t5r2.out

echo -n run 3 ""
$BIN/prod_cons 4 3 50 template_mode |sort > $TEMPLATE/u2t5r3.out 
$BIN/prod_cons 4 3 50 |sort > $OUT/u2t5r3.out 
mdiff u2t5r3.out u2t5r3.out

echo Test 6: producenci i konsumenci, rozne konfiguracje wiele grup 
 
echo -n run 1 ""
$BIN/prod_cons 4 4 50 template_mode |sort > $TEMPLATE/u2t6r1.out 
$BIN/prod_cons 4 4 50 |sort > $OUT/u2t6r1.g1.out  &
$BIN/prod_cons 4 4 50 |sort > $OUT/u2t6r1.g2.out  &
$BIN/prod_cons 4 4 50 |sort > $OUT/u2t6r1.g3.out  &
wait
mdiff u2t6r1.g1.out u2t6r1.out
mdiff u2t6r1.g2.out u2t6r1.out
mdiff u2t6r1.g3.out u2t6r1.out

echo -n run 2 ""
$BIN/prod_cons 3 4 50 template_mode |sort > $TEMPLATE/u2t6r2.out 
$BIN/prod_cons 3 4 50 |sort > $OUT/u2t6r2.g1.out  &
$BIN/prod_cons 3 4 50 |sort > $OUT/u2t6r2.g2.out  &
$BIN/prod_cons 3 4 50 |sort > $OUT/u2t6r2.g3.out  &
wait
mdiff u2t6r2.g1.out u2t6r2.out
mdiff u2t6r2.g2.out u2t6r2.out
mdiff u2t6r2.g3.out u2t6r2.out

echo -n run 3 ""
$BIN/prod_cons 4 3 50 template_mode |sort > $TEMPLATE/u2t6r3.out 
$BIN/prod_cons 4 3 50 |sort > $OUT/u2t6r3.g1.out  &
$BIN/prod_cons 4 3 50 |sort > $OUT/u2t6r3.g2.out  &
$BIN/prod_cons 4 3 50 |sort > $OUT/u2t6r3.g3.out  &
wait
mdiff u2t6r3.g1.out u2t6r3.out
mdiff u2t6r3.g2.out u2t6r3.out
mdiff u2t6r3.g3.out u2t6r3.out


